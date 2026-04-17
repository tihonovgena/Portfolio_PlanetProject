// Fill out your copyright notice in the Description page of Project Settings.




#include "HQSignalSourceComponent.h"


#include "HQSignalSystem.h"
#include "HQSignalSystemSettings.h"

const int32 DX[4] = { 1, -1, 0, 0 };
const int32 DY[4] = { 0, 0, -1, 1 };


UHQSignalSourceComponent::UHQSignalSourceComponent()
{
   PrimaryComponentTick.bCanEverTick = false;
   SignalTraceChannel = ECC_Visibility;
}

void UHQSignalSourceComponent::UpdateSourceSignalField()
{
   GenerateSignalField(SignalSourceData, SignalField);
}

void UHQSignalSourceComponent::GenerateSignalField(const FHQSignalSourceData& Params, TMap<FIntPoint, FSignalCellPoint>& FieldOut)
{
   DECLARE_SCOPE_CYCLE_COUNTER(TEXT("UpdateSignalField"), STAT_UpdateSignalField, STATGROUP_HQSignalSystem);
   
   FieldOut.Empty();
   // TODO how to calculate more precise reserve amount?
   FieldOut.Reserve(Params.Strength / 2.f);
  
   TQueue<FIntPoint> Queue;
  
   FSignalCellPoint StartCell = FSignalCellPoint(FIntPoint(0, 0));

   StartCell.Strength = Params.Strength;
   StartCell.WorldLocation = GetComponentLocation();
   StartCell.LocalLocation = FVector(0, 0, 0);
   StartCell.Type = ESignalCellType::Origin;
  
   Queue.Enqueue(StartCell.CellPoint);
   FieldOut.Add(StartCell.CellPoint, StartCell);
  
   while (!Queue.IsEmpty())
   {
      FIntPoint Current;
      Queue.Dequeue(Current);
      
      // Can't use ref here, map changing during logic
      FSignalCellPoint CurrentCell = FieldOut.FindRef(Current);
      
      for (int32 i = 0; i < 4; ++i)
      {
         FIntPoint Neighbour{CurrentCell.CellPoint.X + DX[i], CurrentCell.CellPoint.Y + DY[i]};
         
         // TODO Change to method something build neighbours?
         // TODO Maybe status it is status but if we want to understand cell blocked for spreading make some variable
         if (!FieldOut.Contains(Neighbour) && CurrentCell.Status == ESignalCellStatus::New && CurrentCell.Strength > 0)
         {
            FSignalCellPoint NeighbourCell;
            if (BuildNeighbourCell(CurrentCell, Neighbour, NeighbourCell, Params.CellSize))
            {
               if (NeighbourCell.Type == ESignalCellType::Reflected)
               {
                  const float RefectionStrength = Params.Strength - NeighbourCell.LocalLocation.Length();
                  CurrentCell.StrengthBonus = RefectionStrength * Params.ReflectionMultiplier;
               }
               else if (NeighbourCell.Type == ESignalCellType::Cliffed)
               {
                  // TODO Some error here, it applies for regular as well
                  float SourceHeight = FVector::DotProduct((GetComponentLocation() - GetOwner()->GetActorLocation()), GetUpVector());
                  const FVector Delta = CurrentCell.WorldLocation - NeighbourCell.WorldLocation;
                  const float DistanceAlongUp = FVector::DotProduct(Delta, GetUpVector());
                  CurrentCell.StrengthBonus = (SourceHeight + DistanceAlongUp) * 2.f;
               }

               FieldOut.Add(NeighbourCell.CellPoint, NeighbourCell);
            }
         }
         
         FSignalCellPoint* NeighbourCellPtr = FieldOut.Find(Neighbour);
         if (NeighbourCellPtr == nullptr) continue;
         
         if (TryUpdateNeighbourStrength(CurrentCell, *NeighbourCellPtr, Params.Strength, Params.CellSize))
         {
            Queue.Enqueue(Neighbour);
         }
      }
      
      if (CurrentCell.Status != ESignalCellStatus::Closed)
      {
         CurrentCell.Status = ESignalCellStatus::Updated;
      }

      // Update current cell with new data
      FieldOut.Emplace(Current, CurrentCell);
     
#if WITH_EDITOR
      if (bSaveSnapshots)
      {
         SignalFieldSnapshots.Snapshot(FieldOut);
      }
#endif
   }

   FieldOut.Shrink();
}

bool UHQSignalSourceComponent::BuildNeighbourCell(const FSignalCellPoint& CurrentCell, const FIntPoint& NeighbourPoint,
   FSignalCellPoint& NeighbourCell, const float CellSize) const
{
   TRACE_CPUPROFILER_EVENT_SCOPE(BuildNeighbourCell);
   NeighbourCell = {NeighbourPoint};
   ComputeNeighbourCellLocation(CurrentCell, NeighbourCell, CellSize);

   // TODO Make distance to project, depending on source params, and what about offset?
   // For now if point can't be projected it means ground too far or trying project to air
   // First should be projected, before do any block checks
   if (!ProjectToGround(NeighbourCell.WorldLocation, 2000.f, 20.f)) return false;

   if (CheckCliffedCell(CurrentCell, NeighbourCell, CellSize, FHitResult()))
   {
      NeighbourCell.Type = ESignalCellType::Cliffed;
      return true;
   }
   
   TRACE_CPUPROFILER_EVENT_SCOPE(CheckBlock);
   const UWorld* World = GetWorld();
   FHitResult Hit;
   World->LineTraceSingleByChannel(Hit, GetComponentLocation(),
   NeighbourCell.WorldLocation, SignalTraceChannel);
   
   if (Hit.bBlockingHit)
   {
      NeighbourCell.Type = CheckReflectedCell(CurrentCell, NeighbourCell, Hit) ? ESignalCellType::Reflected : ESignalCellType::Blocked;
      NeighbourCell.Status = ESignalCellStatus::Closed;
   }
   
   return true;
}

void UHQSignalSourceComponent::ComputeNeighbourCellLocation(const FSignalCellPoint& CurrentCell, FSignalCellPoint& NeighbourCell,
   const float CellSize) const
{
   NeighbourCell.LocalLocation =
   {
      NeighbourCell.CellPoint.X * CellSize,
      NeighbourCell.CellPoint.Y * CellSize,
      0.f
   };
        
   NeighbourCell.WorldLocation = GetComponentTransform().TransformPosition(NeighbourCell.LocalLocation);
}

bool UHQSignalSourceComponent::CheckReflectedCell(const FSignalCellPoint& CurrentCell, const FSignalCellPoint& NeighbourCell,
   const FHitResult& Hit) const
{
   TRACE_CPUPROFILER_EVENT_SCOPE(CheckReflection);
   // TODO For now we can have sometimes not reflected points usually if they are far from source,
   // probably it is not bad but should we solve it?
   return (Hit.Location - NeighbourCell.WorldLocation).Size() < SignalSourceData.CellSize * 1.5f;
}

bool UHQSignalSourceComponent::CheckCliffedCell(const FSignalCellPoint& CurrentCell,
   const FSignalCellPoint& NeighbourCell, const float CellSize, const FHitResult& Hit) const
{
   TRACE_CPUPROFILER_EVENT_SCOPE(CheckCliffed);

   // We take FVector2D to prevent height affect on distance
   const float DistSourceToNeighbour = (FVector2D(GetRelativeLocation()) - FVector2D(NeighbourCell.LocalLocation)).Size();
   const float SourceHeight = GetRelativeLocation().Z;
   
   // TODO magic number here could be change on attribute from SourceSignalData 
   const bool bCanBeCliffed = DistSourceToNeighbour <= SourceHeight + (CellSize * 3.f);
   
   if (bCanBeCliffed)
   {
      // TODO Change into methods, check cliffed cell
      float HeightOriginToNeighbour = 0.f;
      // If current cell is origin, we don't want origin height affects on cliff detection
      if (CurrentCell.Type == ESignalCellType::Origin)
      {
         HeightOriginToNeighbour = SourceHeight;
      }
   
      const FVector Delta = CurrentCell.WorldLocation - NeighbourCell.WorldLocation;
      const float DistanceAlongUp = FVector::DotProduct(Delta, GetUpVector()) - HeightOriginToNeighbour;
      return DistanceAlongUp > CellSize;
   }

   return false;
}

bool UHQSignalSourceComponent::TryUpdateNeighbourStrength(const FSignalCellPoint& CurrentCell, FSignalCellPoint& NeighbourCell,
                                                          const float BaseStrength, const float StepCost) const
{
   TRACE_CPUPROFILER_EVENT_SCOPE(TryUpdateNeighbourStrength);
   
   const float StrengthBonus = FMath::Max(0.f, CurrentCell.StrengthBonus - StepCost);
   float DistanceToCell;
   if (NeighbourCell.Type == ESignalCellType::Cliffed && CurrentCell.Type != ESignalCellType::Cliffed)
   {
      DistanceToCell = (CurrentCell.WorldLocation - NeighbourCell.WorldLocation).Size();
   }
   else
   {
      DistanceToCell = NeighbourCell.LocalLocation.Size();
   }
   const float Strength = BaseStrength - DistanceToCell;
   const float TotalStrength = Strength + StrengthBonus;
        
   if (TotalStrength > NeighbourCell.Strength)
   {
      NeighbourCell.Strength = TotalStrength;
      NeighbourCell.StrengthBonus = StrengthBonus;
            
      if (NeighbourCell.Status == ESignalCellStatus::Updated)
      {
         NeighbourCell.Status = ESignalCellStatus::PendingUpdate;
      }

      return true;
   }

   return false;
}


bool UHQSignalSourceComponent::ProjectToGround(FVector& ProjectedOut, const float Distance, const float Offset) const
{
   TRACE_CPUPROFILER_EVENT_SCOPE(ProjectToGround);
   const UWorld* World = GetWorld();
   const FVector Direction = GetUpVector();
   const FVector TraceEnd = ProjectedOut + (Direction * (-Distance));
   FHitResult Hit;
   if (World->LineTraceSingleByChannel(Hit, ProjectedOut, TraceEnd, SignalTraceChannel))
   {
      ProjectedOut = Hit.Location + (Offset * Direction);
      return true;
   }

   return false;
}


void UHQSignalSourceComponent::BeginPlay()
{
   Super::BeginPlay();

   UHQSignalSystem* SignalSystem = GetWorld()->GetSubsystem<UHQSignalSystem>();
   if (ensure(SignalSystem))
   {
      SignalSystem->RegisterSource(this);
   }

   const UHQSignalSystemSettings* SignalSettings = GetDefault<UHQSignalSystemSettings>();
   SignalTraceChannel = SignalSettings->SignalTraceChannel;
}


void UHQSignalSourceComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
   UHQSignalSystem* SignalSystem = GetWorld()->GetSubsystem<UHQSignalSystem>();
   if (ensure(SignalSystem))
   {
      SignalSystem->UnRegisterSource(this);
   }
  
   Super::EndPlay(EndPlayReason);
}
