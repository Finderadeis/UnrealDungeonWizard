// C 2024 Bianca Bellof @github.com/Finderadeis

#pragma once

#include "CoreMinimal.h"
#include "InteractiveTool.h"
#include "InteractiveToolBuilder.h"
#include "BaseTools/ClickDragTool.h"
#include <CustomTypes/DWDataAsset.h>
#include <CustomTypes/DWDungeonGrid2D.h>
#include "Engine/StaticMeshActor.h"
#include "Modes/DWGenerationMode.h"
#include "DWBaseTool.generated.h"


/**
 * Builder class for UDWBaseTool
 */

UCLASS()
class DW_API UDWBaseToolBuilder : public UInteractiveToolBuilder
{
	GENERATED_BODY()
	
public:
	virtual bool CanBuildTool(const FToolBuilderState& SceneState) const override { return true; }
	virtual UInteractiveTool* BuildTool(const FToolBuilderState& SceneState) const override;
};


/**
 * Property class for UDWBaseTool
 */

UCLASS(Transient)
class DW_API UDWBaseToolProperties : public UInteractiveToolPropertySet
{
	GENERATED_BODY()
public:
	UDWBaseToolProperties();

	UPROPERTY(EditAnywhere, Category = Options)
	TSubclassOf<UDWDataAsset> DungeonPresetsAsset;
};


/**
 * Interactive tool class for UDWBaseTool
 */

UCLASS()
class DW_API UDWBaseTool : public UInteractiveTool, public IClickDragBehaviorTarget
{
	GENERATED_BODY()

public:
	virtual void SetWorld(UWorld* World);

	/** UInteractiveTool overrides */
	virtual void Setup() override;
	virtual void Render(IToolsContextRenderAPI* RenderAPI) override;
	virtual void OnPropertyModified(UObject* PropertySet, FProperty* Property) override;

	/** IClickDragBehaviorTarget implementation */
	virtual FInputRayHit CanBeginClickDragSequence(const FInputDeviceRay& PressPos) override;
	virtual void OnClickPress(const FInputDeviceRay& PressPos) override;
	virtual void OnClickDrag(const FInputDeviceRay& DragPos) override;
	virtual void OnClickRelease(const FInputDeviceRay& ReleasePos) override;
	virtual void OnTerminateDragSequence() override;

	/** IModifierToggleBehaviorTarget implementation (inherited via IClickDragBehaviorTarget) */
	virtual void OnUpdateModifierState(int ModifierID, bool bIsOn) override;


protected:
	UWorld* TargetWorld = nullptr;
	TSharedPtr<DWGenerationMode> GenerationMode = nullptr;
	const FString BaseMeshPath = "/DW/SM_DrawBase";

	static const int ShiftModifierID = 1;
	static const int CtrlModifierID = 2;
	bool bShiftDown = false;
	bool bCtrlDown = false;

	UPROPERTY()
	TObjectPtr<UDWBaseToolProperties> Properties;
	UDWDataAsset* DungeonPresetsAsset;
	TSharedPtr<class FSlateStyleSet> StyleSet;
	TSharedPtr<DWDungeonGrid2D> DungeonGrid;

	AStaticMeshActor* DrawBaseActor;
	TArray<AStaticMeshActor*> SpawnedFloorTiles;
	TArray<AStaticMeshActor*> SpawnedWallTiles;
	AActor* ActorToSpawn;
	TArray<AActor*> SpawnedActors;

	FVector DragStartPos;
	FVector DragEndPos;

	FName OutlinerFolderName = "/Dungeon";

	FInputRayHit FindRayHit(const FRay& Ray, FVector& OutHitPoint);
	FInputRayHit FindRayHitOnGrid(const FRay& Ray, FVector& OutHitPoint, float GridSize);

	void SpawnDungeon();
	void ClearSpawnedActors();

	AStaticMeshActor* SpawnMeshActor(UStaticMesh* Mesh, FVector Location, FRotator Rotation, FVector Scale, FName Name, FString Folder = "NONE");
	AActor* SpawnActor(UClass* ActorClass, FVector Location, FRotator Rotation, FName Name, FString Folder = "NONE");
};