#include "Animations/PS_AnimNotifyStateCanPlayNextCombo.h"

#include "Characters/PS_Character.h"

void UPS_AnimNotifyStateCanPlayNextCombo::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);
	if (APS_Character* APSCharacter = MeshComp ? Cast<APS_Character>(MeshComp->GetOwner()) : nullptr)
	{
		APSCharacter->bCanChangeCombo = true;
	}
}

void UPS_AnimNotifyStateCanPlayNextCombo::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyEnd(MeshComp, Animation, EventReference);
	if (APS_Character* APSCharacter = MeshComp ? Cast<APS_Character>(MeshComp->GetOwner()) : nullptr)
	{
		APSCharacter->bCanChangeCombo = false;
	}
}
