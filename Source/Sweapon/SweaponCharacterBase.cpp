
#include "SweaponCharacterBase.h"
#include "AbilitySystemComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"

// Sets default values
ASweaponCharacterBase::ASweaponCharacterBase()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// ASC 컴포넌트 생성 및 부착
	AbilitySystemComponent = CreateDefaultSubobject<UAbilitySystemComponent>(TEXT("AbilitySystemComponent"));

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

UAbilitySystemComponent* ASweaponCharacterBase::GetAbilitySystemComponent() const
{
	// 현재 캐릭터의 ASC 반환
	return AbilitySystemComponent;
}

// Called when the game starts or when spawned
void ASweaponCharacterBase::BeginPlay()
{
	Super::BeginPlay();
	
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

}

