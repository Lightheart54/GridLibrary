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
	int32 uIndex = UVLocationList[gridIndex][0];
	int32 vIndex = UVLocationList[gridIndex][1];

	//we need to make sure we're mapping to an actual triangle in on the grid
	//Example f = 3
	/*                                                          [9]
	 *                                                          [8]
	 *                                                          [7]
	 *                                                 [9][6][6][6]
	 *                                                 [8][5][5][5]
	 *                                                 [7][4][4][4]
	 *                                        [9][6][6][6][3][3][3]
	 *                                        [8][5][5][5][2][2][2]
	 *                                        [7][4][4][4][1][1][1]
	 *                               [9][6][6][6][3][3][3][0][0][0]
	 *                               [8][5][5][5][2][2][2] |  |  |
	 *                               [7][4][4][4][1][1][1] |  |  |
	 *                      [9][6][6][6][3][3][3][0][0][0] |  |  |
	 *                      [8][5][5][5][2][2][2] |  |  |  |  |  |
	 *                      [7][4][4][4][1][1][1] |  |  |  |  |  |
	 *             [9][6][6][6][3][3][3][0][0][0] |  |  |  |  |  |
	 *             [8][5][5][5][2][2][2] |  |  |  |  |  |  |  |  |
	 *             [7][4][4][4][1][1][1] |  |  |  |  |  |  |  |  |
	 *    [9][6][6][6][3][3][3][0][0][0] |  |  |  |  |  |  |  |  |
	 *    [8][5][5][5][2][2][2] |  |  |  |  |  |  |  |  |  |  |  |
	 *    [7][4][4][4][1][1][1] |  |  |  |  |  |  |  |  |  |  |  |
	 *    [6][3][3][3][0][0][0] |  |  |  |  |  |  |  |  |  |  |  |
	 *    [5][2][2][2] |  |  |  |  |  |  |  |  |  |  |  |  |  |  |
	 *    [4][1][1][1] |  |  |  |  |  |  |  |  |  |  |  |  |  |  |
	 *    [3][0][0][0] |  |  |  |  |  |  |  |  |  |  |  |  |  |  |
	 *    [2] |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |
	 *    [1] |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |
	 *    [0] |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |
	 *     0  1  2  3  4  5  6  7  8  9  10 11 12 13 14 15 16 17 0 
	 */
	
	 /*            [9]     Each Section has four possible triangles,
	 *              |      Representing a side of the base icosahedron
	 *             [8]
	 *              |      [9]-[6]-[6]-[6]                [6] VRef*2
	 *             [7]      | / | / | / '                / |
	 *              |      [8]-[5]-[5]  '             [5]-[5]
	 * [9]-[6]-[6]-[6]      | / | /     '            / | / |
	 *  | / | / | / |      [7]-[4]      '         [4]-[4]-[4]
	 * [8]-[5]-[5]-[5]      | /         '        / | / | / |
	 *  | / | / | / |      [6]          '     [6]-[3]-[3]-[3] VRef*1
	 * [7]-[4]-[4]-[4]      '           '      '           '
	 *  | / | / | / |       '           '      '           '
	 * [6]-[3]-[3]-[3]     [6]-[3]-[3]-[3]     '          [3] VRef*2
	 *  | / | / | / |       | / | / | / '      '         / |
	 * [5]-[2]-[2]-[2]     [5]-[2]-[2]  '      '      [2]-[2]
	 *  | / | / | / |       | / | /     '      '     / | / |
	 * [4]-[1]-[1]-[1]     [4]-[1]      '      '  [1]-[1]-[1]
	 *  | / | / | / |       | /         '      ' / | / | / |
	 * [3]-[0]-[0]-[0]     [3]          '     [3]-[0]-[0]-[0] VRef*1
	 *  |   '   '   '       '           '      '           '
	 * [2]  '   '   '  URef 1           2      1           2
	 *  |   '   '   '
	 * [1]  '   '   '
	 *  |   '   '   '
	 * [0]  '   '   '
	 *  0   1   2   0 <- ColumnNumber % Frequency
	 */

	 //find the reference vector indexes
	int32 ref11, ref12, ref21, ref22;
	DetermineReferenceIndexes(uIndex, vIndex,
		ref11, ref12, ref21, ref22);


	//Determine if we're in an "upper" or "lower" triangle
	int32 vCompare = vIndex - (vIndex / GridFrequency)*GridFrequency;
	int32 uCompare = uIndex % GridFrequency;
	bool upperTriangle = vCompare > uCompare //above the diagonal
		|| vIndex == RectilinearGrid[uIndex].Num() - 1; // At the Top of the section

	FVector icosahedronLocation;
	if (upperTriangle)
	{
		icosahedronLocation = determineTriangleLocation(uIndex, vIndex,ref11,ref22, ref11, ref12);
	}
	else
	{
		icosahedronLocation = determineTriangleLocation(uIndex, vIndex, ref11, ref22, ref21, ref22);
	}

	//normalize and project onto the sphere
	FVector normalizedVector;
	float baseVectorLength;
	icosahedronLocation.ToDirectionAndLength(normalizedVector, baseVectorLength);
	return GridRadius * normalizedVector;
}

int32 UGeodesicGrid::GetLocationIndex_Implementation(const FVector& location) const
{
	return 0;
}

TArray<int32> UGeodesicGrid::GetIndexNeighbors_Implementation(int32 gridIndex) const
{
	TArray<int32> neighborList;
	for (int32 UVIndex = 0; UVIndex < UVLocationList[gridIndex].Num(); UVIndex += 2)
	{
		TArray<int32> locationNeighbors = getLocationNeighbors(UVLocationList[gridIndex][UVIndex],
			UVLocationList[gridIndex][UVIndex + 1]);
		for (int32 neighborIndex : locationNeighbors)
		{
			neighborList.AddUnique(neighborIndex);
		}
	}
	return neighborList;
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

void UGeodesicGrid::populateRefernceColumn(const int32& GridColumn, int32& currentIndexNumber)
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

void UGeodesicGrid::populateGridColumn(const int32& GridColumn, int32& currentIndexNumber)
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

void UGeodesicGrid::AssignNewIndexNumber(const int32& GridColumn, const int32& RowNumber, const int32& currentIndexNumber)
{
	RectilinearGrid[GridColumn][RowNumber] = currentIndexNumber;
	UVLocationList[currentIndexNumber].Add(GridColumn);
	UVLocationList[currentIndexNumber].Add(RowNumber);
}

void UGeodesicGrid::DetermineReferenceIndexes(int32 uIndex, int32 vIndex, int32& ref11, int32& ref12, int32& ref21, int32& ref22) const
{
	//Start with the Column Index
	int32 uRef1 = (uIndex / GridFrequency)*GridFrequency; // The reference column number prior to/at this index

	 //if we're in the bottom indexes of a reference column, 
	//we need to reference to prior reference column as column one
	if (vIndex < GridFrequency && uRef1 == uIndex)
	{
		uRef1 -= GridFrequency;
	}
	int32 uRef2 = uRef1 + GridFrequency; // The reference column number after this index

										 // we might be wrapping around the grid
	if (uRef2 == 5 * GridFrequency)
	{
		uRef2 = 0; // Wrap Around
	}
	else if (uRef1 < 0)
	{
		uRef1 = 4 * GridFrequency;
	}

	//Now Find the Row Indexes
	int32 vRef21 = (vIndex / GridFrequency) * GridFrequency;
	if (vRef21 == vIndex && vRef21 != 0) // We're at the "Top of a Triangle"
	{
		//Reference the Bottom of the Triangle
		vRef21 -= GridFrequency;
	}

	//We're in the first column of a Triangle
	if (uRef1 == uIndex)
	{
		//So the Second column index is 3 below where we're at
		vRef21 -= GridFrequency;
	}
	//The first column reference is 3 above we're we're at
	int32 vRef11 = vRef21 + GridFrequency;

	//if we've done the above logic correctly such that vRef*1 is the lower corner,
	//then these are simple
	int32 vRef12 = vRef11 + GridFrequency;
	int32 vRef22 = vRef21 + GridFrequency;

	ref11 = RectilinearGrid[uRef1][vRef11];
	ref12 = RectilinearGrid[uRef1][vRef12];
	ref21 = RectilinearGrid[uRef2][vRef21];
	ref22 = RectilinearGrid[uRef2][vRef22];
}

TArray<int32> UGeodesicGrid::getLocationNeighbors(int32 uIndex, int32 vIndex) const
{
	TArray<int32> neighborList;
	int32 currentIndex = RectilinearGrid[uIndex][vIndex];
	incrementU(uIndex, vIndex); 
	addIndexToNeighborList(vIndex, currentIndex, uIndex, neighborList); //U+1, V
	++vIndex; 
	addIndexToNeighborList(vIndex, currentIndex, uIndex, neighborList); //U+1, V+1
	decrementU(uIndex, vIndex); 
	addIndexToNeighborList(vIndex, currentIndex, uIndex, neighborList); //U, V+1
	decrementU(uIndex, vIndex);
	--vIndex;
	addIndexToNeighborList(vIndex, currentIndex, uIndex, neighborList); //U-1, V
	--vIndex;
	addIndexToNeighborList(vIndex, currentIndex, uIndex, neighborList); //U-1, V-1
	incrementU(uIndex, vIndex);
	addIndexToNeighborList(vIndex, currentIndex, uIndex, neighborList); //U, V-1

	return neighborList;
}

void UGeodesicGrid::addIndexToNeighborList(int32 vIndex, int32 currentIndex, int32 uIndex, TArray<int32> &neighborList) const
{
	if (vIndex >= 0 && vIndex < RectilinearGrid[uIndex].Num())
	{
		if (currentIndex != RectilinearGrid[uIndex][vIndex])
		{
			neighborList.Add(RectilinearGrid[uIndex][vIndex]);
		}
	}
}

void UGeodesicGrid::decrementU(int32& uIndex, int32& vIndex) const
{
	--uIndex;
	if (uIndex%GridFrequency == 0)
	{
		vIndex += GridFrequency;
	}
	if (uIndex < 0)
	{
		uIndex += GridFrequency * 5;
	}
}

void UGeodesicGrid::incrementU(int32& uIndex, int32& vIndex) const
{
	++uIndex;
	if (uIndex%GridFrequency == 1)
	{
		vIndex -= GridFrequency;
	}
	if (uIndex == GridFrequency * 5)
	{
		uIndex = 0;
	}
}

FVector UGeodesicGrid::determineTriangleLocation(int32 localU, int32 localV, int32 Uref1, int32 Uref2, int32 Vref1, int32 Vref2) const
{
	FVector icosahedronLocation = referenceLocations[Uref1];
	FVector diagonalRefVector = referenceLocations[Uref2] - referenceLocations[Uref1];
	diagonalRefVector /= GridFrequency;
	FVector vRefVector = referenceLocations[Vref2] - referenceLocations[Vref1];
	vRefVector /= GridFrequency;
	icosahedronLocation += diagonalRefVector*localU + vRefVector*(localV - localU);
	return icosahedronLocation;
}


