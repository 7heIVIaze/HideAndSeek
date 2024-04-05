// CopyrightNotice=0 2023 Sunggon Kim kimdave205@gmail.com

#include "MyStoveSDKObject.h"

#define OnLog(x, ...)  if(_OnDeleLog.IsBound() == true)\
                              {\
                                FString log = FString::Printf(TEXT(x), __VA_ARGS__);\
                                _OnDeleLog.ExecuteIfBound(log);\
                              }\
                              UE_LOG(LogTemp, Log, TEXT(x), __VA_ARGS__);\

//Versions less then v2.4.1
//FStoveResult UMyStoveSDKObject::StoveSDKInit(FStoveConfig fConfig)
//{
//    /*Add the 'walkthrough' codes here.*/
//    /*Ignore fConfig received as a function argument from this argument.*/
//
//	return FStoveResult{ 0 };
//}

//Versions or higher v2.4.1
FStoveResult UMyStoveSDKObject::StoveSDKInit(const FStoveConfig& Config)
{
    /*Add the 'walkthrough' codes here.*/
    /*Ignore fConfig received as a function argument from this argument.*/
    FStoveConfig ReplaceConfig{
        "LIVE",
        "8fd9c716765b7028c266bd298b38fe3b944fb565942ee38e54a5b5937592593b",
        "4a41985d0fa6756005def4bca0c7ba5eb63d991dd4819ac933e53386f0a2f10bde5582d346ae1a3a9e47ad01aacc11a6",
        "HIDEANDSEEK_IND",
        STOVE_PC_LOG_LEVEL_DEBUG,
        "" };  // logpath
    FStoveResult ErrorResult = Super::StoveSDKInit(ReplaceConfig);
    if (ErrorResult.Result == STOVE_PC_NO_ERROR)
    {
        OnLog("[Success] StovePC_Init");
    }
    else
    {
        OnLog("[Error] StovePC_Init, Result %d", ErrorResult.Result);
    }
    return ErrorResult;
}

void UMyStoveSDKObject::OnInitComplete()
{
    /*Add the 'walkthrough' codes here.*/
    OnLog("[Success] InitComplete");
}

void UMyStoveSDKObject::OnError(FStoveError Error)
{
    /*Add the 'walkthrough' codes here.*/
    OnLog("[Error]");
    OnLog("FuncType: %d", Error.FunctionType);
    OnLog("Result: %d", Error.ErrorResult.Result);
    OnLog("ExternalError: %d", Error.ExternalError);
}

FStoveResult UMyStoveSDKObject::StoveSDKUnInit()
{
    /*Add the 'walkthrough' codes here.*/

    FStoveResult ErrorResult = Super::StoveSDKUnInit();
    if (ErrorResult.Result == STOVE_PC_NO_ERROR)
    {        
        OnLog("[Success] StovePC_UnInit");    
    }
    else
    {
        OnLog("[Error] StovePC_UnInit, Result %d", ErrorResult.Result);        
    }
     
    return ErrorResult;
}

FStoveResult UMyStoveSDKObject::StoveSDKGetUser()
{
    /*Add the 'walkthrough' codes here.*/
    FStoveResult ErrorResult = Super::StoveSDKGetUser();
    if (ErrorResult.Result == STOVE_PC_NO_ERROR)
    {
        OnLog("[Success] StovePC_GetUser");
    }
    else
    {
        OnLog("[Error] StovePC_GetUser, ErrorResult %d", ErrorResult.Result);
    }
    return ErrorResult;
}

void UMyStoveSDKObject::OnUser(FStoveUser User)
{
    /*Add the 'walkthrough' codes here.*/
    LOGIN_USER_MEMBER_NO = User.MemberNo;
    OnLog("[User]");
    OnLog("MemberNo : %u", User.MemberNo);
    OnLog("Nickname : %s", *(User.Nickname));
    OnLog("GameUserId: %s", *(User.GameUserId));
}

FStoveResult UMyStoveSDKObject::StoveSDKGetToken()
{
    /*Add the 'walkthrough' codes here.*/

    FStoveResult ErrorResult = Super::StoveSDKGetToken();
    if (ErrorResult.Result == StovePCResult::STOVE_PC_NO_ERROR)
    {
        OnLog("[Success] StovePC_GetToken");
    }
    else
    {
        OnLog("[Error] StovePC_GetToken, Result %d", ErrorResult.Result);
    }

    return ErrorResult;
}

void UMyStoveSDKObject::OnToken(FStoveToken Token)
{
    /*Add the 'walkthrough' codes here.*/
    OnLog("[Token]");
    OnLog("Token : %s", *(Token.AccessToken));
}

FStoveResult UMyStoveSDKObject::StoveSDKGetOwnership()
{
    FStoveResult ErrorResult = Super::StoveSDKGetOwnership();
    if (ErrorResult.Result == StovePCResult::STOVE_PC_NO_ERROR)
    {
        /*성공 처리*/
        /*소유권에 관한 정보는 OnOwnership 콜백으로 전달됩니다.*/
    }

    return ErrorResult;
}

void UMyStoveSDKObject::OnOwnership(int size, FStoveOwnership* Ownership)
{
    bool owned = false;

    FStoveOwnership* data = Ownership;
    for (int i = 0; i < size; i++, data++)
    {
        if ((data->MemberNo != LOGIN_USER_MEMBER_NO /*StovePCUser 구조체의 memberNo*/)
            || (data->OwnershipCode != 1 /*1:소유권 획득, 2:소유권 해제(구매 취소한 경우)*/))
        {
            continue;
        }

        if (0 == wcscmp(L"HIDEANDSEEK_IND", *(data->GameId)) && data->GameCode == 3 /* // [GameCode] 3: BASIC 게임, 4: DEMO*/)
        {
            owned = true; // 소유권 확인 변수 true로 설정
        }

        /*DLC를 판매하는 게임일 때만 필요*/
        //if (0 == wcscmp(L"YOUR_DLC_ID", *(data->GameId)) && data->GameCode == 5 /* 5: DLC*/)
        //{
        //    /*YOUR_DLC_ID(DLC ID) 소유권이 있기에 DLC 플레이 허용*/
        //}
    }

    if (owned)
    {
        // 소유권 검증이 정상적으로 완료된 이후 게임진입 로직 작성
    }
    else
    {
        // 소유권 검증실패 후 게임을 종료하고 에러메세지 표출 로직 작성
    }
}

FStoveResult UMyStoveSDKObject::StoveSDKSetLanguage(const FString& Language)
{
    FStoveResult ErrorResult = Super::StoveSDKSetLanguage(Language);
    if (ErrorResult.Result == StovePCResult::STOVE_PC_NO_ERROR)
    {
        // 성공처리 
    }
    return ErrorResult;
}