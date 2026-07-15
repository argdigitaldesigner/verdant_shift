#include "VerdantGrapplingCharacter.h"

#include "CableComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "DrawDebugHelpers.h"
#include "Engine/World.h"
#include "InputCoreTypes.h"
#include "VerdantMovementComponent.h"

AVerdantGrapplingCharacter::AVerdantGrapplingCharacter(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer.SetDefaultSubobjectClass<UVerdantMovementComponent>(ACharacter::CharacterMovementComponentName))
{
	PrimaryActorTick.bCanEverTick = true;

	GrappleCable = CreateDefaultSubobject<UCableComponent>(TEXT("GrappleCable"));
	GrappleCable->SetupAttachment(GetCapsuleComponent());
	GrappleCable->SetRelativeLocation(FVector(0.0f, 0.0f, 40.0f));
	GrappleCable->CableLength = 0.0f;
	GrappleCable->NumSegments = 12;
	GrappleCable->SolverIterations = 1;
	GrappleCable->CableWidth = 2.0f;
	GrappleCable->bEnableStiffness = false;
	GrappleCable->bEnableCollision = false;
	GrappleCable->bAttachEnd = false;
	GrappleCable->SetVisibility(false);
}

UVerdantMovementComponent* AVerdantGrapplingCharacter::GetVerdantMovementComponent() const
{
	return Cast<UVerdantMovementComponent>(GetCharacterMovement());
}

void AVerdantGrapplingCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindKey(EKeys::RightMouseButton, IE_Pressed, this, &AVerdantGrapplingCharacter::HandleGrapplePressed);
	PlayerInputComponent->BindKey(EKeys::RightMouseButton, IE_Released, this, &AVerdantGrapplingCharacter::ReleaseGrapple);
}

void AVerdantGrapplingCharacter::HandleGrapplePressed()
{
	TryStartGrappleFromView();
}

void AVerdantGrapplingCharacter::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if (GetVerdantMovementComponent() && GetVerdantMovementComponent()->IsGrappling())
	{
		UpdateCableEndpoint();
	}
}

bool AVerdantGrapplingCharacter::TryStartGrappleFromView()
{
	const UCameraComponent* ViewCamera = GetFollowCamera();
	UVerdantMovementComponent* Movement = GetVerdantMovementComponent();
	UWorld* World = GetWorld();
	if (!ViewCamera || !Movement || !World)
	{
		return false;
	}

	const FVector TraceStart = ViewCamera->GetComponentLocation();
	const FVector TraceEnd = TraceStart + ViewCamera->GetForwardVector() * GrappleTraceDistance;

	FCollisionQueryParams QueryParams(SCENE_QUERY_STAT(VerdantGrappleTrace), false, this);
	FHitResult Hit;
	const bool bHit = World->SweepSingleByChannel(
		Hit,
		TraceStart,
		TraceEnd,
		FQuat::Identity,
		ECC_Visibility,
		FCollisionShape::MakeSphere(GrappleTraceRadius),
		QueryParams);

	if (bDrawGrappleDebug)
	{
		const FColor DebugColor = bHit ? FColor::Green : FColor::Red;
		DrawDebugLine(World, TraceStart, bHit ? Hit.ImpactPoint : TraceEnd, DebugColor, false, 1.0f, 0, 1.5f);
		if (bHit)
		{
			DrawDebugSphere(World, Hit.ImpactPoint, GrappleTraceRadius, 12, DebugColor, false, 1.0f);
		}
	}

	if (!bHit || !Movement->StartGrappling(Hit.ImpactPoint))
	{
		return false;
	}

	GrappleCable->SetVisibility(true);
	UpdateCableEndpoint();
	return true;
}

void AVerdantGrapplingCharacter::ReleaseGrapple()
{
	if (UVerdantMovementComponent* Movement = GetVerdantMovementComponent())
	{
		Movement->StopGrappling();
	}

	GrappleCable->SetVisibility(false);
}

void AVerdantGrapplingCharacter::UpdateCableEndpoint()
{
	const UVerdantMovementComponent* Movement = GetVerdantMovementComponent();
	if (!Movement || !GrappleCable)
	{
		return;
	}

	GrappleCable->EndLocation = GrappleCable->GetComponentTransform().InverseTransformPosition(Movement->GetGrappleAnchor());
}
