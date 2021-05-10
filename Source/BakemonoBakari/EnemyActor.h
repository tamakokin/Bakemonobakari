// AEnemyActor.h
// �T�v		�F�G�̍s���v���O����(�v���g�^�C�v
// �쐬��	�F��c�I��
// �쐬��	�F2021/5/7
// �X�V����	�F2021/5/10 �v���O������ǋL


#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "EnemyActor.generated.h"

UCLASS()
class POW_API AEnemyActor : public AActor
{
	GENERATED_BODY()
	
public:
	// �R���X�g���N�^
	AEnemyActor();

protected:
	// �Q�[���X�^�[�g���A�܂��͐������ɌĂ΂�鏈��
	virtual void BeginPlay() override;

public:
	// ���t���[���̏���
	virtual void Tick(float DeltaTime) override;

private:
	// �I�[�o�[���b�v�֐�
	UFUNCTION() void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

private:
	// �ړ�����
	void EnemyMove(float _deltaTime);

	// �_���[�W����
	void EnemyDamage();

	// �U������
	void EnemyAttack();


private:

	// �G�l�~�[�̃X�e�[�^�X
	enum ENEMY_STATE
	{
		ENEMY_STATE_NONE = 0,
		ENEMY_STATE_MOVE,
		ENEMY_STATE_IDLE,
		ENEMY_STATE_DAMAGE,
		ENEMY_STATE_STOP,
	};

	// �G�̎��
	enum ENEMY_TYPE
	{
		ENEMY_TYPE_NONE = 0,
		ENEMY_TYPE_STOP,
		ENEMY_TYPE_STRAIGHT,
		ENEMY_TYPE_JUMP,
		ENEMY_TYPE_FRY,
	};

public:
	// blueprint EditAnywhere
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UCapsuleComponent* m_pCapsuleComp;		//	�^�[�Q�b�g�ڐG�p�R���W����
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		USkeletalMeshComponent* m_pEnemyMesh;	// ���b�V��

private:
	UPROPERTY()
		USceneComponent* m_pBase;		// �x�[�X
	UPROPERTY(EditAnywhere)
		FName m_tagName;				// �^�O��

private:
	UPROPERTY(EditAnywhere)
		float m_moveSpeedX;		// �i�s�����̃X�s�[�h

	UPROPERTY(EditAnywhere)
		float m_moveSpeedY;		// ������̈ړ���

	UPROPERTY(EditAnywhere)
		float m_altitudeLimit;	// �ő卂�x

	UPROPERTY(EditAnywhere)
		float m_EnemyHP;		// �G�̂g�o

	UPROPERTY(EditAnywhere)
		float m_EnemyType_Any;	// �G�̎�ޓ��͗p


private:
	ENEMY_STATE m_EnemyState;			// �G�l�~�[�̃X�e�[�^�X
	ENEMY_TYPE m_EnemyType;				// �G�̎��
	AActor* m_pOverlappedActor;			// �I�[�o�[���b�v�����A�N�^�[
};
