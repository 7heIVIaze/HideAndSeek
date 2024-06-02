// CopyrightNotice 2023 Sunggon Kim kimdave205@gmail.com. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "StoveSDKObject.h"
#include "MyStoveSDKObject.generated.h"

DECLARE_DELEGATE_OneParam(FDele_Delegate_Log, FString);

/**
 * 
 */
UCLASS()
class HIDEANDSEEK_API UMyStoveSDKObject : public UStoveSDKObject
{
	GENERATED_BODY()
public:
	UMyStoveSDKObject() {};

	//StoveAPI
public:

	// Versions or higher v2.4.1
	FStoveResult StoveSDKInit(const FStoveConfig& Config) override;
	/* Versions less then v2.4.1
	FStoveResult StoveSDKInit(FStoveConfig fConfig) override;
	*/
	FStoveResult StoveSDKGetToken() override;
	FStoveResult StoveSDKGetUser() override;
	FStoveResult StoveSDKUnInit() override;
	FStoveResult StoveSDKGetOwnership() override;
	FStoveResult StoveSDKSetLanguage(const FString& Language) override;

	//StoveCallback
public:

	virtual void OnInitComplete() override;
	virtual void OnError(FStoveError Error) override;
	virtual void OnToken(FStoveToken Token) override;
	virtual void OnUser(FStoveUser User) override;
	virtual void OnOwnership(int size, FStoveOwnership* Ownership) override;



public:
	FDele_Delegate_Log _OnDeleLog;
	uint64 LOGIN_USER_MEMBER_NO;
	
};
