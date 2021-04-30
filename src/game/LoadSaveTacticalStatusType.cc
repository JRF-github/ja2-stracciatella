#include "FileMan.h"
#include "GameState.h"
#include "LoadSaveData.h"
#include "LoadSaveTacticalStatusType.h"
#include "Overhead.h"


void ExtractTacticalStatusTypeFromFile(HWFILE const f, bool stracLinuxFormat)
{
	UINT32 dataSize = stracLinuxFormat ? TACTICAL_STATUS_TYPE_SIZE_STRAC_LINUX : TACTICAL_STATUS_TYPE_SIZE;

	TacticalStatusType* const s = &gTacticalStatus;
	FileDataReader d{dataSize, f};
	d >> s->uiFlags;
	FOR_EACH(TacticalTeamType, t, s->Team)
	{
		d >> t->bFirstID
		  >> t->bLastID
		  >> skip<2>
		  >> t->RadarColor
		  >> t->bSide
		  >> t->bMenInSector
		  >> t->last_merc_to_radio
		  >> skip<1>
		  >> t->bAwareOfOpposition
		  >> t->bHuman
		  >> skip<2>;
	}
	d >> s->ubCurrentTeam
	  >> skip<1>
	  >> s->sSlideTarget
	  >> skip<4>
	  >> s->uiTimeSinceMercAIStart
	  >> s->fPanicFlags
	  >> skip<5>
	  >> s->ubSpottersCalledForBy
	  >> s->the_chosen_one
	  >> s->uiTimeOfLastInput
	  >> s->uiTimeSinceDemoOn
	  >> skip<7>
	  >> s->fCivGroupHostile
	  >> s->ubLastBattleSectorX
	  >> s->ubLastBattleSectorY
	  >> s->fLastBattleWon
	  >> skip<2>
	  >> s->fVirginSector
	  >> s->fEnemyInSector
	  >> s->fInterruptOccurred
	  >> s->bRealtimeSpeed
	  >> skip<2>
	  >> s->enemy_sighting_on_their_turn_enemy
	  >> skip<1>
	  >> s->fEnemySightingOnTheirTurn
	  >> s->fAutoBandageMode
	  >> s->ubAttackBusyCount
	  >> skip<1>
	  >> s->ubEngagedInConvFromActionMercID
	  >> skip<1>
	  >> s->usTactialTurnLimitCounter
	  >> s->fInTopMessage
	  >> s->ubTopMessageType;
	if(stracLinuxFormat)
	{
		d >> skip<82>;
	}
	else
	{
		d >> skip<40>;
	}
	d >> s->usTactialTurnLimitMax;
	if(stracLinuxFormat)
	{
		d >> skip<2>;
	}
	d >> s->uiTactialTurnLimitClock
	  >> s->fTactialTurnLimitStartedBeep
	  >> s->bBoxingState
	  >> s->bConsNumTurnsNotSeen
	  >> s->ubArmyGuysKilled
	  >> s->sPanicTriggerGridNo
	  >> s->bPanicTriggerIsAlarm
	  >> s->ubPanicTolerance
	  >> s->fAtLeastOneGuyOnMultiSelect
	  >> skip<2>
	  >> s->fKilledEnemyOnAttack
	  >> s->enemy_killed_on_attack
	  >> s->bEnemyKilledOnAttackLevel
	  >> s->ubEnemyKilledOnAttackLocation
	  >> s->fItemsSeenOnAttack
	  >> s->items_seen_on_attack_soldier
	  >> skip<2>
	  >> s->usItemsSeenOnAttackGridNo
	  >> s->fLockItemLocators
	  >> s->ubLastQuoteSaid
	  >> s->ubLastQuoteProfileNUm
	  >> s->fCantGetThrough
	  >> s->sCantGetThroughGridNo
	  >> s->sCantGetThroughSoldierGridNo
	  >> s->cant_get_through
	  >> s->fDidGameJustStart
	  >> skip<1>
	  >> s->ubLastRequesterTargetID
	  >> skip<1>
	  >> s->ubNumCrowsPossible
	  >> skip<4>
	  >> s->fUnLockUIAfterHiddenInterrupt
	  >> s->bNumFoughtInBattle
	  >> skip<1>
	  >> s->uiDecayBloodLastUpdate
	  >> s->uiTimeSinceLastInTactical
	  >> s->fHasAGameBeenStarted
	  >> s->bConsNumTurnsWeHaventSeenButEnemyDoes
	  >> s->fSomeoneHit
	  >> skip<1>
	  >> s->uiTimeSinceLastOpplistDecay
	  >> s->bMercArrivingQuoteBeingUsed
	  >> s->enemy_killed_on_attack_killer
	  >> s->fCountingDownForGuideDescription
	  >> s->bGuideDescriptionCountDown
	  >> s->ubGuideDescriptionToUse
	  >> s->bGuideDescriptionSectorX
	  >> s->bGuideDescriptionSectorY
	  >> s->fEnemyFlags
	  >> s->fAutoBandagePending
	  >> s->fHasEnteredCombatModeSinceEntering
	  >> s->fDontAddNewCrows
	  >> skip<1>
	  >> s->sCreatureTenseQuoteDelay
	  >> skip<2>
	  >> s->uiCreatureTenseQuoteLastUpdate;

	if (!GameState::getInstance()->debugging())
	{
		// Prevent restoring of debug UI modes
		s->uiFlags &= ~(DEBUGCLIFFS | SHOW_Z_BUFFER);
	}
}


void InjectTacticalStatusTypeIntoFile(HWFILE const f)
{
	FileDataWriter d{316, f};
	TacticalStatusType* const s = &gTacticalStatus;
	d << s->uiFlags;
	FOR_EACH(TacticalTeamType const, t, s->Team)
	{
		d << t->bFirstID
		  << t->bLastID
		  << skip<2>
		  << t->RadarColor
		  << t->bSide
		  << t->bMenInSector
		  << t->last_merc_to_radio
		  << skip<1>
		  << t->bAwareOfOpposition
		  << t->bHuman
		  << skip<2>;
	}
	d << s->ubCurrentTeam
	  << skip<1>
	  << s->sSlideTarget
	  << skip<4>
	  << s->uiTimeSinceMercAIStart
	  << s->fPanicFlags
	  << skip<5>
	  << s->ubSpottersCalledForBy
	  << s->the_chosen_one
	  << s->uiTimeOfLastInput
	  << s->uiTimeSinceDemoOn
	  << skip<7>
	  << s->fCivGroupHostile
	  << s->ubLastBattleSectorX
	  << s->ubLastBattleSectorY
	  << s->fLastBattleWon
	  << skip<2>
	  << s->fVirginSector
	  << s->fEnemyInSector
	  << s->fInterruptOccurred
	  << s->bRealtimeSpeed
	  << skip<2>
	  << s->enemy_sighting_on_their_turn_enemy
	  << skip<1>
	  << s->fEnemySightingOnTheirTurn
	  << s->fAutoBandageMode
	  << s->ubAttackBusyCount
	  << skip<1>
	  << s->ubEngagedInConvFromActionMercID
	  << skip<1>
	  << s->usTactialTurnLimitCounter
	  << s->fInTopMessage
	  << s->ubTopMessageType
	  << skip<40>
	  << s->usTactialTurnLimitMax
	  << s->uiTactialTurnLimitClock
	  << s->fTactialTurnLimitStartedBeep
	  << s->bBoxingState
	  << s->bConsNumTurnsNotSeen
	  << s->ubArmyGuysKilled
	  << s->sPanicTriggerGridNo
	  << s->bPanicTriggerIsAlarm
	  << s->ubPanicTolerance
	  << s->fAtLeastOneGuyOnMultiSelect
	  << skip<2>
	  << s->fKilledEnemyOnAttack
	  << s->enemy_killed_on_attack
	  << s->bEnemyKilledOnAttackLevel
	  << s->ubEnemyKilledOnAttackLocation
	  << s->fItemsSeenOnAttack
	  << s->items_seen_on_attack_soldier
	  << skip<2>
	  << s->usItemsSeenOnAttackGridNo
	  << s->fLockItemLocators
	  << s->ubLastQuoteSaid
	  << s->ubLastQuoteProfileNUm
	  << s->fCantGetThrough
	  << s->sCantGetThroughGridNo
	  << s->sCantGetThroughSoldierGridNo
	  << s->cant_get_through
	  << s->fDidGameJustStart
	  << skip<1>
	  << s->ubLastRequesterTargetID
	  << skip<1>
	  << s->ubNumCrowsPossible
	  << skip<4>
	  << s->fUnLockUIAfterHiddenInterrupt
	  << s->bNumFoughtInBattle
	  << skip<1>
	  << s->uiDecayBloodLastUpdate
	  << s->uiTimeSinceLastInTactical
	  << s->fHasAGameBeenStarted
	  << s->bConsNumTurnsWeHaventSeenButEnemyDoes
	  << s->fSomeoneHit
	  << skip<1>
	  << s->uiTimeSinceLastOpplistDecay
	  << s->bMercArrivingQuoteBeingUsed
	  << s->enemy_killed_on_attack_killer
	  << s->fCountingDownForGuideDescription
	  << s->bGuideDescriptionCountDown
	  << s->ubGuideDescriptionToUse
	  << s->bGuideDescriptionSectorX
	  << s->bGuideDescriptionSectorY
	  << s->fEnemyFlags
	  << s->fAutoBandagePending
	  << s->fHasEnteredCombatModeSinceEntering
	  << s->fDontAddNewCrows
	  << skip<1>
	  << s->sCreatureTenseQuoteDelay
	  << skip<2>
	  << s->uiCreatureTenseQuoteLastUpdate;
}
