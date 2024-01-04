#include "Animations/PS_AnimNotifyCanStop.h"
#include "Animations/PS_CharacterAnimInstance.h"
#include "Characters/PS_Character.h"

void UPS_AnimNotifyCanStop::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	if (const UPS_CharacterAnimInstance* AnimInstance = MeshComp ? Cast<UPS_CharacterAnimInstance>(MeshComp->GetAnimInstance()) : nullptr)
	{
		APS_Character* APSCharacter = MeshComp ? CastChecked<APS_Character>(MeshComp->GetOwner()) : nullptr;
		AnimInstance->AnimationCanStop.Broadcast(APSCharacter);
	}
}
