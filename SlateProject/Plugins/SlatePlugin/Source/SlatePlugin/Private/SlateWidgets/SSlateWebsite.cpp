// Fill out your copyright notice in the Description page of Project Settings.


#include "SlateWidgets/SSlateWebsite.h"
#include "SlateBasics.h"
#include "DebugHeader.h"
#include "SlatePlugin.h"

#include "SWebBrowser.h"


void SSlateWebsite::Construct(const FArguments& InArgs)
{
	bCanSupportFocus = true;
	TSharedPtr<SEditableTextBox> sed;
	ComboBoxSourceItems.Empty();

	FSlateFontInfo TitleTextFont = GetEmbossedTextFont();
	FSlateFontInfo smallTextFont = GetEmbossedTextFont();
	TitleTextFont.Size = 30;
	smallTextFont.Size = 12;
	auto slot1 = SNew(SBox);
	ChildSlot
		[
			SNew(SBox)
			.HAlign(HAlign_Center)
			.VAlign(VAlign_Center)

			[
				SNew(SVerticalBox)

				//First vertical slot for website title text


				+SVerticalBox::Slot()
				.AutoHeight()
				.Padding(0, 10)

				[
					SNew(STextBlock)
					.Text(FText::FromString(TEXT("Website Title")))
					.Font(TitleTextFont)
					.Justification(ETextJustify::Center)
					.ColorAndOpacity(FColor::Turquoise)

				]

				//Second vertical slot for captions of title text
				+ SVerticalBox::Slot()
				.AutoHeight()

				[
					SNew(STextBlock)

					.Text(FText::FromString(TEXT("Website Caption")))
					.Font(smallTextFont)
					.Justification(ETextJustify::Center)
					.ColorAndOpacity(FColor::White)
					.ShadowColorAndOpacity(FColor::Turquoise)
					.HighlightColor(FColor::Turquoise)
				]

				//Third vertical slot for username textblock
				+ SVerticalBox::Slot()
				.AutoHeight()
				[
					SNew(STextBlock)
					.Text(FText::FromString(TEXT(" ")))
					.Font(TitleTextFont)
					.Justification(ETextJustify::Left)
					.ColorAndOpacity(FColor::White)
				]

				+ SVerticalBox::Slot()
				.AutoHeight()
				[
					SNew(SEditableTextBox)
					.HintText(FText::FromString("Email Address"))
					.OnTextChanged(FOnTextChanged::CreateSP(this, &SSlateWebsite::SetUsername))
				]


		 	
				+SVerticalBox::Slot()
				.AutoHeight()
				.Padding(0, 10)
				[
					SNew(SButton)
					.Text(FText::FromString(TEXT("Continue")))
					.OnPressed(this, &SSlateWebsite::OnContinueButtonPressed)
					.HAlign(HAlign_Center)
					.ButtonColorAndOpacity(FColor::Turquoise)

				]
			]
		];

}

void SSlateWebsite::SetUsername(const FText& NewText)
{
	Username = NewText.ToString();
	if (NewText.IsEmpty())
	{

		DebugHeader::ShowMsgDialog(EAppMsgType::Ok, TEXT("Username cannot be empty"));
	}
	

}

void SSlateWebsite::OnContinueButtonPressed()
{
	FHttpRequestRef Request = FHttpModule::Get().CreateRequest();
	Request->OnProcessRequestComplete().BindRaw(this, &SSlateWebsite::OnResponseReceived);

	Request->SetURL("Enter your URL here" + Username);
	Request->SetVerb("GET");
	Request->ProcessRequest();
}



void SSlateWebsite::OnResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bConnectedSuccessfully)
{
	TSharedPtr<FJsonObject> ResponseObj;
	TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(Response->GetContentAsString());
	FJsonSerializer::Deserialize(Reader, ResponseObj);

	TArray<TSharedPtr<FJsonValue>> DataArray = ResponseObj->GetArrayField("data");

	for (int32 index = 0; index < DataArray.Num(); index++)
	{
		TSharedPtr<FJsonObject> obj = DataArray[index]->AsObject();
		TenantMap.Emplace(obj->GetStringField("tenantId"), obj->GetStringField("tenantName"));
		ComboBoxSourceItems.Add(MakeShared<FString>(obj->GetStringField("tenantName")));
	}

	DomainCount = ResponseObj->GetIntegerField("count");

	if (DomainCount != 0)
	{
		UE_LOG(LogTemp, Display, TEXT("DomainCount: %d"), DomainCount);

		TSharedRef< SComboBox < TSharedPtr <FString > > > ConstructedComboBox =
			SNew(SComboBox < TSharedPtr <FString > >)
			.OptionsSource(&ComboBoxSourceItems)
			.OnGenerateWidget(this, &SSlateWebsite::OnGenerateComboContent)
			.OnSelectionChanged(this, &SSlateWebsite::OnComboSelectionChanged)
			[
				SAssignNew(ComboDiplayTextBlock, STextBlock)
				.Text(FText::FromString(TEXT("List Tenant Option")))
			];


		FSlateFontInfo TitleTextFont = GetEmbossedTextFont();
		FSlateFontInfo SmallTextFont = GetEmbossedTextFont();
		FSlateFontInfo UserTextFont = GetEmbossedTextFont();
		TitleTextFont.Size = 30;
		SmallTextFont.Size = 10;
		UserTextFont.Size = 12;


		ChildSlot
			[
				SNew(SBox)
				.HAlign(HAlign_Center)
				.VAlign(VAlign_Center)
				[

					SNew(SVerticalBox)

					+ SVerticalBox::Slot()
					.AutoHeight()
					.Padding(0, 8)
					[
						SNew(STextBlock)
						.Text(FText::FromString(TEXT("Website Title")))
						.Font(TitleTextFont)
						.Justification(ETextJustify::Center)
						.ColorAndOpacity(FColor::Turquoise)
					]

					//Second vertical slot for captions of title text
					+ SVerticalBox::Slot()
					.AutoHeight()
					[
						SNew(STextBlock)
						.Text(FText::FromString(TEXT("Website Caption\n\n")))
						.Font(SmallTextFont)
						.Justification(ETextJustify::Center)
						.ColorAndOpacity(FColor::White)
					]

					+ SVerticalBox::Slot()
					.AutoHeight()
					.Padding(0, 8)
					[
						SNew(STextBlock)
						.Text(FText::FromString(Username))
						.Font(UserTextFont)
					]

					+ SVerticalBox::Slot()
					.AutoHeight()
					.Padding(0, 8)
					[
						ConstructedComboBox
					]

					+SVerticalBox::Slot()
					.AutoHeight()
					.Padding(0, 8)
					[
						SNew(SEditableTextBox)
						.HintText(FText::FromString(" Enter Password "))
						.IsPassword(true)
						.OnTextChanged(FOnTextChanged::CreateSP(this, &SSlateWebsite::SetLogin))
					]
					+ SVerticalBox::Slot()
					.AutoHeight()
					.Padding(0, 10)
					[
						SNew(SButton)
						.Text(FText::FromString(TEXT("Sign in")))
						.ForegroundColor(FColor::White)
						.HAlign(HAlign_Center)
						.ButtonColorAndOpacity(FColor::Turquoise)
						.OnPressed(this, &SSlateWebsite::OnSigninButtonPressed)
					]
				]
			];

	}
}

TSharedRef<SWidget> SSlateWebsite::OnGenerateComboContent(TSharedPtr<FString> SourceItem)
{
	TSharedRef <STextBlock> ContructedComboText = SNew(STextBlock)
		.Text(FText::FromString(*SourceItem.Get()));

	return ContructedComboText;
}

void SSlateWebsite::OnComboSelectionChanged(TSharedPtr<FString> SelectedOption,
	ESelectInfo::Type InSelectInfo)
{

	DebugHeader::Print(*SelectedOption.Get(), FColor::Cyan);
	UserSelectedDomain = *SelectedOption.Get();

	ComboDiplayTextBlock->SetText(FText::FromString(*SelectedOption.Get()));
	IsTenantSelected = true;

}

void SSlateWebsite::SetLogin(const FText& NewText)
{
	Password = NewText.ToString();
	if (Password.IsEmpty())
	{

		DebugHeader::ShowMsgDialog(EAppMsgType::Ok, TEXT("Password cannot be empty"));
	}

}

void SSlateWebsite::OnSigninButtonPressed()
{
	if (!Password.IsEmpty() && IsTenantSelected == true)

	{
		FHttpRequestRef Request = FHttpModule::Get().CreateRequest();

		TSharedRef <FJsonObject> RequestObj = MakeShared<FJsonObject>();
		TenantId = *TenantMap.FindKey(UserSelectedDomain);
		RequestObj->SetStringField("tenantId", *TenantMap.FindKey(UserSelectedDomain));
		RequestObj->SetStringField("username", Username);
		RequestObj->SetStringField("password", Password);

		FString RequestBody;
		TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&RequestBody);
		FJsonSerializer::Serialize(RequestObj, Writer);

		Request->OnProcessRequestComplete().BindRaw(this, &SSlateWebsite::OnLoginResponseReceived);
		Request->SetURL("Enter your Login URL");
		Request->SetVerb("POST");
		Request->SetHeader("Content-Type", "application/json");
		Request->SetContentAsString(RequestBody);
		Request->ProcessRequest();
	}
}

void SSlateWebsite::OnLoginResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bConnectedSuccessfully)
{
	TSharedPtr<FJsonObject> ResponseObj;
	TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(Response->GetContentAsString());
	FJsonSerializer::Deserialize(Reader, ResponseObj);

	AccessToken = ResponseObj->GetStringField("accessToken");
	if (!AccessToken.IsEmpty())
	{
		UE_LOG(LogTemp, Display, TEXT("Response: %s"), *AccessToken);

		FHttpRequestRef RequestAssetDetails = FHttpModule::Get().CreateRequest();
		RequestAssetDetails->OnProcessRequestComplete().BindRaw(this, &SSlateWebsite::OnGetAssetListResponseReceived);

		RequestAssetDetails->SetURL("Enter URL");
		RequestAssetDetails->SetVerb("GET");
		RequestAssetDetails->SetHeader("tenant-id", *TenantId);
		RequestAssetDetails->SetHeader("authorization-token", *AccessToken);
		RequestAssetDetails->ProcessRequest();
	}

	else
	{
		DebugHeader::ShowMsgDialog(EAppMsgType::Ok, TEXT("Password is incorrect or is empty"));
	}

}

void SSlateWebsite::OnGetAssetListResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bConnectedSuccessfully)
{
	TSharedPtr<FJsonObject> ResponseObj;
	TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(Response->GetContentAsString());
	FJsonSerializer::Deserialize(Reader, ResponseObj);

	AssetListCount = ResponseObj->GetIntegerField("count");
	UE_LOG(LogTemp, Display, TEXT("Response: %d"), AssetListCount);
	for (AssetListIndex = 0; AssetListIndex < AssetListCount; AssetListIndex++)
	{
		AssetListId = ResponseObj->GetArrayField("data").Last(AssetListCount - AssetListIndex - 1)->AsObject()->GetStringField("assetId");
		AssetListId = AssetListId;
		AssetListVerticalBoxes->AddSlot()
			
			.AutoHeight()
			.Padding(0, 2)
			[
				SNew(SButton)
				
			.ForegroundColor(FColor::White)
			.HAlign(HAlign_Center)
			.ButtonColorAndOpacity(FColor::Turquoise)
			.Tag(*AssetListId)
			[
				SNew(SEditableTextBox)
				.Text(FText::FromString(AssetListId))
			.OnTextCommitted(FOnTextCommitted::CreateSP(this, &SSlateWebsite::OnAssetButtonPressed))



			]

			];


	}

	ChildSlot
		[
			SNew(SScrollBox)

			+ SScrollBox::Slot()
		.HAlign(HAlign_Center)
		.VAlign(VAlign_Center)
		[
			AssetListVerticalBoxes

		]
		];


}

void SSlateWebsite::OnAssetButtonPressed(const FText& ButtonTag, ETextCommit::Type TextType)
{
	FHttpRequestRef RequestAssetDetails = FHttpModule::Get().CreateRequest();

	switch (TextType)
	{
	case ETextCommit::Default:

		break;
	case ETextCommit::OnEnter:
		break;
	case ETextCommit::OnUserMovedFocus:


		RequestAssetDetails->OnProcessRequestComplete().BindRaw(this, &SSlateWebsite::OnGetAssetDetailsResponseReceived);
		RequestAssetDetails->SetURL("Enter your 3D Mesh URL" + ButtonTag.ToString());
		RequestAssetDetails->SetVerb("GET");
		RequestAssetDetails->SetHeader("tenant-id", *TenantId);
		RequestAssetDetails->SetHeader("authorization-token", *AccessToken);
		RequestAssetDetails->ProcessRequest();

		break;
	case ETextCommit::OnCleared:
		break;
	default:
		break;
	}

}

void SSlateWebsite::OnGetAssetDetailsResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bConnectedSuccessfully)
{
	TSharedPtr<FJsonObject> ResponseObj;
	TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(Response->GetContentAsString());
	FJsonSerializer::Deserialize(Reader, ResponseObj);

	UE_LOG(LogTemp, Display, TEXT("Response: %s"), *Response->GetContentAsString());

	AssetId = ResponseObj->GetStringField("assetId");
	Description = ResponseObj->GetStringField("description");
	Title = ResponseObj->GetStringField("title");
	Collections = ResponseObj->GetStringField("collections");
	Tags = ResponseObj->GetStringField("tags");
	DefaultMetadata = ResponseObj->GetObjectField("defaultMetadata");
	DM_12345 = DefaultMetadata->GetStringField("12345");
	Country = DefaultMetadata->GetStringField("Country");
	Latest = DefaultMetadata->GetStringField("Latest");
	Opt = DefaultMetadata->GetStringField("Opt");
	RaphDemo = DefaultMetadata->GetStringField("RaphDemo");
	Req = DefaultMetadata->GetStringField("Req");
	Testing_5 = DefaultMetadata->GetStringField("Testing-5");
	Another = DefaultMetadata->GetStringField("another");
	Cvcvcvcvc = DefaultMetadata->GetStringField("cvcvcvcvc");
	Heheh = DefaultMetadata->GetStringField("heheh");
	Heuheuheu = DefaultMetadata->GetStringField("heuheuheu");
	Klklk = DefaultMetadata->GetStringField("klklk");
	More = DefaultMetadata->GetStringField("more");
	Newtest = DefaultMetadata->GetStringField("newtest");
	Test = DefaultMetadata->GetStringField("test");
	Test1 = DefaultMetadata->GetStringField("test1");
	Test5 = DefaultMetadata->GetStringField("test5");
	Testdefault = DefaultMetadata->GetStringField("testdefault");
	Wasasasas = DefaultMetadata->GetStringField("wasasasas");
	Zxc = DefaultMetadata->GetStringField("zxc");
	Zxzxzx = DefaultMetadata->GetStringField("zxzxzx");
	Zzzzzz = DefaultMetadata->GetStringField("zzzzzz");


	Versions = ResponseObj->GetArrayField("versions");
	AssetsArray = Versions.Last()->AsObject()->GetArrayField("assets");

	for (int32 index = 0; index < AssetsArray.Num(); index++)
	{
		TSharedPtr<FJsonObject> obj = AssetsArray[index]->AsObject();
		AssetMap.Emplace(obj->GetStringField("url"), obj->GetStringField("format"));
		ComboBoxSourceItemsDownload.Add(MakeShared<FString>(obj->GetStringField("format")));
	}


	TSharedRef< SComboBox < TSharedPtr <FString > > > ConstructedComboBoxDownload =
		SNew(SComboBox < TSharedPtr <FString > >)
		.OptionsSource(&ComboBoxSourceItemsDownload)
		.OnGenerateWidget(this, &SSlateWebsite::OnGenerateComboContent)
		.OnSelectionChanged(this, &SSlateWebsite::OnComboSelectionChanged)
		[
			SAssignNew(ComboDiplayTextBlock, STextBlock)
			.Text(FText::FromString(TEXT("Download")))
		];

	FSlateFontInfo TitleTextFont = GetEmbossedTextFont();
	FSlateFontInfo smallTextFont = GetEmbossedTextFont();
	TitleTextFont.Size = 30;
	smallTextFont.Size = 12;

	ChildSlot
		[
			SNew(SScrollBox)

			+ SScrollBox::Slot()

		.HAlign(HAlign_Center)
		.VAlign(VAlign_Center)


		[
			SNew(SVerticalBox)
			+ SVerticalBox::Slot()
		.AutoHeight()
		[
			ConstructedComboBoxDownload
		]
	+ SVerticalBox::Slot()
		.AutoHeight()
		.Padding(0, 10)
		[
			SNew(SButton)
			.Text(FText::FromString(TEXT("Download")))
		.OnPressed(this, &SSlateWebsite::OnDownloadButtonPressed)
		.HAlign(HAlign_Center)
		.ButtonColorAndOpacity(FColor::Turquoise)

		]
	+ SVerticalBox::Slot()
		.AutoHeight()
		[
			SNew(SHorizontalBox)
			+ SHorizontalBox::Slot() // Fieldname
		.AutoWidth()
		[
			SNew(SVerticalBox)
			+ SVerticalBox::Slot()
		.AutoHeight()
		.Padding(0, 8)
		[
			SNew(STextBlock)
			.Text(FText::FromString(TEXT("AssetId: ")))
		.ColorAndOpacity(FColor::Turquoise)
		]
		]
	+ SHorizontalBox::Slot() // Fieldname
		.AutoWidth()
		[
			SNew(SVerticalBox)
			+ SVerticalBox::Slot()
		.AutoHeight()
		.Padding(0, 8)
		[
			SNew(STextBlock)
			.Text(FText::FromString(AssetId))
		.ColorAndOpacity(FColor::White)
		.Font(smallTextFont)
		]
		]
		]

	+ SVerticalBox::Slot()
		.AutoHeight()
		[
			SNew(SHorizontalBox)
			+ SHorizontalBox::Slot() // Fieldname
		.AutoWidth()
		[
			SNew(SVerticalBox)
			+ SVerticalBox::Slot()
		.AutoHeight()
		.Padding(0, 8)
		[
			SNew(STextBlock)
			.Text(FText::FromString(TEXT("Description: ")))
		.ColorAndOpacity(FColor::Turquoise)
		]
		]
	+ SHorizontalBox::Slot() // Fieldname
		.AutoWidth()
		[
			SNew(SVerticalBox)
			+ SVerticalBox::Slot()
		.AutoHeight()
		.Padding(0, 8)
		[
			SNew(STextBlock)
			.Text(FText::FromString(Description))
		.ColorAndOpacity(FColor::White)
		]
		]
		]
	+ SVerticalBox::Slot()
		.AutoHeight()
		[
			SNew(SHorizontalBox)
			+ SHorizontalBox::Slot() // Fieldname
		.AutoWidth()
		[
			SNew(SVerticalBox)
			+ SVerticalBox::Slot()
		.AutoHeight()
		.Padding(0, 8)
		[
			SNew(STextBlock)
			.Text(FText::FromString(TEXT("Title: ")))
		.ColorAndOpacity(FColor::Turquoise)
		]
		]
	+ SHorizontalBox::Slot() // Fieldname
		.AutoWidth()
		[
			SNew(SVerticalBox)
			+ SVerticalBox::Slot()
		.AutoHeight()
		.Padding(0, 8)
		[
			SNew(STextBlock)
			.Text(FText::FromString(Title))
		.ColorAndOpacity(FColor::White)
		]
		]
		]

	+ SVerticalBox::Slot()
		.AutoHeight()
		[
			SNew(SHorizontalBox)
			+ SHorizontalBox::Slot() // Fieldname
		.AutoWidth()
		[
			SNew(SVerticalBox)
			+ SVerticalBox::Slot()
		.AutoHeight()
		.Padding(0, 8)
		[
			SNew(STextBlock)
			.Text(FText::FromString(TEXT("Collections: ")))
		.ColorAndOpacity(FColor::Turquoise)
		]
		]
	+ SHorizontalBox::Slot() // Fieldname
		.AutoWidth()
		[
			SNew(SVerticalBox)
			+ SVerticalBox::Slot()
		.AutoHeight()
		.Padding(0, 8)
		[
			SNew(STextBlock)
			.Text(FText::FromString(Collections))
		]
		]
		]

	+ SVerticalBox::Slot()
		.AutoHeight()
		[
			SNew(SHorizontalBox)
			+ SHorizontalBox::Slot() // Fieldname
		.AutoWidth()
		[
			SNew(SVerticalBox)
			+ SVerticalBox::Slot()
		.AutoHeight()
		.Padding(0, 8)
		[
			SNew(STextBlock)
			.Text(FText::FromString(TEXT("Tags: ")))
		.ColorAndOpacity(FColor::Turquoise)

		]
		]

	+ SHorizontalBox::Slot() // Fieldname
		.AutoWidth()
		[
			SNew(SVerticalBox)
			+ SVerticalBox::Slot()
		.AutoHeight()
		.Padding(0, 8)
		[
			SNew(STextBlock)
			.Text(FText::FromString(Tags))
		]
		]
		]

	+ SVerticalBox::Slot()
		.AutoHeight()
		.Padding(0, 16)
		[
			SNew(STextBlock)
			.Text(FText::FromString(TEXT("DefaultMetadata ")))
		.Font(smallTextFont)
		.ColorAndOpacity(FColor::Turquoise)
		]

	+ SVerticalBox::Slot()
		.AutoHeight()
		[
			SNew(SHorizontalBox)
			+ SHorizontalBox::Slot() // Fieldname
		.AutoWidth()
		[
			SNew(SVerticalBox)
			+ SVerticalBox::Slot()
		.AutoHeight()
		.Padding(0, 5)
		[
			SNew(STextBlock)
			.Text(FText::FromString(TEXT("DM_12345: ")))
		.ColorAndOpacity(FColor::White)
		]

	+ SVerticalBox::Slot()
		.AutoHeight()
		.Padding(0, 5)
		[
			SNew(STextBlock)
			.Text(FText::FromString(TEXT("Country: ")))
		.ColorAndOpacity(FColor::White)
		]


	+ SVerticalBox::Slot()
		.AutoHeight()
		.Padding(0, 5)
		[
			SNew(STextBlock)
			.Text(FText::FromString(TEXT("Latest: ")))
		.ColorAndOpacity(FColor::White)
		]


	+ SVerticalBox::Slot()
		.AutoHeight()
		.Padding(0, 5)
		[
			SNew(STextBlock)
			.Text(FText::FromString(TEXT("Opt: ")))
		.ColorAndOpacity(FColor::White)
		]
	+ SVerticalBox::Slot()
		.AutoHeight()
		.Padding(0, 5)
		[
			SNew(STextBlock)
			.Text(FText::FromString(TEXT("RaphDemo: ")))
		.ColorAndOpacity(FColor::White)
		]
	+ SVerticalBox::Slot()
		.AutoHeight()
		.Padding(0, 5)
		[
			SNew(STextBlock)
			.Text(FText::FromString(TEXT("Req: ")))
		.ColorAndOpacity(FColor::White)
		]
	+ SVerticalBox::Slot()
		.AutoHeight()
		.Padding(0, 5)
		[
			SNew(STextBlock)
			.Text(FText::FromString(TEXT("Testing-5: ")))
		.ColorAndOpacity(FColor::White)
		]

	+ SVerticalBox::Slot()
		.AutoHeight()
		.Padding(0, 5)
		[
			SNew(STextBlock)
			.Text(FText::FromString(TEXT("Another: ")))
		.ColorAndOpacity(FColor::White)
		]
	+ SVerticalBox::Slot()
		.AutoHeight()
		.Padding(0, 5)
		[
			SNew(STextBlock)
			.Text(FText::FromString(TEXT("Cvcvcvcvc: ")))
		.ColorAndOpacity(FColor::White)
		]

	+ SVerticalBox::Slot()
		.AutoHeight()
		.Padding(0, 5)
		[
			SNew(STextBlock)
			.Text(FText::FromString(TEXT("Heheh: ")))
		.ColorAndOpacity(FColor::White)
		]

	+ SVerticalBox::Slot()
		.AutoHeight()
		.Padding(0, 5)
		[
			SNew(STextBlock)
			.Text(FText::FromString(TEXT("Heuheuheu: ")))
		.ColorAndOpacity(FColor::White)
		]
	+ SVerticalBox::Slot()
		.AutoHeight()
		.Padding(0, 5)
		[
			SNew(STextBlock)
			.Text(FText::FromString(TEXT("Klklk: ")))
		.ColorAndOpacity(FColor::White)
		]

	+ SVerticalBox::Slot()
		.AutoHeight()
		.Padding(0, 5)
		[
			SNew(STextBlock)
			.Text(FText::FromString(TEXT("More: ")))
		.ColorAndOpacity(FColor::White)
		]
	+ SVerticalBox::Slot()
		.AutoHeight()
		.Padding(0, 5)
		[
			SNew(STextBlock)
			.Text(FText::FromString(TEXT("Newtest: ")))
		.ColorAndOpacity(FColor::White)
		]
	+ SVerticalBox::Slot()
		.AutoHeight()
		.Padding(0, 5)
		[
			SNew(STextBlock)
			.Text(FText::FromString(TEXT("Test: ")))
		.ColorAndOpacity(FColor::White)
		]
	+ SVerticalBox::Slot()
		.AutoHeight()
		.Padding(0, 5)
		[
			SNew(STextBlock)
			.Text(FText::FromString(TEXT("Test1: ")))
		.ColorAndOpacity(FColor::White)
		]
	+ SVerticalBox::Slot()
		.AutoHeight()
		.Padding(0, 5)
		[
			SNew(STextBlock)
			.Text(FText::FromString(TEXT("Test5: ")))
		.ColorAndOpacity(FColor::White)
		]
	+ SVerticalBox::Slot()
		.AutoHeight()
		.Padding(0, 5)
		[
			SNew(STextBlock)
			.Text(FText::FromString(TEXT("Testdefault: ")))
		.ColorAndOpacity(FColor::White)
		]
	+ SVerticalBox::Slot()
		.AutoHeight()
		.Padding(0, 5)
		[
			SNew(STextBlock)
			.Text(FText::FromString(TEXT("Wasasasas: ")))
		.ColorAndOpacity(FColor::White)
		]
	+ SVerticalBox::Slot()
		.AutoHeight()
		.Padding(0, 5)
		[
			SNew(STextBlock)
			.Text(FText::FromString(TEXT("Zxc: ")))
		.ColorAndOpacity(FColor::White)
		]
	+ SVerticalBox::Slot()
		.AutoHeight()
		.Padding(0, 5)
		[
			SNew(STextBlock)
			.Text(FText::FromString(TEXT("Zxzxzx: ")))
		.ColorAndOpacity(FColor::White)
		]
	+ SVerticalBox::Slot()
		.AutoHeight()
		.Padding(0, 5)
		[
			SNew(STextBlock)
			.Text(FText::FromString(TEXT("Zzzzzz: ")))
		.ColorAndOpacity(FColor::White)
		]
		]

	+ SHorizontalBox::Slot() // Fieldname
		.AutoWidth()
		[
			SNew(SVerticalBox)
			+ SVerticalBox::Slot()
		.AutoHeight()
		.Padding(0, 5)
		[
			SNew(STextBlock)
			.Text(FText::FromString(DM_12345))
		]


	+ SVerticalBox::Slot()
		.AutoHeight()
		.Padding(0, 5)
		[
			SNew(STextBlock)
			.Text(FText::FromString(Country))
		]


	+ SVerticalBox::Slot()
		.AutoHeight()
		.Padding(0, 5)
		[
			SNew(STextBlock)
			.Text(FText::FromString(Latest))
		]



	+SVerticalBox::Slot()
		.AutoHeight()
		.Padding(0, 5)
		[
			SNew(STextBlock)
			.Text(FText::FromString(Opt))
		]



	+SVerticalBox::Slot()
		.AutoHeight()
		.Padding(0, 5)
		[
			SNew(STextBlock)
			.Text(FText::FromString(RaphDemo))
		]



	+SVerticalBox::Slot()
		.AutoHeight()
		.Padding(0, 5)
		[
			SNew(STextBlock)
			.Text(FText::FromString(Req))
		]


	+ SVerticalBox::Slot()
		.AutoHeight()
		.Padding(0, 5)
		[
			SNew(STextBlock)
			.Text(FText::FromString(Testing_5))
		]

	+ SVerticalBox::Slot()
		.AutoHeight()
		.Padding(0, 5)
		[
			SNew(STextBlock)
			.Text(FText::FromString(Another))
		]

	+ SVerticalBox::Slot()
		.AutoHeight()
		.Padding(0, 5)
		[
			SNew(STextBlock)
			.Text(FText::FromString(Cvcvcvcvc))
		]

	+ SVerticalBox::Slot()
		.AutoHeight()
		.Padding(0, 5)
		[
			SNew(STextBlock)
			.Text(FText::FromString(Heheh))
		]


	+ SVerticalBox::Slot()
		.AutoHeight()
		.Padding(0, 5)
		[
			SNew(STextBlock)
			.Text(FText::FromString(Heuheuheu))
		]

	+ SVerticalBox::Slot()
		.AutoHeight()
		.Padding(0, 5)
		[
			SNew(STextBlock)
			.Text(FText::FromString(Klklk))
		]

	+ SVerticalBox::Slot()
		.AutoHeight()
		.Padding(0, 5)
		[
			SNew(STextBlock)
			.Text(FText::FromString(More))
		]

	+ SVerticalBox::Slot()
		.AutoHeight()
		.Padding(0, 5)
		[
			SNew(STextBlock)
			.Text(FText::FromString(Newtest))
		]

	+ SVerticalBox::Slot()
		.AutoHeight()
		.Padding(0, 5)
		[
			SNew(STextBlock)
			.Text(FText::FromString(Test))
		]

	+ SVerticalBox::Slot()
		.AutoHeight()
		.Padding(0, 5)
		[
			SNew(STextBlock)
			.Text(FText::FromString(Test1))
		]

	+ SVerticalBox::Slot()
		.AutoHeight()
		.Padding(0, 5)
		[
			SNew(STextBlock)
			.Text(FText::FromString(Test5))
		]

	+ SVerticalBox::Slot()
		.AutoHeight()
		.Padding(0, 5)
		[
			SNew(STextBlock)
			.Text(FText::FromString(Testdefault))
		]

	+ SVerticalBox::Slot()
		.AutoHeight()
		.Padding(0, 5)
		[
			SNew(STextBlock)
			.Text(FText::FromString(Wasasasas))
		]
	+ SVerticalBox::Slot()
		.AutoHeight()
		.Padding(0, 5)
		[
			SNew(STextBlock)
			.Text(FText::FromString(Zxc))
		]

	+ SVerticalBox::Slot()
		.AutoHeight()
		.Padding(0, 5)
		[
			SNew(STextBlock)
			.Text(FText::FromString(Zxzxzx))
		]
	+ SVerticalBox::Slot()
		.AutoHeight()
		.Padding(0, 5)
		[
			SNew(STextBlock)
			.Text(FText::FromString(Zzzzzz))
		]
		]
		]


		]

		];

}

void SSlateWebsite::OnDownloadButtonPressed()
{
	
	FHttpRequestRef RequestAssetDetails = FHttpModule::Get().CreateRequest();
	RequestAssetDetails->OnProcessRequestComplete().BindRaw(this, &SSlateWebsite::OnDownloadURLResponseReceived);
	RequestAssetDetails->SetURL(*AssetMap.FindKey(UserSelectedDomain));
	RequestAssetDetails->SetVerb("GET");

	RequestAssetDetails->ProcessRequest();
}

void SSlateWebsite::OnDownloadURLResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bConnectedSuccessfully)
{
	TSharedPtr<FJsonObject> ResponseObj;
	TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(Response->GetContentAsString());
	FJsonSerializer::Deserialize(Reader, ResponseObj);
	
	AssetRaw = Response->GetContent();
	AssetPath = FPaths::ProjectContentDir() + "\\" + AssetId;
	if (UserSelectedDomain.Contains("fbx"))
	{
		AssetPath = AssetPath + "\\fbx" + "\\" + AssetId + "." + *UserSelectedDomain;
	}
	else if (UserSelectedDomain.Contains("glb"))
	{
		AssetPath = AssetPath + "\\glb" + "\\" + AssetId + "." + *UserSelectedDomain;
	}
	else if (UserSelectedDomain.Contains("original"))
	{
		AssetPath = AssetPath + "\\glb" + "\\" + AssetId + "." + *UserSelectedDomain;
	}
	else if (UserSelectedDomain.Contains("usdz"))
	{
		AssetPath = AssetPath + "\\usdz" + "\\" + AssetId + "." + *UserSelectedDomain;
	}
	else if (UserSelectedDomain.Contains("png"))
	{
		AssetPath = AssetPath + "\\png" + "\\" + AssetId + "." + *UserSelectedDomain;
	}
	else
	{
		AssetPath = AssetPath + "\\other" + "\\" + AssetId + ".fbx";
	}

	UE_LOG(LogTemp, Display, TEXT("Asset file path will be: %s"), *AssetPath);
	FFileHelper::SaveArrayToFile(AssetRaw, *AssetPath);

}
