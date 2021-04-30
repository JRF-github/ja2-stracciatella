// -*-coding: utf-8-unix;-*-

#include "gtest/gtest.h"

#include "LoadSaveData.h"

#include "externalized/RustInterface.h"
#include "externalized/TestUtils.h"


TEST(LoadSaveData, integers)
{
	BYTE buf[100];

	{
		DataWriter writer(buf);
		writer.writeU8(0x01);
		writer.writeU8(0x00);
		writer.writeU8(0xff);

		DataReader reader(buf);
		EXPECT_EQ(reader.readU8(), 0x01);
		EXPECT_EQ(reader.readU8(), 0x00);
		EXPECT_EQ(reader.readU8(), 0xff);

		EXPECT_EQ(reader.getConsumed(), 3u);
		EXPECT_EQ(writer.getConsumed(), 3u);
	}

	{
		DataWriter writer(buf);
		writer.writeU16(0x0123);
		writer.writeU16(0x0000);
		writer.writeU16(0xffff);

		DataReader reader(buf);
		EXPECT_EQ(reader.readU16(), 0x0123);
		EXPECT_EQ(reader.readU16(), 0x0000);
		EXPECT_EQ(reader.readU16(), 0xffff);

		EXPECT_EQ(reader.getConsumed(), 6u);
		EXPECT_EQ(writer.getConsumed(), 6u);
	}

	{
		DataWriter writer(buf);
		writer.writeU32(0x01234567);
		writer.writeU32(0x00000000);
		writer.writeU32(0xffffffff);

		DataReader reader(buf);
		EXPECT_EQ(reader.readU32(), 0x01234567u);
		EXPECT_EQ(reader.readU32(), 0x00000000u);
		EXPECT_EQ(reader.readU32(), 0xffffffffu);

		EXPECT_EQ(reader.getConsumed(), 12u);
		EXPECT_EQ(writer.getConsumed(), 12u);
	}
}

TEST(LoadSaveData, writeUTF16English)
{
	BYTE buf[100];

	DataWriter writer(buf);
	writer.writeUTF16("test", 5);

	// read as 5 uint16
	{
		DataReader reader(buf);
		EXPECT_EQ(reader.readU16(), 't');
		EXPECT_EQ(reader.readU16(), 'e');
		EXPECT_EQ(reader.readU16(), 's');
		EXPECT_EQ(reader.readU16(), 't');
		EXPECT_EQ(reader.readU16(), 0x0000);

		EXPECT_EQ(reader.getConsumed(), 10u);
		EXPECT_EQ(writer.getConsumed(), 10u);
	}
}

TEST(LoadSaveData, writeUTF16Russian)
{
	BYTE buf[100];

	DataWriter writer(buf);
	writer.writeUTF16("тест", 5);

	// read as 5 uint16
	{
		DataReader reader(buf);
		EXPECT_EQ(reader.readU16(), 0x0442);
		EXPECT_EQ(reader.readU16(), 0x0435);
		EXPECT_EQ(reader.readU16(), 0x0441);
		EXPECT_EQ(reader.readU16(), 0x0442);
		EXPECT_EQ(reader.readU16(), 0x000);
	}
}

TEST(LoadSaveData, readUTF16)
{
	BYTE buf[100];
	DataWriter writer(buf);

	writer.writeU16(0x0442);
	writer.writeU16(0x0435);
	writer.writeU16(0x0441);
	writer.writeU16(0x0442);
	writer.writeU16(0x000);

	{
		DataReader reader(buf);
		EXPECT_EQ(reader.readUTF16(5), "тест");

		EXPECT_EQ(reader.getConsumed(), 10u);
		EXPECT_EQ(writer.getConsumed(), 10u);
	}
}

TEST(LoadSaveData, readUTF32)
{
	BYTE buf[100];
	DataWriter writer(buf);

	writer.writeU32(0x00000442);
	writer.writeU32(0x00000435);
	writer.writeU32(0x00000441);
	writer.writeU32(0x00000442);
	writer.writeU32(0x00000000);

	{
		DataReader reader(buf);
		EXPECT_EQ(reader.readUTF32(5), "тест");

		EXPECT_EQ(reader.getConsumed(), 20u);
		EXPECT_EQ(writer.getConsumed(), 20u);
	}
}

TEST(LoadSaveData, floatAndDoubleFormat)
{
	// We need to make sure that on the current platform
	// float and double have the same binary representation
	// as on Windows.  Otherwise, there will be problems with
	// loading saved games made on Windows.

	ST::string floatsPath = FileMan::joinPaths(GetExtraDataDir(), "unittests/datatypes/floats.bin");
	ST::string doublesPath = FileMan::joinPaths(GetExtraDataDir(), "unittests/datatypes/doubles.bin");

	// // Test data were previously written with the following code.
	// {
	//   float f;
	//   FILE *floats = fopen(floatsPath, "wb");
	//   f = 0;              fwrite(&f, sizeof(float), 1, floats);
	//   f = 1;              fwrite(&f, sizeof(float), 1, floats);
	//   f = -1;             fwrite(&f, sizeof(float), 1, floats);
	//   f = 1.1234678f;     fwrite(&f, sizeof(float), 1, floats);
	//   f = 12345.678f;     fwrite(&f, sizeof(float), 1, floats);
	//   fclose(floats);

	//   double d;
	//   FILE *doubles = fopen(doublesPath, "wb");
	//   d = 0;              fwrite(&d, sizeof(double), 1, doubles);
	//   d = 1;              fwrite(&d, sizeof(double), 1, doubles);
	//   d = -1;             fwrite(&d, sizeof(double), 1, doubles);
	//   d = 1.1234678;      fwrite(&d, sizeof(double), 1, doubles);
	//   d = 12345.678;      fwrite(&d, sizeof(double), 1, doubles);
	//   fclose(doubles);

	//   printf("float size: %d\n", sizeof(float));
	//   printf("double size: %d\n", sizeof(double));
	// }

	ASSERT_EQ(sizeof(float),  4u);
	ASSERT_EQ(sizeof(double), 8u);

	{
		RustPointer<VecU8> buf(Fs_read(floatsPath.c_str()));
		ASSERT_TRUE(buf);
		ASSERT_EQ(VecU8_len(buf.get()), sizeof(float) * 5);
		float f;

		DataReader S{VecU8_as_ptr(buf.get())};
		EXTR_FLOAT(S, f); EXPECT_EQ(f, 0         );
		EXTR_FLOAT(S, f); EXPECT_EQ(f, 1         );
		EXTR_FLOAT(S, f); EXPECT_EQ(f, -1        );
		EXTR_FLOAT(S, f); EXPECT_EQ(f, 1.1234678f);
		EXTR_FLOAT(S, f); EXPECT_EQ(f, 12345.678f);
		ASSERT_EQ(S.getConsumed(), sizeof(float) * 5);
	}

	{
		RustPointer<VecU8> buf(Fs_read(doublesPath.c_str()));
		ASSERT_TRUE(buf);
		ASSERT_EQ(VecU8_len(buf.get()), sizeof(double) * 5);
		double d;

		DataReader S{VecU8_as_ptr(buf.get())};
		EXTR_DOUBLE(S, d); EXPECT_EQ(d, 0         );
		EXTR_DOUBLE(S, d); EXPECT_EQ(d, 1         );
		EXTR_DOUBLE(S, d); EXPECT_EQ(d, -1        );
		EXTR_DOUBLE(S, d); EXPECT_EQ(d, 1.1234678 );
		EXTR_DOUBLE(S, d); EXPECT_EQ(d, 12345.678 );
		ASSERT_EQ(S.getConsumed(), sizeof(double) * 5);
	}
}


#include "../game/Tactical/Overhead.h"

TEST(LoadSaveData, overloadedOperators)
{
	uint8_t buf[100];
	// Required setup for Soldier2ID and ID2Soldier
	Menptr[5].ubID = 5;
	Menptr[8].ubID = 8;

	int i{4};
	int16_t i16{-2};
	uint8_t ui8{20};
	double d{7.5};
	float f{-0.25};
	SOLDIERTYPE *stp{&Menptr[5]};
	SOLDIERTYPE const *stcp{&Menptr[8]};
	void *vp{(void*)0x12345678};
	std::array<int8_t, 3> i8a{-1, 5, 80};

	DataWriter dw{buf};
	dw << i << i16 << ui8 << d << skip<10> << (int32_t)0 << f << stp << stcp << vp << i8a;
	EXPECT_EQ(dw.getConsumed(), 42);

	i = i16 = ui8 = d = f = 0;
	stcp = stp = nullptr;
	i8a = { 0, 0, 0};
	vp = (void*)0xcafe; // assign some non-null value to check that pointers get set to nullptr

	DataReader dr{buf};
	dr >> i >> i16 >> ui8 >> d >> skip<14> >> f >> stp >> stcp >> vp >> i8a;
	EXPECT_EQ(dr.getConsumed(), 42);

	EXPECT_EQ(i, 4);
	EXPECT_EQ(i16, -2);
	EXPECT_EQ(ui8, 20);
	EXPECT_EQ(d, 7.5);
	EXPECT_EQ(f, -0.25);
	EXPECT_EQ(stp, &Menptr[5]);
	EXPECT_EQ(stcp, &Menptr[8]);
	EXPECT_EQ(vp, nullptr);
	EXPECT_EQ(i8a[2], 80);
}
