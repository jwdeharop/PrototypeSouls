#include "Animations/PS_AnimNotifyCanStop.h"
#include "Animations/PS_CharacterAnimInstance.h"
#include "Characters/PS_PlayerCharacter.h"

void UPS_AnimNotifyCanStop::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	if (const UPS_CharacterAnimInstance* AnimInstance = MeshComp ? Cast<UPS_CharacterAnimInstance>(MeshComp->GetAnimInstance()) : nullptr)
	{
		APS_PlayerCharacter* APSCharacter = MeshComp ? CastChecked<APS_PlayerCharacter>(MeshComp->GetOwner()) : nullptr;
		AnimInstance->AnimationCanStop.Broadcast(APSCharacter);
	}
}
