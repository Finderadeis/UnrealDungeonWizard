// C 2024 Bianca Bellof @github.com/Finderadeis


#include "DWBaseTool.h"
#include "InteractiveToolManager.h"
#include "BaseBehaviors/ClickDragBehavior.h"
#include "Modes/DWGenerationMode_Maze.h"
#include "Modes/DWGenerationMode_LinearRooms.h"
#include "Modes/DWGenerationMode_Draw.h"
#include "Engine/StaticMeshActor.h"



UInteractiveTool* UDWBaseToolBuilder::BuildTool(const FToolBuilderState& SceneState) const
{
	UDWBaseTool* NewTool = NewObject<UDWBaseTool>(SceneState.ToolManager);
	NewTool->SetWorld(SceneState.World);
	return NewTool;
}

UDWBaseToolProperties::UDWBaseToolProperties()
{
}

void UDWBaseTool::SetWorld(UWorld* World)
{
	this->TargetWorld = World;
}

void UDWBaseTool::Setup()
{
	UInteractiveTool::Setup();
	UClickDragInputBehavior* MouseBehavior = NewObject<UClickDragInputBehavior>();
	MouseBehavior->Modifiers.RegisterModifier(ShiftModifierID, FInputDeviceState::IsShiftKeyDown);
	MouseBehavior->Modifiers.RegisterModifier(CtrlModifierID, FInputDeviceState::IsCtrlKeyDown);
	MouseBehavior->Initialize(this);
	AddInputBehavior(MouseBehavior);
	bShiftDown = false;
	bCtrlDown = false;

	Properties = NewObject<UDWBaseToolProperties>(this, "DungeonPresets");
	AddToolPropertySource(Properties);

	DungeonGrid = MakeShared<DWDungeonGrid2D>(DWDungeonGrid2D());
}

void UDWBaseTool::Render(IToolsContextRenderAPI* RenderAPI)
{
	if (!DungeonPresetsAsset)
	{
		return;
	}
	if (GenerationMode == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("Generation Mode is null"));
		return;
	}

	GenerationMode->Render(RenderAPI);
}

void UDWBaseTool::OnPropertyModified(UObject* PropertySet, FProperty* Property)
{
	TSubclassOf<UDWDataAsset> tmp = Properties->DungeonPresetsAsset;
	DungeonPresetsAsset = tmp.GetDefaultObject();
	DungeonGrid->Reset(DungeonPresetsAsset->MaxWidth, DungeonPresetsAsset->MaxDepth);

	if (DrawBaseActor)
	{
		DrawBaseActor->Destroy();
	}
	ClearSpawnedActors();
	DungeonGrid->Reset(DungeonPresetsAsset->MaxWidth, DungeonPresetsAsset->MaxDepth);
	float BaseScaleX = DungeonPresetsAsset->MaxWidth * DungeonPresetsAsset->MeshSize;
	float BaseScaleY = DungeonPresetsAsset->MaxDepth * DungeonPresetsAsset->MeshSize;
	FVector Location = FVector(-DungeonPresetsAsset->MeshSize / 2, -DungeonPresetsAsset->MeshSize / 2, 0);
	UStaticMesh* BaseMesh = Cast<UStaticMesh>(StaticLoadObject(UStaticMesh::StaticClass(), nullptr, *BaseMeshPath));
	DrawBaseActor = SpawnMeshActor(BaseMesh, Location, FRotator::ZeroRotator, FVector(BaseScaleX, BaseScaleY, 0.1f), FName("BaseMesh"), OutlinerFolderName.ToString());


	switch (DungeonPresetsAsset->GenerationMode)
	{
		case EDWGenerationMode::EGenMode_PureMaze:
		GenerationMode = MakeShared<DWGenerationMode_Maze>(DWGenerationMode_Maze());
		break;
		case EDWGenerationMode::EGenMode_LinearRooms:
		GenerationMode = MakeShared<DWGenerationMode_LinearRooms>(DWGenerationMode_LinearRooms());
		break;
		case EDWGenerationMode::EGenMode_DrawRooms:
		GenerationMode = MakeShared<DWGenerationMode_Draw>(DWGenerationMode_Draw());
		break;
		default:
		GenerationMode = MakeShared<DWGenerationMode>(DWGenerationMode());
		UE_LOG(LogTemp, Warning, TEXT("No valid Generation mode found"));
		break;
	}
	
	GenerationMode->Setup(TargetWorld, DungeonPresetsAsset, DungeonGrid);
	GenerationMode->OnPropertyModified(PropertySet, Property);
}

FInputRayHit UDWBaseTool::CanBeginClickDragSequence(const FInputDeviceRay& PressPos)
{
	FVector Temp;
	FInputRayHit Result = FindRayHit(PressPos.WorldRay, Temp);
	return Result;
}

void UDWBaseTool::OnClickPress(const FInputDeviceRay& PressPos)
{
	if (!DungeonPresetsAsset)
	{
		UE_LOG(LogTemp, Warning, TEXT("No Dungeon Presets Asset set"));
		return;
	}

	if (bShiftDown && bCtrlDown)
	{
		SpawnDungeon();
		return;
	}

	FInputRayHit HitResult = FindRayHitOnGrid(PressPos.WorldRay, DragStartPos, DungeonPresetsAsset->MeshSize);
	GenerationMode->OnClickPress(DragStartPos);
}

void UDWBaseTool::OnClickDrag(const FInputDeviceRay& DragPos)
{
	if (!DungeonPresetsAsset)
	{
		return;
	}

	FInputRayHit HitResult = FindRayHitOnGrid(DragPos.WorldRay, DragEndPos, DungeonPresetsAsset->MeshSize);
	GenerationMode->OnClickDrag(DragEndPos);
}

void UDWBaseTool::OnClickRelease(const FInputDeviceRay& ReleasePos)
{
	if (!DungeonPresetsAsset)
	{
		return;
	}

	FInputRayHit HitResult = FindRayHitOnGrid(ReleasePos.WorldRay, DragEndPos, DungeonPresetsAsset->MeshSize);
	GenerationMode->OnClickRelease(DragEndPos);
}

void UDWBaseTool::OnTerminateDragSequence()
{
	UE_LOG(LogTemp, Warning, TEXT("UNHANDLED DRAG TERMINATE"));
}

void UDWBaseTool::OnUpdateModifierState(int ModifierID, bool bIsOn)
{
	if (ModifierID == ShiftModifierID)
	{
		bShiftDown = bIsOn;
	}
	if (ModifierID == CtrlModifierID)
	{
		bCtrlDown = bIsOn;
	}
	GenerationMode->OnUpdateModifierState(ModifierID, bIsOn);
}

FInputRayHit UDWBaseTool::FindRayHit(const FRay& Ray, FVector& OutHitPoint)
{
	FCollisionObjectQueryParams QueryParams(FCollisionObjectQueryParams::AllObjects);
	FHitResult Result;
	bool bHitWorld = TargetWorld->LineTraceSingleByObjectType(Result, Ray.Origin, Ray.PointAt(999999), QueryParams);
	if (bHitWorld)
	{
		OutHitPoint = Result.ImpactPoint;
		return FInputRayHit(Result.Distance);
	}
	return FInputRayHit();
}

FInputRayHit UDWBaseTool::FindRayHitOnGrid(const FRay& Ray, FVector& OutHitPoint, float GridSize)
{
	FInputRayHit HitResult = FindRayHit(Ray, OutHitPoint);
	float NewX = FMath::FloorToInt(OutHitPoint.X / GridSize) * GridSize;
	float NewY = FMath::FloorToInt(OutHitPoint.Y / GridSize) * GridSize;
	OutHitPoint = FVector(NewX, NewY, OutHitPoint.Z);
	return FInputRayHit();
}


void UDWBaseTool::SpawnDungeon()
{
	float TileSize = DungeonPresetsAsset->MeshSize;
	int WallUnits = DungeonPresetsAsset->WallUnits;
	bool AddCeiling = DungeonPresetsAsset->bAddCeiling;
	int32 CurrentWallLightChance = DungeonPresetsAsset->WallLightChance;

	ClearSpawnedActors();

	UStaticMesh* Mesh = nullptr;
	float X, Y;
	FVector Location = FVector::ZeroVector;
	FRotator Rotation = FRotator::ZeroRotator;
	FVector Scale = FVector::OneVector;
	FName Name = FName("UndefinedAsset");
	FString Folder = OutlinerFolderName.ToString();

	for (int i = 0; i < DungeonGrid->GetSize(); i++)
	{
		FIntVector2 Pos = DungeonGrid->GetPosition(i);
		if (DungeonGrid->GetValue(i) == EDWGridCellType::ECellType_Floor)
		{
			bool bWallNorth = DungeonGrid->GetValue(Pos.X, Pos.Y - 1) != EDWGridCellType::ECellType_Floor;
			bool bWallSouth = DungeonGrid->GetValue(Pos.X, Pos.Y + 1) != EDWGridCellType::ECellType_Floor;
			bool bWallEast = DungeonGrid->GetValue(Pos.X + 1, Pos.Y) != EDWGridCellType::ECellType_Floor;
			bool bWallWest = DungeonGrid->GetValue(Pos.X - 1, Pos.Y) != EDWGridCellType::ECellType_Floor;

			Mesh = DungeonPresetsAsset->FloorMeshes[FMath::RandRange(0, DungeonPresetsAsset->FloorMeshes.Num() - 1)];
			X = Pos.X * TileSize;
			Y = Pos.Y * TileSize;
			Location = FVector(X, Y, 0.0f);
			Rotation = FRotator::ZeroRotator;
			Scale = FVector::OneVector;
			Name = FName("Floor_" + FString::Printf(TEXT("%04d"), SpawnedFloorTiles.Num()));
			Folder = OutlinerFolderName.ToString().Append("/Floors");
			SpawnedFloorTiles.Add(SpawnMeshActor(Mesh, Location, Rotation, Scale, Name, Folder));

			if (AddCeiling)
			{
				Location = FVector(X, Y, TileSize * WallUnits);
				Rotation = FRotator::ZeroRotator;
				Name = FName("Ceiling_" + FString::Printf(TEXT("%04d"), SpawnedFloorTiles.Num()));
				Folder = OutlinerFolderName.ToString().Append("/Ceiling");
				SpawnedFloorTiles.Add(SpawnMeshActor(Mesh, Location, Rotation, Scale, Name, Folder));
			}

			if (bWallNorth)
			{
				for (int j = 0; j < WallUnits; j++)
				{
					Mesh = DungeonPresetsAsset->WallMeshes[FMath::RandRange(0, DungeonPresetsAsset->WallMeshes.Num() - 1)];
					X = Pos.X * TileSize;
					Y = Pos.Y * TileSize - TileSize / 2;
					Location = FVector(X, Y, j * TileSize + TileSize / 2);
					Rotation = FRotator(0, 0, 0);
					Name = FName("Wall_" + FString::Printf(TEXT("%04d"), SpawnedWallTiles.Num()));
					Folder = OutlinerFolderName.ToString().Append("/Walls");
					SpawnedWallTiles.Add(SpawnMeshActor(Mesh, Location, Rotation, Scale, Name, Folder));

					if (DungeonPresetsAsset->WallLightSources.Num() > 0)
					{
						if (FMath::RandRange(0, 100) <= CurrentWallLightChance)
						{
							Y += DungeonPresetsAsset->WallLightOffset;
							Location = FVector(X, Y, j * TileSize + TileSize / 2);
							ActorToSpawn = DungeonPresetsAsset->WallLightSources[FMath::RandRange(0, DungeonPresetsAsset->FloorMeshes.Num() - 1)].GetDefaultObject();
							Name = FName("Light_" + FString::Printf(TEXT("%04d"), SpawnedActors.Num()));
							Folder = OutlinerFolderName.ToString().Append("/Lighting");
							SpawnedActors.Add(SpawnActor(ActorToSpawn->GetClass(), Location, Rotation, Name, Folder));

							CurrentWallLightChance = DungeonPresetsAsset->WallLightChance;
						}
						else
						{
							CurrentWallLightChance++;
						}
					}
				}
			}

			if (bWallEast)
			{
				for (int j = 0; j < WallUnits; j++)
				{
					Mesh = DungeonPresetsAsset->WallMeshes[FMath::RandRange(0, DungeonPresetsAsset->WallMeshes.Num() - 1)];
					X = Pos.X * TileSize + TileSize / 2;
					Y = Pos.Y * TileSize;
					Location = FVector(X, Y, j * TileSize + TileSize / 2);
					Rotation = FRotator(0, 90, 0);
					Name = FName("Wall_" + FString::Printf(TEXT("%04d"), SpawnedWallTiles.Num()));
					Folder = OutlinerFolderName.ToString().Append("/Walls");
					SpawnedWallTiles.Add(SpawnMeshActor(Mesh, Location, Rotation, Scale, Name, Folder));

					if (DungeonPresetsAsset->WallLightSources.Num() > 0)
					{
						if (FMath::RandRange(0, 100) <= CurrentWallLightChance)
						{
							X -= DungeonPresetsAsset->WallLightOffset;
							Location = FVector(X, Y, j * TileSize + TileSize / 2);
							ActorToSpawn = DungeonPresetsAsset->WallLightSources[FMath::RandRange(0, DungeonPresetsAsset->FloorMeshes.Num() - 1)].GetDefaultObject();
							Name = FName("Light_" + FString::Printf(TEXT("%04d"), SpawnedActors.Num()));
							Folder = OutlinerFolderName.ToString().Append("/Lighting");
							SpawnedActors.Add(SpawnActor(ActorToSpawn->GetClass(), Location, Rotation, Name, Folder));

							CurrentWallLightChance = DungeonPresetsAsset->WallLightChance;
						}
						else
						{
							CurrentWallLightChance++;
						}
					}
				}
			}

			if (bWallSouth)
			{
				for (int j = 0; j < WallUnits; j++)
				{
					Mesh = DungeonPresetsAsset->WallMeshes[FMath::RandRange(0, DungeonPresetsAsset->WallMeshes.Num() - 1)];
					X = Pos.X * TileSize;
					Y = Pos.Y * TileSize + TileSize / 2;
					Location = FVector(X, Y, j * TileSize + TileSize / 2);
					Rotation = FRotator(0, 180, 0);
					Name = FName("Wall_" + FString::Printf(TEXT("%04d"), SpawnedWallTiles.Num()));
					Folder = OutlinerFolderName.ToString().Append("/Walls");
					SpawnedWallTiles.Add(SpawnMeshActor(Mesh, Location, Rotation, Scale, Name, Folder));

					if (DungeonPresetsAsset->WallLightSources.Num() > 0)
					{
						if (FMath::RandRange(0, 100) <= CurrentWallLightChance)
						{
							Y -= DungeonPresetsAsset->WallLightOffset;
							Location = FVector(X, Y, j * TileSize + TileSize / 2);
							ActorToSpawn = DungeonPresetsAsset->WallLightSources[FMath::RandRange(0, DungeonPresetsAsset->FloorMeshes.Num() - 1)].GetDefaultObject();
							Name = FName("Light_" + FString::Printf(TEXT("%04d"), SpawnedActors.Num()));
							Folder = OutlinerFolderName.ToString().Append("/Lighting");
							SpawnedActors.Add(SpawnActor(ActorToSpawn->GetClass(), Location, Rotation, Name, Folder));

							CurrentWallLightChance = DungeonPresetsAsset->WallLightChance;
						}
						else
						{
							CurrentWallLightChance++;
						}
					}
				}
			}

			if (bWallWest)
			{
				for (int j = 0; j < WallUnits; j++)
				{
					Mesh = DungeonPresetsAsset->WallMeshes[FMath::RandRange(0, DungeonPresetsAsset->WallMeshes.Num() - 1)];
					X = Pos.X * TileSize - TileSize / 2;
					Y = Pos.Y * TileSize;
					Location = FVector(X, Y, j * TileSize + TileSize / 2);
					Rotation = FRotator(0, -90, 0);
					Name = FName("Wall_" + FString::Printf(TEXT("%04d"), SpawnedWallTiles.Num()));
					Folder = OutlinerFolderName.ToString().Append("/Walls");
					SpawnedWallTiles.Add(SpawnMeshActor(Mesh, Location, Rotation, Scale, Name, Folder));

					if (DungeonPresetsAsset->WallLightSources.Num() > 0)
					{
						if (FMath::RandRange(0, 100) <= CurrentWallLightChance)
						{
							X += DungeonPresetsAsset->WallLightOffset;
							Location = FVector(X, Y, j * TileSize + TileSize / 2);
							ActorToSpawn = DungeonPresetsAsset->WallLightSources[FMath::RandRange(0, DungeonPresetsAsset->FloorMeshes.Num() - 1)].GetDefaultObject();
							Name = FName("Light_" + FString::Printf(TEXT("%04d"), SpawnedActors.Num()));
							Folder = OutlinerFolderName.ToString().Append("/Lighting");
							SpawnedActors.Add(SpawnActor(ActorToSpawn->GetClass(), Location, Rotation, Name, Folder));

							CurrentWallLightChance = DungeonPresetsAsset->WallLightChance;
						}
						else
						{
							CurrentWallLightChance++;
						}
					}
				}
			}
		}
	}
}

void UDWBaseTool::ClearSpawnedActors()
{
	for (AActor* Actor : SpawnedFloorTiles)
	{
		Actor->Destroy();
	}
	for (AActor* Actor : SpawnedWallTiles)
	{
		Actor->Destroy();
	}
	for (AActor* Actor : SpawnedActors)
	{
		Actor->Destroy();
	}
	SpawnedFloorTiles.Empty();
	SpawnedWallTiles.Empty();
	SpawnedActors.Empty();
}

AStaticMeshActor* UDWBaseTool::SpawnMeshActor(UStaticMesh* Mesh, FVector Location, FRotator Rotation, FVector Scale, FName Name, FString Folder)
{
	UWorld* World = GEditor->GetEditorWorldContext().World();
	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	AStaticMeshActor* MeshActor = World->SpawnActor<AStaticMeshActor>(Location, Rotation, SpawnParams);
	MeshActor->GetStaticMeshComponent()->SetStaticMesh(Mesh);
	MeshActor->GetStaticMeshComponent()->SetWorldScale3D(Scale);
	MeshActor->SetActorLabel(Name.ToString());
	if (Folder != "NONE") MeshActor->SetFolderPath(FName(Folder));
	return MeshActor;
}

AActor* UDWBaseTool::SpawnActor(UClass* ActorClass, FVector Location, FRotator Rotation, FName Name, FString Folder)
{
	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	AActor* SpawnedActor = GEditor->GetEditorWorldContext().World()->SpawnActor<AActor>(ActorClass, Location, Rotation, SpawnParams);
	SpawnedActor->SetActorLabel(Name.ToString());
	SpawnedActor->SetFolderPath(FName(Folder));
	if (Folder != "NONE") SpawnedActor->SetFolderPath(FName(Folder));
	return SpawnedActor;
}