// Fill out your copyright notice in the Description page of Project Settings.


#include "Server/MainMenu.h"

#include "Components/Button.h"
#include "Components/WidgetSwitcher.h"
#include "Components/EditableTextBox.h"


bool UMainMenu::Initialize()
{
	bool Success = Super::Initialize();
	if (!Success) return false;

	/*if (!HostButton)
		return false;
	HostButton->OnClicked.AddDynamic(this, &UMainMenu::HostServer);*/

	if (!JoinButton)
		return false;
	JoinButton->OnClicked.AddDynamic(this, &UMainMenu::OpenJoinMenu);

	/*if (!CancelJoinMenuButton)
		return false;
	CancelJoinMenuButton->OnClicked.AddDynamic(this, &UMainMenu::OpenMainMenu);

	if (!ConfirmJoinMenuButton)
		return false;
	ConfirmJoinMenuButton->OnClicked.AddDynamic(this, &UMainMenu::JoinServer);*/

	if (!QuitButton)
		return false;
	QuitButton->OnClicked.AddDynamic(this, &UMainMenu::QuitPressed);

	return true;
}

void UMainMenu::HostServer()
{

	if (MenuInterface != nullptr)
	{
		MenuInterface->Host();
	}
}

void UMainMenu::JoinServer()
{
	if (nullptr != MenuInterface)
	{
		/*if (IPAddressField)
		{
			const FString& Address = IPAddressField->GetText().ToString();
			MenuInterface->Join(Address);
		}*/
	}
}

void UMainMenu::OpenJoinMenu()
{
	HostServer();
}

void UMainMenu::QuitPressed()
{
	UWorld* World = GetWorld();
	if (World)
	{
		APlayerController* PlayerController = World->GetFirstPlayerController();
		if (PlayerController)
		{
			PlayerController->ConsoleCommand("quit");
		}
	}
}