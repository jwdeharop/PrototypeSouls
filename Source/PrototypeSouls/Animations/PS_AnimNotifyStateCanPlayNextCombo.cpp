#include "Animations/PS_AnimNotifyStateCanPlayNextCombo.h"
#include "Characters/Player/PS_PlayerCharacter.h"

void UPS_AnimNotifyStateCanPlayNextCombo::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);
	if (APS_PlayerCharacter* APSCharacter = MeshComp ? Cast<APS_PlayerCharacter>(MeshComp->GetOwner()) : nullptr)
	{
		APSCharacter->bCanChangeCombo = true;
	}
}

void UPS_AnimNotifyStateCanPlayNextCombo::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyEnd(MeshComp, Animation, EventReference);
	if (APS_PlayerCharacter* APSCharacter = MeshComp ? Cast<APS_PlayerCharacter>(MeshComp->GetOwner()) : nullptr)
	{
		APSCharacter->bCanChangeCombo = false;
	}
}
