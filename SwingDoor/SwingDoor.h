// Harrison McGuire 
// UE4 Version 4.18.3
// https://github.com/Harrison1/unrealcpp 
// https://severallevels.io 
// https://harrisonmcguire.com
// WIP

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SwingDoor.generated.h"

UCLASS()
class UNREALCPP_API ASwingDoor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASwingDoor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	void SwingOpen();

	FTimerHandle MemberTimerHandle;

	bool isOpen;
	
};