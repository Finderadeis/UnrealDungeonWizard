// C 2024 Bianca Bellof @github.com/Finderadeis


#include "Modes/DWGenerationMode_Draw.h"

DWGenerationMode_Draw::DWGenerationMode_Draw()
{
}

void DWGenerationMode_Draw::Setup(UWorld* World, UDWDataAsset* PresetsAsset, TSharedPtr<DWDungeonGrid2D> Grid)
{
	DWGenerationMode::Setup(World, PresetsAsset, Grid);
}

void DWGenerationMode_Draw::Render(IToolsContextRenderAPI* RenderAPI)
{
	DWGenerationMode::Render(RenderAPI);

	RenderGrid();
	RenderDungeonOutline();
	if (bShiftDown)
	{
		RenderDragRect(FColor::Purple);
	}
	else if (bCtrlDown)
	{
		RenderDragRect(FColor::Orange);
	}
	else
	{
		RenderDragRect(FColor::Emerald);
	}
}

void DWGenerationMode_Draw::OnPropertyModified(UObject* PropertySet, FProperty* Property)
{
	DWGenerationMode::OnPropertyModified(PropertySet, Property);
}

void DWGenerationMode_Draw::OnClickPress(const FVector& Position)
{
	DWGenerationMode::OnClickPress(Position);
}

void DWGenerationMode_Draw::OnClickDrag(const FVector& Position)
{
	DWGenerationMode::OnClickDrag(Position);
}

void DWGenerationMode_Draw::OnClickRelease(const FVector& Position)
{
	DWGenerationMode::OnClickRelease(Position);

	int StartIndex = DungeonGrid->GetIndexFromWorldPosition(DragStartPos, DungeonPresetsAsset->MeshSize);
	int EndIndex = DungeonGrid->GetIndexFromWorldPosition(DragEndPos, DungeonPresetsAsset->MeshSize);
	if (StartIndex > EndIndex)
	{
		int Temp = StartIndex;
		StartIndex = EndIndex;
		EndIndex = Temp;
	}

	if (bShiftDown)
	{
		ConnectRooms(GetRoomAtPosition(DungeonGrid->GetPosition(StartIndex)), GetRoomAtPosition(DungeonGrid->GetPosition(EndIndex)));
	}
	else
	{
		EDWGridCellType Value = bCtrlDown ? EDWGridCellType::ECellType_Empty : EDWGridCellType::ECellType_Floor;
		DungeonGrid->FillRect(StartIndex, EndIndex, Value);
	}

	UpdateRooms();

	DragStartPos = FVector::ZeroVector;
	DragEndPos = FVector::ZeroVector;
}

void DWGenerationMode_Draw::OnTerminateDragSequence()
{
	DWGenerationMode::OnTerminateDragSequence();
}

void DWGenerationMode_Draw::OnUpdateModifierState(int ModifierID, bool bIsOn)
{
	DWGenerationMode::OnUpdateModifierState(ModifierID, bIsOn);
}

void DWGenerationMode_Draw::Generate()
{
	DWGenerationMode::Generate();
}

void DWGenerationMode_Draw::RenderGrid()
{
	FColor::FromHex("212121");
	float Offset = DungeonPresetsAsset->MeshSize / 2;
	FVector Start = FVector(-Offset, -Offset, 0);
	FLinearColor Color = FLinearColor::Black;
	Color = Color.CopyWithNewOpacity(0.1f);
	FVector End = FVector(DungeonGrid->GetWidth() * DungeonPresetsAsset->MeshSize - Offset, -Offset, 0);
	for (int i = 0; i <= DungeonPresetsAsset->MaxDepth; i++)
	{
		FVector A = FVector(Start.X, Start.Y + i * DungeonPresetsAsset->MeshSize, Start.Z);
		FVector B = FVector(End.X, Start.Y + i * DungeonPresetsAsset->MeshSize, Start.Z);
		PDI->DrawTranslucentLine(A, B, Color, SDPG_Foreground, 3.0f, 0.0f, true);
	}

	Start = FVector(-Offset, -Offset, 0);
	End = FVector(-Offset, DungeonGrid->GetDepth() * DungeonPresetsAsset->MeshSize - Offset, 0);
	for (int i = 0; i <= DungeonGrid->GetWidth(); i++)
	{
		FVector A = FVector(Start.X + i * DungeonPresetsAsset->MeshSize, Start.Y, Start.Z);
		FVector B = FVector(Start.X + i * DungeonPresetsAsset->MeshSize, End.Y, Start.Z);
		PDI->DrawTranslucentLine(A, B, Color, SDPG_Foreground, 3.0f, 0.0f, true);
	}
}

void DWGenerationMode_Draw::RenderDungeonOutline()
{
	float Offset = DungeonPresetsAsset->MeshSize / 2;
	for (int i = 0; i < DungeonGrid->GetSize(); i++)
	{
		if (DungeonGrid->GetValue(i) == EDWGridCellType::ECellType_Floor)
		{
			FIntVector2 Pos = DungeonGrid->GetPosition(i);
			if (DungeonGrid->GetValue(Pos.X - 1, Pos.Y) == EDWGridCellType::ECellType_Empty)
			{
				PDI->DrawLine(FVector(Pos.X * DungeonPresetsAsset->MeshSize - Offset, Pos.Y * DungeonPresetsAsset->MeshSize - Offset, 0.1),
							  FVector(Pos.X * DungeonPresetsAsset->MeshSize - Offset, (Pos.Y + 1) * DungeonPresetsAsset->MeshSize - Offset, 0.1),
							  FColor::Emerald, SDPG_MAX, 3.0f, 0.0f, true);
			}
			if (DungeonGrid->GetValue(Pos.X + 1, Pos.Y) == EDWGridCellType::ECellType_Empty)
			{
				PDI->DrawLine(FVector((Pos.X + 1) * DungeonPresetsAsset->MeshSize - Offset, Pos.Y * DungeonPresetsAsset->MeshSize - Offset, 0.1),
							  FVector((Pos.X + 1) * DungeonPresetsAsset->MeshSize - Offset, (Pos.Y + 1) * DungeonPresetsAsset->MeshSize - Offset, 0.1),
							  FColor::Emerald, SDPG_MAX, 3.0f, 0.0f, true);
			}
			if (DungeonGrid->GetValue(Pos.X, Pos.Y - 1) == EDWGridCellType::ECellType_Empty)
			{
				PDI->DrawLine(FVector(Pos.X * DungeonPresetsAsset->MeshSize - Offset, Pos.Y * DungeonPresetsAsset->MeshSize - Offset, 0.1),
							  FVector((Pos.X + 1) * DungeonPresetsAsset->MeshSize - Offset, Pos.Y * DungeonPresetsAsset->MeshSize - Offset, 0.1),
							  FColor::Emerald, SDPG_MAX, 3.0f, 0.0f, true);
			}
			if (DungeonGrid->GetValue(Pos.X, Pos.Y + 1) == EDWGridCellType::ECellType_Empty)
			{
				PDI->DrawLine(FVector(Pos.X * DungeonPresetsAsset->MeshSize - Offset, (Pos.Y + 1) * DungeonPresetsAsset->MeshSize - Offset, 0.1),
							  FVector((Pos.X + 1) * DungeonPresetsAsset->MeshSize - Offset, (Pos.Y + 1) * DungeonPresetsAsset->MeshSize - Offset, 0.1),
							  FColor::Emerald, SDPG_MAX, 3.0f, 0.0f, true);
			}
		}
	}
}

void DWGenerationMode_Draw::RenderDragRect(FColor Color)
{
	float Offset = DungeonPresetsAsset->MeshSize / 2;
	if (DragStartPos != DragEndPos)
	{
		TArray<FVector> Points;
		Points.Add(FVector(DragStartPos.X - Offset, DragStartPos.Y - Offset, 0.1));
		Points.Add(FVector(DragEndPos.X - Offset, DragStartPos.Y - Offset, 0.1));
		Points.Add(FVector(DragEndPos.X - Offset, DragEndPos.Y - Offset, 0.1));
		Points.Add(FVector(DragStartPos.X - Offset, DragEndPos.Y - Offset, 0.1));

		for (int i = 0; i < Points.Num(); i++)
		{
			FVector A = Points[i];
			FVector B = Points[(i + 1) % Points.Num()];
			PDI->DrawLine(A, B, Color, SDPG_MAX, 3.0f, 0.0f, true);
		}
	}
}

void DWGenerationMode_Draw::UpdateRooms()
{
	Rooms.Empty();
	TArray<FIntVector2> HandledCells;
	for (int i = 0; i < DungeonGrid->GetSize(); i++)
	{
		FIntVector2 Pos = DungeonGrid->GetPosition(i);

		if (!HandledCells.Contains(Pos))
		{
			HandledCells.Add(Pos);

			if (DungeonGrid->GetValue(i) == EDWGridCellType::ECellType_Floor)
			{
				TArray<FIntVector2> NewRoomTiles;
				NewRoomTiles.Add(Pos);

				bool EndReached = false;
				int CurrentIndex = 0;
				while (!EndReached)
				{
					for (FIntVector2 Direction : MapDirections)
					{
						FIntVector2 NewPos = NewRoomTiles[CurrentIndex] + Direction;
						if (DungeonGrid->GetValue(NewPos) == EDWGridCellType::ECellType_Floor && !NewRoomTiles.Contains(NewPos) && !HandledCells.Contains(NewPos))
						{
							NewRoomTiles.Add(NewPos);
							HandledCells.Add(NewPos);
						}
					}
					CurrentIndex++;
					if (CurrentIndex >= NewRoomTiles.Num())
					{
						EndReached = true;
					}
				}

				DWRoom NewRoom;
				NewRoom.Tiles = NewRoomTiles;
				Rooms.Add(NewRoom);
			}
		}
	}
}

DWRoom* DWGenerationMode_Draw::GetRoomAtPosition(const FIntVector2& Position)
{
		for (int i = 0; i < Rooms.Num(); i++)
		{
			if (Rooms[i].Tiles.Contains(Position))
			{
				return &Rooms[i];
			}
		}
		return nullptr;
}

void DWGenerationMode_Draw::ConnectRooms(DWRoom* RoomA, DWRoom* RoomB)
{
	if (!RoomA || !RoomB)
	{
		return;
	}

	if (RoomA == RoomB)
	{
		return;
	}
	int StartIndex = DungeonGrid->GetIndexFromWorldPosition(DragStartPos, DungeonPresetsAsset->MeshSize);
	int EndIndex = DungeonGrid->GetIndexFromWorldPosition(DragEndPos, DungeonPresetsAsset->MeshSize);

	if (StartIndex > EndIndex)
	{
		int Temp = StartIndex;
		StartIndex = EndIndex;
		EndIndex = Temp;
	}
	FIntVector2 StartPos = DungeonGrid->GetPosition(StartIndex);
	FIntVector2 EndPos = DungeonGrid->GetPosition(EndIndex);

	FIntVector2 Start, End;
	TArray<FIntVector2> PossibleDoors;
	for (int i = 0; i < RoomA->Tiles.Num(); i++)
	{
		if (DungeonGrid->CountCardinalNeighbours(RoomA->Tiles[i]) == 3 && IsTileInRect(RoomA->Tiles[i], StartPos, EndPos))
		{
			PossibleDoors.Add(RoomA->Tiles[i]);
		}
	}
	if (PossibleDoors.Num() == 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("No possible doors Room 1"));
		return;
	}
	Start = PossibleDoors[FMath::RandRange(0, PossibleDoors.Num() - 1)];
	PossibleDoors.Empty();
	for (int i = 0; i < RoomB->Tiles.Num(); i++)
	{
		if (DungeonGrid->CountCardinalNeighbours(RoomB->Tiles[i]) == 3 && IsTileInRect(RoomB->Tiles[i], StartPos, EndPos))
		{
			PossibleDoors.Add(RoomB->Tiles[i]);
		}
	}
	if (PossibleDoors.Num() == 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("No possible doors Room 2"));
		return;
	}
	End = PossibleDoors[FMath::RandRange(0, PossibleDoors.Num() - 1)];

	RoomA->ExitPoints.Add(Start);
	RoomB->ExitPoints.Add(End);

	FIntVector2 Min = FIntVector2(FMath::Min(StartPos.X, EndPos.X), FMath::Min(StartPos.Y, EndPos.Y));
	FIntVector2 Max = FIntVector2(FMath::Max(StartPos.X, EndPos.X), FMath::Max(StartPos.Y, EndPos.Y));
	Max.X--;
	Max.Y--;
	GeneratePath(Start, End, Min, Max);
}

void DWGenerationMode_Draw::GeneratePath(FIntVector2 Start, FIntVector2 End, FIntVector2 PathMin, FIntVector2 PathMax)
{

	int LoopCount = 0;
	FIntVector2 Current = Start;
	FIntVector2 Target = End;

	bool XDirection = End.X - Start.X > End.Y - Start.Y;

	if (XDirection)
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
		if (XDirection)
		{
			PossibleDirections.Remove(FIntVector2(-1, 0));
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

bool DWGenerationMode_Draw::IsTileInRect(FIntVector2 Position, FIntVector2 Start, FIntVector2 End)
{
		FIntVector2 StartPos = FIntVector2(FMath::Min(Start.X, End.X), FMath::Min(Start.Y, End.Y));
		FIntVector2 EndPos = FIntVector2(FMath::Max(Start.X, End.X), FMath::Max(Start.Y, End.Y));
	
		if (Position.X >= StartPos.X && Position.X < EndPos.X && Position.Y >= StartPos.Y && Position.Y < EndPos.Y)
		{
			return true;
		}
		return false;
}
