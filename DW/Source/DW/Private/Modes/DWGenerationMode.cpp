// C 2024 Bianca Bellof @github.com/Finderadeis


#include "Modes/DWGenerationMode.h"

DWGenerationMode::DWGenerationMode()
{
}

void DWGenerationMode::Setup(UWorld* World, UDWDataAsset* PresetsAsset, TSharedPtr<DWDungeonGrid2D> Grid)
{
	TargetWorld = World;
	DungeonPresetsAsset = PresetsAsset;
	DungeonGrid = Grid;
	Rooms.Empty();
	DragStartPos = FVector::ZeroVector;
	DragEndPos = FVector::ZeroVector;
}

void DWGenerationMode::Render(IToolsContextRenderAPI* RenderAPI)
{
	PDI = RenderAPI->GetPrimitiveDrawInterface();
}

void DWGenerationMode::OnPropertyModified(UObject* PropertySet, FProperty* Property)
{
	Generate();
}

void DWGenerationMode::OnClickPress(const FVector& Position)
{
	DragStartPos = Position;
	DragEndPos = Position;
}

void DWGenerationMode::OnClickDrag(const FVector& Position)
{
	DragEndPos = Position;
}

void DWGenerationMode::OnClickRelease(const FVector& Position)
{
}

void DWGenerationMode::OnTerminateDragSequence()
{
}

void DWGenerationMode::OnUpdateModifierState(int ModifierID, bool bIsOn)
{
	if (ModifierID == ShiftModifierID)
	{
		bShiftDown = bIsOn;
	}
	if (ModifierID == CtrlModifierID)
	{
		bCtrlDown = bIsOn;
	}
}

void DWGenerationMode::Generate()
{
}

void DWGenerationMode::ShuffleDirections()
{
	for (int32 i = 3; i > 0; i--)
	{
		int32 RandomIndex = FMath::RandRange(0, i);
		if (RandomIndex != i)
		{
			MapDirections.Swap(i, RandomIndex);
		}
	}
}
