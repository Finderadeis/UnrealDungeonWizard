// C 2024 Bianca Bellof @github.com/Finderadeis

#pragma once

#include "CoreMinimal.h"
#include "BaseTools/ClickDragTool.h"
#include <CustomTypes/DWDataAsset.h>
#include <CustomTypes/DWDungeonGrid2D.h>
#include "Engine/StaticMeshActor.h"

class DW_API DWGenerationMode
{
public:
	DWGenerationMode();

	virtual void Setup(UWorld* World, UDWDataAsset* PresetsAsset, TSharedPtr<DWDungeonGrid2D> Grid);
	virtual void Render(IToolsContextRenderAPI* RenderAPI);
	virtual void OnPropertyModified(UObject* PropertySet, FProperty* Property);

	virtual void OnClickPress(const FVector& Position);
	virtual void OnClickDrag(const FVector& Position);
	virtual void OnClickRelease(const FVector& Position);
	virtual void OnTerminateDragSequence();

	virtual void OnUpdateModifierState(int ModifierID, bool bIsOn);

protected:
	UWorld* TargetWorld = nullptr;
	FPrimitiveDrawInterface* PDI = nullptr;
	static const int ShiftModifierID = 1;
	static const int CtrlModifierID = 2;
	bool bShiftDown = false;
	bool bCtrlDown = false;
	UDWDataAsset* DungeonPresetsAsset;
	TSharedPtr<DWDungeonGrid2D> DungeonGrid;
	TArray<FIntVector2> MapDirections{ FIntVector2{1,0}, FIntVector2{-1,0} , FIntVector2{0,1} , FIntVector2{0,-1} };
	TArray<DWRoom> Rooms;
	FVector DragStartPos;
	FVector DragEndPos;

	virtual void Generate();
	virtual void ShuffleDirections();
};
