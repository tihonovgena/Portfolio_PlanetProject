#include "HQSignalSystem.h"

#include "HQSignalSourceComponent.h"

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

FHQSignalData UHQSignalSystem::GetSignalData(const FVector& Location) const
{
	float StrongestValue = 0.f;
	
	for (TWeakObjectPtr<UHQSignalSourceComponent> Source : SignalSources)
	{
		ensure(Source.IsValid());

		const AActor* Owner = Source->GetOwner();
		ensure(Owner);

		const FHQSignalSourceData& SourceData = Source->GetSignalSourceData();
		if (!SourceData.IsValid()) continue;
		
		FVector OwnerLocation = Owner->GetActorLocation();

		const float Distance = FVector::Dist(Location, OwnerLocation);
		if (Distance > SourceData.Range) continue;

		float TempSignalValue = 1 - (Distance / SourceData.Range);
		
		TempSignalValue = SourceData.Attenuation == nullptr ?
			TempSignalValue : SourceData.Attenuation->GetFloatValue(TempSignalValue);
		
		StrongestValue = FMath::Max(StrongestValue, TempSignalValue);
	}
	
	int32 SignalStrengthInt = FMath::CeilToInt(StrongestValue * MAXSIGNALTYPES);
	const float NormalizedValue = FMath::IsNearlyEqual(StrongestValue, 1.f) ?
		1.f : 1 - (SignalStrengthInt - (StrongestValue * MAXSIGNALTYPES));
	const EHQSignalStrength::Type SignalStrength = static_cast<EHQSignalStrength::Type>(SignalStrengthInt);
	return FHQSignalData(SignalStrength, StrongestValue, NormalizedValue);
}
