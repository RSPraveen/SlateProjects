// Fill out your copyright notice in the Description page of Project Settings.

#pragma once


#include "Http.h"
#include "Widgets/SCompoundWidget.h"

class SSlateWebsite : public SCompoundWidget
{


	SLATE_BEGIN_ARGS(SSlateWebsite) {}

	SLATE_END_ARGS()


public:
	void Construct(const FArguments& InArgs);

private:

	UPROPERTY()
		FString Username;

	UPROPERTY()
		FString TenantId;

	UPROPERTY()
		FString AccessToken;



	UPROPERTY()
		bool IsTenantSelected = false;

	UPROPERTY()
		FString Password;

	TMap<FString, FString> TenantMap;
	TMap<FString, FString> AssetMap;

	FString UserSelectedDomain;

	FSlateFontInfo GetEmbossedTextFont() const { return FCoreStyle::Get().GetFontStyle(FName("EmbossedText")); }
	//Start of Asset Details Structure
	UPROPERTY()
		FString AssetListId;
	UPROPERTY()
		TMap<FString, FString> AssetListIdMap;
	UPROPERTY()
		FString AssetId;
	UPROPERTY()
		int32 AssetListIndex;
	UPROPERTY()
		FString Description;
	UPROPERTY()
		FString Title;
	UPROPERTY()
		FString Collections;
	UPROPERTY()
		FString Tags;
	UPROPERTY()
		TSharedPtr<FJsonObject> DefaultMetadata;
	UPROPERTY()
		FString DM_12345;
	UPROPERTY()
		FString Country;
	UPROPERTY()
		FString Latest;
	UPROPERTY()
		FString Opt;
	UPROPERTY()
		FString RaphDemo;
	UPROPERTY()
		FString Req;
	UPROPERTY()
		FString Testing_5;
	UPROPERTY()
		FString Another;
	UPROPERTY()
		FString Cvcvcvcvc;
	UPROPERTY()
		FString Heheh;
	UPROPERTY()
		FString Heuheuheu;
	UPROPERTY()
		FString Klklk;
	UPROPERTY()
		FString More;
	UPROPERTY()
		FString Newtest;
	UPROPERTY()
		FString Test;
	UPROPERTY()
		FString Test1;
	UPROPERTY()
		FString Test5;
	UPROPERTY()
		FString Testdefault;
	UPROPERTY()
		FString Wasasasas;
	UPROPERTY()
		FString Zxc;
	UPROPERTY()
		FString Zxzxzx;
	UPROPERTY()
		FString Zzzzzz;
	UPROPERTY()
		TSharedPtr<FJsonObject> TemplateMetadata;
	UPROPERTY()
		TArray<TSharedPtr<FJsonValue>> AssetsArray;
	UPROPERTY()
		TArray<TSharedPtr<FJsonValue>> Versions;
	UPROPERTY()
		FString Number;
	UPROPERTY()
		FString CreatedBy;
	UPROPERTY()
		FString CreatedDateTime;
	UPROPERTY()
		FString ThumbnailUrl;
	UPROPERTY()
		FString Assets;
	UPROPERTY()
		FString Format;
	UPROPERTY()
		FString IsOriginal;
	UPROPERTY()
		FString Url;
	UPROPERTY()
		FString AssetPath;
	//End of Asset Details Structure

	void SetUsername(const FText& NewText);
	void OnContinueButtonPressed();
	void OnAssetButtonPressed(const FText& ButtonTag, ETextCommit::Type TextType);
	void OnDownloadButtonPressed();
	void SetLogin(const FText& Password);
	void OnSigninButtonPressed();
	bool IsUsernameValid = false;

	TSharedPtr<STextBlock> TenantTextBlock;
	TSharedPtr<STextBlock> PasswordTextBlock;

	void OnResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bConnectedSuccessfully);
	void OnLoginResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bConnectedSuccessfully);
	void OnGetAssetDetailsResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bConnectedSuccessfully);
	void OnGetAssetListResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bConnectedSuccessfully);
	void OnDownloadURLResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bConnectedSuccessfully);

	UPROPERTY()
		int32 DomainCount;
	UPROPERTY()
		int32 AssetListCount;

	UPROPERTY()
		TArray<uint8> AssetRaw;

#pragma region ComboBox
	
	TArray< TSharedPtr <FString> > ComboBoxSourceItems;
	TArray< TSharedPtr <FString> > ComboBoxSourceItemsDownload;

	TSharedRef<SWidget> OnGenerateComboContent(TSharedPtr<FString> SourceItem);

	void OnComboSelectionChanged(TSharedPtr<FString> SelectedOption, ESelectInfo::Type InSelectInfo);

	TSharedPtr<STextBlock> ComboDiplayTextBlock;
	TSharedRef<SVerticalBox> AssetListVerticalBoxes = SNew(SVerticalBox);

#pragma endregion

};