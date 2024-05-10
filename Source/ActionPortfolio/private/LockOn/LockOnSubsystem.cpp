// Fill out your copyright notice in the Description page of Project Settings.


#include "LockOn/LockOnSubsystem.h"
#include "LockOn/LockOnTargetComponent.h"

ULockOnSubsystem::ULockOnSubsystem()
{
}

void ULockOnSubsystem::RegisterLockOnTarget(ULockOnTargetComponent* InTarget)
{
	check(InTarget);
	Targets.Add(InTarget);
}

void ULockOnSubsystem::UnregisterLockOnTarget(ULockOnTargetComponent* InTarget)
{
	check(InTarget);
	Targets.Remove(InTarget);
	
}

bool ULockOnSubsystem::GetLockOnAbleTargets(TArray<ULockOnTargetComponent*>& OutArray, const TDelegate<bool(ULockOnTargetComponent*)>& InDel)
{
	bool bResult = false;

	for (auto& Target : Targets)
	{
		ULockOnTargetComponent* TargetCom = Target.Get();
		if (InDel.Execute(TargetCom))
		{
			bResult = true;
			OutArray.Add(TargetCom);
		}
	}

	return bResult;
}
