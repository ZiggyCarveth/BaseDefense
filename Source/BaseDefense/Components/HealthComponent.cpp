// Fill out your copyright notice in the Description page of Project Settings.

#include "HealthComponent.h"
#include "GameFramework/Actor.h"
#include "FloatingInfo.h"
#include "Components/WidgetComponent.h"
#include "UnrealNetwork.h"
#include "Engine/World.h"
#include "Public/TimerManager.h"
#include "DamageTextActor.h"
#include "Engine/World.h"
// Sets default values for this component's properties
UHealthComponent::UHealthComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;
	bReplicates = true;

	// ...
}



void UHealthComponent::OnRep_SetHealth()
{
	//SpawnDamageText();
	//OnRep_LastDamage();
	LastKnownHealth = Health;

	if (FloatingWidget == nullptr)
	{
		UWidgetComponent* WidgetComponent = Cast<UWidgetComponent>(GetOwner()->FindComponentByClass(UWidgetComponent::StaticClass()));
		if (WidgetComponent)
		{
			FloatingWidget = Cast<UFloatingInfo>(WidgetComponent->GetUserWidgetObject());
		}
	}

	if (FloatingWidget != nullptr)
	{
		FTimerHandle FuzeTimerHandle;
		GetWorld()->GetTimerManager().ClearTimer(FuzeTimerHandle);

		FloatingWidget->SetMaxHealth(MaxHealth);
		FloatingWidget->SetHealth(Health);
	}
	else
	{
		FTimerHandle FuzeTimerHandle;
		GetWorld()->GetTimerManager().SetTimer(FuzeTimerHandle, this, &UHealthComponent::OnRep_SetHealth, 0.05f, false);

	}
}

void UHealthComponent::OnRep_LastDamage()
{
	if (LastDamage > 0 && GetOwner()->WasRecentlyRendered())
	{
		SpawnDamageText(FString::SanitizeFloat(LastDamage), FColor::Red);
	}
}



void UHealthComponent::OnRep_LastHeal()
{
	/*if (LastHeal > 0 && GetOwner()->WasRecentlyRendered())
	{
		SpawnDamageText(FString::SanitizeFloat(LastHeal), FColor::Green);
	}*/
}

void UHealthComponent::OnRep_SetMaxHealth()
{
	if (FloatingWidget == nullptr)
	{
		UWidgetComponent* WidgetComponent = Cast<UWidgetComponent>(GetOwner()->FindComponentByClass(UWidgetComponent::StaticClass()));
		if (WidgetComponent)
		{
			FloatingWidget = Cast<UFloatingInfo>(WidgetComponent->GetUserWidgetObject());
		}
	}

	if (FloatingWidget != nullptr)
	{
		FTimerHandle FuzeTimerHandle;
		GetWorld()->GetTimerManager().ClearTimer(FuzeTimerHandle);

		FloatingWidget->SetMaxHealth(MaxHealth);
	}
	else
	{
		FTimerHandle FuzeTimerHandle;
		GetWorld()->GetTimerManager().SetTimer(FuzeTimerHandle, this, &UHealthComponent::OnRep_SetMaxHealth, 0.05f, false);

	}
}

void UHealthComponent::Initialise(float AMaxHealth)
{
	MaxHealth = AMaxHealth;
	Health = AMaxHealth;
	LastKnownHealth = MaxHealth;
	UWidgetComponent* WidgetComponent = Cast<UWidgetComponent>(GetOwner()->FindComponentByClass(UWidgetComponent::StaticClass()));
	if (WidgetComponent)
	{
		FloatingWidget = Cast<UFloatingInfo>(WidgetComponent->GetUserWidgetObject());
	}
	if (FloatingWidget)
	{
		FloatingWidget->SetMaxHealth(AMaxHealth);
		FloatingWidget->SetHealth(Health);

	}
}

void UHealthComponent::TakeDamage(float ADamage)
{
	LastDamage = ADamage;

	if (Health - ADamage <= 0)
	{
		Health = 0;
		GetOwner()->Destroy();
	}
	else
	{
		Health -= ADamage;
		if (FloatingWidget)
		{
			FloatingWidget->SetHealth(Health);
		}
	}

	//SpawnDamageText();
	OnRep_LastDamage();

	LastKnownHealth = Health;

}

float UHealthComponent::Heal(float AHeal)
{
	float ReturnFloat = 0;
	if (Health + AHeal > MaxHealth)
	{
		ReturnFloat = Health + AHeal - MaxHealth;
		Health = MaxHealth;
	}
	else
	{
		Health += AHeal;
		ReturnFloat = AHeal;
	}
	if (FloatingWidget)
	{
		FloatingWidget->SetHealth(Health);
	}
	OnRep_LastHeal();

	LastHeal = Health;

	return ReturnFloat;
}

void UHealthComponent::BeginPlay()
{
	Super::BeginPlay();

	UWidgetComponent* WidgetComponent = Cast<UWidgetComponent>(GetOwner()->FindComponentByClass(UWidgetComponent::StaticClass()));
	if (WidgetComponent)
	{
		FloatingWidget = Cast<UFloatingInfo>(WidgetComponent->GetUserWidgetObject());
	}
}

void UHealthComponent::SpawnDamageText(FString AText, FColor AColor)
{
	ADamageTextActor* DamageTextActor = nullptr;
	FVector OwnerLoc = GetOwner()->GetActorLocation();
	UWorld* World = GetWorld();
	FActorSpawnParameters SpawnParams;
	SpawnParams.bNoFail = true;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	DamageTextActor = Cast<ADamageTextActor>(GetOwner()->GetWorld()->SpawnActor<ADamageTextActor>(ADamageTextActor::StaticClass(), OwnerLoc, FRotator(0.0f), SpawnParams));
	if (DamageTextActor != nullptr)
	{
		DamageTextActor->Initialise(AText, FColor::Red);
	}

}

void UHealthComponent::GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(UHealthComponent, MaxHealth);
	DOREPLIFETIME(UHealthComponent, Health);
	DOREPLIFETIME(UHealthComponent, LastDamage);
	DOREPLIFETIME(UHealthComponent, LastHeal);
}