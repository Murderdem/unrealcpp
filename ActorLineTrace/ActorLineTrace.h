// Harrison McGuire
// UE4 Version 4.18.1
// https://github.com/Harrison1/unrealcpp
// https://severallevels.io

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ActorLineTrace.generated.h"

UCLASS()
class UNREALCPP_API AActorLineTrace : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AActorLineTrace();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	
	
};
