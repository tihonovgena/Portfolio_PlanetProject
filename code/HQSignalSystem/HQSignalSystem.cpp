// Fill out your copyright notice in the Description page of Project Settings.


#include "HQSignalSystem.h"

#include "HQSignalSourceComponent.h"
#include "Kismet/KismetMathLibrary.h"

constexpr uint8 MAXSIGNALTYPES = EHQSignalStrength::Max - 1;

void UHQSignalSystem::RegisterSource(UHQSignalSourceComponent* Source)
{
	if (Source == nullptr) return;
	
	SignalSources.AddUnique(Source);
}

void UHQSignalSystem::UnRegisterSource(UHQSignalSourceComponent* Source)
{
	if (Source == nullptr) return;
	
	SignalSources.Remove(Source);
}

void UHQSignalSystem::GetSignalData(const FVector& Location, FHQSignalData& DataOut) const
{
	float StrongestValue = 0.f;
	UHQSignalSourceComponent* StrongestSource = nullptr;
	
	for (TWeakObjectPtr<UHQSignalSourceComponent> Source : SignalSources)
	{
		ensure(Source.IsValid());

		const AActor* Owner = Source->GetOwner();
		ensure(Owner);

		const FHQSignalSourceData& SourceData = Source->GetSourceSignalData();
		if (!SourceData.IsValid()) continue;

		// Check direction to prevent receiving signal from mirrored location
		FVector SourceLocation = Owner->GetActorLocation();
		const float DotProduct = FVector::DotProduct(Location, SourceLocation);
		if (DotProduct <=0)  continue;
		
		// Transform actor location to point cell location
		FTransform SourceTransform = Source->GetOwner()->GetActorTransform();
		FVector LocalLocation = UKismetMathLibrary::InverseTransformLocation(SourceTransform, Location);
		LocalLocation.Z = 0.f;
		const int PointX = FMath::FloorToInt(LocalLocation.X / SourceData.CellSize);
		const int PointY = FMath::FloorToInt(LocalLocation.Y / SourceData.CellSize);
		FIntPoint PointLocation{PointX, PointY};

		// Get signal for PointLocation of receiver, if not found, no signal
		const TMap<FIntPoint, FSignalCellPoint>& SignalField = Source->GetSignalField();
		if (!SignalField.Contains(PointLocation)) continue;
		FSignalCellPoint CurrentSignalCell = SignalField.FindRef(PointLocation);
		float CurrentStrength = CurrentSignalCell.Strength;
		
		const float NormalizedStrength = FMath::Clamp(CurrentStrength / SourceData.Strength, 0.f, 1.f);
		StrongestValue = FMath::Max(StrongestValue, NormalizedStrength);
	}
	
	int32 SignalStrengthInt = FMath::CeilToInt(StrongestValue * MAXSIGNALTYPES);
	const float NormalizedValue = FMath::IsNearlyEqual(StrongestValue, 1.f) ?
		1.f : 1 - (SignalStrengthInt - (StrongestValue * MAXSIGNALTYPES));
	const EHQSignalStrength::Type SignalStrength = static_cast<EHQSignalStrength::Type>(SignalStrengthInt);
	
	DataOut = {SignalStrength, StrongestValue, NormalizedValue};
}
