
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "BakemonoBakariCharacter.generated.h"

UCLASS(config=Game)
class ABakemonoBakariCharacter : public ACharacter
{
	GENERATED_BODY()

protected:

	/** Called for side to side input */
	void MoveRight(float Val);

	// APawn interface
	virtual void SetupPlayerInputComponent(class UInputComponent* InputComponent) override;
	// End of APawn interface


public:
	ABakemonoBakariCharacter();
};
