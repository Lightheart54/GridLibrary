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

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Gridding")
		int32 GetNumberOfElements() const;
		virtual int32 GetNumberOfElements_Implementation() const override;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Gridding")
		FVector GetIndexLocation(int32 gridIndex) const;
		FVector GetIndexLocation_Implementation(int32 gridIndex) const override;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Gridding")
		int32 GetLocationIndex(const FVector& location) const;
		int32 GetLocationIndex_Implementation(const FVector& location) const override;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Gridding")
		TArray<int32> GetIndexNeighbors(int32 gridIndex) const;
		TArray<int32> GetIndexNeighbors_Implementation(int32 gridIndex) const override;
	
};
