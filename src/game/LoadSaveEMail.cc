#include "Debug.h"
#include "FileMan.h"
#include "LoadSaveData.h"
#include "LoadSaveEMail.h"


static void LoadEMailFromFile(HWFILE const File)
{
	UINT32 uiSizeOfSubject;
	FileRead(File, &uiSizeOfSubject, sizeof(UINT32)); // XXX HACK000B
	FileSeek(File, uiSizeOfSubject, FILE_SEEK_FROM_CURRENT); // XXX HACK000B

	UINT16	usOffset;
	UINT16	usLength;
	UINT8		ubSender;
	UINT32	iDate;
	INT32		iFirstData;
	UINT32	uiSecondData;
	BOOLEAN fRead;

	FileDataReader{44, File}
	  >> usOffset >> usLength >> ubSender
	  >> skip<3>
	  >> iDate
	  >> skip<4>
	  >> iFirstData >> uiSecondData
	  >> skip<16>
	  >> fRead
	  >> skip<3>;

	AddEmailMessage(usOffset, usLength, iDate, ubSender, fRead, iFirstData, uiSecondData);
}


void LoadEmailFromSavedGame(HWFILE const File)
{
	ShutDownEmailList();

	UINT32 uiNumOfEmails;
	FileRead(File, &uiNumOfEmails, sizeof(UINT32));

	for (UINT32 cnt = 0; cnt < uiNumOfEmails; cnt++)
	{
		LoadEMailFromFile(File);
	}
}


static void SaveEMailIntoFile(HWFILE const File, Email const* const Mail)
{
	FileDataWriter{48, File}
	  << (UINT32)0	// was size of subject
	  << Mail->usOffset
	  << Mail->usLength
	  << Mail->ubSender
	  << skip<3>
	  << Mail->iDate
	  << skip<4>
	  << Mail->iFirstData
	  << Mail->uiSecondData
	  << skip<16>
	  << Mail->fRead
	  << skip<3>;
}


void SaveEmailToSavedGame(HWFILE const File)
{
	const Email* pEmail;

	// Count the emails
	UINT32 uiNumOfEmails = 0;
	for (pEmail = pEmailList; pEmail != NULL; pEmail = pEmail->Next)
	{
		uiNumOfEmails++;
	}
	FileWrite(File, &uiNumOfEmails, sizeof(UINT32));

	for (pEmail = pEmailList; pEmail != NULL; pEmail = pEmail->Next)
	{
		SaveEMailIntoFile(File, pEmail);
	}
}
