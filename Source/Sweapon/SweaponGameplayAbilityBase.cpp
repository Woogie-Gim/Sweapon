

#include "SweaponGameplayAbilityBase.h"

USweaponGameplayAbilityBase::USweaponGameplayAbilityBase()
{
	// 기본적으로 인스턴싱 정책을 InstancedPerActor로 설정 (각 액터마다 어빌리티 인스턴스 관리)
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
	InputID = 0;
}

void USweaponGameplayAbilityBase::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	// 블루프린트에 등록된 Cost GE를 엔진에게 정식으로 반영하라고 명령
	if (CommitAbilityCost(Handle, ActorInfo, ActivationInfo))
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
	}
}

void USweaponGameplayAbilityBase::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}