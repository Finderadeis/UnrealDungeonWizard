// C 2024 Bianca Bellof @github.com/Finderadeis

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "GameFramework/Actor.h"
#include "DWDataAsset.generated.h"

UENUM()
enum class EDWGenerationMode : uint8
{
	EGenMode_PureMaze UMETA(DisplayName = "Pure Maze"),
	EGenMode_LinearRooms UMETA(DisplayName = "Linear Rooms"),
	EGenMode_DrawRooms UMETA(DisplayName = "Draw Layout"),
};
UCLASS()
class DW_API UDWDataAsset : public UPrimaryDataAsset
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Generation Mode", meta =
				(ToolTip = "Determines how dungeon will be created"))
	EDWGenerationMode GenerationMode;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Size Presets", meta =
			  (ToolTip = "Max dungeon width \n(number of tiles in X direction)"))
	int32 MaxWidth;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Size Presets", meta =
			  (ToolTip = "Max dungeon depth \n(number of tiles in Y direction)"))
	int32 MaxDepth;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Size Presets", meta =
			  (ToolTip = "Size of used tile meshes \nCompare 1 standard cube = 100"))
	float MeshSize;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Asset Presets", meta =
			  (ToolTip = "All static meshes to randomly select from for floors during generation"))
	TArray<UStaticMesh*> FloorMeshes;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Asset Presets", meta =
			  (ToolTip = "All static meshes to randomly select from for walls during generation"))
	TArray<UStaticMesh*> WallMeshes;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Asset Presets", meta =
			  (ToolTip = "How many meshes to combine for one wall (Height)"))
	int32 WallUnits;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Asset Presets", meta =
			  (ToolTip = "Add Wallconnector meshes on wall edges"))
	bool bUseWallConnectors;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Asset Presets", meta =
			  (ToolTip = "All static meshes to randomly select from for wall connectors during generation."
			  , EditCondition = "bUseWallConnectors"))
	TArray<UStaticMesh*> WallConnectorMeshes;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Asset Presets", meta =
			  (ToolTip = "All static meshes to randomly select from for ceiling during generation"))
	TArray<UStaticMesh*> CeilingMeshes;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Asset Presets", meta =
			  (ToolTip = "Add Ceiling meshes"))
	bool bAddCeiling;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Asset Presets", meta =
			  (ToolTip = "All Blueprints to randomly select from for wall mounted light sources."
			   , EditCondition = "GenerationMode == EDWGenerationMode::EGenMode_DrawRooms"))
	TArray<TSubclassOf<AActor>> WallLightSources;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Asset Presets", meta =
			  (ToolTip = "Wall light offset (so your lightsource does not stick inside the wall)"))
	float WallLightOffset;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Asset Presets", meta =
			  (ToolTip = "Average Per Wall Propability of spawning Wall Light Source\n0 to 100 percent"
			   , ClampMin=0, ClampMax=100))
	int WallLightChance;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Rooms Presets", meta =
			  (ToolTip = "How many rooms to generate"
			   , EditCondition = "GenerationMode == EDWGenerationMode::EGenMode_LinearRooms"))
	int32 NumberOfRooms;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Rooms Presets", meta =
			  (ToolTip = "Min edge length for room generation"
			   , EditCondition = "GenerationMode == EDWGenerationMode::EGenMode_LinearRooms"))
	int32 MinRoomSize;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Rooms Presets", meta =
			  (ToolTip = "Min edge length for room generation"
			   , EditCondition = "GenerationMode == EDWGenerationMode::EGenMode_LinearRooms"))
	int32 MaxRoomSize;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Rooms Presets", meta =
			  (ToolTip = "Min distance between rooms"
			   , EditCondition = "GenerationMode == EDWGenerationMode::EGenMode_LinearRooms"))
	int32 MinRoomDistance;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Rooms Presets", meta =
			  (ToolTip = "Max distance between rooms"
			   , EditCondition = "GenerationMode == EDWGenerationMode::EGenMode_LinearRooms"))
	int32 MaxRoomDistance;
};
