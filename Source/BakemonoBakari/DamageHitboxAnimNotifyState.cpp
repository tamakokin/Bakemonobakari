// Fill out your copyright notice in the Description page of Project Settings.
//�v���C���[�U����hitbox�𐶐�
//�X�V����
//2021/05/10 �쐬�ҁF��
//

#include "DamageHitboxAnimNotifyState.h"

void UDamageHitboxAnimNotifyState::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration);
}

void UDamageHitboxAnimNotifyState::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::NotifyEnd(MeshComp, Animation);
}

void UDamageHitboxAnimNotifyState::NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration)
{
	Super::NotifyTick(MeshComp, Animation, TotalDuration);
}

//bool UDamageHitboxAnimNotifyState::CanBePlaced(UAnimSequenceBase* Animation)const
//{
	//if (Animation)
	//{
		//if (Animation->IsA(UAnimSequence::StaticClass()) || Animation->IsA(UAnimMontage::StaticClass()))
		//{
			//return(true);
		//}
	//}
	//return(false);
//}