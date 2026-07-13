#pragma once

#include "CoreMinimal.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "VerdantMovementComponent.generated.h"

UENUM(BlueprintType)
enum class EVerdantCustomMovementMode : uint8
{
	None UMETA(Hidden),
	Grappling UMETA(DisplayName = "Grappling")
};

/**
 * Character movement component with an analytical rope constraint.
 *
 * Grappling preserves momentum by removing only the velocity component that
 * would increase the distance from the anchor while the rope is taut.
 */
UCLASS(ClassGroup = Movement, meta = (BlueprintSpawnableComponent))
class VERDANTSHIFT_API UVerdantMovementComponent : public UCharacterMovementComponent
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "Verdant|Grapple")
	bool StartGrappling(const FVector& TargetAnchor);

	UFUNCTION(BlueprintCallable, Category = "Verdant|Grapple")
	void StopGrappling();

	UFUNCTION(BlueprintPure, Category = "Verdant|Grapple")
	bool IsGrappling() const;

	UFUNCTION(BlueprintPure, Category = "Verdant|Grapple")
	FVector GetGrappleAnchor() const { return GrappleAnchor; }

	UFUNCTION(BlueprintPure, Category = "Verdant|Grapple")
	float GetCableLength() const { return MaxCableLength; }

protected:
	virtual void PhysCustom(float DeltaTime, int32 Iterations) override;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "Verdant|Grapple")
	FVector GrappleAnchor = FVector::ZeroVector;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "Verdant|Grapple", meta = (ClampMin = "0.0"))
	float MaxCableLength = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Verdant|Grapple", meta = (ClampMin = "0.0"))
	float MinimumCableLength = 100.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Verdant|Grapple", meta = (ClampMin = "0.0", ClampMax = "1.0"))
	float LandingNormalThreshold = 0.7f;
};
