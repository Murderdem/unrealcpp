// Harrison McGuire
// UE4 Version 4.18.3
// https://github.com/Harrison1/unrealcpp
// https://severallevels.io
// https://harrisonmcguire.com

// helpful links
// https://docs.unrealengine.com/latest/INT/API/Runtime/Engine/Components/FTimeline/
// https://docs.unrealengine.com/latest/INT/API/Runtime/Engine/Components/UTimelineComponent/index.html
// https://wiki.unrealengine.com/Timeline_in_c%2B%2B#How_to_use_Timeline_in_C.2B.2B
// https://docs.unrealengine.com/latest/INT/API/Runtime/Engine/Curves/UCurveFloat/GetFloatValue/index.html
// https://docs.unrealengine.com/latest/INT/API/Runtime/Engine/Curves/UCurveFloat/
// https://docs.unrealengine.com/latest/INT/API/Runtime/Engine/Components/UPrimitiveComponent/OnComponentBeginOverlap/
// https://docs.unrealengine.com/latest/INT/API/Runtime/Engine/Components/UPrimitiveComponent/OnComponentEndOverlap/
// https://docs.unrealengine.com/latest/INT/BlueprintAPI/Math/Vector/UnrotateVector/
// https://docs.unrealengine.com/latest/INT/API/Runtime/Engine/Kismet/UKismetMathLibrary/LessLess_VectorRotator/index.html
// https://docs.unrealengine.com/latest/INT/API/Runtime/Engine/Kismet/UKismetMathLibrary/index.html

#include "OpenDoorTimelineCurve.h"
#include "Kismet/KismetMathLibrary.h"
#include "GameFramework/Character.h"
#include "Components/InputComponent.h"

// Sets default values
AOpenDoorTimelineCurve::AOpenDoorTimelineCurve()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

    Open = false;

    MyBoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("My Box Component"));
    MyBoxComponent->InitBoxExtent(FVector(50,50,50));
    RootComponent = MyBoxComponent;

    Door = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("My Mesh"));
    Door->SetRelativeLocation(FVector(0.0f, 50.0f, -50.0f));
    Door->SetupAttachment(RootComponent);

    MyBoxComponent->OnComponentBeginOverlap.AddDynamic(this, &AOpenDoorTimelineCurve::OnOverlapBegin);
    MyBoxComponent->OnComponentEndOverlap.AddDynamic(this, &AOpenDoorTimelineCurve::OnOverlapEnd);

}

// Called when the game starts or when spawned
void AOpenDoorTimelineCurve::BeginPlay()
{
	Super::BeginPlay();

    RotateValue = 1.0f;

    if (OpenCurve)
    {
        FOnTimelineFloat TimelineCallback;
        TimelineCallback.BindUFunction(this, FName("ControlDoor"));
        MyTimeline.AddInterpFloat(OpenCurve, TimelineCallback);
    }

    ToggleDoor();
}

// Called every frame
void AOpenDoorTimelineCurve::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

    MyTimeline.TickTimeline(DeltaTime);
}

void AOpenDoorTimelineCurve::OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{

    if ( (OtherActor != nullptr ) && (OtherActor != this) && ( OtherComp != nullptr ) ) 
    {

        // APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
        ACharacter* OurPlayerCharacter = UGameplayStatics::GetPlayerCharacter(this, 0);
        // APawn* OurPlayerCharacter = UGameplayStatics::GetPlayerPawn(this, 0);

        // Bind action event
        // OurPlayerCharacter->SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
        // {
        //     check(PlayerInputComponent);

        //     PlayerInputComponent->BindAction("Action", IE_Pressed, this, &AOpenDoorTimelineCurve::ToggleDoor);
        // }
        
        
        
    }


    if ( (OtherActor != nullptr ) && (OtherActor != this) && ( OtherComp != nullptr ) ) 
    {
        FVector PawnLocation = OtherActor->GetActorLocation();
        FVector Direction = GetActorLocation() - PawnLocation;
        Direction = UKismetMathLibrary::LessLess_VectorRotator(Direction, GetActorRotation());

        if(Direction.X > 0.0f)
        {
            RotateValue = 1.0f;
        }
        else
        {
            RotateValue = -1.0f;
        }

        DoorRotation = Door->RelativeRotation;
    }

}

void AOpenDoorTimelineCurve::OnOverlapEnd(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
    if ( (OtherActor != nullptr ) && (OtherActor != this) && ( OtherComp != nullptr ) )  
    {
        DoorRotation = Door->RelativeRotation;
    }
}

void AOpenDoorTimelineCurve::ControlDoor(float Value)
{
    if(Open) 
    {
        TimelineValue = MyTimeline.GetPlaybackPosition();
        CurveFloatValue = RotateValue*OpenCurve->GetFloatValue(TimelineValue);

        FQuat NewRotation = FQuat(FRotator(0.f, CurveFloatValue, 0.f));

        if (GEngine) 
        { 
            GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("Curve Float Value: %f"), CurveFloatValue));
        }

        Door->SetRelativeRotation(NewRotation);
    }
    else 
    {
        TimelineValue = MyTimeline.GetPlaybackPosition();
        CurveFloatValue = RotateValue*OpenCurve->GetFloatValue(TimelineValue);

        FQuat NewRotation = FQuat(FRotator(0.f, CurveFloatValue, 0.f));

        if (GEngine) 
        { 
            GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("Timeline Value: %f"), CurveFloatValue));
        }

        Door->SetRelativeRotation(NewRotation);
    }
}

void AOpenDoorTimelineCurve::ToggleDoor() 
{
    Open = !Open;

    if(Open)
    {
        Open = false;
        MyTimeline.PlayFromStart();
    }
    else 
    {
        Open = false;
        MyTimeline.Reverse();
    }

}