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

	/*
	 * Subdivides the Grid.
	 * Existing grid vertex numbers are invalidated.
	 */
	UFUNCTION(BlueprintCallable, Category = "Grid Properties")
	void IncrementFrequency();

	/*
	* Removes a subdivision from the Grid. This function does nothing if 
	* the frequency is odd or 0. Existing grid vertex numbers are preserved.
	*/
	UFUNCTION(BlueprintCallable, Category = "Grid Properties")
	void DecrementFrequency();

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

private:
	/* Constructs a Geodesic Grid With the Specified Frequency */
	void buildGrid();
	void buildIcosahedronRefernceLocations();

	TArray<TArray<int32>> RectilinearGrid;
	TArray<FVector> referenceLocations;
	TArray<TArray<int32>> UVLocationList;
	
	void populateRefernceColumn(int32 GridColumn, int32& currentIndexNumber);
	void populateGridColumn(int32 GridColumn, int32& currentIndexNumber);
	void AssignNewIndexNumber(int32 GridColumn, int32 RowNumber, int32 currentIndexNumber);

};
