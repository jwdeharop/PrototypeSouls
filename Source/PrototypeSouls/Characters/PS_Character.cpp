#include "Characters/PS_Character.h"
#include "AbilitySystemComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/PS_AbilitySystemComponent.h"
#include "Components/PS_MovementComponent.h"
#include "Controllers/PS_PlayerController.h"
#include "DataAssets/PS_InputConfig.h"
#include "DataAssets/PS_WeaponComboConfig.h"
#include "Engine/LocalPlayer.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "GAS/Abilities/PS_GameplayAbility.h"
#include "GAS/AttributeSets/PS_PlayerAttributeSet.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"
#include "InputActionValue.h"
#include "Libraries/PS_NetLibrary.h"
#include "PS_PlayerState.h"
#include "UnrealNetwork.h"
#include "Weapons/PS_Weapon.h"

APS_Character::APS_Character(const class FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer.SetDefaultSubobjectClass<UPS_MovementComponent>(ACharacter::CharacterMovementComponentName))
{
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);
		
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	GetCharacterMovement()->bOrientRotationToMovement = false;	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f);

	GetCharacterMovement()->JumpZVelocity = 700.f;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MaxWalkSpeed = 500.f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;
	GetCharacterMovement()->BrakingDecelerationFalling = 1500.0f;

	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 400.0f;	
	CameraBoom->bUsePawnControlRotation = true;

	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	FollowCamera->bUsePawnControlRotation = false;
}

UAbilitySystemComponent* APS_Character::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

UPS_PlayerAttributeSet* APS_Character::GetPlayerAttributeSet() const
{
	return PlayerAttributeSet;
}

bool APS_Character::IsDodging() const
{
	return AbilitySystemComponent ? AbilitySystemComponent->HasMatchingGameplayTag(FGameplayTag::RequestGameplayTag("Ability.Dodge")) : false;
}

bool APS_Character::IsMoving() const
{
	return !AuxMovementVector.IsNearlyZero();
}

APS_Weapon* APS_Character::GetCurrentWeapon() const
{
	return CurrentWeapon.IsValid() ? CurrentWeapon.Get() : nullptr;
}

float APS_Character::GetMaxSpeed() const
{
	return PlayerAttributeSet ? PlayerAttributeSet->GetMaxWalkSpeed() : 0.f;
}

FPS_ComboWeaponInfo APS_Character::GetCurrentWeaponComboInfo() const
{
	APS_Weapon* Weapon = GetCurrentWeapon();
	const FPS_ComboWeaponInfo* WeaponInfo = WeaponComboConfig ? WeaponComboConfig->WeaponConfig.FindByPredicate([&Weapon](const FPS_ComboWeaponInfo& ComboConfig)
	{
		return Weapon->GetType() == ComboConfig.Type;
	}) : nullptr;

	return WeaponInfo ? *WeaponInfo : FPS_ComboWeaponInfo();
}

void APS_Character::BeginPlay()
{
	Super::BeginPlay();

	const APS_PlayerState* APSPlayerState = GetPlayerState<APS_PlayerState>();
	if (APSPlayerState)
	{
		OnRep_PlayerState();
	}
	else
	{
		if (APS_PlayerController* APSPlayerController = GetController<APS_PlayerController>())
		{
			APSPlayerController->OnControllerGetsPlayerState.AddUObject(this, &ThisClass::OnControllerGetsPlayerState);
		}
	}
	bUseControllerRotationYaw = false;

	if (UPS_NetLibrary::IsServer(this))
	{
		// In case InitialWeapon is set, we will spawn it. This spawn will occur only on server side.
		if (InitialWeapon)
		{
			const TArray<FName>& Sockets = InitialWeapon->GetDefaultObject<APS_Weapon>()->GetWeaponSockets();
			for (const FName Socket : Sockets)
			{
				if (APS_Weapon* SpawnedWeapon = GetWorld()->SpawnActor<APS_Weapon>(InitialWeapon))
				{
					CurrentWeapon = SpawnedWeapon;
					CurrentWeapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, Socket);
				}
			}
		}
	}

	const APlayerController* PlayerController = Cast<APlayerController>(Controller);
	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = PlayerController ? ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()) : nullptr)
	{
		Subsystem->AddMappingContext(DefaultMappingContext, 0);
	}
}

void APS_Character::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	APS_PlayerState* APSPlayerState = GetPlayerState<APS_PlayerState>();
	UPS_AbilitySystemComponent* UPSAbilitySystemComponent = APSPlayerState ? CastChecked<UPS_AbilitySystemComponent>(APSPlayerState->GetAbilitySystemComponent()) : nullptr;
	if (!UPSAbilitySystemComponent)
		return;

	AbilitySystemComponent = UPSAbilitySystemComponent;
	AbilitySystemComponent->InitAbilityActorInfo(APSPlayerState, this);
	PlayerAttributeSet = APSPlayerState->GetPlayerAttributeSet();
	InitializeAttributes();
	AddCharacterAbilities();
}

void APS_Character::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME_CONDITION_NOTIFY(APS_Character, AuxMovementVector, COND_None, REPNOTIFY_OnChanged);
	DOREPLIFETIME_CONDITION_NOTIFY(APS_Character, CurrentWeapon, COND_OwnerOnly, REPNOTIFY_OnChanged);
}

void APS_Character::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();
	APS_PlayerState* APSPlayerState = GetPlayerState<APS_PlayerState>();
	UPS_AbilitySystemComponent* UPSAbilitySystemComponent = APSPlayerState ? CastChecked<UPS_AbilitySystemComponent>(APSPlayerState->GetAbilitySystemComponent()) : nullptr;
	if (!UPSAbilitySystemComponent)
		return;

	AbilitySystemComponent = UPSAbilitySystemComponent;
	AbilitySystemComponent->InitAbilityActorInfo(APSPlayerState, this);
	PlayerAttributeSet = APSPlayerState->GetPlayerAttributeSet();
}

void APS_Character::Server_SetAuxMovementVector_Implementation(const FVector2D& MovementVector)
{
	AuxMovementVector = MovementVector;
	AuxMovementVector.Normalize();
}

void APS_Character::AddCharacterAbilities()
{
	if (!UPS_NetLibrary::IsServer(this) || !AbilitySystemComponent || AbilitySystemComponent->bAbilitiesGranted)
		return;

	for (TSubclassOf<UPS_GameplayAbility> Ability : Abilities)
	{
		FGameplayAbilitySpec AbilitySpec = FGameplayAbilitySpec(Ability, 1, static_cast<int32>(Ability.GetDefaultObject()->InputID), this);
		const FGameplayTag& InputTag = Ability.GetDefaultObject()->InputTag;
		if (InputTag.IsValid())
		{
			AbilitySpec.DynamicAbilityTags.AddTag(InputTag);
		}
		AbilitySystemComponent->GiveAbility(AbilitySpec);
	}

	AbilitySystemComponent->bAbilitiesGranted = true;
}

void APS_Character::OnAbilityInputPressed(FGameplayTag GameplayTag)
{
	if (AbilitySystemComponent)
	{
		AbilitySystemComponent->AbilityInputTagPressed(GameplayTag);
	}
}

void APS_Character::OnAbilityInputReleased(FGameplayTag GameplayTag)
{
	if (AbilitySystemComponent)
	{
		AbilitySystemComponent->AbilityInputTagReleased(GameplayTag);
	}
}

void APS_Character::OnCurrentSpeedChanged(const FOnAttributeChangeData& OnAttributeChangeData) const
{
	if (UCharacterMovementComponent* CharacterMovementComponent = GetCharacterMovement())
	{
		CharacterMovementComponent->MaxWalkSpeed = OnAttributeChangeData.NewValue;
	}
}

void APS_Character::InitializeAttributes() const
{
	if (!AbilitySystemComponent || !DefaultAttributes)
		return;

	FGameplayEffectContextHandle EffectContext = AbilitySystemComponent->MakeEffectContext();
	EffectContext.AddSourceObject(this);

	const FGameplayEffectSpecHandle NewHandle = AbilitySystemComponent->MakeOutgoingSpec(DefaultAttributes, 1, EffectContext);
	AbilitySystemComponent->ApplyGameplayEffectSpecToTarget(*NewHandle.Data.Get(), AbilitySystemComponent);
}

void APS_Character::OnControllerGetsPlayerState(APS_PlayerState* APSPlayerState)
{
	OnRep_PlayerState();
}

void APS_Character::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent)) {
		
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &ACharacter::Jump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &APS_Character::Move);
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Completed, this, &APS_Character::StopMoving);
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &APS_Character::Look);

		if (InputConfigDataAsset)
		{
			for (const FPS_InputConfig& InputConfig : InputConfigDataAsset->InputConfig)
			{
				EnhancedInputComponent->BindAction(InputConfig.InputAction, ETriggerEvent::Triggered, this, &ThisClass::OnAbilityInputPressed, InputConfig.AbilityTag);
				EnhancedInputComponent->BindAction(InputConfig.InputAction, ETriggerEvent::Completed, this, &ThisClass::OnAbilityInputReleased, InputConfig.AbilityTag);
			}
		}
	}
}

void APS_Character::Move(const FInputActionValue& Value)
{
	if (!Controller || IsDodging())
		return;

	bUseControllerRotationYaw = true;
	const FVector2D MovementVector = Value.Get<FVector2D>();
	const FRotator Rotation = Controller->GetControlRotation();
	const FRotator YawRotation(0, Rotation.Yaw, 0);
	const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

	Server_SetAuxMovementVector(MovementVector);

	AddMovementInput(ForwardDirection, MovementVector.Y);
	AddMovementInput(RightDirection, MovementVector.X);
}

void APS_Character::Look(const FInputActionValue& Value)
{
	if (!Controller)
		return;

	const FVector2D LookAxisVector = Value.Get<FVector2D>();

	AddControllerYawInput(LookAxisVector.X);
	AddControllerPitchInput(LookAxisVector.Y);
}

void APS_Character::StopMoving()
{
	Server_SetAuxMovementVector(FVector2D::ZeroVector);
	bUseControllerRotationYaw = false;
}

