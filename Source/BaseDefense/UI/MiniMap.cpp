// Fill out your copyright notice in the Description page of Project Settings.

#include "MiniMap.h"
#include "Components/Image.h"
#include "Engine/World.h"
#include "Building.h"
#include "EnemyChar.h"
#include "BDGameState.h"
#include "BDPlayerState.h"
#include "Components/Image.h"
#include "Components/CanvasPanel.h"
#include "Components/CanvasPanelSlot.h"


void UMiniMap::NativeTick(const FGeometry& MyGeometry, float DeltaTime)
{
	Super::NativeTick(MyGeometry, DeltaTime);

	for (auto Iterator = Buildings.CreateIterator(); Iterator; ++Iterator)
	{
		if (Iterator->Key.IsValid())
		{
			if (Iterator->Value == nullptr)
			{
				if (MapWidth == 0)
				{
					ABDGameState* State = Cast<ABDGameState>(GetWorld()->GetGameState());

					MapWidth = State->OrthoWidth;
					MapPosition = State->MapCapturePosition;	
				}


				UImage* Image = NewObject<UImage>(UImage::StaticClass());
				Iterator->Value = Image;
				Image->SetColorAndOpacity(FLinearColor::Green);
				BuildingsPanel->AddChild(Image);
				UCanvasPanelSlot* CanvasPanelSlot = (UCanvasPanelSlot*)Image->Slot;
				CanvasPanelSlot->SetSize(FVector2D(BuildingSize, BuildingSize));
				CanvasPanelSlot->SetPosition(GetCanvasLocation(Iterator->Key->GetActorLocation()));
			}
		}
		else
		{
			Iterator.RemoveCurrent();
		}
	}

	for (auto Iterator = EnemyCharacters.CreateIterator(); Iterator; ++Iterator)
	{
		if (Iterator->Key.IsValid())
		{
			if (Iterator->Value == nullptr)
			{
				if (MapWidth == 0)
				{
					ABDGameState* State = Cast<ABDGameState>(GetWorld()->GetGameState());

					MapWidth = State->OrthoWidth;
					MapPosition = State->MapCapturePosition;

				}
				UImage* Image = NewObject<UImage>(UImage::StaticClass());
				Iterator->Value = Image;
				Image->SetColorAndOpacity(FLinearColor::Red);
				EnemiesPanel->AddChild(Image);
				UCanvasPanelSlot* CanvasPanelSlot = (UCanvasPanelSlot*)Image->Slot;
				CanvasPanelSlot->SetSize(FVector2D(EnemySize, EnemySize));
				CanvasPanelSlot->SetPosition(GetCanvasLocation(Iterator->Key->GetActorLocation()));
			}
			else
			{
				if (MapWidth == 0)
				{
					ABDGameState* State = Cast<ABDGameState>(GetWorld()->GetGameState());

					MapWidth = State->OrthoWidth;
					MapPosition = State->MapCapturePosition;

				}

				UCanvasPanelSlot* CanvasPanelSlot = (UCanvasPanelSlot*)Iterator->Value->Slot;

				CanvasPanelSlot->SetPosition(GetCanvasLocation(Iterator->Key->GetActorLocation()));
			}
		}
		else
		{
			Iterator.RemoveCurrent();
		}
	}


	if (Players.Num() != GetWorld()->GetGameState()->PlayerArray.Num())
	{
		for (APlayerState* AState : GetWorld()->GetGameState()->PlayerArray)
		{
			if (!Players.Contains(AState->GetPawn()))
			{
				Players.Add(AState->GetPawn(), nullptr);
			}
		}
	}

	for (auto Iterator = Players.CreateIterator(); Iterator; ++Iterator)
	{
		if (Iterator->Key.IsValid())
		{
			if (Iterator->Value == nullptr)
			{
				if (MapWidth == 0)
				{
					ABDGameState* State = Cast<ABDGameState>(GetWorld()->GetGameState());

					MapWidth = State->OrthoWidth;
					MapPosition = State->MapCapturePosition;

				}
				UImage* Image = NewObject<UImage>(UImage::StaticClass());
				Iterator->Value = Image;
				Image->SetColorAndOpacity(FLinearColor::Yellow);
				PlayersPanel->AddChild(Image);
				UCanvasPanelSlot* CanvasPanelSlot = (UCanvasPanelSlot*)Image->Slot;
				CanvasPanelSlot->SetSize(FVector2D(PlayerSize, PlayerSize));
				CanvasPanelSlot->SetPosition(GetCanvasLocation(Iterator->Key->GetActorLocation()));
			}
			else
			{
				if (MapWidth == 0)
				{
					ABDGameState* State = Cast<ABDGameState>(GetWorld()->GetGameState());

					MapWidth = State->OrthoWidth;
					MapPosition = State->MapCapturePosition;

				}

				UCanvasPanelSlot* CanvasPanelSlot = (UCanvasPanelSlot*)Iterator->Value->Slot;

				CanvasPanelSlot->SetPosition(GetCanvasLocation(Iterator->Key->GetActorLocation()));
			}
		}
		else
		{
			Iterator.RemoveCurrent();
		}
	}


}

FVector2D UMiniMap::GetCanvasLocation(FVector ALocation)
{
	FVector2D TempLoc = FVector2D(ALocation);
	TempLoc = FVector2D(TempLoc.X - MapPosition.X, TempLoc.Y - MapPosition.Y);
	TempLoc = FVector2D(-TempLoc.X, -TempLoc.Y);

	TempLoc = FVector2D(TempLoc.X + MapWidth / 2, -TempLoc.Y + MapWidth / 2);


	float Y = (TempLoc.X / MapWidth * WidgetSize - EnemySize / 2);
	float X = (TempLoc.Y / MapWidth * WidgetSize - EnemySize / 2);

	return FVector2D(X, Y);
}

void UMiniMap::SetUp()
{

}

void UMiniMap::Refresh()
{

}

void UMiniMap::AddBuilding(TWeakObjectPtr<class ABuilding> ABuildingInput)
{
	if (Buildings.Num() == 0)
	{
		ABDGameState* State = Cast<ABDGameState>(GetWorld()->GetGameState());
		if (State != nullptr)
		{
			for (TWeakObjectPtr<ABuilding> ATempBuilding : State->Buildings)
			{
				Buildings.Add(ATempBuilding, nullptr);

			}
		}

	}
	else
	{
		Buildings.Add(ABuildingInput, nullptr);
	}
}

void UMiniMap::AddEnemyCharacter(TWeakObjectPtr<class AEnemyChar> AnEnemyChar)
{
	if (EnemyCharacters.Num() == 0)
	{
		ABDGameState* State = Cast<ABDGameState>(GetWorld()->GetGameState());
		if (State != nullptr)
		{
			for (TWeakObjectPtr<AEnemyChar> ATempEnemy : State->EnemyCharacters)
			{
				EnemyCharacters.Add(ATempEnemy, nullptr);

			}
		}

	}
	else
	{
		EnemyCharacters.Add(AnEnemyChar, nullptr);
	}
}