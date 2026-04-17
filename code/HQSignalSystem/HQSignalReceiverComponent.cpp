// Fill out your copyright notice in the Description page of Project Settings.


#include "HQSignalReceiverComponent.h"

#include "HQSignalSystem.h"

UHQSignalReceiverComponent::UHQSignalReceiverComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UHQSignalReceiverComponent::StartSignalUpdate()
{
	FTimerManager& TimerManager = GetWorld()->GetTimerManager();
	TimerManager.SetTimer(SignalUpdateTimer, this, &ThisClass::OnSignalTimerUpdated, SignalUpdateRate, true);
}

void UHQSignalReceiverComponent::StopSignalUpdate()
{
	FTimerManager& TimerManager = GetWorld()->GetTimerManager();
	if (TimerManager.IsTimerActive(SignalUpdateTimer))
	{
		TimerManager.ClearTimer(SignalUpdateTimer);
	}
}

void UHQSignalReceiverComponent::TickComponent(float DeltaTime, enum ELevelTick TickType,
	FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (bUpdateOnTick)
	{
		OnSignalTimerUpdated();
	}
}

void UHQSignalReceiverComponent::BeginPlay()
{
	Super::BeginPlay();

	if (bAutoActive && !bUpdateOnTick)
	{
		StartSignalUpdate();
	}

	SignalSystem = GetWorld()->GetSubsystem<UHQSignalSystem>();
	ensure(SignalSystem);
}

void UHQSignalReceiverComponent::OnSignalTimerUpdated()
{
	DECLARE_SCOPE_CYCLE_COUNTER(TEXT("ReceiveSignalData"), STAT_ReceiveSignalData, STATGROUP_HQSignalSystem);
	
	// TODO logic of changed data, do not broadcast if not changed
	FHQSignalData SignalData;
	SignalSystem->GetSignalData(GetOwner()->GetActorLocation(), SignalData); 
	OnSignalUpdated.Broadcast(SignalData);
}
