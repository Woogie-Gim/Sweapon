
#include "SweaponAttributeSet.h"
#include "GameplayEffectExtension.h" // 데이터 구조체(FGameplayEffectModCallbackData) 사용을 위함

USweaponAttributeSet::USweaponAttributeSet()
{
	// 매크로가 만들어준 Init 함수들로 스탯 초기값 설정
	InitHealth(100.0f);
	InitMaxHealth(100.0f);
	InitMana(50.0f);
	InitMaxMana(50.0f);
	InitStamina(100.0f);
	InitMaxStamina(100.0f);
	InitAttackPower(10.0f);
	InitDefensePower(5.0f);
}

void USweaponAttributeSet::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{
	Super::PreAttributeChange(Attribute, NewValue);

	// 값이 변하기 직전에 최대치/최소치 제한 (Clamp)
	if (Attribute == GetHealthAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.0f, GetMaxHealth());
	}
	else if (Attribute == GetManaAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.0f, GetMaxMana());
	}
	else if (Attribute == GetStaminaAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.0f, GetMaxStamina());
	}
}

void USweaponAttributeSet::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);

	// 데미지나 포션 등 이펙트 적용이 끝난 직후 한 번 더 제한 확인
	if (Data.EvaluatedData.Attribute == GetHealthAttribute())
	{
		SetHealth(FMath::Clamp(GetHealth(), 0.0f, GetMaxHealth()));
	}
}