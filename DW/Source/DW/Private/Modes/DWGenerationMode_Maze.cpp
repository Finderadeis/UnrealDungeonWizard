// C 2024 Bianca Bellof @github.com/Finderadeis


#include "Modes/DWGenerationMode_Maze.h"

DWGenerationMode_Maze::DWGenerationMode_Maze()
{
}

void DWGenerationMode_Maze::Setup(UWorld* World, UDWDataAsset* PresetsAsset, TSharedPtr<DWDungeonGrid2D> Grid)
{
	DWGenerationMode::Setup(World, PresetsAsset, Grid);
}

void DWGenerationMode_Maze::Render(IToolsContextRenderAPI* RenderAPI)
{
	DWGenerationMode::Render(RenderAPI);
}

void DWGenerationMode_Maze::OnPropertyModified(UObject* PropertySet, FProperty* Property)
{
	DWGenerationMode::OnPropertyModified(PropertySet, Property);
}

void DWGenerationMode_Maze::OnClickPress(const FVector& Position)
{
	DWGenerationMode::OnClickPress(Position);
}

void DWGenerationMode_Maze::OnClickDrag(const FVector& Position)
{
	DWGenerationMode::OnClickDrag(Position);
}

void DWGenerationMode_Maze::OnClickRelease(const FVector& Position)
{
	DWGenerationMode::OnClickRelease(Position);
}

void DWGenerationMode_Maze::OnTerminateDragSequence()
{
	DWGenerationMode::OnTerminateDragSequence();
}

void DWGenerationMode_Maze::OnUpdateModifierState(int ModifierID, bool bIsOn)
{
	DWGenerationMode::OnUpdateModifierState(ModifierID, bIsOn);
}

void DWGenerationMode_Maze::Generate()
{
	RecursionDepth = 0;
	DungeonGrid->Reset(DungeonPresetsAsset->MaxWidth, DungeonPresetsAsset->MaxDepth);
	int StartX = DungeonPresetsAsset->MaxWidth / 2;
	int StartY = DungeonPresetsAsset->MaxDepth / 2;
	GenerateRecursive(StartX, StartY);
}

void DWGenerationMode_Maze::GenerateRecursive(int X, int Y)
{
	RecursionDepth++;
	if (RecursionDepth > 100000)
	{
		UE_LOG(LogTemp, Warning, TEXT("Something went wrong. RecursionDepth limit reached"));
		return;
	}
	if (DungeonGrid->CountCardinalNeighbours(FIntVector2(X, Y)) >= 2)
	{
		return;
	}

	DungeonGrid->SetValue(FIntVector2(X, Y), EDWGridCellType::ECellType_Floor);
	ShuffleDirections();
	for (FIntVector2 Dir : MapDirections)
	{
		int NewX = X + Dir.X;
		int NewY = Y + Dir.Y;
		GenerateRecursive(NewX, NewY);
	}
}
