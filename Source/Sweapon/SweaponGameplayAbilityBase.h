
#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "SweaponGameplayAbilityBase.generated.h"

/**
 * 
 */
UCLASS()
class SWEAPON_API USweaponGameplayAbilityBase : public UGameplayAbility
{
	GENERATED_BODY()
	
	public:
	USweaponGameplayAbilityBase();

	// 어빌리티에 매핑할 입력 ID (Enhanced Input과 연동할 때 사용)
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	int32 InputID;

protected:
	// 어빌리티가 발동될 때 호출되는 핵심 함수
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

	// 어빌리티가 종료될 때 호출되는 함수
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;
	
};
