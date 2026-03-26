// Fill out your copyright notice in the Description page of Project Settings.


#include "HQSignalReceiverComponent.h"

#include "HQSignalSystem.h"

UHQSignalReceiverComponent::UHQSignalReceiverComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
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

void UHQSignalReceiverComponent::BeginPlay()
{
	Super::BeginPlay();

	if (bAutoActive)
	{
		StartSignalUpdate();
	}

	SignalSystem = GetWorld()->GetSubsystem<UHQSignalSystem>();
	ensure(SignalSystem);
}

void UHQSignalReceiverComponent::OnSignalTimerUpdated()
{
	// TODO logic of changed data, do not broadcast if not changed
	const FHQSignalData SignalData = SignalSystem->GetSignalData(GetOwner()->GetActorLocation());
	OnSignalUpdated.Broadcast(SignalData);
}
