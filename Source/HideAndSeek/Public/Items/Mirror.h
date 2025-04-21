// CopyrightNotice 2023 Sunggon Kim kimdave205@gmail.com. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Items/ActiveItem.h"
#include "Mirror.generated.h"

UCLASS()
class HIDEANDSEEK_API AMirror : public AActiveItem
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMirror();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	virtual bool UseItem() override;

	////////////////
	// PROPERTIES //
	////////////////
public:
	// Mirror use sound
	UPROPERTY(VisibleAnywhere, Category = Light)
	class USoundCue* MirrorCue;

};
