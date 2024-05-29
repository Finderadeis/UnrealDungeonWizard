// C 2024 Bianca Bellof @github.com/Finderadeis


#include "Modes/DWGenerationMode_LinearRooms.h"

DWGenerationMode_LinearRooms::DWGenerationMode_LinearRooms()
{
}

void DWGenerationMode_LinearRooms::Setup(UWorld* World, UDWDataAsset* PresetsAsset, TSharedPtr<DWDungeonGrid2D> Grid)
{
	DWGenerationMode::Setup(World, PresetsAsset, Grid);
}

void DWGenerationMode_LinearRooms::Render(IToolsContextRenderAPI* RenderAPI)
{
	DWGenerationMode::Render(RenderAPI);
}

void DWGenerationMode_LinearRooms::OnPropertyModified(UObject* PropertySet, FProperty* Property)
{
	DWGenerationMode::OnPropertyModified(PropertySet, Property);
}

void DWGenerationMode_LinearRooms::OnClickPress(const FVector& Position)
{
	DWGenerationMode::OnClickPress(Position);
}

void DWGenerationMode_LinearRooms::OnClickDrag(const FVector& Position)
{
	DWGenerationMode::OnClickDrag(Position);
}

void DWGenerationMode_LinearRooms::OnClickRelease(const FVector& Position)
{
	DWGenerationMode::OnClickRelease(Position);
}

void DWGenerationMode_LinearRooms::OnTerminateDragSequence()
{
	DWGenerationMode::OnTerminateDragSequence();
}

void DWGenerationMode_LinearRooms::OnUpdateModifierState(int ModifierID, bool bIsOn)
{
	DWGenerationMode::OnUpdateModifierState(ModifierID, bIsOn);
}

void DWGenerationMode_LinearRooms::Generate()
{
		int MaxSize = DungeonPresetsAsset->NumberOfRooms * (DungeonPresetsAsset->MaxRoomSize + DungeonPresetsAsset->MaxRoomDistance);
		DungeonGrid->Reset(MaxSize, MaxSize);
		int X = 1;
		int Y = 1;
		int Width = FMath::RandRange(DungeonPresetsAsset->MinRoomSize, DungeonPresetsAsset->MaxRoomSize);
		int Depth = FMath::RandRange(DungeonPresetsAsset->MinRoomSize, DungeonPresetsAsset->MaxRoomSize);
	
		for (int i = 0; i < DungeonPresetsAsset->NumberOfRooms; i++)
		{
			if (Width < Depth)
			{
				X = (X + Width) + FMath::RandRange(DungeonPresetsAsset->MinRoomDistance, DungeonPresetsAsset->MaxRoomDistance);
				Y = FMath::RandRange(Y, (Y + Depth));
			}
			else
			{
				X = FMath::RandRange(X, (X + Width));
				Y = (Y + Depth) + FMath::RandRange(DungeonPresetsAsset->MinRoomDistance, DungeonPresetsAsset->MaxRoomDistance);
			}
	
			Width = FMath::RandRange(DungeonPresetsAsset->MinRoomSize, DungeonPresetsAsset->MaxRoomSize);
			Depth = FMath::RandRange(DungeonPresetsAsset->MinRoomSize, DungeonPresetsAsset->MaxRoomSize);
	
			GenerateRoom(FIntVector2(X, Y), Width, Depth);
		}
	
		ConnectAllRooms();
}

void DWGenerationMode_LinearRooms::GenerateRoom(FIntVector2 StartPosition, int Width, int Depth)
{
	TArray<FIntVector2> RoomTiles;
	for (int i = StartPosition.X; i < StartPosition.X + Width; i++)
	{
		for (int j = StartPosition.Y; j < StartPosition.Y + Depth; j++)
		{
			DungeonGrid->SetValue(FIntVector2(i, j), EDWGridCellType::ECellType_Floor);
			RoomTiles.Add(FIntVector2(i, j));
		}
	}
	DWRoom NewRoom;
	NewRoom.Tiles = RoomTiles;
	Rooms.Add(NewRoom);
}

void DWGenerationMode_LinearRooms::ConnectAllRooms()
{
	for (int i = 0; i < Rooms.Num() - 1; i++)
	{
		ConnectRooms(&Rooms[i], &Rooms[i + 1]);
	}
}

void DWGenerationMode_LinearRooms::ConnectRooms(DWRoom* RoomA, DWRoom* RoomB)
{
	FIntVector2 Start = RoomA->Tiles[FMath::RandRange(0, RoomA->Tiles.Num() - 1)];
	FIntVector2 End = RoomB->Tiles[FMath::RandRange(0, RoomB->Tiles.Num() - 1)];
	
	FIntVector2 StartDistance = FIntVector2(0, 0);
	FIntVector2 StartMin = FIntVector2(0, 0);
	FIntVector2 StartMax = FIntVector2(0, 0);
	for (int i = 0; i < RoomA->Tiles.Num(); i++)
	{
		if (RoomA->Tiles[i].Y == Start.Y)
		{
			if (RoomA->Tiles[i].X > Start.X && RoomA->Tiles[i].X - Start.X > StartDistance.X)
			{
				StartDistance.X = RoomA->Tiles[i].X - Start.X;
			}
			if (RoomA->Tiles[i].X < StartMin.X)
			{
				StartMin.X = RoomA->Tiles[i].X;
			}
			if (RoomA->Tiles[i].X > StartMax.X)
			{
				StartMax.X = RoomA->Tiles[i].X;
			}
		}
		if (RoomA->Tiles[i].X == Start.X)
		{
			if (RoomA->Tiles[i].Y > Start.Y && RoomA->Tiles[i].Y - Start.Y > StartDistance.Y)
			{
				StartDistance.Y = RoomA->Tiles[i].Y - Start.Y;
			}
			if (RoomA->Tiles[i].Y < StartMin.Y)
			{
				StartMin.Y = RoomA->Tiles[i].Y;
			}
			if (RoomA->Tiles[i].Y > StartMax.Y)
			{
				StartMax.Y = RoomA->Tiles[i].Y;
			}
		}
	}

	FIntVector2 EndDistance = FIntVector2(0, 0);
	FIntVector2 EndMin = FIntVector2(0, 0);
	FIntVector2 EndMax = FIntVector2(0, 0);
	for (int i = 0; i < RoomB->Tiles.Num(); i++)
	{
		if (RoomB->Tiles[i].Y == Start.Y)
		{
			if (RoomB->Tiles[i].X < Start.X && Start.X - RoomB->Tiles[i].X > EndDistance.X)
			{
				EndDistance.X = Start.X - RoomB->Tiles[i].X;
			}
			if (RoomB->Tiles[i].X < EndMin.X)
			{
				EndMin.X = RoomB->Tiles[i].X;
			}
			if (RoomB->Tiles[i].X > EndMax.X)
			{
				EndMax.X = RoomB->Tiles[i].X;
			}
		}
		if (RoomB->Tiles[i].X == Start.X)
		{
			if (RoomB->Tiles[i].Y < Start.Y && Start.Y - RoomB->Tiles[i].Y > EndDistance.Y)
			{
				EndDistance.Y = Start.Y - RoomB->Tiles[i].Y;
			}
			if (RoomB->Tiles[i].Y < EndMin.Y)
			{
				EndMin.Y = RoomB->Tiles[i].Y;
			}
			if (RoomB->Tiles[i].Y > EndMax.Y)
			{
				EndMax.Y = RoomB->Tiles[i].Y;
			}
		}
	}

	if (StartMax.X - StartMin.X < StartMax.Y - StartMin.Y)
	{
		Start = FIntVector2(Start.X + StartDistance.X, Start.Y);
		End = FIntVector2(End.X - EndDistance.X, End.Y);
	}
	else
	{
		Start = FIntVector2(Start.X, Start.Y + StartDistance.Y);
		End = FIntVector2(End.X, End.Y - EndDistance.Y);
	}
	RoomA->ExitPoints.Add(Start);
	RoomB->ExitPoints.Add(End);

	FIntVector2 PathMin = FIntVector2(FMath::Min(Start.X, End.X), FMath::Min(Start.Y, End.Y));
	FIntVector2 PathMax = FIntVector2(FMath::Max(Start.X, End.X), FMath::Max(Start.Y, End.Y));

	GeneratePath(Start, End, PathMin, PathMax);
}

void DWGenerationMode_LinearRooms::GeneratePath(FIntVector2 Start, FIntVector2 End, FIntVector2 PathMin, FIntVector2 PathMax)
{
	int LoopCount = 0;
	bool ConnectXDirection = End.X - Start.X > End.Y - Start.Y;
	FIntVector2 Current = Start;
	FIntVector2 Target = ConnectXDirection ? FIntVector2(End.X-1, End.Y) : FIntVector2(End.X, End.Y-1);
	if (ConnectXDirection)
	{
		PathMin.X = FMath::Max(PathMin.X, Start.X);
		PathMax.X = FMath::Min(PathMax.X, End.X);
	}
	else
	{
		PathMin.Y = FMath::Max(PathMin.Y, Start.Y);
		PathMax.Y = FMath::Min(PathMax.Y, End.Y);
	}

	while (LoopCount < 10000)
	{
		LoopCount++;
		if (LoopCount >= 10000)UE_LOG(LogTemp, Warning, TEXT("LoopCount %d"), LoopCount);
		
		DungeonGrid->SetValue(Current, EDWGridCellType::ECellType_Floor);
		
		if (Current == Target)
		{
			break;
		}
		
		TArray<FIntVector2> PossibleDirections = { FIntVector2(0, 1), FIntVector2(1, 0), FIntVector2(0, -1), FIntVector2(-1, 0) };
		if (ConnectXDirection)
		{
			PossibleDirections.Remove(FIntVector2(-1,0));
			if (Current.X == PathMax.X)
			{
				PossibleDirections.Remove(FIntVector2(1, 0));
			}
			if (Current.Y == PathMin.Y)
			{
				PossibleDirections.Remove(FIntVector2(0, -1));
			}
			if (Current.Y == PathMax.Y)
			{
				PossibleDirections.Remove(FIntVector2(0, 1));
			}
		}
		else
		{
			PossibleDirections.Remove(FIntVector2(0, -1));
			if (Current.Y == PathMax.Y)
			{
				PossibleDirections.Remove(FIntVector2(0, 1));
			}
			if (Current.X == PathMin.X)
			{
				PossibleDirections.Remove(FIntVector2(-1, 0));
			}
			if (Current.X == PathMax.X)
			{
				PossibleDirections.Remove(FIntVector2(1, 0));
			}
		}
		
		Current += PossibleDirections[FMath::RandRange(0, PossibleDirections.Num() - 1)];
	}
}
