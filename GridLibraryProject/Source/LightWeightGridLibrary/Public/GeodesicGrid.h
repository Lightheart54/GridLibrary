// Copyright 2016 Cullen Sarles, All rights resevered

#pragma once

#include "Components/SceneComponent.h"
#include "LightWeightGridInterface.h"
#include "GeodesicGrid.generated.h"

/*
 * A Geodesic Grid of a Sphere with a specified radius and frequency
 * The grid is based upon the Self-Organizing Map as presented in:
 * Wu, Yingxin and Masahio Takatsuka. "Spherical self-organizing map using efficient indexed geodesic data structure"
 *    Elsevier, Neural Networks 19, 2006, 900-910
 * See Documentation for more information concerning the grid structure
 */
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class LIGHTWEIGHTGRIDLIBRARY_API UGeodesicGrid : public USceneComponent, public ILightWeightGridInterface
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UGeodesicGrid();
		
	// See ILightWeightGridInterface::GetNumbeOfElements
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Grid Properties")
		int32 GetNumberOfElements() const;
		virtual int32 GetNumberOfElements_Implementation() const override;

	// See ILightWeightGridInterface::GetIndexLocation
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Grid Properties")
		FVector GetIndexLocation(int32 gridIndex) const;
		FVector GetIndexLocation_Implementation(int32 gridIndex) const override;

		

	// See ILightWeightGridInterface::GetLocationIndex
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Grid Properties")
		int32 GetLocationIndex(const FVector& location) const;
		int32 GetLocationIndex_Implementation(const FVector& location) const override;
		
	// See ILightWeightGridInterface::GetIndexNeighbors
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Grid Properties")
		TArray<int32> GetIndexNeighbors(int32 gridIndex) const;
		TArray<int32> GetIndexNeighbors_Implementation(int32 gridIndex) const override;
	
	/* Gets the Grid Frequency*/
	UFUNCTION(BlueprintCallable, Category = "Grid Properties")
		int32 GetGridFrequency() const;

	/* Sets the Grid Frequency.
	 * This rebuilds the grid. Existing grid vertex numbers are invalidated.
	 */
	UFUNCTION(BlueprintCallable, Category = "Grid Properties")
		void SetGridFrequency(int32 newFrequency);

	/* Gets the Grid Radius*/
	UFUNCTION(BlueprintCallable, Category = "Grid Properties")
		float GetGridRadius() const;

	/* Sets the Grid Radius.
	* This changes the grid locations. Previously Retrieve Grid Locations are invalidated.
	*/
	UFUNCTION(BlueprintCallable, Category = "Grid Properties")
		void SetGridRadius(float newRadius);

#if WITH_EDITOR
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
#endif

protected:
	/* The Number of Vertexes Between the Base Icosahedron Vertexes */
	UPROPERTY(EditAnywhere, Category = "Grid Properties", meta = (ClampMin = 1, UIMin = 1))
		int32 GridFrequency;

	/* The Radius of the Grid's Circumscribing Sphere*/
	UPROPERTY(EditAnywhere, Category = "Grid Properties", meta = (ClampMin = 0.1, UIMin = 0.1))
		float GridRadius;
	
	/* The number of Vertexes in the grid */
	UPROPERTY(VisibleAnywhere, Category = "Grid Properties")
		int32 NumberOfVertexes;

#ifdef UE_BUILD_DEBUG
	UPROPERTY(EditAnywhere, Category = "Debug Properties", meta = (ClampMin = 0.1, UIMin = 0.1))
		float DebugDisplayDuration;
#endif

private:
	float icosahedronInteriorAngle;

	/* Constructs a Geodesic Grid With the Specified Frequency */
	void buildGrid();
	void buildIcosahedronRefernceLocations();

	TArray<TArray<int32>> RectilinearGrid;
	TArray<FVector> referenceLocations;
	TArray<TArray<int32>> UVLocationList; //Each List is 2*n long, where n is the number of occurances of an index
	
	void populateRefernceColumn(const int32& GridColumn, int32& currentIndexNumber);
	void populateGridColumn(const int32& GridColumn, int32& currentIndexNumber);
	void AssignNewIndexNumber(const int32& GridColumn, const int32& RowNumber,
		const int32& currentIndexNumber);

	void DetermineReferenceIndexes(int32 uIndex, int32 vIndex, int32& uLocal, int32& vLocal,
		int32& uRef1, int32& uRef2, int32& vRef1, int32& vRef2) const;
	FVector determineTriangleLocation(int32 localU, int32 localV,
		int32 Uref1, int32 Uref2, int32 Vref1, int32 Vref2) const;
	FVector projectVectorOntoSphere(const FVector& icosahedronLocation) const;
	
	void DetermineReferenceIndexesForLocationMapping(const TArray<int32>& refenceIndexes,
		int32 &Uref1, int32 &Uref2, int32 &Vref1, int32 &Vref2) const;
	FVector projectVectorOntoIcosahedronFace(const FVector& positionOnSphere,
		const FVector& refPoint, const FVector& uDir, const FVector& vDir) const;

	TArray<int32> getLocationNeighbors(int32 uIndex, int32 vIndex) const;
	void addIndexToNeighborList(int32 vIndex, int32 currentIndex,
		int32 uIndex, TArray<int32> &neighborList) const;

	void decrementU(int32& uIndex, int32& vIndex) const;
	void incrementU(int32& uIndex, int32& vIndex) const;
	
};
