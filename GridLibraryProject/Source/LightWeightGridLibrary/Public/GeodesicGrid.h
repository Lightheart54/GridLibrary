// Copyright 2016 Cullen Sarles, All rights resevered

#pragma once

#include "Components/SceneComponent.h"
#include "LightWeightGridInterface.h"
#include "GeodesicGrid.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class LIGHTWEIGHTGRIDLIBRARY_API UGeodesicGrid : public USceneComponent, public ILightWeightGridInterface
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UGeodesicGrid();

	// Called when the game starts
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void TickComponent( float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction ) override;

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
	 * This rebuilds the grid, so any prior references to it are now invalid
	 */
	UFUNCTION(BlueprintCallable, Category = "Grid Properties")
		void SetGridFrequency(int32 newFrequency);

	/* Gets the Grid Radius*/
	UFUNCTION(BlueprintCallable, Category = "Grid Properties")
		float GetGridRadius() const;

	/* Sets the Grid Radius.
	* This rebuilds the grid, so any prior references to it are now invalid
	*/
	UFUNCTION(BlueprintCallable, Category = "Grid Properties")
		void SetGridRadius(float newRadius);

#if WITH_EDITOR
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
#endif

protected:
	/* The Number of Vertexes Between the Base Icosahedron Vertexes */
	UPROPERTY(EditAnywhere, Category = "Grid Properties", meta = (ClampMin = 0, UIMin = 0))
		int32 GridFrequency;

	/* The Radius of the Grid's Circumscribing Sphere*/
	UPROPERTY(EditAnywhere, Category = "Grid Properties", meta = (ClampMin = 0.0, UIMin = 0.0))
		float GridRadius;

private:
	/* Constructs a Geodesic Grid With the Specified Frequency */
	void buildGrid();
};
