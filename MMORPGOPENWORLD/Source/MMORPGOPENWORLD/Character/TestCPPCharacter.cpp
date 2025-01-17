#include "TestCPPCharacter.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"
#include "EnhancedInputComponent.h"
#include "../Skill/SkillDataListAsset.h"
#include "EnhancedInputSubsystems.h"
#include "ABCharacterStatComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include <Net/UnrealNetwork.h>


//////////////////////////////////////////////////////////////////////////
// ATestCPPCharacter

ATestCPPCharacter::ATestCPPCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f); // ...at this rotation rate

	// Note: For faster iteration times these variables, and many more, can be tweaked in the Character Blueprint
	// instead of recompiling to adjust them
	GetCharacterMovement()->JumpZVelocity = 800.f;
	GetCharacterMovement()->AirControl = 0.1f;
	GetCharacterMovement()->MaxWalkSpeed = 1000.f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 100.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;

	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 1500.0f;
	CameraBoom->bUsePawnControlRotation = true;

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	FollowCamera->bUsePawnControlRotation = false;

	CharacterStat = CreateDefaultSubobject<UABCharacterStatComponent>(TEXT("CharacterStat"));
}

void ATestCPPCharacter::BeginPlay()
{
	// Call the base class  
	Super::BeginPlay();

	//Add Input Mapping Context
	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}

	if (CharacterStat)
	{
		CharacterStat->SetHp(CharacterStat->GetMaxHp());
	}

	AssignRandomSkill();
}

void ATestCPPCharacter::Fire()
{
	
}

void ATestCPPCharacter::Fire1()
{
	if (Skill1 == nullptr)
		return;

	if (HasAuthority()) // 서버에서 호출일 때
	{
		Skill1->SetFire(this, GetWorld());
	}
	else
	{
		ServerFire1(); // 서버에 요청
	}
}

void ATestCPPCharacter::Fire2()
{
	if (Skill2 == nullptr)
		return;

	if (HasAuthority()) // 서버에서 호출일 때
	{
		Skill2->SetFire(this, GetWorld());
	}
	else
	{
		ServerFire2(); // 서버에 요청
	}
}

void ATestCPPCharacter::Fire3()
{
	if (Skill3 == nullptr)
		return;

	if (HasAuthority()) // 서버에서 호출일 때
	{
		Skill3->SetFire(this, GetWorld());
	}
	else
	{
		ServerFire3(); // 서버에 요청
	}
}

void ATestCPPCharacter::Fire4()
{
	if (Skill4 == nullptr)
		return;

	if (HasAuthority()) // 서버에서 호출일 때
	{
		Skill4->SetFire(this, GetWorld());
	}
	else
	{
		ServerFire4(); // 서버에 요청
	}
}

void ATestCPPCharacter::ServerFire1_Implementation()
{
	Skill1->SetFire(this, GetWorld()); // 서버에서 호출
}

bool ATestCPPCharacter::ServerFire1_Validate()
{
	return true; // 검증 로직 추가 가능
}

void ATestCPPCharacter::ServerFire2_Implementation()
{
	Skill2->SetFire(this, GetWorld()); // 서버에서 호출
}

bool ATestCPPCharacter::ServerFire2_Validate()
{
	return true; // 검증 로직 추가 가능
}

void ATestCPPCharacter::ServerFire3_Implementation()
{
	Skill3->SetFire(this, GetWorld()); // 서버에서 호출
}

bool ATestCPPCharacter::ServerFire3_Validate()
{
	return true; // 검증 로직 추가 가능
}

void ATestCPPCharacter::ServerFire4_Implementation()
{
	Skill4->SetFire(this, GetWorld()); // 서버에서 호출
}

bool ATestCPPCharacter::ServerFire4_Validate()
{
	return true; // 검증 로직 추가 가능
}

// Input

void ATestCPPCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent)) {

		//Jumping
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Triggered, this, &ACharacter::Jump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);

		//Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ATestCPPCharacter::Move);

		//Looking
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &ATestCPPCharacter::Look);

		EnhancedInputComponent->BindAction(FireAction, ETriggerEvent::Completed, this, &ATestCPPCharacter::Fire1);
		EnhancedInputComponent->BindAction(FireAction1, ETriggerEvent::Completed, this, &ATestCPPCharacter::Fire2);
		EnhancedInputComponent->BindAction(FireAction2, ETriggerEvent::Completed, this, &ATestCPPCharacter::Fire3);
		EnhancedInputComponent->BindAction(FireAction3, ETriggerEvent::Completed, this, &ATestCPPCharacter::Fire4);
	}

}

void ATestCPPCharacter::Move(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D MovementVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);

		// get right vector 
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		// add movement 
		AddMovementInput(ForwardDirection, MovementVector.Y);
		AddMovementInput(RightDirection, MovementVector.X);
	}
}

void ATestCPPCharacter::Look(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// add yaw and pitch input to controller
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(LookAxisVector.Y);
	}
}

void ATestCPPCharacter::AssignRandomSkill()
{
	// 스킬 리스트의 개수를 가져옵니다.
	int32 SkillCount = SkillList.Num();

	if (SkillCount > 0)
	{
		// 랜덤 인덱스를 생성합니다.
		int32 RandomIndex = UKismetMathLibrary::RandomIntegerInRange(0, SkillCount - 1);
		// 랜덤으로 선택한 스킬을 할당합니다.
		Skill4 = SkillList[RandomIndex];
		Skill3 = SkillList[(RandomIndex + 1) % (SkillCount - 1)];
		Skill2 = SkillList[(RandomIndex + 2) % (SkillCount - 1)];
		Skill1 = SkillList[(RandomIndex + 3) % (SkillCount - 1)];
	}


}

void ATestCPPCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	// SkillList 복제 추가
	DOREPLIFETIME(ATestCPPCharacter, SkillList);
	DOREPLIFETIME(ATestCPPCharacter, Skill);
	DOREPLIFETIME(ATestCPPCharacter, Skill1);
	DOREPLIFETIME(ATestCPPCharacter, Skill2);
	DOREPLIFETIME(ATestCPPCharacter, Skill3);
	DOREPLIFETIME(ATestCPPCharacter, Skill4);
}