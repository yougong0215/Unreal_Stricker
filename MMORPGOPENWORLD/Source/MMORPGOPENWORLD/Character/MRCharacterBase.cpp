// Fill out your copyright notice in the Description page of Project Settings.


#include "MRCharacterBase.h"

// Sets default values
AMRCharacterBase::AMRCharacterBase()
{
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AMRCharacterBase::BeginPlay()
{
	Super::BeginPlay();
	
}

void AMRCharacterBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AMRCharacterBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

