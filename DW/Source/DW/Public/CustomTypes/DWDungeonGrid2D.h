// C 2024 Bianca Bellof @github.com/Finderadeis

#pragma once

#include "CoreMinimal.h"

struct DW_API DWRoom
{
	TArray<FIntVector2> Tiles;
	TArray<FIntVector2> ExitPoints;
};

UENUM()
enum class EDWGridCellType : uint8
{
	ECellType_Invalid,
	ECellType_Empty,
	ECellType_Floor,
};

class DW_API DWDungeonGrid2D
{
public:
	DWDungeonGrid2D();
	DWDungeonGrid2D(int Width, int Depth);

	void Reset(int NewWidth, int NewDepth);

	int GetSize();
	int GetWidth();
	int GetDepth();
	int GetIndex(FIntVector2 Position);
	int GetIndexFromWorldPosition(const FVector& WorldPosition, const float& TileSize);
	FIntVector2 GetPosition(int Index);

	bool IsInBounds(FIntVector2 Position);
	bool IsInBounds(int Index);

	bool SetValue(FIntVector2 Position, EDWGridCellType Value);
	bool SetValue(int Index, EDWGridCellType Value);

	bool FillRect(FIntVector2 Start, FIntVector2 End, EDWGridCellType Value);
	bool FillRect(int Start, int End, EDWGridCellType Value);

	EDWGridCellType GetValue(FIntVector2 Position);
	EDWGridCellType GetValue(int X, int Y);
	EDWGridCellType GetValue(int Index);

	int CountCardinalNeighbours(FIntVector2 Position);
	int CountCardinalNeighbours(int Index);
	int CountInterCardinalNeighbours(FIntVector2 Position);
	int CountInterCardinalNeighbours(int Index);
	int CountAllNeighbours(FIntVector2 Position);
	int CountAllNeighbours(int Index);

private:
	int Width;
	int Depth;
	TArray<EDWGridCellType> GridMap;
	TArray<FIntVector2> MapDirections = { FIntVector2(0, 1), FIntVector2(1, 0), FIntVector2(0, -1), FIntVector2(-1, 0) };
};
