
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AbilitySystemInterface.h"
#include "InputActionValue.h"
#include "GameplayTagContainer.h"
#include "SweaponCharacterBase.generated.h"

class UAbilitySystemComponent;
class USpringArmComponent;
class UCameraComponent;
class UInputMappingContext;
class UInputAction;
class USweaponAttributeSet;
class USweaponGameplayAbilityBase;

UCLASS()
class SWEAPON_API ASweaponCharacterBase : public ACharacter, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ASweaponCharacterBase();

	// ASC 반환 인터페이스 오버라이드
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// 컨트롤러가 이 캐릭터에 빙의할 때 호출되는 함수 오버라이드
	virtual void PossessedBy(AController* NewController) override;

	// 어빌리티 시스템 제어 컴포넌트
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Abilities")
	UAbilitySystemComponent* AbilitySystemComponent;

	// 스피링 암 컴포넌트
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
	USpringArmComponent* CameraBoom;

	// 캐릭터를 따라다니는 메인 카메라 컴포넌트
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
	UCameraComponent* FollowCamera;

	// 블루프린트에서 매핑 컨텍스트(IMC)를 지정할 변수
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input", meta = (AllowPrivateAccess = "true"))
	UInputMappingContext* DefaultMappingContext;

	// 블루프린트에서 점프, 이동, 시점 액션(IA)을 지정할 변수
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input", meta = (AllowPrivateAccess = "true"))
	UInputAction* MoveAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input", meta = (AllowPrivateAccess = "true"))
	UInputAction* LookAction;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Abilities")
	USweaponAttributeSet* AttributeSet;

	// 임시 무기 외형 컴포넌트
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapon")
	UStaticMeshComponent* WeaponMesh;

	// 캐릭터가 기본적으로 가지게 될 어빌리티 목록 (에디터에서 지정)
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Abilities")
	TArray<TSubclassOf<USweaponGameplayAbilityBase>> DefaultAbilities;

	// 입력 액션 (마우스 좌클릭용)
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input", meta = (AllowPrivateAccess = "true"))
	UInputAction* AttackAction;
	
	// 실제 이동과 시점 회전을 처리할 함수 선언
	void Move(const FInputActionValue& Value);
	void Look(const FInputActionValue& Value);

	// 기본 무기 장착 및 태그 부여
	void EquipDefaultWeapon();

	// 입력이 들어왔을 때 ASC에 신호를 보낼 함수
	void InputAttackPressed();

	// 기본 어빌리티들을 ASC에 등록하는 함수
	void GiveDefaultAbilities();

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
