#pragma once

#include "CoreMinimal.h"
#include "VerdantShiftCharacter.h"
#include "VerdantGrapplingCharacter.generated.h"

class UVerdantMovementComponent;
class UCableComponent;

/** Character base for the isolated grappling prototype Blueprint. */
UCLASS(Blueprintable)
class VERDANTSHIFT_API AVerdantGrapplingCharacter : public AVerdantShiftCharacter
{
	GENERATED_BODY()

public:
	AVerdantGrapplingCharacter(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	virtual void Tick(float DeltaSeconds) override;

	UFUNCTION(BlueprintPure, Category = "Verdant|Grapple")
	UVerdantMovementComponent* GetVerdantMovementComponent() const;

	UFUNCTION(BlueprintCallable, Category = "Verdant|Grapple")
	bool TryStartGrappleFromView();

	UFUNCTION(BlueprintCallable, Category = "Verdant|Grapple")
	void ReleaseGrapple();

protected:
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Verdant|Grapple")
	UCableComponent* GrappleCable;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Verdant|Grapple|Trace", meta = (ClampMin = "100.0"))
	float GrappleTraceDistance = 5000.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Verdant|Grapple|Trace", meta = (ClampMin = "0.0"))
	float GrappleTraceRadius = 30.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Verdant|Grapple|Debug")
	bool bDrawGrappleDebug = true;

private:
	void HandleGrapplePressed();
	void UpdateCableEndpoint();
};
