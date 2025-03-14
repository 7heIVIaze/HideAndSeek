// CopyrightNotice 2023 Sunggon Kim kimdave205@gmail.com. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InputActionValue.h"
#include "Components/AudioComponent.h"
#include "Resource/InteractInterface.h"
#include "Resource/CustomDataTables.h"
#include "Player/PlayerStatComponent.h"
#include "Components/TimelineComponent.h"
#include "HorrorGameCharacter.generated.h"

class UInputComponent;
class USkeletalMeshComponent;
class USceneComponent;
class UCameraComponent;
class UAnimMontage;
class USoundBase;
class UPointLightComponent;
class USpotLightComponent;

//UENUM(BlueprintType)
//enum class Item_Num : uint8
//{
//	CigetLighter UMETA(DisplayName = "CigarLighter"),
//	FlashLight UMETA(DisplayName = "FlashLight"),
//	Key UMETA(DisplayName = "Key"),
//	Timer UMETA(DisplayName = "Timer"),
//	Sword UMETA(DisplayName = "Sword"),
//	Bell UMETA(DisplayName = "Bell"),
//	Mirror UMETA(DisplayName = "Mirror"),
//	Extinguisher UMETA(DisplayName = "Extinguisher"),
//	Cutter UMETA(DisplayName = "Cutter"),
//	SoulLantern UMETA(DisplayName = "SoulLantern"),
//};

UENUM(BlueprintType)
enum class Player_Status : uint8
{
	Loading UMETA(DisplayName = "Loading"),
	Survive UMETA(DisplayName = "Survive"),
	Chased UMETA(DisplayName = "Chased"),
	Catched UMETA(DisplayName = "Catched"),
	Stunned UMETA(DisplayName = "Stunned"),
	Hiding UMETA(DisplayName = "Hiding"), // 이 상태는 숨은 상태를 나타내는 것이 아닌, 숨은 오브젝트에 넣을 상태임.
	Died UMETA(DisplayName = "Died"),
	Clear UMETA(DisplayName = "Clear"),
	Ending UMETA(DisplayName = "Ending"),
};

UCLASS(config=Game)
class AHorrorGameCharacter : public ACharacter
{
	GENERATED_BODY()

	/** Pawn mesh: 1st person view (arms; seen only by self) */
	UPROPERTY(VisibleDefaultsOnly, Category=Mesh)
	USkeletalMeshComponent* Mesh1P;

	/** First person camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	UCameraComponent* FirstPersonCameraComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class USpringArmComponent> SpringArmComp;

	// 인벤토리 컴포넌트
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Inventory, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInventoryComponent> InventoryComp;

	// 플레이어 스탯 컴포넌트
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Status, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UPlayerStatComponent> PlayerStatComp;

	// 인터렉션 컴포넌트
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = LineTrace, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInteractionComponent> InteractionComp;

public:
	// 아이템 컴포넌트들.
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Light, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UChildActorComponent> CigarlighterComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Light, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UChildActorComponent> FlashlightComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Light, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UChildActorComponent> LanternComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Item, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UChildActorComponent> MirrorComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Item, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UChildActorComponent> SwordComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Item, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UChildActorComponent> FEComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Item, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UChildActorComponent> CompassComp;
	
public:
	AHorrorGameCharacter();

protected:
	virtual void BeginPlay();

	virtual void Tick(float DeltaTime);


public: // Unreal Property
	/** MappingContext */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputMappingContext* DefaultMappingContext;

	/** Look Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	class UInputAction* LookAction;

	/** Jump Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* JumpAction;

	/** Move Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* MoveAction;

	/** Game Pause Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* GamePauseAction;

	/** Sprint Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	class UInputAction* SprintAction;

	/** Crouch Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	class UInputAction* CrouchAction;

	/** Interaction Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	class UInputAction* InteractionAction;

	/** Use Item Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	class UInputAction* UseItemAction;

	/** Use Item Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	class UInputAction* LightingAction;

	/** Change Item Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	class UInputAction* ChangeItemAction;

	///** Bool for AnimBP to switch to another animation set */
	//UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Weapon)
	//bool bHasRifle;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Sprint", meta = (AllowPrivateAccess = "true"))
	int32 Stamina;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "FlashLight", meta = (AllowPrivateAccess = "true"))
	int32 FlashLightBattery;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Extinguisher", meta = (AllowPrivateAccess = "true"))
	int32 ExtinguisherLeft = 100;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Cutter", meta = (AllowPrivateAccess = "true"))
	int32 CutterDurability = 5;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Patience")
	int32 Patience = 0;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Patience")
	int32 PatienceToReduce = -1;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Patience")
	bool bIsPatienceReduce = false;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Walk")
	bool bIsMove = false;

	// 플레이어가 달리고 있는 중인지
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Sprint")
	bool bIsSprinting;

	// 플레이어가 웅크렸는지.
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Crouch")
	bool bIsCrouch;

	// 플레이어가 조명 아이템(랜턴 제외)을 켰는지. 요괴 감지 목적도 있음.
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Boolean)
	bool bIsLightOn;

	/*UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "FlashLight")
	bool bIsFlashLightOn;*/

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "FlashLight")
	bool bFLIntenseDown;

	//UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "CigarLight")
	//bool bIsCigarLightOn;

	/*UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Sound")
	bool bisSoundOn;*/

	/*UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Sound")
	bool bIsHiding = false;*/
	
	// 플레이어가 랜턴 아이템을 켰는지
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Lantern")
	bool bIsLanternOn = false;

	/*UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Status")
	Player_Status PlayerStatus = Player_Status::Survive;*/

	//UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "SoundCue")
	//class USoundCue* TurnOnSoundCue;

	/*UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "SoundCue")
	class USoundCue* CigarLightOnSoundCue;*/

	/*UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "SoundCue")
	class USoundCue* CigarLightOffSoundCue;*/

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "SoundCue")
		class USoundCue* KeySoundCue; // 열쇠 사용 소리

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "SoundCue")
		class USoundCue* CutterSoundCue; // 절단기 사용 소리

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "SoundCue")
		class USoundCue* PanicSoundCue; // 패닉 시 낼 소리

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "SoundCue")
		class USoundCue* ItemGetSoundCue; // 아이템을 얻을 시 낼 소리

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "SoundCue")
		class USoundCue* ExtinguisherCue; // 소화기 사용 소리

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "SoundCue")
		class USoundCue* ReaperSoundCue; // 리퍼 스킬 사용 소리

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "SoundCue")
		class USoundCue* StunSoundCue; // 기절 소리

	// About Item
	// 현재 선택한 인벤토리 인덱스
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UI")
	int32 CurrentItemNum = -1;

	// 소팅 후 다시 찾기 위한 아이템 번호
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UI")
	int32 SelectedItemNumber = 0;

	// 현재 인벤토리 내부의 아이템 수.
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UI")
	int32 InventoryNum = -1;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UI")
	FText InteractText = NSLOCTEXT("AHorrorGameCharacter", "InteractText", "");

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UI")
	FText ErrorInteractText = NSLOCTEXT("AHorrorGameCharacter", "ErrorText", "");

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Patience")
	bool bIsCooldown = false;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Patience")
	bool bIsScreaming = false;

	UPROPERTY()
	bool bIsBellSoundOn = false;

public: // Unreal Component
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	TArray<FHorrorGameItemData> Inventory;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "UI")
	TSubclassOf<class UGameUI> GameUIClass;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UI")
	TObjectPtr<class UGameUI> GameUIWidget;

	/*UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "CigarLight")
	TObjectPtr<UPointLightComponent> CigarLight;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "FlashLight")
	TObjectPtr<USpotLightComponent> FlashLight;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	TObjectPtr<UChildActorComponent> Lantern;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	TObjectPtr<UChildActorComponent> Sword;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Item")
	TObjectPtr<UChildActorComponent> Mirror;*/

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CurrentItems")
	TSubclassOf<class AItemClass> ItemActors;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Sound")
	TObjectPtr<UAudioComponent> Sound;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Sound")
	TObjectPtr<UAudioComponent> SprintSound;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Sound")
	TObjectPtr<UAudioComponent> Turnon;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Sound")
	TObjectPtr<UAudioComponent> PanicSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sound")
	TObjectPtr<UAudioComponent> CigarLightOnSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sound")
	TObjectPtr<UAudioComponent> CigarLightOffSound;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Sound")
	TObjectPtr<UAudioComponent>BellSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
		FVector MuzzleOffset;

	UPROPERTY(EditDefaultsOnly, Category = Projectile)
	TSubclassOf<class ATimerProjectile_cpp> ProjectileClass;

	UPROPERTY(EditAnywhere, Category = "Extinguisher")
	TObjectPtr<class UNiagaraComponent> SmokeComponent;
	
	UPROPERTY(EditDefaultsOnly, Category = Projectile)
	TSubclassOf<class AGlowStick> GlowStickClass;

	UPROPERTY()
	class AHorrorGamePlayerController* HorrorGamePlayerController;

	UPROPERTY(VisibleAnywhere, Category = "LightMesh")
		FTimeline FlickeringLight; // Create Timeline

	UPROPERTY(VisibleAnywhere, Category = "Camera")
		FTimeline RotateCameraTimeline; // Create Timeline

	UPROPERTY(VisibleAnywhere, Category = "Camera")
		FTimeline SprintCameraTimeline; // Create Timeline

	UPROPERTY(VisibleAnywhere, Category = "Camera")
		FTimeline CameraShakeTimeline; // Create Timeline

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Projectile)
	TObjectPtr<class UTimelineComponent> FlashlightTimeline;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Projectile)
	TObjectPtr<UCurveFloat> WalkingSway;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = SpringArm)
	float SprintSwaySpeed = 5.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = SpringArm)
	float WalkSwaySpeed = 0.25f;

	UPROPERTY(EditAnywhere, Category = "LightMesh")
		UCurveFloat* CurveFloat; // Timeline Curve -> 불빛 깜빡이는거 효과 줄 변수

	UPROPERTY(EditAnywhere, Category = "Camera")
		UCurveFloat* RotateCurveFloat; // Timeline Curve -> 카메라 회전하는 것 float 변수

	UPROPERTY(EditAnywhere, Category = "Camera")
		UCurveFloat* SprintCurveFloat; // Timeline Curve -> 카메라 FOV 설정을 변경할 float 변수

	UPROPERTY(EditAnywhere, Category = "Camera")
		UCurveFloat* ShakeCurveFloat; // Timeline Curve -> 카메라 쉐이크 설정을 변경할 float 변수

	UPROPERTY(EditAnywhere, Category = "ReaperEffect")
		class UMaterialInstance* PostProcessMaterialInstance;

	UPROPERTY(EditAnywhere, Category = "ReaperEffect")
		class UMaterialInstanceDynamic* PostProcessDynamicInstance;

	UPROPERTY(EditAnywhere, Category = "LightMesh")
		class UMaterialInstance* PostProcessMaterial;
	/*UPROPERTY(EditAnywhere, Category = "PPM")
		UCurveFloat* PPMFloat;*/ // Timeline Curve -> 거울 사용 시 색상 반전 효과 줄 변수

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "LightBoolean")
		int32 CreatureNum = 0; // Creature의 개수(불 깜박임을 위해서)


	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "RayCast")
		float TraceLength = 150.f; // Raycast 용 사거리

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Door")
		TObjectPtr<class UBoxComponent> DoorBoxComp;
		
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Camera")
	TSubclassOf<class UCameraShakeBase> CameraShake;

	float FlashIntensity = 15000.f, CigarIntensity = 5000.f;
	bool bCanExtinguisherUse = true, bCanItemGet = true;
	int32 SwordCount, MirrorCount, BellCount;

	UPROPERTY(BlueprintReadWrite)
		float MouseSensitive;

	UPROPERTY()
		bool bIsCleared;

	bool bIsFinishUnlock = false;

public: // Unreal Function

	/** Setter to set the bool */
	/*UFUNCTION(BlueprintCallable, Category = Weapon)
	void SetHasRifle(bool bNewHasRifle);*/

	/** Getter for the bool */
	/*UFUNCTION(BlueprintCallable, Category = Weapon)
	bool GetHasRifle();*/

	/*UFUNCTION(BlueprintCallable, Category = "FlashLight")
	void AddCigarLight();

	UFUNCTION(BlueprintCallable, Category = "FlashLight")
	void UseCigarLight();

	UFUNCTION()
	void CigarLightOn();

	UFUNCTION(BlueprintCallable, Category = "FlashLight")
	void AddFlashLight();

	UFUNCTION(BlueprintCallable, Category = "FlashLight")
	void UseFlashLight();

	UFUNCTION(BlueprintCallable, Category = "Key")
	void AddKey();

	UFUNCTION(BlueprintCallable, Category = "Key")
	void UseKey();

	UFUNCTION(BlueprintCallable, Category = "Timer")
	void AddTimer();

	UFUNCTION(BlueprintCallable, Category = "Timer")
	void UseTimer();

	UFUNCTION(BlueprintCallable, Category = "Sword")
	void AddSword();

	UFUNCTION(BlueprintCallable, Category = "Sword")
	void UseSword();

	UFUNCTION(BlueprintCallable, Category = "Bell")
	void AddBell();

	UFUNCTION(BlueprintCallable, Category = "Bell")
	void UseBell();

	UFUNCTION(BlueprintCallable, Category = "Mirror")
	void AddMirror();

	UFUNCTION(BlueprintCallable, Category = "Mirror")
	void UseMirror();

	UFUNCTION(BlueprintCallable, Category = "Extinguisher")
	void AddExtinguisher();

	UFUNCTION(BlueprintCallable, Category = "Extinguisher")
	bool UseExtinguisher();*/

	UFUNCTION(BlueprintCallable)
	FVector GetCameraComponentLocation();

	UFUNCTION(BlueprintCallable)
	FVector GetCameraComponentForwardVector();

	UFUNCTION(BlueprintCallable)
	bool TakeItems(class AItemClass* TakenItem);

	UFUNCTION(BlueprintCallable)
	bool ChangeItems(class AItemClass* TakenItem);

	// Use lighting item function
	UFUNCTION(BlueprintCallable)
	void UseLightingItem();

	// Change lighting item function
	UFUNCTION(BlueprintCallable)
	void ChangeLightingItem();

	// Turn on/off cigarlgihter.
	UFUNCTION(BlueprintCallable)
	bool ToggleCigarlight(bool inIsLightOn);

	// Set up the durability of flashlight
	UFUNCTION(BlueprintCallable)
	bool SetFlashlight(int inDurability);

	// Turn on/off flashlight.
	UFUNCTION(BlueprintCallable)
	bool ToggleFlashlight(bool inIsLightOn);

	// Get actor to interact with.
	UFUNCTION(BlueprintCallable)
	AActor* GetActorToInteract();

	// Throw the noise item.
	UFUNCTION(BlueprintCallable)
	bool UseNoiseItem();

	// Use Sword
	UFUNCTION(BlueprintCallable)
	bool UseSword();

	// Use Mirror effect
	UFUNCTION(BlueprintCallable)
	bool UseMirror();

	// 소화기 내구도 초기화 함수.
	UFUNCTION(BlueprintCallable)
	bool SetExtinguisher(int inDurability);

	// Spray the fire extinguisher
	UFUNCTION(BlueprintCallable)
	bool UseExtinguisher();

	// Turn on/off the lantern
	UFUNCTION(BlueprintCallable)
	bool SetLanternOn(bool inIsLightOn);

	// Put the glowstick under the feet.
	UFUNCTION(BlueprintCallable)
	bool UseGlowstick();

	// Use the compass
	UFUNCTION(BlueprintCallable)
	bool UseCompass();

	// Change the currently held item (enable child actor visibility of the currently held item)
	UFUNCTION(BlueprintCallable)
	void SwitchItem(const FHorrorGameItemData& CurrentItemData, int32 CurrentItemIndex);

	// Flashlight Sway
	UFUNCTION(BlueprintCallable)
	void HandleFlashlightSway(float Value);

	UFUNCTION(BlueprintCallable, Category = "Door")
		void OnDoorBoxBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION(BlueprintCallable, Category = "Door")
		void OnDoorBoxEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	// UFUNCTION(BlueprintCallable, Category = "Extinguisher")
	//void OnSprayOverlap(const TArray<FOverlapResult>& OverlapResults);
	
	/*UFUNCTION(BlueprintCallable, Category = "Cutter")
	void AddCutter();*/

	UFUNCTION(BlueprintCallable, Category = "Cutter")
	void UseCutter();

	//UFUNCTION(BlueprintCallable, Category = "SoulLantern")
	//	void AddLantern();

	/*UFUNCTION(BlueprintCallable, Category = "SoulLantern")
	void UseLantern();*/

	/*UFUNCTION(BlueprintCallable)
		void AddGlowStick();*/

	/*UFUNCTION(BlueprintCallable)
		void UseGlowStick();*/

	/*UFUNCTION()
	void StaminaChange();*/
	
	//UFUNCTION()
	//void FlashLightBatteryChange();

	/*UFUNCTION()
	void CurrentItem();*/

	// 상태 업데이트용 델리게이트 바인딩 함수.
	UFUNCTION(BlueprintCallable)
	void Death();

	UFUNCTION(BlueprintCallable)
	void SetPlayerStatus(EPlayerStatus PlayerStatus);

	UFUNCTION(BlueprintCallable)
	int32 GetStamina();

	/*UFUNCTION(BlueprintCallable)
	int32 GetFlashLightBattery();*/

	/*UFUNCTION(BlueprintCallable)
	int32 GetExtinguisherLeft();*/

	UFUNCTION(BlueprintCallable)
	float GetConfusionPoint();

	/*UFUNCTION(BlueprintCallable)
	FText GetInteractText();

	UFUNCTION(BlueprintCallable)
	FText GetErrorInteractText();*/

	UFUNCTION(BlueprintCallable)
	bool GetIsHiding();

	UFUNCTION(BlueprintCallable)
	bool GetIsSprinting();

	UFUNCTION(BlueprintCallable)
	EPlayerStatus GetPlayerStatus();

	UFUNCTION(BlueprintCallable)
	void AddConfusionPoint(int32 value);

	UFUNCTION(BlueprintCallable)
	void BellSoundFinish();

	//UFUNCTION(BlueprintCallable)
	//void BreatheSoundFinish();

	UFUNCTION(BlueprintCallable)
	void SetExplainText(FText text, int32 time);

	/*UFUNCTION(BlueprintCallable)
	void SetErrorText(FText text, int32 time);*/

	/*UFUNCTION(BlueprintCallable)
	void SetMainWidget(UGameUI* inMainWidget);*/

	UFUNCTION(BlueprintCallable)
	void SetReaperLookPlayer(bool inReaperWatchPlayer);

	UFUNCTION(BlueprintCallable)
		bool GetReaperLookPlayer();

	UFUNCTION(BlueprintCallable)
		void SetArchiveGetText(FText inText);

	// 인벤토리의 아이템을 아이템 번호 순으로 정렬하는 함수ㅊ
	//UFUNCTION(BlueprintCallable)
	//void InventorySorting();

	UFUNCTION(BlueprintCallable)
	UInventoryComponent* GetInventoryComponent() const { return InventoryComp; }

	UFUNCTION(BlueprintCallable)
	UPlayerStatComponent* GetStatComponent() const { return PlayerStatComp; }

private:
	FTimerHandle _loopLightTimerHandle;// , ArchiveTextTimer;
	// FTimerHandle _CigarLightTimerHandle;
	int32 TextTimer = 0, ErrorTextTimer = 0;
	float cnt = 0.f, ErrorTextCount = 0.f, StunTimer = 0.f, TimeStopTimer = 0.f, CooldownTimer = 0.f, ArchiveTextTimer = 0.f;
	float InWaterSpeedDown = 0.6f;
	float ReaperWatchElapsedTime = 0.f; // 리퍼가 능력을 사용하기 시작한 시간
	float PixelateIntensity = 0.f; // 리퍼가 능력을 사용해서 시야가 일그러지는 정도
	//bool bIsPlayerSwordGet = false, bIsPlayerMirrorGet = false, bIsPlayerBellGet = false;

	bool bIsInWater = false, bIsTimeStop = false, bIsTimeStopChange = false, bIsArchiveTextOn = false;
	bool bShouldAttack = false;
	bool bNotifyAttackStart = false;
	bool bReaperWatchPlayer = false; // 리퍼가 능력을 사용하기 시작했는가?
	
	//AActor* HitActor;
	FRotator LookAtRotation;
	FVector BeforeHideLocation;
	FRotator BeforeHideRotation;

	UPROPERTY()
		UDataTable* ItemTable;

protected:
	/** Called for movement input */
	void Move(const FInputActionValue& Value);

	/** Called for looking input */
	void Look(const FInputActionValue& Value);

	/* Called for Sprint Input */
	UFUNCTION(BlueprintCallable, Category = "Sprint")
	void BeginSprint();
	
	UFUNCTION(BlueprintCallable, Category = "Sprint")
	void EndSprint();

	/* Called for Crouch Input*/
	UFUNCTION(BlueprintCallable, Category = "Crouch")
	void BeginCrouch();

	UFUNCTION(BlueprintCallable, Category = "Crouch")
	void EndCrouch();

	// 아이템을 사용하는 함수.
	UFUNCTION(BlueprintCallable)
	void UseItem();

	// 아이템을 교체하는 함수
	UFUNCTION(BlueprintCallable)
	void ChangeInventoryItem(const FInputActionValue& Value);

	// 상호작용 함수.
	UFUNCTION(BlueprintCallable)
	void DoInteraction();

	UFUNCTION(BlueprintCallable)
	void DoHoldInteraction();

protected:
	// APawn interface
	virtual void SetupPlayerInputComponent(UInputComponent* InputComponent) override;
	// End of APawn interface

public:
	/** Returns Mesh1P subobject **/
	USkeletalMeshComponent* GetMesh1P() const { return Mesh1P; }
	/** Returns FirstPersonCameraComponent subobject **/
	UCameraComponent* GetFirstPersonCameraComponent() const { return FirstPersonCameraComponent; }

	/*UFUNCTION(BlueprintCallable, Category = "Interaction")
	bool GetLineTraceSingle(AActor* &HitActor);*/

	UFUNCTION(BlueprintCallable, Category = "CameraNoise")
	void SetCameraComponentNoise(int32 WhichStatus); // 0: 노이즈 해제, 1: 근처에 있음, 2: 추격 시작

	UFUNCTION(BlueprintCallable)
	void SetPlayerSetting(); // 플레이어가 설정한 세팅

	//UFUNCTION(BlueprintCallable, Category = "Inventory")
	//	void SelectItem(FKey fkey);
	
	/*UFUNCTION(BlueprintCallable, Category = "Inventory")
	void ScrollUpItem();

	UFUNCTION(BlueprintCallable, Category = "Inventory")
	void ScrollDownItem();*/

	UFUNCTION(BlueprintCallable, Category = "Speed")
	void SetIsInWater(bool value);

	/*UFUNCTION(BlueprintCallable, Category = "Inventory")
	int32 GetCurrentItemNumber();

	UFUNCTION(BlueprintCallable, Category = "Inventory")
	FText GetCurrentItemName();*/

	UFUNCTION(BlueprintCallable, Category = "Object")
	int32 GetObjectNumbers();

	/*UFUNCTION(BlueprintCallable, Category = "Object")
		int32 GetSwordNumbers();

	UFUNCTION(BlueprintCallable, Category = "Object")
		int32 GetMirrorNumbers();

	UFUNCTION(BlueprintCallable, Category = "Object")
		int32 GetBellNumbers();*/

	UFUNCTION(BlueprintCallable, Category = "Boolean")
	bool GetIsLightOn();

	UFUNCTION(BlueprintCallable)
		bool GetIsTimeStop();

	UFUNCTION(BlueprintCallable)
		bool GetIsTimeStopChange();

	UFUNCTION(BlueprintCallable)
		void LevelStart();

	UFUNCTION(BlueprintCallable)
		void LightFlicker(float value);

	UFUNCTION(BlueprintCallable)
		void SetIsTimeStop(bool value);

	UFUNCTION(BlueprintCallable)
		void SetNotifyAttackStart(const bool value);

	UFUNCTION(BlueprintCallable)
		bool GetNotifyAttackStart();

	UFUNCTION(BlueprintCallable)
		void AttackCheck(bool value);

	UFUNCTION(BlueprintCallable)
		bool GetAttackCheck();

	UFUNCTION(BlueprintCallable)
		bool GetIsScreaming();

	UFUNCTION(BlueprintCallable)
		void SetPanicScreamEnd();

	UFUNCTION(BlueprintCallable)
		void SetBrightness(float Value);

	UFUNCTION(BlueprintCallable)
		void OnFocus(FVector TargetLocation);

	UFUNCTION(BlueprintCallable)
		void OnFocusRotation(float inLerpAlpha);

	UFUNCTION(BlueprintCallable)
		void OnFocusFinished();

	UFUNCTION(BlueprintCallable)
		void OnSprintCameraView(float inLerpAlpha);

	UFUNCTION(BlueprintCallable)
		void OnAnnounce();

	UFUNCTION(BlueprintCallable)
		void OnCameraShake(float inTimeCurve);

	UFUNCTION(BlueprintCallable)
		void StartCameraShake();
};

