// C 2024 Bianca Bellof @github.com/Finderadeis

#pragma once

#include "CoreMinimal.h"
#include "Modes/DWGenerationMode.h"

class DW_API DWGenerationMode_Maze : public DWGenerationMode
{
public:
	DWGenerationMode_Maze();

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

	int RecursionDepth = 0;
	void GenerateRecursive(int X, int Y);
};
