// Copyright 2016 Cullen Sarles, All rights resevered

#include "LightWeightGridPluginPCH.h"
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
	icosahedronInteriorAngle = 0.0f;
#ifdef UE_BUILD_DEBUG
	DebugDisplayDuration = 20.0f;
	displayDebugLines = false;
#endif

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
	
	 //find the reference vector indexes
	int32 uLocal = 0, vLocal = 0,
		uRef1 = 0, uRef2 = 0,
		vRef1 = 0, vRef2 = 0;
	DetermineReferenceIndexes(uIndex, vIndex, uLocal, vLocal,
		uRef1, uRef2, vRef1, vRef2);

	FVector icosahedronLocation = determineTriangleLocation(uLocal, vLocal, uRef1, uRef2, vRef1, vRef2);
	return projectVectorOntoSphere(icosahedronLocation);
}

int32 UGeodesicGrid::GetLocationIndex_Implementation(const FVector& location) const
{
	//start by normalizing the position
	FVector locationOnUnitSphere = location / FMath::Sqrt(FVector::DotProduct(location, location));
	//find the three closest reference points
	TArray<int32> referenceIndexes; //Man I miss initializer lists
	referenceIndexes.SetNumZeroed(12);
	referenceIndexes[0] = 0;
	referenceIndexes[1] = 1;
	referenceIndexes[2] = 2;
	referenceIndexes[3] = 3;
	referenceIndexes[4] = 4;
	referenceIndexes[5] = 5;
	referenceIndexes[6] = 6;
	referenceIndexes[7] = 7;
	referenceIndexes[8] = 8;
	referenceIndexes[9] = 9;
	referenceIndexes[10] = 10; 
	referenceIndexes[11] = 11;

	referenceIndexes.Sort([&](const int32& pos1, const int32& pos2)->bool
	{
		return FVector::DotProduct(locationOnUnitSphere, referenceLocations[pos1])
					> FVector::DotProduct(locationOnUnitSphere, referenceLocations[pos2]);
	});

	//if we're within a the icosahedron interior angle / (2* GridFrequency) of the closest point, we're at that point
	if (FMath::Acos(FVector::DotProduct(locationOnUnitSphere, referenceLocations[referenceIndexes[0]])) <= (icosahedronInteriorAngle/(2* GridFrequency)))
	{
		return referenceIndexes[0];
	}

	referenceIndexes.RemoveAt(3, 9);
	referenceIndexes.Sort(); //sort so that 10 and 11 are at the end, if we have them we need to do special things

	int32 Uref1 = 0, Uref2 = 0, Vref1 = 0, Vref2 = 0;
	DetermineReferenceIndexesForLocationMapping(referenceIndexes, Uref1, Uref2, Vref1, Vref2);
#ifdef UE_BUILD_DEBUG
	if (displayDebugLines)
	{
		DrawDebugPoint(GetWorld(), GetIndexLocation_Implementation(Uref1), 20.0f, FColor::Cyan, false, DebugDisplayDuration);
		DrawDebugPoint(GetWorld(), GetIndexLocation_Implementation(Uref2), 20.0f, FColor::Cyan, false, DebugDisplayDuration);
		DrawDebugPoint(GetWorld(), GetIndexLocation_Implementation(Vref1)*1.05, 20.0f, FColor::Blue, false, DebugDisplayDuration);
		DrawDebugPoint(GetWorld(), GetIndexLocation_Implementation(Vref2)*1.05, 20.0f, FColor::Blue, false, DebugDisplayDuration);
	}
#endif
	
	//Now we need to determine the localU and localV
	FVector uReferenceVector = referenceLocations[Uref2] - referenceLocations[Uref1];
	float uReferenceVectorLength = FMath::Sqrt(FVector::DotProduct(uReferenceVector, uReferenceVector));
	FVector uReferenceVectorDirection = uReferenceVector / uReferenceVectorLength;
	float uBasisLength = uReferenceVectorLength / GridFrequency;
	FVector uBasis = uReferenceVectorDirection*uBasisLength;
	FVector vReferenceVector = referenceLocations[Vref2] - referenceLocations[Vref1];
	float vReferenceVectorLength = FMath::Sqrt(FVector::DotProduct(vReferenceVector, vReferenceVector));
	FVector vReferenceVectorDirection = vReferenceVector / vReferenceVectorLength;
	float vBasisLength = vReferenceVectorLength / GridFrequency;
	FVector vBasis = vReferenceVectorDirection*vBasisLength;

	FVector icosahedronVector = projectVectorOntoIcosahedronFace(locationOnUnitSphere, referenceLocations[Uref1],
		uReferenceVectorDirection, vReferenceVectorDirection);
	FVector localVectorOnFace = icosahedronVector - referenceLocations[Uref1];

#ifdef UE_BUILD_DEBUG
	if (displayDebugLines)
	{
		DrawDebugPoint(GetWorld(), projectVectorOntoSphere(icosahedronVector), 20.0f, FColor::Red, false, DebugDisplayDuration);
		DrawDebugLine(GetWorld(), GetIndexLocation_Implementation(Uref1), projectVectorOntoSphere(icosahedronVector), FColor::Emerald, false, DebugDisplayDuration, 0, 1.0f);
		DrawDebugLine(GetWorld(), GetIndexLocation_Implementation(Uref1),
			GetIndexLocation_Implementation(Uref1) + (GetIndexLocation_Implementation(Uref2) - GetIndexLocation_Implementation(Uref1)) / GridFrequency,
			FColor::White, false, DebugDisplayDuration, 0, 1.0f);
		DrawDebugLine(GetWorld(), GetIndexLocation_Implementation(Uref1),
			GetIndexLocation_Implementation(Uref1) + (GetIndexLocation_Implementation(Vref2) - GetIndexLocation_Implementation(Vref1)) / GridFrequency,
			FColor::Yellow, false, DebugDisplayDuration, 0, 1.0f);
	}
#endif
	/* need to map the localvectorOnFace into the non-orthononormal basis
	 * h(U*U) + k(U*V) = W * U
	 * h(U*V) + k(V*V) = W * V
	 * with Cramer's rule we get
	 * h = ((W * U)(V*V) - (U*V)(W*V))/((U*U)(V*V) - (U*V)^2)
	 * v = ((W * V)(U*U) - (U*V)(W*U))/((U*U)(V*V) - (U*V)^2)
	 */
	float wDotU = FVector::DotProduct(localVectorOnFace, uBasis);
	float wDotV = FVector::DotProduct(localVectorOnFace, vBasis);
	float uDotV = FVector::DotProduct(uBasis, vBasis);
	float uDotU = FVector::DotProduct(uBasis, uBasis);
	float vDotV = FVector::DotProduct(vBasis, vBasis);
	float h = (wDotU*vDotV - uDotV*wDotV) / (uDotU*vDotV - uDotV*uDotV);
	float v = (wDotV*uDotU - uDotV*wDotU) / (uDotU*vDotV - uDotV*uDotV);

#ifdef UE_BUILD_DEBUG
	if (displayDebugLines)
	{
		DrawDebugLine(GetWorld(), GetIndexLocation_Implementation(Uref1),
				GetIndexLocation_Implementation(Uref1)
				+ h*((GetIndexLocation_Implementation(Uref2) - GetIndexLocation_Implementation(Uref1)) / GridFrequency)
				+ v*((GetIndexLocation_Implementation(Vref2) - GetIndexLocation_Implementation(Vref1)) / GridFrequency),
				FColor::Magenta, false, DebugDisplayDuration, 0, 2.0f);
	}
#endif


	int32 LocalUIndex = FMath::RoundToInt(h / uReferenceVectorLength);
	int32 LocalVIndex = FMath::RoundToInt(v / vReferenceVectorLength);

	//now find the actual u and v index
	int32 UIndex = UVLocationList[Uref1][0];
	int32 VIndex = UVLocationList[Uref1][1];
	for (int32 i = 0 ; i < LocalUIndex; ++i)
	{
		incrementU(UIndex, VIndex);
		++VIndex;
	}
	VIndex += LocalVIndex;

	return RectilinearGrid[UIndex][VIndex];
}

void UGeodesicGrid::DetermineReferenceIndexesForLocationMapping(const TArray<int32>& refenceIndexes,
	int32 &Uref1, int32 &Uref2, int32 &Vref1, int32 &Vref2 ) const
{
	if (refenceIndexes[2] != 10 && refenceIndexes[2] != 11)
	{
		//We're in a cleanly defined triangle
		//find the points that share a uIndex
		//Because of the way the points are ordered in the beginning, the u grouped points will be adjacent in the list
		if (UVLocationList[refenceIndexes[0]][0] == UVLocationList[refenceIndexes[1]][0])
		{
			Vref1 = refenceIndexes[0];
			Vref2 = refenceIndexes[1];
			if (refenceIndexes[2] != 9) //Handle Wrap Around
			{
				Uref1 = Vref1;
				Uref2 = refenceIndexes[2];
			}
			else
			{
				Uref1 = refenceIndexes[2];
				Uref2 = Vref2;
			}
		}
		else if (UVLocationList[refenceIndexes[1]][0] == UVLocationList[refenceIndexes[2]][0])
		{
			Vref1 = refenceIndexes[1];
			Vref2 = refenceIndexes[2];
			if (refenceIndexes[0] != 0)  //Handle Wrap Around
			{
				Uref1 = refenceIndexes[0];
				Uref2 = Vref2;
			}
			else
			{
				Uref1 = Vref1;
				Uref2 = refenceIndexes[0];
			}
		}
	}
	else
	{
		if (refenceIndexes[2] == 10)
		{
			if (refenceIndexes[0] == 0 && refenceIndexes[1] == 8)  //Handle Wrap Around
			{
				Uref1 = 8;
				Uref2 = 0;
				Vref1 = 10;
				Vref2 = 0;
			}
			else
			{
				Uref1 = refenceIndexes[0];
				Uref2 = refenceIndexes[1];
				Vref1 = 10;
				Vref2 = refenceIndexes[1];
			}
		}
		else //refenceIndexes[2] == 11
		{
			if (refenceIndexes[0] == 1 && refenceIndexes[1] == 9)  //Handle Wrap Around
			{
				Uref1 = 9;
				Uref2 = 1;
				Vref1 = 9;
				Vref2 = 11;
			}
			else
			{
				Uref1 = refenceIndexes[0];
				Uref2 = refenceIndexes[1];
				Vref1 = refenceIndexes[0];
				Vref2 = 11;
			}
		}
	}
}

FVector UGeodesicGrid::projectVectorOntoIcosahedronFace(const FVector& positionOnSphere, const FVector& refPoint, const FVector& uDir, const FVector& vDir) const
{
	FVector planeVec = FVector::CrossProduct(uDir, vDir);
	planeVec /= FMath::Sqrt(FVector::DotProduct(planeVec, planeVec));
	FVector offsetVector = positionOnSphere - refPoint;
	float offsetNormalProjection = FVector::DotProduct(offsetVector, planeVec);
	return positionOnSphere - offsetNormalProjection*planeVec;
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
	Super::PostEditChangeProperty(PropertyChangedEvent);
}

void UGeodesicGrid::buildGrid()
{
	// update the number of Vertexes
	NumberOfVertexes = 10 * (GridFrequency * GridFrequency) + 2;
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

	for (int32 GridColumn = 1; GridColumn < 5* GridFrequency; ++GridColumn)
	{
		//We're in one of the reference columns we've already taken care of it
		if (GridColumn % GridFrequency == 0) 
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
	x *= pointMagnitudeRatio;
	z *= pointMagnitudeRatio;

	referenceLocations.Reset(12);
	referenceLocations.SetNumZeroed(12);

	//Bottom Point
	referenceLocations[10][0] = -z;
	referenceLocations[10][1] = 0;
	referenceLocations[10][2] = -x;

	//Point 0
	//-y, -z
	referenceLocations[0][0] = 0;
	referenceLocations[0][1] = -x;
	referenceLocations[0][2] = -z;

	//Point 2
	//-x,-y
	referenceLocations[2][0] = -x;
	referenceLocations[2][1] = -z;
	referenceLocations[2][2] = 0;

	//Point 4
	//-x,+z
	referenceLocations[4][0] = -z;
	referenceLocations[4][1] = 0;
	referenceLocations[4][2] = x;

	//Point 6
	//-x,+y
	referenceLocations[6][0] = -x;
	referenceLocations[6][1] = z;
	referenceLocations[6][2] = 0;

	//Point 8
	//+y,-z
	referenceLocations[8][0] = 0;
	referenceLocations[8][1] = x;
	referenceLocations[8][2] = -z;

	//Top Point
	referenceLocations[11][0] = z;
	referenceLocations[11][1] = 0;
	referenceLocations[11][2] = x;

	//Point 1
	//+x,-y
	referenceLocations[1][0] = x;
	referenceLocations[1][1] = -z;
	referenceLocations[1][2] = 0;

	//Point 3
	//-y,+z
	referenceLocations[3][0] = 0;
	referenceLocations[3][1] = -x;
	referenceLocations[3][2] = z;

	//Point 5
	//+y,+z
	referenceLocations[5][0] = 0;
	referenceLocations[5][1] = x;
	referenceLocations[5][2] = z;

	//Point 7
	//+x,+y
	referenceLocations[7][0] = x;
	referenceLocations[7][1] = z;
	referenceLocations[7][2] = 0;

	//Point 9
	//+x,-z
	referenceLocations[9][0] = z;
	referenceLocations[9][1] = 0;
	referenceLocations[9][2] = -x;

	icosahedronInteriorAngle = FMath::Acos(FVector::DotProduct(referenceLocations[0], referenceLocations[1]));
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

void UGeodesicGrid::DetermineReferenceIndexes(int32 uIndex, int32 vIndex, int32& uLocal, int32& vLocal,
	int32& uRef1, int32& uRef2, int32& vRef1, int32& vRef2) const
{
	//10*(f^2) + 2
	//we need to make sure we're mapping to an actual triangle in on the grid
	//Example f = 3
	/*                                                         [9]
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

	//Start with the Column Index
	int32 uRefIndex1 = (uIndex / GridFrequency)*GridFrequency; // The reference column number prior to/at this index

	 //if we're in the bottom indexes of a reference column, 
	//we need to reference to prior reference column as column one
	if (vIndex < GridFrequency && uRefIndex1 == uIndex)
	{
		uRefIndex1 -= GridFrequency;
	}
	int32 uRefIndex2 = uRefIndex1 + GridFrequency; // The reference column number after this index

										 // we might be wrapping around the grid
	if (uRefIndex2 == 5 * GridFrequency)
	{
		uRefIndex2 = 0; // Wrap Around
	}
	else if (uRefIndex1 < 0)
	{
		uRefIndex1 = 4 * GridFrequency;
	}

	//Now Find the Row Indexes
	int32 vRefIndex21 = (vIndex / GridFrequency) * GridFrequency;
	if (vRefIndex21 == vIndex // We're at the "Top of a Triangle"
		&& (vRefIndex21 != 0 //If we're not in the very bottom row
		&& (vRefIndex21 != GridFrequency && uRefIndex1 == uIndex))) //And especially not the bottom leftmost index in a section
	{
		//Reference the Bottom of the Triangle
		vRefIndex21 -= GridFrequency;
	}

	//We're in the first column of a Triangle
	if (uRefIndex1 == uIndex)
	{
		//So the Second column index is a grid frequency below where we're at
		vRefIndex21 -= GridFrequency;
	}
	//The first column reference is a grid frequency above we're we're at
	int32 vRefIndex11 = vRefIndex21 + GridFrequency;

	//if we've done the above logic correctly such that vRef*1 is the lower corner,
	//then these are simple
	int32 vRefIndex12 = vRefIndex11 + GridFrequency;
	int32 vRefIndex22 = vRefIndex21 + GridFrequency;

	//Determine the Local U and V offsets from URefIndex1,VRefIndex11
	uLocal = uIndex - uRefIndex1;
	if (uIndex == uRefIndex2)
	{
		//we're in the last column
		uLocal = GridFrequency;
	}
	vLocal = 0;
	if (uLocal == 0)
	{
		vLocal = vIndex - vRefIndex11;
	}
	else
	{
		vLocal = vIndex - vRefIndex21;
	}

	//Retrieve the actual index numbers for the reference indexes
	//Start with the diagonal direction
	uRef1 = RectilinearGrid[uRefIndex1][vRefIndex11];
	uRef2 = RectilinearGrid[uRefIndex2][vRefIndex22];

	//Then find the "Vertical" Depending upon if we
	//are in an upper or lower triangle
	bool upperTriangle = vLocal > uLocal //above the diagonal
		|| vIndex == RectilinearGrid[uIndex].Num() - 1; // At the Top of the section
	if (upperTriangle)
	{
		vRef1 = uRef1;
		vRef2 = RectilinearGrid[uRefIndex1][vRefIndex12];
	}
	else
	{
		vRef1 = RectilinearGrid[uRefIndex2][vRefIndex21];
		vRef2 = uRef2;
	}
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
	if (uIndex%GridFrequency == 0)
	{
		vIndex -= GridFrequency;
	}
	++uIndex;
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

FVector UGeodesicGrid::projectVectorOntoSphere(const FVector& icosahedronLocation) const
{
	//Central Projection
	float baseVectorLength = FVector::DotProduct(icosahedronLocation, icosahedronLocation);
	baseVectorLength = FMath::Sqrt(baseVectorLength);
	return icosahedronLocation * GridRadius / baseVectorLength;
}

