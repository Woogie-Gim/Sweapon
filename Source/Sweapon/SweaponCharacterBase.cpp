
#include "SweaponCharacterBase.h"
#include "AbilitySystemComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "GameFramework/Controller.h"
#include "SweaponAttributeSet.h"
#include "SweaponGameplayAbilityBase.h"

// Sets default values
ASweaponCharacterBase::ASweaponCharacterBase()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// ASC 컴포넌트 생성 및 부착
	AbilitySystemComponent = CreateDefaultSubobject<UAbilitySystemComponent>(TEXT("AbilitySystemComponent"));

	// Attribute Set 생성 및 캐릭터 부착
	AttributeSet = CreateDefaultSubobject<USweaponAttributeSet>(TEXT("AttributeSet"));

	// 무기 메시 컴포넌트 생성 및 소켓 부착
	WeaponMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("WeaponMesh"));
	WeaponMesh->SetupAttachment(GetMesh(), TEXT("WeaponSocket"));
	WeaponMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision); // 무기 자체의 물리 충돌 방지

	// 스프링 암 생성 및 부착
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	// 스프링 암을 캐릭터의 최상위 컴포넌트(캡슐)에 부착
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 400.0f; // 카메라와의 기본 거리를 400으로 설정
	CameraBoom->bUsePawnControlRotation = true; // 마우스 움직임에 따라 스프링 암이 회전하도록 허용
	CameraBoom->SocketOffset = FVector(0.0f, 0.0f, 50.0f);

	// 카메라 생성 및 부착
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	// 카메라를 스프링 암의 끝단(소켓)에 부착
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	FollowCamera->bUsePawnControlRotation = false; // 카메라는 스프링 암을 따라가기만 하면 되므로 자체 회전 x
}

// Called when the game starts or when spawned
void ASweaponCharacterBase::BeginPlay()
{
	Super::BeginPlay();

	// 게임이 시작될 때 컨트롤러에 IMC(매핑 컨텍스트)를 등록
	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}
	
}

// Called every frame
void ASweaponCharacterBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ASweaponCharacterBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// InputComponent를 향상된 입력 컴포넌트로 캐스팅하여 IA를 바인딩
	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent))
	{
		// MoveAction이 실행될 때(Triggered) Move 함수 호출
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ASweaponCharacterBase::Move);

		// LookAction이 실행될 때(Triggered) Look 함수 호출
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &ASweaponCharacterBase::Look);
		
		// 공격 입력이 시작될 때(Started) 실행할 함수 연결
		EnhancedInputComponent->BindAction(AttackAction, ETriggerEvent::Started, this, &ASweaponCharacterBase::InputAttackPressed);
	}

}

void ASweaponCharacterBase::Move(const FInputActionValue& Value)
{
	// 입력값(W,A,S,D)은 2차원 벡터(X, Y)로 들어옴
	FVector2D MovementVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// 카메라가 바라보는 방향을 기준으로 앞(Forward)과 옆(Right)을 계산
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		// 캐릭터에게 이동 명령
		AddMovementInput(ForwardDirection, MovementVector.Y);
		AddMovementInput(RightDirection, MovementVector.X);
	}
}

void ASweaponCharacterBase::Look(const FInputActionValue& Value)
{
	// 마우스 이동값도 2차원 벡터로 들어옴
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// 컨트롤러의 좌우 회전(Yaw)과 상하 회전(Pitch)에 마우스 이동값을 더함
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(LookAxisVector.Y);
	}
}

UAbilitySystemComponent* ASweaponCharacterBase::GetAbilitySystemComponent() const
{
	// 현재 캐릭터의 ASC 반환
	return AbilitySystemComponent;
}

void ASweaponCharacterBase::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	// 플레이어 컨트롤러가 캐릭터에 Possess된 직후에 GAS를 초기화
	if (AbilitySystemComponent)
	{
		AbilitySystemComponent->InitAbilityActorInfo(this, this);

		// 컨트롤러 빙의 및 GAS 초기화 완료 후 어빌리티 부여
		GiveDefaultAbilities();
	}
}

void ASweaponCharacterBase::EquipDefaultWeapon()
{
	if (AbilitySystemComponent)
	{
		// Weapon.Sword 태그 검색 및 ASC에 직접 부여
		FGameplayTag SwordTag = FGameplayTag::RequestGameplayTag(FName("Weapon.Sword"));
		AbilitySystemComponent->AddLooseGameplayTag(SwordTag);
	}
}

void ASweaponCharacterBase::GiveDefaultAbilities()
{
	// 서버 권한이 있거나 싱글플레이 환경일 때 어빌리티를 부여
	if (HasAuthority() && AbilitySystemComponent)
	{
		for (TSubclassOf<USweaponGameplayAbilityBase>& AbilityClass : DefaultAbilities)
		{
			if (AbilityClass)
			{
				// 에디터에서 설정한 InputID를 가져와서 스펙을 만듦
				int32 SelectedInputID = AbilityClass.GetDefaultObject()->InputID;

				FGameplayAbilitySpec Spec(AbilityClass, 1, SelectedInputID);
				AbilitySystemComponent->GiveAbility(Spec);
			}
		}
	}
}

void ASweaponCharacterBase::InputAttackPressed()
{
	if (AbilitySystemComponent)
	{
		int32 AttackInputID = 1;

		TArray<FGameplayAbilitySpec> ActivatableAbilities = AbilitySystemComponent->GetActivatableAbilities();

		for (const FGameplayAbilitySpec& Spec : ActivatableAbilities)
		{
			if (Spec.InputID == AttackInputID)
			{
				AbilitySystemComponent->TryActivateAbility(Spec.Handle);
			}
		}
	}
}