#include "VerdantMovementComponent.h"

#include "GameFramework/Character.h"

bool UVerdantMovementComponent::StartGrappling(const FVector& TargetAnchor)
{
	if (!CharacterOwner || !UpdatedComponent || TargetAnchor.ContainsNaN())
	{
		return false;
	}

	const float InitialDistance = FVector::Distance(UpdatedComponent->GetComponentLocation(), TargetAnchor);
	if (InitialDistance < MinimumCableLength)
	{
		return false;
	}

	GrappleAnchor = TargetAnchor;
	MaxCableLength = InitialDistance;
	SetMovementMode(MOVE_Custom, static_cast<uint8>(EVerdantCustomMovementMode::Grappling));
	return true;
}

void UVerdantMovementComponent::StopGrappling()
{
	if (IsGrappling())
	{
		SetMovementMode(MOVE_Falling);
	}
}

bool UVerdantMovementComponent::IsGrappling() const
{
	return MovementMode == MOVE_Custom
		&& CustomMovementMode == static_cast<uint8>(EVerdantCustomMovementMode::Grappling);
}

void UVerdantMovementComponent::PhysCustom(float DeltaTime, int32 Iterations)
{
	if (!IsGrappling())
	{
		Super::PhysCustom(DeltaTime, Iterations);
		return;
	}

	if (DeltaTime < MIN_TICK_TIME || !CharacterOwner || !UpdatedComponent)
	{
		return;
	}

	RestorePreAdditiveRootMotionVelocity();
	Velocity.Z += GetGravityZ() * DeltaTime;
	ApplyRootMotionToVelocity(DeltaTime);

	FVector CurrentLocation = UpdatedComponent->GetComponentLocation();
	FVector ToAnchor = GrappleAnchor - CurrentLocation;
	const float CurrentDistance = ToAnchor.Size();

	if (CurrentDistance >= MaxCableLength && CurrentDistance > UE_KINDA_SMALL_NUMBER)
	{
		const FVector RopeNormal = ToAnchor / CurrentDistance;
		const float RadialSpeed = FVector::DotProduct(Velocity, RopeNormal);

		// RopeNormal points toward the anchor. A negative radial speed is moving
		// away from it, so remove only that component and preserve tangent speed.
		if (RadialSpeed < 0.0f)
		{
			Velocity -= RopeNormal * RadialSpeed;
		}

		const float Overstretch = CurrentDistance - MaxCableLength;
		if (Overstretch > UE_KINDA_SMALL_NUMBER)
		{
			FHitResult CorrectionHit(1.0f);
			SafeMoveUpdatedComponent(
				RopeNormal * Overstretch,
				UpdatedComponent->GetComponentQuat(),
				true,
				CorrectionHit);

			if (CorrectionHit.IsValidBlockingHit())
			{
				HandleImpact(CorrectionHit, DeltaTime, RopeNormal * Overstretch);
			}
		}
	}

	const FVector MoveDelta = Velocity * DeltaTime;
	FHitResult Hit(1.0f);
	SafeMoveUpdatedComponent(MoveDelta, UpdatedComponent->GetComponentQuat(), true, Hit);

	if (Hit.IsValidBlockingHit())
	{
		HandleImpact(Hit, DeltaTime, MoveDelta);
		SlideAlongSurface(MoveDelta, 1.0f - Hit.Time, Hit.Normal, Hit, true);

		if (Hit.Normal.Z > LandingNormalThreshold)
		{
			StopGrappling();
		}
	}

	UpdateComponentVelocity();
}
