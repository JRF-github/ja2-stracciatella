#ifndef _LAPTOP_SAVE_H_
#define _LAPTOP_SAVE_H_

#include "Item_Types.h"
#include "Store_Inventory.h"
#include <array>
#include <vector>

#define MAX_BOOKMARKS			20



#define MAX_PURCHASE_AMOUNT		10




#define SPECK_QUOTE__ALREADY_TOLD_PLAYER_THAT_LARRY_RELAPSED	0x00000001
#define SPECK_QUOTE__SENT_EMAIL_ABOUT_LACK_OF_PAYMENT		0x00000002


struct LIFE_INSURANCE_PAYOUT
{
	BOOLEAN fActive;
	UINT8   ubSoldierID;
	UINT8   ubMercID;
	INT32   iPayOutPrice;
};


struct LAST_HIRED_MERC_STRUCT
{
	BOOLEAN fHaveDisplayedPopUpInLaptop; // Is set when the popup gets displayed, reset when entering laptop again.
	INT32   iIdOfMerc;
	UINT32  uiArrivalTime;
};


struct BobbyRayPurchaseStruct
{
	UINT16  usItemIndex;
	UINT8   ubNumberPurchased;
	INT8    bItemQuality;
	UINT16  usBobbyItemIndex; //Item number in the BobbyRayInventory structure
	BOOLEAN fUsed; //Indicates wether or not the item is from the used inventory or the regular inventory
};


struct BobbyRayOrderStruct
{
	BOOLEAN	fActive;
	BobbyRayPurchaseStruct BobbyRayPurchase[ MAX_PURCHASE_AMOUNT ];
	UINT8	ubNumberPurchases;
};


//used when the player goes to bobby rays when it is still down
enum
{
	BOBBYR_NEVER_BEEN_TO_SITE,
	BOBBYR_BEEN_TO_SITE_ONCE,
	BOBBYR_ALREADY_SENT_EMAIL,
};


struct LaptopSaveInfoStruct
{
	//General Laptop Info
	BOOLEAN gfNewGameLaptop; //Is it the firs time in Laptop
	std::array<BOOLEAN, 20> fVisitedBookmarkAlready; // have we visitied this site already?
	std::array<INT32, MAX_BOOKMARKS> iBookMarkList;


	INT32   iCurrentBalance; // current players balance


	//IMP Information
	BOOLEAN fIMPCompletedFlag; // Has the player Completed the IMP process
	BOOLEAN fSentImpWarningAlready; // Has the Imp email warning already been sent


	//Personnel Info
	std::array<INT16, 256> ubDeadCharactersList;
	std::array<INT16, 256> ubLeftCharactersList;
	std::array<INT16, 256> ubOtherCharactersList;


	// MERC site info
	UINT8  gubPlayersMercAccountStatus;
	UINT32 guiPlayersMercAccountNumber;
	UINT8  gubLastMercIndex; //  the index of the last hirable merc profile in M.E.R.C.


	// Aim Site


	// BobbyRay Site
	STORE_INVENTORY BobbyRayInventory[ MAXITEMS ];
	STORE_INVENTORY BobbyRayUsedInventory[ MAXITEMS ];

	std::vector<BobbyRayOrderStruct> BobbyRayOrdersOnDeliveryArray;
	UINT8 usNumberOfBobbyRayOrderUsed; // The number of items in the array that are used


	// Flower Site
	//NONE

	// Insurance Site
	std::vector<LIFE_INSURANCE_PAYOUT> pLifeInsurancePayouts;
	UINT8   ubNumberLifeInsurancePayoutUsed; // The number of items in the array that are used

	BOOLEAN fBobbyRSiteCanBeAccessed;

	UINT8   ubPlayerBeenToMercSiteStatus;
	BOOLEAN fFirstVisitSinceServerWentDown;
	BOOLEAN fNewMercsAvailableAtMercSite;
	BOOLEAN fSaidGenericOpeningInMercSite;
	BOOLEAN fSpeckSaidFloMarriedCousinQuote;
	BOOLEAN fHasAMercDiedAtMercSite;

	std::array<UINT16, BOBBY_RAY_LISTS> usInventoryListLength;

	INT32   iVoiceId;

	UINT8   ubHaveBeenToBobbyRaysAtLeastOnceWhileUnderConstruction;

	BOOLEAN fMercSiteHasGoneDownYet;

	UINT8   ubSpeckCanSayPlayersLostQuote;

	LAST_HIRED_MERC_STRUCT sLastHiredMerc;

	INT32   iCurrentHistoryPage;
	INT32   iCurrentFinancesPage;
	INT32   iCurrentEmailPage;

	UINT32  uiSpeckQuoteFlags;

	UINT32  uiFlowerOrderNumber;

	UINT32  uiTotalMoneyPaidToSpeck;

	UINT8   ubLastMercAvailableId; // deprecated
};


extern LaptopSaveInfoStruct LaptopSaveInfo;

extern BobbyRayPurchaseStruct BobbyRayPurchases[ MAX_PURCHASE_AMOUNT ];


void LoadLaptopInfoFromSavedGame(HWFILE);
void SaveLaptopInfoToSavedGame(HWFILE);

#endif
