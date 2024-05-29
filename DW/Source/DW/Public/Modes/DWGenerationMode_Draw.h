// C 2024 Bianca Bellof @github.com/Finderadeis

#pragma once

#include "CoreMinimal.h"
#include "Modes/DWGenerationMode.h"

class DW_API DWGenerationMode_Draw : public DWGenerationMode
{
public:
	DWGenerationMode_Draw();

	virtual void Setup(UWorld* World, UDWDataAsset* PresetsAsset, TSharedPtr<DWDungeonGrid2D> Grid) override;
	virtual void Render(IToolsContextRenderAPI* RenderAPI) override;
	virtual void OnPropertyModified(UObject* PropertySet, FProperty* Property) override;

	virtual void OnClickPress(const FVector& Position) override;
	virtual void OnClickDrag(const FVector& Position) override;
	virtual void OnClickRelease(const FVector& Position) override;
	virtual void OnTerminateDragSequence() override;

	virtual void OnUpdateModifierState(int ModifierID, bool bIsOn) override;

protected:
	virtual void Generate() override;

	void RenderGrid();
	void RenderDungeonOutline();
	void RenderDragRect(FColor Color);

	TArray<DWRoom> Rooms;
	void UpdateRooms();
	DWRoom* GetRoomAtPosition(const FIntVector2& Position);
	void ConnectRooms(DWRoom* RoomA, DWRoom* RoomB);
	void GeneratePath(FIntVector2 Start, FIntVector2 End, FIntVector2 PathMin, FIntVector2 PathMax);
	bool IsTileInRect(FIntVector2 Position, FIntVector2 Start, FIntVector2 End);
};
