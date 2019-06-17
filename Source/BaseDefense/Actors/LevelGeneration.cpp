// Fill out your copyright notice in the Description page of Project Settings.


#include "LevelGeneration.h"
#include "BDGameInstance.h"
#include "BDGameState.h"
#include "Engine/World.h"
//#include "C:\Users\Ziggy\Documents\Unreal Projects\BaseDefense\Plugins\SimplexNoise-master\Source\SimplexNoise\Public\SimplexNoiseBPLibrary.h"
//#include "Engine/Texture2D.h"
#include "Public/ImageUtils.h"
#include <ConstructorHelpers.h>
#include <Engine/StaticMesh.h>
#include <Components/StaticMeshComponent.h>
#include <Components/SceneComponent.h>
#include <Components/HierarchicalInstancedStaticMeshComponent.h>
#include <Math/Quat.h>
#include "StaticFunctionLibrary.h"
#include "GenericPlatform/GenericPlatformMath.h"
#include "SearchGraph.h"
#include "Algo/Reverse.h"

//#include "UObject/ConstructorHelpers.h"

// Sets default values
ALevelGeneration::ALevelGeneration()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.

	bReplicates = true;


	PrimaryActorTick.bCanEverTick = false;
	this->SetActorScale3D(FVector(1));
	SceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("SceneComponent"));
	SceneComponent->SetMobility(EComponentMobility::Static);
	SetRootComponent(SceneComponent);
	
	//TreeHISMC
	//RockHISMC
	//MudHISMC
	//GrassHISMC
	//CoalHISMC
	//IronHISMC

	TreeHISMC = CreateDefaultSubobject<UHierarchicalInstancedStaticMeshComponent>(TEXT("TreeHISMC"));
	RockHISMC = CreateDefaultSubobject<UHierarchicalInstancedStaticMeshComponent>(TEXT("RockHISMC"));
	MudHISMC = CreateDefaultSubobject<UHierarchicalInstancedStaticMeshComponent>(TEXT("MudHISMC"));
	GrassHISMC = CreateDefaultSubobject<UHierarchicalInstancedStaticMeshComponent>(TEXT("GrassHISMC"));
	CoalHISMC = CreateDefaultSubobject<UHierarchicalInstancedStaticMeshComponent>(TEXT("CoalHISMC"));
	IronHISMC = CreateDefaultSubobject<UHierarchicalInstancedStaticMeshComponent>(TEXT("IronHISMC"));
	WaterHISMC = CreateDefaultSubobject<UHierarchicalInstancedStaticMeshComponent>(TEXT("WaterHISMC"));
	TreeHISMC->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
	RockHISMC->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
	MudHISMC->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
	GrassHISMC->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
	CoalHISMC->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
	IronHISMC->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
	WaterHISMC->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
	TreeHISMC->SetWorldScale3D(FVector(1));
	RockHISMC->SetWorldScale3D(FVector(1));
	MudHISMC->SetWorldScale3D(FVector(1));
	GrassHISMC->SetWorldScale3D(FVector(1));
	CoalHISMC->SetWorldScale3D(FVector(1));
	IronHISMC->SetWorldScale3D(FVector(1));

	//static ConstructorHelpers::FObjectFinder<UStaticMesh> Tree(TEXT("StaticMesh'/Game/PolygonFantasyRivals/Meshes/Props/SM_Prop_Tree_02.SM_Prop_Tree_02'"));
	static ConstructorHelpers::FObjectFinder<UStaticMesh> Tree(TEXT("StaticMesh'/Game/VertexAnimations/SM_Tree2_AnimVertTest_00.SM_Tree2_AnimVertTest_00'"));
	static ConstructorHelpers::FObjectFinder<UStaticMesh> Rock(TEXT("StaticMesh'/Game/PolygonDungeons/Meshes/Environments/Rocks/SM_Env_Rock_Flat_Large_03.SM_Env_Rock_Flat_Large_03'"));
	static ConstructorHelpers::FObjectFinder<UStaticMesh> Mud(TEXT("StaticMesh'/Game/Geometry/Meshes/1M_Cube.1M_Cube'"));
	static ConstructorHelpers::FObjectFinder<UStaticMesh> Grass(TEXT("StaticMesh'/Game/Geometry/Meshes/1M_Cube_2.1M_Cube_2'"));
	static ConstructorHelpers::FObjectFinder<UStaticMesh> Water(TEXT("StaticMesh'/Game/Geometry/Meshes/WaterCube.WaterCube'"));

	static ConstructorHelpers::FObjectFinder<UStaticMesh> Coal(TEXT("StaticMesh'/Game/Meshes/Coal.Coal'"));
	static ConstructorHelpers::FObjectFinder<UStaticMesh> Iron(TEXT("StaticMesh'/Game/Meshes/Iron.Iron'"));

	GridSize = 100;


	TreeHISMC->SetStaticMesh(Tree.Object);
	RockHISMC->SetStaticMesh(Rock.Object);
	MudHISMC->SetStaticMesh(Mud.Object);
	GrassHISMC->SetStaticMesh(Grass.Object);
	CoalHISMC->SetStaticMesh(Coal.Object);
	IronHISMC->SetStaticMesh(Iron.Object);

	WaterHISMC->SetStaticMesh(Water.Object);

	CoalHISMC->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	IronHISMC->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	//IronHISMC->collision
	int Culling = 10000;
	TreeHISMC->SetMobility(EComponentMobility::Static);
	RockHISMC->SetMobility(EComponentMobility::Static);
	MudHISMC->SetMobility(EComponentMobility::Static);
	GrassHISMC->SetMobility(EComponentMobility::Static);
	CoalHISMC->SetMobility(EComponentMobility::Static);
	IronHISMC->SetMobility(EComponentMobility::Static);
	/*TreeHISMC->InstanceStartCullDistance = Culling;
	RockHISMC->InstanceStartCullDistance = Culling;
	MudHISMC->InstanceStartCullDistance = Culling;
	GrassHISMC->InstanceStartCullDistance = Culling;
	CoalHISMC->InstanceStartCullDistance = Culling;
	IronHISMC->InstanceStartCullDistance = Culling;

	TreeHISMC->InstanceEndCullDistance = Culling;
	RockHISMC->InstanceEndCullDistance = Culling;
	MudHISMC->InstanceEndCullDistance = Culling;
	GrassHISMC->InstanceEndCullDistance = Culling;
	CoalHISMC->InstanceEndCullDistance = Culling;
	IronHISMC->InstanceEndCullDistance = Culling;*/
	 
	/*TreeHISMC->SetCullDistance(Culling);
	RockHISMC->SetCullDistance(Culling);
	MudHISMC->SetCullDistance(Culling);
	GrassHISMC->SetCullDistance(Culling);
	CoalHISMC->SetCullDistance(Culling);
	IronHISMC->SetCullDistance(Culling);*/



	FGenerationData CoalData;
	CoalData.Type = WorldGridType::Coal;
	CoalData.HISM = CoalHISMC;
	CoalData.Frequency = 10;
	CoalData.CutOff = 0.9;
	//CoalData.BaseModelSize = FVector(3.75, 3.75, 3.75);
	CoalData.ScaleWithHeigh = false;
	CoalData.BaseModelSize = FVector(0.23, 0.23, 0.23);
	//CoalData.InvertPlacement = true;
	CoalData.RotateRandomly = true;

	GenerationData.Add(CoalData.Type, CoalData);


	FGenerationData IronData;
	IronData.Type = WorldGridType::Iron;
	IronData.HISM = IronHISMC;
	IronData.Frequency = 10;
	IronData.CutOff = 0.9;
	//IronData.BaseModelSize = FVector(3.75, 3.75, 3.75);
	IronData.ScaleWithHeigh = false;
	IronData.BaseModelSize = FVector(0.23, 0.23, 0.23);
	//IronData.InvertPlacement = true;
	IronData.RotateRandomly = true;

	GenerationData.Add(IronData.Type, IronData);

	FGenerationData TreeData;
	TreeData.Type = WorldGridType::Tree;
	TreeData.HISM = TreeHISMC;
	TreeData.Frequency = 7;
	TreeData.CutOff = 0.7;
	TreeData.ScaleWithHeigh = true;
	TreeData.BaseModelSize = FVector(2, 2, 2);
	TreeData.MaxModelSize = FVector(4, 4, 6);
	/*TreeData.RandHeightVariance = 0.5;
	TreeData.RandWidthVariance = .25;
	TreeData.RandXYVariance = 1;
	TreeData.RotateRandomly = true;*/
	GenerationData.Add(TreeData.Type, TreeData);

	FGenerationData RockData;
	RockData.Type = WorldGridType::Rock;
	RockData.HISM = RockHISMC;
	RockData.Frequency = 7;
	RockData.CutOff = 0.65;
	RockData.ScaleWithHeigh = true;

	RockData.BaseModelSize = FVector(0.2, 0.2, 1);
	RockData.MaxModelSize = FVector(0.2, 0.2, 1.8);
	RockData.RandHeightVariance = 0.4;
	RockData.RandWidthVariance = 0;
	RockData.RandXYVariance = -.2;
	RockData.RotateRandomly = true;
	//RockData.InvertPlacement = true;
	RockData.ZHeight = 0;
	GenerationData.Add(RockData.Type, RockData);


	FGenerationData MudData;
	MudData.Type = WorldGridType::Mud;
	MudData.HISM = MudHISMC;
	MudData.Frequency = 7;
	MudData.CutOff = -100;
	MudData.ScaleWithHeigh = false;
	MudData.BaseModelSize = FVector(1, 1, 2);
	MudData.RandHeightVariance = 0;
	MudData.RandWidthVariance = 0;
	MudData.RandXYVariance = 0;
	//MudData.InvertPlacement = true;
	MudData.RotateRandomly = false;
	MudData.ZHeight = -100;
	GenerationData.Add(MudData.Type, MudData);

	FGenerationData GrassData;
	GrassData.Type = WorldGridType::Grass;
	GrassData.HISM = GrassHISMC;
	GrassData.Frequency = 7;
	GrassData.CutOff = 0.05;
	GrassData.ScaleWithHeigh = false;
	GrassData.BaseModelSize = FVector(1, 1, 2);
	GrassData.RandHeightVariance = 0;
	GrassData.RandWidthVariance = 0;
	GrassData.RandXYVariance = 0;
	GrassData.RotateRandomly = false;
	GrassData.ZHeight = -100;
	GenerationData.Add(GrassData.Type, GrassData);


	FGenerationData WaterData;
	WaterData.Type = WorldGridType::Water;
	WaterData.HISM = WaterHISMC;
	WaterData.Frequency = 7;
	WaterData.CutOff = 0.6;
	WaterData.ScaleWithHeigh = false;
	WaterData.BaseModelSize = FVector(1, 1, 2);
	WaterData.RandHeightVariance = 0;
	WaterData.RandWidthVariance = 0;
	WaterData.RandXYVariance = 0;
	WaterData.RotateRandomly = false;
	WaterData.ZHeight = 0;
	GenerationData.Add(WaterData.Type, WaterData);


}

// Called when the game starts or when spawned
void ALevelGeneration::BeginPlay()
{
	Super::BeginPlay();

	if (Role == ROLE_Authority)
	{
		Seed = FindValidSeed();
		GenerateWorld(Seed);

	}
	else if (Role == ROLE_SimulatedProxy)
	{
		OnRep_SetSeed();
	}

}


int ALevelGeneration::FindValidSeed()
{
	bool IsValidSeed = false;

	int TempSeed = 0;
	int SeedCount = 0;


	while (IsValidSeed == false)
	{
		TempSeed = FMath::RandRange((int32)1, (int32)99999);

		GenerateGrids(TempSeed);

		IsValidSeed = TestGrids();
		SeedCount++;
	}
	UE_LOG(LogTemp, Warning, TEXT("FindValidSeed seedcount: %i"), SeedCount);

	return TempSeed;

}

float ALevelGeneration::AStarPathLength(TArray<uint8> AMaze, int AMazeLength, int AStart, int AnEnd)
{
	FASGraph Graph;
	Graph.Nodes = AMaze;
	Graph.GridSize = AMazeLength;
	FGraphAStar<FASGraph> GraphAStar = FGraphAStar<FASGraph>(Graph);
	TArray<int32> OutPath;

	FQueryFilter Qf;
	Qf.MapSize = AMazeLength;
	Qf.Maze = AMaze;
	GraphAStar.FindPath(AStart, AnEnd, Qf, OutPath);
	
	float Cost = 0;
	if (OutPath.Num() > 0)
	{
		Cost = Qf.GetTraversalCost(OutPath[0], OutPath[OutPath.Num() - 1]);

	}
	if (OutPath.Num() > 0)
	{
		for (int i = 1; i < OutPath.Num(); i++)
		{
			Cost += Qf.GetTraversalCost(OutPath[i - 1], OutPath[i]);
		}

	}

	UE_LOG(LogTemp, Warning, TEXT("AStarPathLength: %f"), Cost);


	return Cost;
}



bool ALevelGeneration::TestGrids()
{
	//CreateNavGrid
	

	TArray<uint8> NavGrid;
	int NavGridSize = WorldGridSize;
	NavGrid.SetNum((NavGridSize) * (NavGridSize));
	for (int i = 0; i < TerrainGrid.Num(); i++)
	{
		if (GroundGrid[i] == WorldGridType::Water)
		{
			NavGrid[i] = 1;
		}

		if (TerrainGrid[i] == WorldGridType::Rock || TerrainGrid[i] == WorldGridType::Tree)
		{
			NavGrid[i] = 1;
		}
	}

	if (NavGrid[0] == 1)
	{
		UE_LOG(LogTemp, Warning, TEXT("TestGrids: False Start"));
		return false;
	}

	//Resize navgrid to have empty borders

	//NavGrid.SetNum(0, NavGridSize + 1);

	TArray<uint8> BorderedNavGrid = NavGrid;


	int BorderedIndex = 0;
	for (int i = 0; i < NavGrid.Num(); i++)
	{
		int Remainder = i % WorldGridSize;

		//Add left border
		if (Remainder == 0)
		{
			BorderedNavGrid.Insert(0, BorderedIndex);
			BorderedIndex++;
		}

		//Add right border
		if (Remainder == WorldGridSize - 1)
		{
			BorderedNavGrid.Insert(0, BorderedIndex + 1);
			BorderedIndex++;
		}

		BorderedIndex++;

	}

	////Add top and bottom
	//BorderedNavGrid.AddZeroed(WorldGridSize + 2);
	//BorderedNavGrid.InsertZeroed(0, WorldGridSize + 2);
	////Half of total grid size to get middle node.
	//int MiddleNode = ((WorldGridSize + 2) * (WorldGridSize + 2)) / 2;
	////int PathLength = AStarPathLength(BorderedNavGrid, NavGridSize + 2, 0, MiddleNode);
	//float PathLength = AStarPathLength(BorderedNavGrid, NavGridSize + 2, 0, MiddleNode);

	int MiddleNode = ((WorldGridSize) * (WorldGridSize)) / 2;

	float PathLength = AStarPathLength(NavGrid, NavGridSize, 0, MiddleNode);

	if (NavGrid[MiddleNode] == 1)
	{
		UE_LOG(LogTemp, Warning, TEXT("TestGrids: NavGrid[MiddleNode] == 1"));
		return false;
	}

	return (PathLength > 0);
	//return (PathLength == 0);
}

void ALevelGeneration::GenerateGrids(int ASeed)
{
	TerrainGrid.Empty();
	TerrainElevation.Empty();
	GroundGrid.Empty();
	GroundElevation.Empty();

	FGenerationData CoalData = (*GenerationData.Find(WorldGridType::Coal));
	FGenerationData IronData = (*GenerationData.Find(WorldGridType::Iron));
	FGenerationData TreeData = (*GenerationData.Find(WorldGridType::Tree));
	FGenerationData RockData = (*GenerationData.Find(WorldGridType::Rock));
	FGenerationData WaterData = (*GenerationData.Find(WorldGridType::Water));
	FGenerationData MudData = (*GenerationData.Find(WorldGridType::Mud));
	FGenerationData GrassData = (*GenerationData.Find(WorldGridType::Grass));

	TArray<float> CoalGrid = CreateSimplexGrid(ASeed, WorldGridSize, CoalData.Frequency);
	TArray<float> IronGrid = CreateSimplexGrid(ASeed + 1, WorldGridSize, IronData.Frequency);

	TArray<float> TreeGrid = CreateSimplexGrid(ASeed + 2, WorldGridSize, TreeData.Frequency);
	TArray<float> RockGrid = CreateSimplexGrid(ASeed + 3, WorldGridSize, RockData.Frequency);
	TArray<float> WaterGrid = CreateSimplexGrid(ASeed + 4, WorldGridSize, WaterData.Frequency);

	//Grass uses the tree grid

	//Mud doesnt need a grid because its default
	TArray<float> MudGrid;
	MudGrid.AddZeroed(WorldGridSize * WorldGridSize);




	//Coal
	AddGridToGrid(CoalGrid, TerrainGrid, TerrainElevation, CoalData.CutOff, true, CoalData.Type);

	//Iron
	AddGridToGrid(IronGrid, TerrainGrid, TerrainElevation, IronData.CutOff, true, IronData.Type);

	//Trees
	AddGridToGrid(TreeGrid, TerrainGrid, TerrainElevation, TreeData.CutOff, true, TreeData.Type);

	//Rocks
	AddGridToGrid(RockGrid, TerrainGrid, TerrainElevation, RockData.CutOff, true, RockData.Type);

	//Mud
	AddGridToGrid(TreeGrid, GroundGrid, GroundElevation, MudData.CutOff, true, MudData.Type);

	//Grass
	AddGridToGrid(TreeGrid, GroundGrid, GroundElevation, GrassData.CutOff, true, GrassData.Type);

	//Water affects both ground and terrain
	AddGridToGrid(WaterGrid, GroundGrid, GroundElevation, WaterData.CutOff, true, WaterData.Type);
	AddGridToGrid(WaterGrid, TerrainGrid, GroundElevation, WaterData.CutOff, true, WaterData.Type);
}



void ALevelGeneration::GenerateWorldFromGrids(int ASeed)
{
}


void ALevelGeneration::OnRep_SetSeed()
{

	if (Seed != 0)
	{
		GenerateWorld(Seed);
	}
}


void ALevelGeneration::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ALevelGeneration, Seed);
	/*DOREPLIFETIME(ALevelGeneration, RockHISMC);
	DOREPLIFETIME(ALevelGeneration, MudHISMC);
	DOREPLIFETIME(ALevelGeneration, GrassHISMC);
	DOREPLIFETIME(ALevelGeneration, CoalHISMC);
	DOREPLIFETIME(ALevelGeneration, IronHISMC);*/


	//TreeHISMC
//RockHISMC
//MudHISMC
//GrassHISMC
//CoalHISMC
//IronHISMC
}

void ALevelGeneration::GenerateWorld(int ASeed)
{
	if (HasGenerated == false)
	{
		
		FGenerationData CoalData = (*GenerationData.Find(WorldGridType::Coal));
		FGenerationData IronData = (*GenerationData.Find(WorldGridType::Iron));
		FGenerationData TreeData = (*GenerationData.Find(WorldGridType::Tree));
		FGenerationData RockData = (*GenerationData.Find(WorldGridType::Rock));
		FGenerationData WaterData = (*GenerationData.Find(WorldGridType::Water));
		FGenerationData MudData = (*GenerationData.Find(WorldGridType::Mud));
		FGenerationData GrassData = (*GenerationData.Find(WorldGridType::Grass));

		SpawnMeshes(CoalData, TerrainGrid, TerrainElevation);
		SpawnMeshes(IronData, TerrainGrid, TerrainElevation);

		SpawnMeshes(MudData, GroundGrid, GroundElevation);
		SpawnMeshes(GrassData, GroundGrid, GroundElevation);
		//SpawnMeshes(WaterData, GroundGrid, GroundElevation);
		SpawnMeshes(TreeData, TerrainGrid, TerrainElevation);
		SpawnMeshes(RockData, TerrainGrid, TerrainElevation);
		HasGenerated = true;
	}

}


void ALevelGeneration::SpawnMeshes(FGenerationData AGenerationData, TArray<WorldGridType> AFromGrid, TArray<float> AFromElevation)
{

	int Count = 0;
	for (int i = 0; i < AFromGrid.Num(); i++)
	{
		bool SpawnAMesh = false;

		if (AGenerationData.InvertPlacement == false)
		{
			if (AFromGrid[i] == AGenerationData.Type)
			{
				SpawnAMesh = true;

			}
		}
		else
		{
			if (AFromGrid[i] != AGenerationData.Type)
			{
				SpawnAMesh = true;
			}
		}

		if (SpawnAMesh == true)
		{
			int Remainder = i % WorldGridSize;
			int Quotient = i / WorldGridSize;
			int Y = Remainder * GridSize;
			int X = Quotient * GridSize;

			float RandXY = 0;
			float RandZ = 0;
			if (AGenerationData.RandXYVariance > 0)
			{
				RandXY = FMath::FRandRange(0, AGenerationData.RandXYVariance);
				RandZ = FMath::FRandRange(0, AGenerationData.RandZVariance);
			}



			//Work out the height at the current elevation

			FVector ElevationModelSize = AGenerationData.BaseModelSize;

			if (AGenerationData.ScaleWithHeigh == true)
			{
				float ElevationMultiplier = 0.0f;
				if (1 - AGenerationData.CutOff > 0)
				{
					ElevationMultiplier = (AFromElevation[i] - AGenerationData.CutOff) / (1 - AGenerationData.CutOff);
				}

				ElevationModelSize += (AGenerationData.MaxModelSize - AGenerationData.BaseModelSize) * ElevationMultiplier;

			}

			FTransform Trans = FTransform(FVector(-X + RandXY, Y + RandXY, AGenerationData.ZHeight + RandZ));


			if (AGenerationData.RotateRandomly)
			{
				int RandomRotation = FMath::RandRange(0, 359);

				Trans.SetRotation(FQuat((FRotator(0, RandomRotation, 0))));
			}

			float RandHeightVariance = FMath::RandRange(0.0f, AGenerationData.RandHeightVariance);
			float RandWidthVariance = FMath::RandRange(0.0f, AGenerationData.RandWidthVariance);
			FVector TransScale = FVector(ElevationModelSize + FVector(RandWidthVariance, RandWidthVariance, RandHeightVariance));

			int CoinFlip = FMath::RandRange(0, 1);
			if (CoinFlip == 0)
			{
				TransScale = FVector(-TransScale.X, -TransScale.Y, TransScale.Z);
			}

			Trans.SetScale3D(TransScale);
			Count++;
			AGenerationData.HISM->AddInstance(FTransform(Trans));
		}

	}

	UE_LOG(LogTemp, Warning, TEXT("SpawnMeshes %s: Number %i, out of %i"), *AGenerationData.HISM->GetName(), Count, AFromGrid.Num());

}

void ALevelGeneration::AddGridToGrid(TArray<float> AFromGrid, TArray<WorldGridType>& AToGrid, TArray<float>& AToElevation, float ACutOff, bool AMoreThan, WorldGridType AType)
{
	int AboveCounter = 0;
	int BelowCounter = 0;

	if (AToGrid.Num() < AFromGrid.Num())
	{
		AToGrid.AddZeroed(AFromGrid.Num() - AToGrid.Num());
	}

	if (AToElevation.Num() < AFromGrid.Num())
	{
		AToElevation.AddZeroed(AFromGrid.Num() - AToElevation.Num());
	}

	for (int i = 0; i < AFromGrid.Num(); i++)
	{
		if (AMoreThan)
		{
			if (AFromGrid[i] >= ACutOff)
			{
				AToGrid[i] = AType;
				AToElevation[i] = AFromGrid[i];
				AboveCounter++;
			}
			else
			{
				BelowCounter++;
			}
		}
		else
		{
			if (AFromGrid[i] < ACutOff)
			{
				AToGrid[i] = AType;
				AToElevation[i] = AFromGrid[i];
				AboveCounter++;
			}
			else
			{
				BelowCounter++;
			}
		}
	}



	//UE_LOG(LogTemp, Warning, TEXT("GenerateGrid: Number above : %f, %i, below : %i, out of %i, %f, %f, %f, %f, %f"), ACutOff, AboveCounter, BelowCounter, AFromGrid.Num(), AFromGrid[0], AFromGrid[1], AFromGrid[2], AFromGrid[3], AFromGrid[4]);
	//UE_LOG(LogTemp, Warning, TEXT("GenerateGrid: Number above : %f, %i, below : %i, out of %i, %f, %f, %f, %f, %f"), ACutOff, AboveCounter, BelowCounter, AFromGrid.Num(), AFromGrid[256], AFromGrid[257], AFromGrid[258], AFromGrid[259], AFromGrid[260]);




	
}

//
TArray<float> ALevelGeneration::CreateSimplexGrid(int ASeed, int ASize, float AFrequency)
{
	//if (!HasSetUp) return;
	TArray<float> Elevation;

	UStaticFunctionLibrary::setNoiseSeed(ASeed);

	for (int y = 0; y < ASize; y++)
	{
		// 
		for (int x = 0; x < ASize; x++)
		{


			float nx = float(x) / float(ASize);
			float ny = float(y) / float(ASize);
			float Data = UStaticFunctionLibrary::SimplexNoiseInRange2D(nx * AFrequency, ny * AFrequency, 0.0f, -1.0f);
			//Elevation.Add(Data / 2 + 0.5f);
			Elevation.Add(Data);
		}
	}

	return Elevation;
}

void ALevelGeneration::Setup()
{
	ABDGameState* GameState = Cast<ABDGameState>(GetWorld()->GetGameState());
	WorldGridSize = GameState->WorldGridSize;
	

}


