// Copyright 2016 Cullen Sarles, All rights resevered

#include "LightWeightGridLibrary.h"
#include "GeodesicGrid.h"


// Sets default values for this component's properties
UGeodesicGrid::UGeodesicGrid()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	bWantsBeginPlay = true;
	PrimaryComponentTick.bCanEverTick = false;
	GridFrequency = 0;
	GridRadius = 100.0f;
	// ...
}


// Called when the game starts
void UGeodesicGrid::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UGeodesicGrid::TickComponent( float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction )
{
	Super::TickComponent( DeltaTime, TickType, ThisTickFunction );

	// ...
}

int32 UGeodesicGrid::GetNumberOfElements_Implementation() const
{
	return 0;
}

FVector UGeodesicGrid::GetIndexLocation_Implementation(int32 gridIndex) const
{
	return FVector(0.0, 0.0, 0.0);
}

int32 UGeodesicGrid::GetLocationIndex_Implementation(const FVector& location) const
{
	return 0;
}

TArray<int32> UGeodesicGrid::GetIndexNeighbors_Implementation(int32 gridIndex) const
{
	return TArray<int32>();
}

int32 UGeodesicGrid::GetGridFrequency() const
{
	return GridFrequency;
}

void UGeodesicGrid::SetGridFrequency(int32 newFrequency)
{
	GridFrequency = newFrequency;
	buildGrid();
}

float UGeodesicGrid::GetGridRadius() const
{
	return GridRadius;
}

void UGeodesicGrid::SetGridRadius(float newRadius)
{
	GridRadius = newRadius;
	buildGrid();
}

void UGeodesicGrid::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	FName PropertyName = (PropertyChangedEvent.Property != NULL) ? PropertyChangedEvent.Property->GetFName() : NAME_None;
	if ((PropertyName == GET_MEMBER_NAME_CHECKED(UGeodesicGrid, GridFrequency)) || (PropertyName == GET_MEMBER_NAME_CHECKED(UGeodesicGrid, GridRadius)))
	{
		buildGrid();
	}
	else
	{
		Super::PostEditChangeProperty(PropertyChangedEvent);
	}
}

void UGeodesicGrid::buildGrid()
{
	
}

