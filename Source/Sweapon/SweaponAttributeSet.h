

#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "abilitySystemComponent.h"		// 매크로 사용을 위한 필수 헤더
#include "SweaponAttributeSet.generated.h"

// GAS 전용 : Get, Set, Init 함수를 자동으로 만들어주는 매크로
#define ATTRIBUTE_ACCESSORS(ClassName, PropertyName) \
		GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName) \
		GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName) \
		GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName) \
		GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)

UCLASS()
class SWEAPON_API USweaponAttributeSet : public UAttributeSet
{
	GENERATED_BODY()
	
public:
	USweaponAttributeSet();

	// 스탯이 변하기 직전/직후에 호출되는 엔진 함수 오버라이드
	virtual void PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue) override;
	virtual void PostGameplayEffectExecute(const struct FGameplayEffectModCallbackData& Data) override;

	// 스탯

	// 체력
	UPROPERTY(BlueprintReadOnly, Category = "Attributes")
	FGameplayAttributeData Health;
	ATTRIBUTE_ACCESSORS(USweaponAttributeSet, Health)

	UPROPERTY(BlueprintReadOnly, Category = "Attributes")
	FGameplayAttributeData MaxHealth;
	ATTRIBUTE_ACCESSORS(USweaponAttributeSet, MaxHealth)

	// 마나 (마법 무기용)
	UPROPERTY(BlueprintReadOnly, Category = "Attributes")
	FGameplayAttributeData Mana;
	ATTRIBUTE_ACCESSORS(USweaponAttributeSet, Mana)

	UPROPERTY(BlueprintReadOnly, Category = "Attributes")
	FGameplayAttributeData MaxMana;
	ATTRIBUTE_ACCESSORS(USweaponAttributeSet, MaxMana)

	// 스태미나 (검 무기용)
	UPROPERTY(BlueprintReadOnly, Category = "Attributes")
	FGameplayAttributeData Stamina;
	ATTRIBUTE_ACCESSORS(USweaponAttributeSet, Stamina)

	UPROPERTY(BlueprintReadOnly, Category = "Attributes")
	FGameplayAttributeData MaxStamina;
	ATTRIBUTE_ACCESSORS(USweaponAttributeSet, MaxStamina)

	// 기본 전투력 수치
	UPROPERTY(BlueprintReadOnly, Category = "Attributes")
	FGameplayAttributeData AttackPower;
	ATTRIBUTE_ACCESSORS(USweaponAttributeSet, AttackPower)

	UPROPERTY(BlueprintReadOnly, Category = "Attributes")
	FGameplayAttributeData DefensePower;
	ATTRIBUTE_ACCESSORS(USweaponAttributeSet, DefensePower)
};
