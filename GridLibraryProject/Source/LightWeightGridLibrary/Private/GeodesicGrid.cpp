// Copyright 2016 Cullen Sarles, All rights resevered

#include "LightWeightGridLibrary.h"
#include "GeodesicGrid.h"


// Sets default values for this component's properties
UGeodesicGrid::UGeodesicGrid() : Super()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	bWantsBeginPlay = false;
	PrimaryComponentTick.bCanEverTick = false;
	GridFrequency = 1;
	GridRadius = 100.0f;
	buildGrid();
}

int32 UGeodesicGrid::GetNumberOfElements_Implementation() const
{
	return NumberOfVertexes;
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

void UGeodesicGrid::IncrementFrequency()
{

}

void UGeodesicGrid::DecrementFrequency()
{

}

float UGeodesicGrid::GetGridRadius() const
{
	return GridRadius;
}

void UGeodesicGrid::SetGridRadius(float newRadius)
{
	GridRadius = newRadius;
	buildIcosahedronRefernceLocations();
}

void UGeodesicGrid::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	FName PropertyName = (PropertyChangedEvent.Property != NULL) ? PropertyChangedEvent.Property->GetFName() : NAME_None;
	if ((PropertyName == GET_MEMBER_NAME_CHECKED(UGeodesicGrid, GridFrequency)))
	{
		buildGrid();
	}
	else if (PropertyName == GET_MEMBER_NAME_CHECKED(UGeodesicGrid, GridRadius))
	{
		buildIcosahedronRefernceLocations();
	}
	else
	{
		Super::PostEditChangeProperty(PropertyChangedEvent);
	}
}

void UGeodesicGrid::buildGrid()
{
	// update the number of Vertexes
	NumberOfVertexes = 10 * GridFrequency * GridFrequency + 2;
	buildIcosahedronRefernceLocations();
	//Prep The UV Location List for population
	UVLocationList.Empty(NumberOfVertexes);
	UVLocationList.SetNumZeroed(NumberOfVertexes);

	//Prep To Rebuild the Grid
	RectilinearGrid.Empty(5 * GridFrequency);
	RectilinearGrid.SetNum(5 * GridFrequency);

	int32 currentIndexNumber = 12; // numbers 0 through 11 are resevered for the key indexes;
	
	/* Populate the Reference Columns, do this first so we can reference them when populating the other columns without
	 * Having to go back and correct the first one
	 */
	populateRefernceColumn(0 * GridFrequency, currentIndexNumber);
	populateRefernceColumn(1 * GridFrequency, currentIndexNumber);
	populateRefernceColumn(2 * GridFrequency, currentIndexNumber);
	populateRefernceColumn(3 * GridFrequency, currentIndexNumber);
	populateRefernceColumn(4 * GridFrequency, currentIndexNumber);

	for (int32 GridColumn = 0; GridColumn < 5; ++GridColumn)
	{
		//We're in one of the reference columns we've already taken care of it
		if (GridColumn % 5 == 0) 
		{
			continue; 
		}
		populateGridColumn(GridColumn, currentIndexNumber);		
	}
}

void UGeodesicGrid::buildIcosahedronRefernceLocations()
{
	float x = 1;
	float z = (1 + FMath::Sqrt(5)) / 2.0;
	float baseHedronRadius = FMath::Sqrt(x*x + z*z);
	float pointMagnitudeRatio = 1.0 / baseHedronRadius;
	x *= pointMagnitudeRatio * GridRadius;
	z *= pointMagnitudeRatio * GridRadius;

	referenceLocations.Reset(12);
	referenceLocations.SetNumZeroed(12);

	//Top Point
	referenceLocations[10][0] = z;
	referenceLocations[10][1] = 0;
	referenceLocations[10][2] = x;
		
	//Bottom Point
	referenceLocations[11][0] = -z;
	referenceLocations[11][1] = 0;
	referenceLocations[11][2] = -x;

	//Point 0
	//+x,-z
	referenceLocations[0][0] = z;
	referenceLocations[0][1] = 0;
	referenceLocations[0][2] = -x;

	//Point 1
	//+x,-y
	referenceLocations[1][0] = x;
	referenceLocations[1][1] = -z;
	referenceLocations[1][2] = 0;

	//Point2
	//+y,-z
	referenceLocations[2][0] = 0;
	referenceLocations[2][1] = x;
	referenceLocations[2][2] = -z;

	//Point3
	//+x,+y
	referenceLocations[3][0] = x;
	referenceLocations[3][1] = z;
	referenceLocations[3][2] = 0;

	//Point4
	//-x,+y
	referenceLocations[4][0] = -x;
	referenceLocations[4][1] = z;
	referenceLocations[4][2] = 0;

	//Point5
	//+y,+z
	referenceLocations[5][0] = 0;
	referenceLocations[5][1] = x;
	referenceLocations[5][2] = z;

	//Point6
	//-x,-y
	referenceLocations[6][0] = -x;
	referenceLocations[6][1] = -z;
	referenceLocations[6][2] = 0;

	//Point7
	//-x,+z
	referenceLocations[7][0] = -z;
	referenceLocations[7][1] = 0;
	referenceLocations[7][2] = x;
	
	//Point8
	//-y, -z
	referenceLocations[8][0] = 0;
	referenceLocations[8][1] = -x;
	referenceLocations[8][2] = -z;

	//Point9
	//-y,+z
	referenceLocations[9][0] = 0;
	referenceLocations[9][1] = -x;
	referenceLocations[9][2] = z;
}

void UGeodesicGrid::populateRefernceColumn(int32 GridColumn, int32& currentIndexNumber)
{
	RectilinearGrid[GridColumn].Empty(3 * GridFrequency + 1);
	RectilinearGrid[GridColumn].SetNumZeroed(3 * GridFrequency + 1);

	//Set the top and bottom Index Numbers
	AssignNewIndexNumber(GridColumn, 0, 10);
	AssignNewIndexNumber(GridColumn, 3 * GridFrequency, 11);

	//Set the interior index reference numbers
	for (int32 RowNumber = 1; RowNumber < RectilinearGrid[GridColumn].Num()-1; ++ RowNumber )
	{
		if (RowNumber % GridFrequency == 0)
		{		
			AssignNewIndexNumber(GridColumn, RowNumber,
				(RowNumber / GridFrequency) - 1		//	-1 because we're starting at zero
				+ (GridColumn / GridFrequency) * 2);	//	*2 there are two addition reference points per column
		}
		else
		{
			AssignNewIndexNumber(GridColumn, RowNumber, currentIndexNumber);
			++currentIndexNumber;
		}
	}
}

void UGeodesicGrid::populateGridColumn(int32 GridColumn, int32& currentIndexNumber)
{
	RectilinearGrid[GridColumn].Empty(2 * GridFrequency + 1);
	RectilinearGrid[GridColumn].SetNumZeroed(2 * GridFrequency + 1);

	//Set the top and bottom Index Numbers / Referencing the equivalent column locations
	int32 BottomReferenceColumn = (GridColumn/GridFrequency) * GridFrequency;
	int32 BottomReferenceRow = GridFrequency - (GridColumn % GridFrequency);
	AssignNewIndexNumber(GridColumn, 0, RectilinearGrid[BottomReferenceColumn][BottomReferenceRow]);

	int32 TopReferenceColumn = (GridColumn / GridFrequency)*GridFrequency + GridFrequency;
	if (TopReferenceColumn > 4* GridFrequency)
	{
		TopReferenceColumn = 0; //Wrap Around
	}
	int32 TopReferenceRow = 2 * GridFrequency + (GridFrequency - (GridColumn%GridFrequency));
	AssignNewIndexNumber(GridColumn, 2 * GridFrequency, RectilinearGrid[TopReferenceColumn][TopReferenceRow]);

	for (int32 RowNumber = 1; RowNumber < RectilinearGrid[GridColumn].Num() -1 ; ++RowNumber)
	{
		AssignNewIndexNumber(GridColumn, RowNumber, currentIndexNumber);
		++currentIndexNumber;
	}
}

void UGeodesicGrid::AssignNewIndexNumber(int32 GridColumn, int32 RowNumber, int32 currentIndexNumber)
{
	RectilinearGrid[GridColumn][RowNumber] = currentIndexNumber;
	UVLocationList[currentIndexNumber].Add(GridColumn);
	UVLocationList[currentIndexNumber].Add(RowNumber);
}

