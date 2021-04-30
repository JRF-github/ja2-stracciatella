#include "Debug.h"
#include "FileMan.h"
#include "LoadSaveData.h"
#include "LoadSaveObjectType.h"
#include "LoadSaveRealObject.h"
#include "Overhead.h"


static DataReader & operator>>(DataReader & d, vector_3 & v)
{
	d >> v.x >> v.y >> v.z;
	return d;
}


static DataWriter & operator<<(DataWriter & d, vector_3 const& v)
{
	d << v.x << v.y << v.z;
	return d;
}


void ExtractRealObjectFromFile(HWFILE const file, REAL_OBJECT* const o)
{
	FileDataReader d{256, file};
	d >> o->fAllocated
	  >> o->fAlive
	  >> o->fApplyFriction
	  >> skip<2>
	  >> o->fVisible
	  >> o->fInWater
	  >> o->fTestObject
	  >> o->fTestEndedWithCollision
	  >> o->fTestPositionNotSet
	  >> skip<2>
	  >> o->TestZTarget
	  >> o->OneOverMass
	  >> o->AppliedMu
	  >> o->Position
	  >> o->TestTargetPosition
	  >> o->OldPosition
	  >> o->Velocity
	  >> o->OldVelocity
	  >> o->InitialForce
	  >> o->Force
	  >> o->CollisionNormal
	  >> o->CollisionVelocity
	  >> o->CollisionElasticity
	  >> o->sGridNo
	  >> skip<6>
	  >> o->pNode
	  >> o->pShadow
	  >> o->sConsecutiveCollisions
	  >> o->sConsecutiveZeroVelocityCollisions
	  >> o->iOldCollisionCode
	  >> o->dLifeLength
	  >> o->dLifeSpan;
	ExtractObject(d, &o->Obj);
	d >> o->fFirstTimeMoved
	  >> skip<1>
	  >> o->sFirstGridNo
	  >> o->owner
	  >> o->ubActionCode
	  >> skip<2>
	  >> o->target
	  >> skip<3>
	  >> o->fDropItem
	  >> skip<3>
	  >> o->uiNumTilesMoved
	  >> o->fCatchGood
	  >> o->fAttemptedCatch
	  >> o->fCatchAnimOn
	  >> o->fCatchCheckDone
	  >> o->fEndedWithCollisionPositionSet
	  >> skip<3>
	  >> o->EndedWithCollisionPosition
	  >> o->fHaveHitGround
	  >> o->fPotentialForDebug
	  >> o->sLevelNodeGridNo
	  >> o->uiSoundID
	  >> o->ubLastTargetTakenDamage
	  >> skip<3>;
}


void InjectRealObjectIntoFile(HWFILE const file, REAL_OBJECT const* const o)
{
	FileDataWriter d{256, file};
	d << o->fAllocated
	  << o->fAlive
	  << o->fApplyFriction
	  << skip<2>
	  << o->fVisible
	  << o->fInWater
	  << o->fTestObject
	  << o->fTestEndedWithCollision
	  << o->fTestPositionNotSet
	  << skip<2>
	  << o->TestZTarget
	  << o->OneOverMass
	  << o->AppliedMu
	  << o->Position
	  << o->TestTargetPosition
	  << o->OldPosition
	  << o->Velocity
	  << o->OldVelocity
	  << o->InitialForce
	  << o->Force
	  << o->CollisionNormal
	  << o->CollisionVelocity
	  << o->CollisionElasticity
	  << o->sGridNo
	  << skip<6>
	  << o->pNode
	  << o->pShadow
	  << o->sConsecutiveCollisions
	  << o->sConsecutiveZeroVelocityCollisions
	  << o->iOldCollisionCode
	  << o->dLifeLength
	  << o->dLifeSpan;
	InjectObject(d, &o->Obj);
	d << o->fFirstTimeMoved
	  << 1
	  << o->sFirstGridNo
	  << Soldier2ID(o->owner)
	  << o->ubActionCode
	  << skip<2>
	  << Soldier2ID(o->target)
	  << skip<3>
	  << o->fDropItem
	  << skip<3>
	  << o->uiNumTilesMoved
	  << o->fCatchGood
	  << o->fAttemptedCatch
	  << o->fCatchAnimOn
	  << o->fCatchCheckDone
	  << o->fEndedWithCollisionPositionSet
	  << skip<3>
	  << o->EndedWithCollisionPosition
	  << o->fHaveHitGround
	  << o->fPotentialForDebug
	  << o->sLevelNodeGridNo
	  << o->uiSoundID
	  << o->ubLastTargetTakenDamage
	  << skip<3>;
}
