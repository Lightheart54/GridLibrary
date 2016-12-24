// Copyright 2016 Cullen Sarles All Rights Reserved.

#pragma once
#include "LightWeightGridInterface.generated.h"

/**
 * The LightWeightGridInterface provides an interface for accessing different grid types
 * Specifically Methods are provided for:
 *  - Translating to and from a grid index to world space
 *  - Retrieving the Neighboring Indexes for a given Index
 *  - Specifying the reference point (world 3d space) for the grid
 */
UINTERFACE(Blueprintable)
class LIGHTWEIGHTGRIDLIBRARY_API ULightWeightGridInterface : public UInterface
{
	GENERATED_UINTERFACE_BODY()
};

class LIGHTWEIGHTGRIDLIBRARY_API ILightWeightGridInterface
{
	GENERATED_IINTERFACE_BODY()
public:
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Gridding")
		int32 GetNumberOfElements() const;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Gridding")
		FVector GetIndexLocation(int32 gridIndex) const;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Gridding")
		int32 GetLocationIndex(const FVector& location) const;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Gridding")
		TArray<int32> GetIndexNeighbors(int32 gridIndex) const;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Gridding")
		void SetGridTransform(const FTransform& newTransform);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Gridding")
		void SetGridLocation(const FVector& newLocation);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Gridding")
		FTransform GetGridTransform() const;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Gridding")
		FVector GetGridLocation() const;
};
