// C 2024 Bianca Bellof @github.com/Finderadeis


#include "CustomTypes/DWDungeonGrid2D.h"

DWDungeonGrid2D::DWDungeonGrid2D()
{
	Width = 0;
	Depth = 0;
	GridMap = TArray<EDWGridCellType>();
}

DWDungeonGrid2D::DWDungeonGrid2D(int Width, int Depth)
{
	this->Width = Width;
	this->Depth = Depth;
	GridMap = TArray<EDWGridCellType>();

	for (auto i = 0; i < Width * Depth; i++)
	{
		GridMap.Add(EDWGridCellType::ECellType_Empty);
	}
}

void DWDungeonGrid2D::Reset(int NewWidth, int NewDepth)
{
	Width = NewWidth;
	Depth = NewDepth;
	GridMap.Empty();

	for (auto i = 0; i < Width * Depth; i++)
	{
		GridMap.Add(EDWGridCellType::ECellType_Empty);
	}
}

int DWDungeonGrid2D::GetSize()
{
	return GridMap.Num();
}

int DWDungeonGrid2D::GetWidth()
{
	return Width;
}

int DWDungeonGrid2D::GetDepth()
{
	return Depth;
}

int DWDungeonGrid2D::GetIndex(FIntVector2 Position)
{
	return Position.X + Position.Y * Width;
}

int DWDungeonGrid2D::GetIndexFromWorldPosition(const FVector& WorldPosition, const float& TileSize)
{
	int X = FMath::FloorToInt(WorldPosition.X / TileSize);
	int Y = FMath::FloorToInt(WorldPosition.Y / TileSize);
	return GetIndex(FIntVector2(X, Y));
}

FIntVector2 DWDungeonGrid2D::GetPosition(int Index)
{
	return FIntVector2(Index % Width, Index / Width);
}

bool DWDungeonGrid2D::IsInBounds(FIntVector2 Position)
{
	return IsInBounds(GetIndex(Position));
}

bool DWDungeonGrid2D::IsInBounds(int Index)
{
	return Index >= 0 && Index < GridMap.Num() ;
}

bool DWDungeonGrid2D::SetValue(FIntVector2 Position, EDWGridCellType Value)
{
	return SetValue(GetIndex(Position), Value);
}

bool DWDungeonGrid2D::SetValue(int Index, EDWGridCellType Value)
{
	if (IsInBounds(Index))
	{
		GridMap[Index] = Value;
		return true;
	}
	return false;
}

bool DWDungeonGrid2D::FillRect(FIntVector2 Start, FIntVector2 End, EDWGridCellType Value)
{
	if(IsInBounds(Start) && IsInBounds(End))
	{
		FIntVector2 Min = FIntVector2(FMath::Min(Start.X, End.X), FMath::Min(Start.Y, End.Y));
		FIntVector2 Max = FIntVector2(FMath::Max(Start.X, End.X), FMath::Max(Start.Y, End.Y));

		bool Success = true;
		for (int x = Min.X; x < Max.X; x++)
		{
			for (int y = Min.Y; y < Max.Y; y++)
			{
				if(!SetValue(FIntVector2(x, y), Value))
				{
					Success = false;
				}
			}
		}
		return Success;
	}
	return false;
}

bool DWDungeonGrid2D::FillRect(int Start, int End, EDWGridCellType Value)
{
	return FillRect(GetPosition(Start), GetPosition(End), Value);
}

EDWGridCellType DWDungeonGrid2D::GetValue(FIntVector2 Position)
{
	return GetValue(GetIndex(Position));
}

EDWGridCellType DWDungeonGrid2D::GetValue(int X, int Y)
{
	return GetValue(FIntVector2(X, Y));
}

EDWGridCellType DWDungeonGrid2D::GetValue(int Index)
{
	if (IsInBounds(Index))
	{
		return GridMap[Index];
	}
	return EDWGridCellType::ECellType_Invalid;
}

int DWDungeonGrid2D::CountCardinalNeighbours(FIntVector2 Position)
{
	if(Position.X <= 0 || Position.X >= Width - 1 || Position.Y <= 0 || Position.Y >= Depth - 1)
	{
		return 5;
	}
	int Count = 0;
	if (GetValue(Position.X - 1, Position.Y) == EDWGridCellType::ECellType_Floor) Count++;
	if (GetValue(Position.X + 1, Position.Y) == EDWGridCellType::ECellType_Floor) Count++;
	if (GetValue(Position.X, Position.Y - 1) == EDWGridCellType::ECellType_Floor) Count++;
	if (GetValue(Position.X, Position.Y + 1) == EDWGridCellType::ECellType_Floor) Count++;
	return Count;
}

int DWDungeonGrid2D::CountCardinalNeighbours(int Index)
{
	return CountCardinalNeighbours(GetPosition(Index));
}

int DWDungeonGrid2D::CountInterCardinalNeighbours(FIntVector2 Position)
{
	if (Position.X <= 0 || Position.X >= Width - 1 || Position.Y <= 0 || Position.Y >= Depth - 1)
	{
		return 5;
	}
	int Count = 0;
	if (GetValue(Position.X - 1, Position.Y -1) == EDWGridCellType::ECellType_Floor) Count++;
	if (GetValue(Position.X + 1, Position.Y -1) == EDWGridCellType::ECellType_Floor) Count++;
	if (GetValue(Position.X - 1, Position.Y + 1) == EDWGridCellType::ECellType_Floor) Count++;
	if (GetValue(Position.X + 1, Position.Y + 1) == EDWGridCellType::ECellType_Floor) Count++;
	return Count;
}

int DWDungeonGrid2D::CountInterCardinalNeighbours(int Index)
{
	return CountInterCardinalNeighbours(GetPosition(Index));
}

int DWDungeonGrid2D::CountAllNeighbours(FIntVector2 Position)
{
	if (Position.X <= 0 || Position.Y <= 0 || Position.X >= Width - 1 || Position.Y >= Depth - 1) return 9;
	return CountCardinalNeighbours(Position) + CountInterCardinalNeighbours(Position);
}

int DWDungeonGrid2D::CountAllNeighbours(int Index)
{
	return CountAllNeighbours(GetPosition(Index));
}