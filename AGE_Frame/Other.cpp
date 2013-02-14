#include "../AGE_Frame.h"
using boost::lexical_cast;

void AGE_Frame::OnOpen(wxCommandEvent &Event)
{
	wxCommandEvent Selected;

	if(!SkipOpenDialog)
	{
		AGE_OpenDialog OpenBox(this, NeedDat);

		OpenBox.CheckBox_GenieVer->SetSelection(GameVersion);

		switch(DatUsed)
		{
			case 0:
			{
				OpenBox.Radio_DatFileLocation->SetValue(true);
				Selected.SetEventType(wxEVT_COMMAND_RADIOBUTTON_SELECTED);
				Selected.SetId(OpenBox.Radio_DatFileLocation->GetId());
				Selected.SetInt(true);
				OpenBox.GetEventHandler()->ProcessEvent(Selected);
			}
			break;
			case 1:
			{
			}
			break;
			case 2:
			{
				OpenBox.Radio_ApfFileLocation->SetValue(true);
				Selected.SetEventType(wxEVT_COMMAND_RADIOBUTTON_SELECTED);
				Selected.SetId(OpenBox.Radio_ApfFileLocation->GetId());
				Selected.SetInt(true);
				OpenBox.GetEventHandler()->ProcessEvent(Selected);
			}
			break;
			case 3:
			{
			}
			break;
		}

		OpenBox.DriveLetterBox->ChangeValue(DriveLetter);
		if(AGEwindow == 1) OpenBox.WindowCountBox->ChangeValue(lexical_cast<string>(SimultaneousFiles));
		OpenBox.Path_DatFileLocation->SetPath(DatFileName);
		OpenBox.Path_ApfFileLocation->SetPath(ApfFileName);

		OpenBox.CheckBox_LangFileLocation->SetValue(LangsUsed & 1);
		Selected.SetEventType(wxEVT_COMMAND_CHECKBOX_CLICKED);
		Selected.SetId(OpenBox.CheckBox_LangFileLocation->GetId());
		Selected.SetInt(LangsUsed & 1);
		OpenBox.GetEventHandler()->ProcessEvent(Selected);

		OpenBox.CheckBox_LangX1FileLocation->SetValue(LangsUsed & 2);
		Selected.SetEventType(wxEVT_COMMAND_CHECKBOX_CLICKED);
		Selected.SetId(OpenBox.CheckBox_LangX1FileLocation->GetId());
		Selected.SetInt(LangsUsed & 2);
		OpenBox.GetEventHandler()->ProcessEvent(Selected);

		OpenBox.CheckBox_LangX1P1FileLocation->SetValue(LangsUsed & 4);
		Selected.SetEventType(wxEVT_COMMAND_CHECKBOX_CLICKED);
		Selected.SetId(OpenBox.CheckBox_LangX1P1FileLocation->GetId());
		Selected.SetInt(LangsUsed & 4);
		OpenBox.GetEventHandler()->ProcessEvent(Selected);

		OpenBox.Path_LangFileLocation->SetPath(LangFileName);
		OpenBox.Path_LangX1FileLocation->SetPath(LangX1FileName);
		OpenBox.Path_LangX1P1FileLocation->SetPath(LangX1P1FileName);
		OpenBox.CheckBox_LangWrite->SetValue(WriteLangs);
		OpenBox.CheckBox_LangWriteToLatest->SetValue(LangWriteToLatest);

		if(OpenBox.ShowModal() != wxID_OK) return; // What this does?

		GameVersion = OpenBox.CheckBox_GenieVer->GetSelection();
		if(OpenBox.Radio_DatFileLocation->GetValue())
		{
			DatUsed = 0;
		}
		else if(OpenBox.Radio_ApfFileLocation->GetValue())
		{
			DatUsed = 2;
		}
		else
		{
			DatUsed = 3;
		}

		DriveLetter = OpenBox.DriveLetterBox->GetValue();
		if(AGEwindow == 1) SimultaneousFiles = lexical_cast<int>(OpenBox.WindowCountBox->GetValue());
		DatFileName = OpenBox.Path_DatFileLocation->GetPath();
		ApfFileName = OpenBox.Path_ApfFileLocation->GetPath();

		if(OpenBox.CheckBox_LangFileLocation->IsChecked())
		{
			LangsUsed |= 1;
		}
		else
		{
			LangsUsed &= ~1;
		}
		if(OpenBox.CheckBox_LangX1FileLocation->IsChecked())
		{
			LangsUsed |= 2;
		}
		else
		{
			LangsUsed &= ~2;
		}
		if(OpenBox.CheckBox_LangX1P1FileLocation->IsChecked())
		{
			LangsUsed |= 4;
		}
		else
		{
			LangsUsed &= ~4;
		}

		LangFileName = OpenBox.Path_LangFileLocation->GetPath();
		LangX1FileName = OpenBox.Path_LangX1FileLocation->GetPath();
		LangX1P1FileName = OpenBox.Path_LangX1P1FileLocation->GetPath();
		WriteLangs = OpenBox.CheckBox_LangWrite->GetValue();
		LangWriteToLatest = OpenBox.CheckBox_LangWriteToLatest->GetValue();
	}

	switch(GameVersion)
	{
		case 0:
			GenieVersion = genie::GV_AoE;
			break;
		case 1:
			GenieVersion = genie::GV_RoR;
			break;
		case 2:
			GenieVersion = genie::GV_AoK;
			break;
		case 3:
			GenieVersion = genie::GV_TC;
			break;
		case 4:
			GenieVersion = genie::GV_SWGB;
			break;
		case 5:
			GenieVersion = genie::GV_CC;
			break;
		default:
		GenieVersion = genie::GV_None;
	}

	if(Lang != NULL)
	{
		delete Lang;
		Lang = NULL;
	}
	if(LangX != NULL)
	{
		delete LangX;
		LangX = NULL;
	}
	if(LangXP != NULL)
	{
		delete LangXP;
		LangXP = NULL;
	}

	if(LangsUsed & 1)
	{
		if(WriteLangs)
		{
			Lang = new genie::LangFile();
			Lang->setDefaultCharset("Windows-1252");
			try
			{
				Lang->load(LangFileName.c_str());
			}
			catch(std::ios_base::failure e)
			{
				wxMessageBox("Failed to load "+LangFileName);
				delete Lang;
				Lang = NULL;
				return;
			}
		}
		else LanguageDLL[0] = LoadLibrary(LangFileName.c_str());
	}
	if(LangsUsed & 2)
	{
		if(WriteLangs)
		{
			LangX = new genie::LangFile();
			LangX->setDefaultCharset("Windows-1252");
			try
			{
				LangX->load(LangX1FileName.c_str());
			}
			catch(std::ios_base::failure e)
			{
				wxMessageBox("Failed to load "+LangX1FileName);
				delete LangX;
				LangX = NULL;
				return;
			}
		}
		else LanguageDLL[1] = LoadLibrary(LangX1FileName.c_str());
	}
	if(LangsUsed & 4)
	{
		if(WriteLangs)
		{
			LangXP = new genie::LangFile();
			LangXP->setDefaultCharset("Windows-1252");
			try
			{
				LangXP->load(LangX1P1FileName.c_str());
			}
			catch(std::ios_base::failure e)
			{
				wxMessageBox("Failed to load "+LangX1P1FileName);
				delete LangXP;
				LangXP = NULL;
				return;
			}
		}
		else LanguageDLL[2] = LoadLibrary(LangX1P1FileName.c_str());
	}

	switch(DatUsed)
	{
		case 0:
		{
			if(GenieFile != NULL)
			{
				delete GenieFile;
				GenieFile = NULL;
			}

			{
				SetStatusText("Reading file...", 0);
				wxBusyCursor WaitCursor;

				GenieFile = new genie::DatFile();
				try
				{
					GenieFile->setGameVersion(GenieVersion);
					GenieFile->load(DatFileName.c_str());
				}
				catch(std::ios_base::failure e)
				{
					wxMessageBox("Failed to load "+DatFileName);
					delete GenieFile;
					GenieFile = NULL;
					return;
				}
			}
		}
		break;
		case 2:
		{

		}
		break;
	}

	if(GenieFile != NULL)
	{	// Without these, nothing can be edited.
		SetStatusText("Listing...", 0);
		wxBusyCursor WaitCursor;
		//wxMessageBox("Started to open the file!");
		//Units_Civs_List->SetSelection(0);

//		No research gaia fix.
		for(short loop=0; loop < GenieFile->Civs[0].Units.size(); loop++)
			GenieFile->Civs[0].Units[loop].Enabled = GenieFile->Civs[1].Units[loop].Enabled;
//		ID and pointer fixes.
		for(short loop=0; loop < GenieFile->Civs.size(); loop++)
		{
			for(short loop2=0; loop2 < GenieFile->Civs[loop].Units.size(); loop2++)
			{
				if(GenieFile->Civs[loop].UnitPointers[loop2] != 0)
				{
					GenieFile->Civs[loop].UnitPointers[loop2] = 1;
					if(EnableIDFix)
					{
						GenieFile->Civs[loop].Units[loop2].ID1 = loop2;
						GenieFile->Civs[loop].Units[loop2].ID2 = loop2;
						if(GameVersion >= 2)
						GenieFile->Civs[loop].Units[loop2].ID3 = loop2;
						else
						if(GenieFile->Civs[loop].Units[loop2].Type >= 40 && GenieFile->Civs[loop].Units[loop2].Type <= 80)
						for(short loop3=0; loop3 < GenieFile->Civs[loop].Units[loop2].Bird.Commands.size(); loop3++)
						GenieFile->Civs[loop].Units[loop2].Bird.Commands[loop3].ID = loop3;
					}
				}
			}
		}
		if(EnableIDFix)
		{
			for(short loop=0; loop < GenieFile->PlayerColours.size(); loop++)
			{
				GenieFile->PlayerColours[loop].ID = loop;
			}
			for(short loop=0; loop < GenieFile->Sounds.size(); loop++)
			{
				GenieFile->Sounds[loop].ID = loop;
			}
			if(GameVersion >= 4)
			for(short loop=0; loop < GenieFile->UnitLines.size(); loop++)
			{
				GenieFile->UnitLines[loop].ID = loop;
			}
		}
		for(short loop=0; loop < GenieFile->Graphics.size(); loop++)
		{
			if(GenieFile->GraphicPointers[loop] != 0)
			{
				GenieFile->GraphicPointers[loop] = 1;
				if(EnableIDFix)
				GenieFile->Graphics[loop].ID = loop;
			}
		}
		for(short loop=0; loop < GenieFile->TerrainRestrictions.size(); loop++)
		{
			if(GenieFile->TerrainRestrictionPointers1[loop] != 0)
			GenieFile->TerrainRestrictionPointers1[loop] = 1;
			if(GameVersion >= 2)
			if(GenieFile->TerrainRestrictionPointers2[loop] != 0)
			GenieFile->TerrainRestrictionPointers2[loop] = 1;
		}

		Added = false;

		if(UnitCommands_ComboBox_Types->GetCount() > 0)
		{
			UnitCommands_ComboBox_Types->Clear();
		}
		UnitCommands_ComboBox_Types->Append("Unused Ability/Invalid Ability");	// Selection 0
		UnitCommands_ComboBox_Types->Append("Ability to Garrison");	// Selection 1
		UnitCommands_ComboBox_Types->Append("Ability to Mine Gold");
		UnitCommands_ComboBox_Types->Append("Ability to Mine Stone");
		UnitCommands_ComboBox_Types->Append("Ability to Fish, Forage, or Farm");
		UnitCommands_ComboBox_Types->Append("Ability to Rebuild");
		UnitCommands_ComboBox_Types->Append("Unknown Animal Ability");
		UnitCommands_ComboBox_Types->Append("Ability to Attack");
		UnitCommands_ComboBox_Types->Append("Ability to Fly");
		UnitCommands_ComboBox_Types->Append("Unknown Predator Animal Ability");
		UnitCommands_ComboBox_Types->Append("Ability to Unload (Boat-Like)");
		UnitCommands_ComboBox_Types->Append("Ability to Auto-Attack");
		UnitCommands_ComboBox_Types->Append("Unknown Farm Ability");
		UnitCommands_ComboBox_Types->Append("Ability to Build");
		UnitCommands_ComboBox_Types->Append("Ability to Convert");
		UnitCommands_ComboBox_Types->Append("Ability to Heal");
		UnitCommands_ComboBox_Types->Append("Ability to Repair");
		UnitCommands_ComboBox_Types->Append("Ability to Get Auto-converted?");
		UnitCommands_ComboBox_Types->Append("Type 109, Sub -1");
		UnitCommands_ComboBox_Types->Append("Ability to Chop Wood");
		UnitCommands_ComboBox_Types->Append("Ability to Hunt Prey Animals");
		UnitCommands_ComboBox_Types->Append("Ability to Hunt Predator Animals");
		UnitCommands_ComboBox_Types->Append("Ability to Trade");
		UnitCommands_ComboBox_Types->Append("Ability to Generate Wonder Victory*");
		UnitCommands_ComboBox_Types->Append("Type 121, Sub -1");
		UnitCommands_ComboBox_Types->Append("Ability to Mine Porex (Ore)");
		UnitCommands_ComboBox_Types->Append("Ability to Unpack & Attack");
		UnitCommands_ComboBox_Types->Append("Type 131, Sub -1");
		UnitCommands_ComboBox_Types->Append("Ability to Pickup Unit");
		UnitCommands_ComboBox_Types->Append("Ability to Kidnap Unit");
		UnitCommands_ComboBox_Types->Append("Ability to Deposit Unit");	// Selection 30
		UnitCommands_ComboBox_Types->SetSelection(0);

		if(Units_ComboBox_GarrisonType->GetCount() > 0)
		{
			Units_ComboBox_GarrisonType->Clear();
		}
		Units_ComboBox_GarrisonType->Append("No Type/Invalid Type");	// Selection 0
		if(GameVersion >= 2)
		{
			Units_ComboBox_GarrisonType->Append("0 - None");	// Selection 1
			Units_ComboBox_GarrisonType->Append("1 - Villager");
			Units_ComboBox_GarrisonType->Append("2 - Infantry");
			Units_ComboBox_GarrisonType->Append("3 - Villager + Infantry");
			Units_ComboBox_GarrisonType->Append("4 - Cavalry");
			Units_ComboBox_GarrisonType->Append("5 - Cavalry + Villager");
			Units_ComboBox_GarrisonType->Append("6 - Cavalry + Infantry");
			Units_ComboBox_GarrisonType->Append("7 - Cavalry + Infantry + Villager");
			Units_ComboBox_GarrisonType->Append("8 - Monk");
			Units_ComboBox_GarrisonType->Append("9 - Monk + Villager");
			Units_ComboBox_GarrisonType->Append("10 - Monk + Infantry");
			Units_ComboBox_GarrisonType->Append("11 - Monk + Infantry + Villager");
			Units_ComboBox_GarrisonType->Append("12 - Monk + Cavalry");
			Units_ComboBox_GarrisonType->Append("13 - Monk + Villager + Cavalry");
			Units_ComboBox_GarrisonType->Append("14 - Monk + Cavalry + Infantry");
			Units_ComboBox_GarrisonType->Append("15 - Monk + Villager + Infantry + Cavalry");
		}
		else if(GameVersion >= 4)
		{
			Units_ComboBox_GarrisonType->Append("0 - None");	// Selection 1
			Units_ComboBox_GarrisonType->Append("1 - Worker");
			Units_ComboBox_GarrisonType->Append("2 - Infantry");
			Units_ComboBox_GarrisonType->Append("3 - Worker + Infantry");
			Units_ComboBox_GarrisonType->Append("4 - Mounted");
			Units_ComboBox_GarrisonType->Append("5 - Mounted + Worker");
			Units_ComboBox_GarrisonType->Append("6 - Mounted + Infantry");
			Units_ComboBox_GarrisonType->Append("7 - Mounted + Infantry + Worker");
			Units_ComboBox_GarrisonType->Append("8 - Jedi");
			Units_ComboBox_GarrisonType->Append("9 - Jedi + Worker");
			Units_ComboBox_GarrisonType->Append("10 - Jedi + Infantry");
			Units_ComboBox_GarrisonType->Append("11 - Jedi + Infantry + Worker");
			Units_ComboBox_GarrisonType->Append("12 - Jedi + Mounted");
			Units_ComboBox_GarrisonType->Append("13 - Jedi + Worker + Mounted");
			Units_ComboBox_GarrisonType->Append("14 - Jedi + Mounted + Infantry");
			Units_ComboBox_GarrisonType->Append("15 - Jedi + Worker + Infantry + Mounted");
		}
		Units_ComboBox_GarrisonType->SetSelection(0);

		wxArrayString AoEClasses, SWGBClasses;
		AoEClasses.Alloc(62);
		AoEClasses.Add("0 - Archer");	// Selection 1
		AoEClasses.Add("1 - Artifact");
		AoEClasses.Add("2 - Trade Boat");
		AoEClasses.Add("3 - Building");
		AoEClasses.Add("4 - Civilian");
		AoEClasses.Add("5 - Sea Fish");
		AoEClasses.Add("6 - Soldier");
		AoEClasses.Add("7 - Berry Bush");
		AoEClasses.Add("8 - Stone Mine");
		AoEClasses.Add("9 - Prey Animal");
		AoEClasses.Add("10 - Predator Animal");
		AoEClasses.Add("11 - Other/Dead/Projectile");
		AoEClasses.Add("12 - Cavalry");
		AoEClasses.Add("13 - Siege Weapon");
		AoEClasses.Add("14 - Terrain");
		AoEClasses.Add("15 - Tree");
		AoEClasses.Add("16 - Tree Stump");
		AoEClasses.Add("17 - Unused");
		AoEClasses.Add("18 - Priest");
		AoEClasses.Add("19 - Trade Cart");
		AoEClasses.Add("20 - Transport Boat");
		AoEClasses.Add("21 - Fishing Boat");
		AoEClasses.Add("22 - War Boat");
		AoEClasses.Add("23 - Conquistador");
		AoEClasses.Add("24 - War Elephant");
		AoEClasses.Add("25 - Unused");
		AoEClasses.Add("26 - Elephant Archer");
		AoEClasses.Add("27 - Wall");
		AoEClasses.Add("28 - Phalanx");
		AoEClasses.Add("29 - Unused");
		AoEClasses.Add("30 - Flag");
		AoEClasses.Add("31 - Unused");
		AoEClasses.Add("32 - Gold Mine");
		AoEClasses.Add("33 - Shore Fish");
		AoEClasses.Add("34 - Cliff");
		AoEClasses.Add("35 - Petard/Chariot");
		AoEClasses.Add("36 - Cavalry Archer");
		AoEClasses.Add("37 - Dolphin/Smoke");
		AoEClasses.Add("38 - Bird");
		AoEClasses.Add("39 - Gate/Slinger");
		AoEClasses.Add("40 - Pile");
		AoEClasses.Add("41 - Pile of Resource");
		AoEClasses.Add("42 - Relic");
		AoEClasses.Add("43 - Monk with Relic");
		AoEClasses.Add("44 - Hand Cannoneer");
		AoEClasses.Add("45 - Two Handed Swordsman");
		AoEClasses.Add("46 - Pikeman");
		AoEClasses.Add("47 - Scout Cavalry");
		AoEClasses.Add("48 - Ore Mine");
		AoEClasses.Add("49 - Farm");
		AoEClasses.Add("50 - Spearman");
		AoEClasses.Add("51 - Packed Siege Unit");
		AoEClasses.Add("52 - Tower");
		AoEClasses.Add("53 - Boarding Boat");
		AoEClasses.Add("54 - Unpacked Siege Unit");
		AoEClasses.Add("55 - Scorpion");
		AoEClasses.Add("56 - Raider");
		AoEClasses.Add("57 - Cavalry Raider");
		AoEClasses.Add("58 - Sheep");
		AoEClasses.Add("59 - King");
		AoEClasses.Add("60 - Unused");
		AoEClasses.Add("61 - Horse");
		SWGBClasses.Alloc(65);
		SWGBClasses.Add("0 - Unused");	// Selection 1
		SWGBClasses.Add("1 - Nerf/Bantha");
		SWGBClasses.Add("2 - Fambaa");
		SWGBClasses.Add("3 - Unused");
		SWGBClasses.Add("4 - Wild Animal");
		SWGBClasses.Add("5 - Monster/Trouble");
		SWGBClasses.Add("6 - Wall");
		SWGBClasses.Add("7 - Farm");
		SWGBClasses.Add("8 - Gate");
		SWGBClasses.Add("9 - Fortress/A-A Turret");
		SWGBClasses.Add("10 - Turret");
		SWGBClasses.Add("11 - Cruiser");
		SWGBClasses.Add("12 - Unused");
		SWGBClasses.Add("13 - Destroyer");
		SWGBClasses.Add("14 - Utility Trawler");
		SWGBClasses.Add("15 - Frigate 1");
		SWGBClasses.Add("16 - A-A Destroyer 1");
		SWGBClasses.Add("17 - Transport Ship");
		SWGBClasses.Add("18 - Building");
		SWGBClasses.Add("19 - Doppleganger");
		SWGBClasses.Add("20 - Other/Dead/Projectile");
		SWGBClasses.Add("21 - Command Base");
		SWGBClasses.Add("22 - Cliff");
		SWGBClasses.Add("23 - Fish");
		SWGBClasses.Add("24 - Unused");
		SWGBClasses.Add("25 - Shore Fish");
		SWGBClasses.Add("26 - Game Engine Stuff");
		SWGBClasses.Add("27 - Fruit Bush");
		SWGBClasses.Add("28 - Holocron");
		SWGBClasses.Add("29 - Nova");
		SWGBClasses.Add("30 - Ore");
		SWGBClasses.Add("31 - Tree/Carbon");
		SWGBClasses.Add("32 - Artillery");
		SWGBClasses.Add("33 - A-A Mobile");
		SWGBClasses.Add("34 - Undeployed Cannon");
		SWGBClasses.Add("35 - Pummel");
		SWGBClasses.Add("36 - Cannon");
		SWGBClasses.Add("37 - Unused");
		SWGBClasses.Add("38 - Unused");
		SWGBClasses.Add("39 - Frigate 2");
		SWGBClasses.Add("40 - A-A Destroyer 2");
		SWGBClasses.Add("41 - Unused");
		SWGBClasses.Add("42 - Bridge/Eye Candy");
		SWGBClasses.Add("43 - Bomber");
		SWGBClasses.Add("44 - Bounty Hunter");
		SWGBClasses.Add("45 - Cargo Trader");
		SWGBClasses.Add("46 - Mixed 1");
		SWGBClasses.Add("47 - Scout");
		SWGBClasses.Add("48 - Fighter");
		SWGBClasses.Add("49 - Grenade Trooper");
		SWGBClasses.Add("50 - Jedi");
		SWGBClasses.Add("51 - Jedi with Holocron");
		SWGBClasses.Add("52 - Trooper");
		SWGBClasses.Add("53 - War Machine");
		SWGBClasses.Add("54 - Medic");
		SWGBClasses.Add("55 - A-A Trooper");
		SWGBClasses.Add("56 - Mounted Trooper");
		SWGBClasses.Add("57 - Fambaa Shield Generator");
		SWGBClasses.Add("58 - Workers");
		SWGBClasses.Add("59 - Air Transport");
		SWGBClasses.Add("60 - Horse-like Animal");
		SWGBClasses.Add("61 - Power Droid");
		SWGBClasses.Add("62 - Air Cruiser");
		SWGBClasses.Add("63 - Geonosian Warrior");
		SWGBClasses.Add("64 - Jedi Starfighter");

		if(Effects_ComboBox_Class->GetCount() > 0) Effects_ComboBox_Class->Clear();
		Effects_ComboBox_Class->Append("No Class/Invalid Class");	// Selection 0
		if(GameVersion < 4)
		{
			Effects_ComboBox_Class->Append(AoEClasses);
		}
		else
		{
			Effects_ComboBox_Class->Append(SWGBClasses);
		}
		Effects_ComboBox_Class->SetSelection(0);

		Customs = new wxFileConfig(wxEmptyString, "Tapsa", "age2lists.ini", wxEmptyString, wxCONFIG_USE_LOCAL_FILE | wxCONFIG_USE_RELATIVE_PATH);
		long ExtraCount;
		Customs->Read("Count/ExtraCount", &ExtraCount, 5);
		wxString MoveHolder = "";
		for(short loop=0; loop < 3; loop++)
		{
			if(Attacks_ComboBox_Class[loop]->GetCount() > 0)
			{
				Attacks_ComboBox_Class[loop]->Clear();
			}
			Attacks_ComboBox_Class[loop]->Append("Unused Class/No Class");	// Selection 0
			if(GameVersion < 2) // AoE and RoR
			{	// Use "atc -1|arc -1|disa" to discover these!
				Attacks_ComboBox_Class[loop]->Append("0 - Stone Defense & Fire Galley");
				Attacks_ComboBox_Class[loop]->Append("1 - Stone Defense & Archers");
				Attacks_ComboBox_Class[loop]->Append("2 - Unused");
				Attacks_ComboBox_Class[loop]->Append("3 - Base Pierce");
				Attacks_ComboBox_Class[loop]->Append("4 - Base Melee");
				Attacks_ComboBox_Class[loop]->Append("5 - Unused");
				Attacks_ComboBox_Class[loop]->Append("6 - Buildings");
				Attacks_ComboBox_Class[loop]->Append("7 - Priests");
				Attacks_ComboBox_Class[loop]->Append("8 - Cavalry");
				Attacks_ComboBox_Class[loop]->Append("9 - Infantry");
				Attacks_ComboBox_Class[loop]->Append("10 - Stone Defense");
				Attacks_ComboBox_Class[loop]->Append("11 - Unused");
				Attacks_ComboBox_Class[loop]->Append("12 - Villagers & Gazelles & Medusa");
				Attacks_ComboBox_Class[loop]->Append("13 - Unused");
				Attacks_ComboBox_Class[loop]->Append("14 - Unused");
				Attacks_ComboBox_Class[loop]->Append("15 - Unused");
				Attacks_ComboBox_Class[loop]->Append("16 - Unused");
				Attacks_ComboBox_Class[loop]->Append("17 - Unused");
				Attacks_ComboBox_Class[loop]->Append("18 - Unused");
				Attacks_ComboBox_Class[loop]->Append("19 - Unused");
				Attacks_ComboBox_Class[loop]->Append("20 - Unused");
				Attacks_ComboBox_Class[loop]->Append("21 - Unused");
				Attacks_ComboBox_Class[loop]->Append("22 - Unused");
				Attacks_ComboBox_Class[loop]->Append("23 - Unused");
				Attacks_ComboBox_Class[loop]->Append("24 - Unused");
				Attacks_ComboBox_Class[loop]->Append("25 - Unused");
				Attacks_ComboBox_Class[loop]->Append("26 - Unused");
				Attacks_ComboBox_Class[loop]->Append("27 - Unused");
				Attacks_ComboBox_Class[loop]->Append("28 - Unused");
				Attacks_ComboBox_Class[loop]->Append("29 - Unused");
				Attacks_ComboBox_Class[loop]->Append("30 - Unused");	// Selection 31
			}
			else if(GameVersion < 4) // AoK and TC
			{
				Attacks_ComboBox_Class[loop]->Append("0 - Unused");
				Attacks_ComboBox_Class[loop]->Append("1 - Infantry");	// Selection 2
				Attacks_ComboBox_Class[loop]->Append("2 - Turtle Ships");
				Attacks_ComboBox_Class[loop]->Append("3 - Base Pierce");
				Attacks_ComboBox_Class[loop]->Append("4 - Base Melee");
				Attacks_ComboBox_Class[loop]->Append("5 - War Elephants");
				Attacks_ComboBox_Class[loop]->Append("6 - Unused");
				Attacks_ComboBox_Class[loop]->Append("7 - Unused");
				Attacks_ComboBox_Class[loop]->Append("8 - Cavalry");
				Attacks_ComboBox_Class[loop]->Append("9 - Unused");
				Attacks_ComboBox_Class[loop]->Append("10 - Unused");
				Attacks_ComboBox_Class[loop]->Append("11 - All Buildings (except Port)");
				Attacks_ComboBox_Class[loop]->Append("12 - Unused");
				Attacks_ComboBox_Class[loop]->Append("13 - Stone Defense");
				Attacks_ComboBox_Class[loop]->Append("14 - Unused");
				Attacks_ComboBox_Class[loop]->Append("15 - Archers");
				Attacks_ComboBox_Class[loop]->Append("16 - Ships & Camels & Saboteurs");
				Attacks_ComboBox_Class[loop]->Append("17 - Rams");
				Attacks_ComboBox_Class[loop]->Append("18 - Trees");
				Attacks_ComboBox_Class[loop]->Append("19 - Unique Units (except Turtle Ship)");
				Attacks_ComboBox_Class[loop]->Append("20 - Siege Weapons");
				Attacks_ComboBox_Class[loop]->Append("21 - Standard Buildings");
				Attacks_ComboBox_Class[loop]->Append("22 - Walls & Gates");
				Attacks_ComboBox_Class[loop]->Append("23 - Unused");
				Attacks_ComboBox_Class[loop]->Append("24 - Boars");
				Attacks_ComboBox_Class[loop]->Append("25 - Monks");
				Attacks_ComboBox_Class[loop]->Append("26 - Castle");
				Attacks_ComboBox_Class[loop]->Append("27 - Spearmen");
				Attacks_ComboBox_Class[loop]->Append("28 - Cavalry Archers");
				Attacks_ComboBox_Class[loop]->Append("29 - Eagle Warriors");
				Attacks_ComboBox_Class[loop]->Append("30 - Unused");	// Selection 31
			}
			else // SWGB and CC
			{
				Attacks_ComboBox_Class[loop]->Append("0 - Aircraft");	// Selection 1
				// Airspeeder
				// AIR SHIPS!!!
				// Geonosian Warrior
				// Wild Gungan Flyer
				Attacks_ComboBox_Class[loop]->Append("1 - Heavy Assault Machines");	// Selection 2
				// Assault Mech
				// AT-AT
				// Blizzards
				Attacks_ComboBox_Class[loop]->Append("2 - Heavy Weapons");
				// Undeployed Cannon
				// Artillery
				// A-A Mobiles
				// Pummels
				// Strike Mechs
				// Mech Destroyers
				// Cannon
				// Decimator
				// AT-AT
				// Echo Base Ion Cannon
				// Blizzards
				// Evok Catapult
				Attacks_ComboBox_Class[loop]->Append("3 - Base Melee/Armor");
				Attacks_ComboBox_Class[loop]->Append("4 - Base Ranged/DuraArmor");
				Attacks_ComboBox_Class[loop]->Append("5 - Jedis & Bounty Hunters");
				// Jedi
				// Jedi with Holocron
				// Bounty Hunter
				Attacks_ComboBox_Class[loop]->Append("6 - Assault Machines");
				// Destroyer Droids
				// Strike Mechs
				// Mech Destroyers
				// Assault Mechs
				// Scouts
				// Jabba's Sail Barge
				// Desert Skiff
				// Decimator
				// AT-AT
				// Blizzards
				Attacks_ComboBox_Class[loop]->Append("7 - Decimators");
				// Assault Mechs
				// Decimator
				// AT-AT
				// Blizzards
				Attacks_ComboBox_Class[loop]->Append("8 - Shield & Power Units");
				// Power Cores
				// Shield Wall
				// Shield Generators
				// Droid Storage Facility
				// Power Droids
				// Echo Base Power Generator
				Attacks_ComboBox_Class[loop]->Append("9 - Ships");
				// Utility Trawler
				// Frigates
				// Cruisers
				// Destroyers
				// A-A Destroyers
				// Transport Ships
				Attacks_ComboBox_Class[loop]->Append("10 - Submarines");
				// Frigates
				// Underwater Prefab Shelters
				// Gungan Buildings
				Attacks_ComboBox_Class[loop]->Append("11 - All Buildings");
				// BUILDINGS!!!
				Attacks_ComboBox_Class[loop]->Append("12 - Unused");
				Attacks_ComboBox_Class[loop]->Append("13 - Defense Buildings");
				// Gate
				// Turrets
				// A-A Turrets
				// Echo Base Ion Cannon
				// Fortress
				// Theed Arch
				Attacks_ComboBox_Class[loop]->Append("14 - Troopers");
				// Royal Crusaders
				// Berserker
				// Chewbacca
				// Bounty Hunter
				// Troopers
				// Mounted Troopers
				// Grenade Trooper
				// A-A Troopers
				// Acklay
				// Nexu
				// Klaatu
				// Nikito
				// Reek
				// Rancor
				Attacks_ComboBox_Class[loop]->Append("15 - Mounted Troopers");
				// Berserker
				// Scouts
				// Mounted Troopers
				// Anakin's Podracer
				// Landspeeder
				// Sebulba's Podracer
				// Skyhopper
				Attacks_ComboBox_Class[loop]->Append("16 - Cruisers");
				// Cruisers
				Attacks_ComboBox_Class[loop]->Append("17 - Pummels & Cannons");
				// Undeployed Cannon
				// Pummels
				// Cannon
				Attacks_ComboBox_Class[loop]->Append("18 - Unused");
				Attacks_ComboBox_Class[loop]->Append("19 - Workers");
				// B'omarr Temple
				// Underwater Prefab Shelters
				// Asteroid Supply Depot
				// Boorka's Palace
				// Adv A-A Turret
				// Reytha Soldier
				// Mara Jade
				// R2-D2
				// Battleship Cores
				// Landed Freighter
				// Han Solo on Tauntaun
				Attacks_ComboBox_Class[loop]->Append("20 - Destroyers");
				// Destroyers
				Attacks_ComboBox_Class[loop]->Append("21 - Standard Buildings");
				// BUILDINGS!!!
				Attacks_ComboBox_Class[loop]->Append("22 - Walls & Gates");
				// GATES
				// WALLS
				// Theed Arch
				Attacks_ComboBox_Class[loop]->Append("23 - Air Cruisers");
				// Air Cruisers
				// Blockade Runner
				// Star Destroyer
				// Deathstar
				Attacks_ComboBox_Class[loop]->Append("24 - Wild Animals");
				// Wild Fambaa
				// Acklay
				// Falumpaset
				// Nexu
				// Reek
				// Dewback
				// Ronto
				// Fambaa
				// Massif
				// Orray
				// Shaak
				// Rancor
				Attacks_ComboBox_Class[loop]->Append("25 - Unused");
				Attacks_ComboBox_Class[loop]->Append("26 - Fortress");
				// Fortress
				Attacks_ComboBox_Class[loop]->Append("27 - Unused");
				Attacks_ComboBox_Class[loop]->Append("28 - Unused");
				Attacks_ComboBox_Class[loop]->Append("29 - Unused");
				Attacks_ComboBox_Class[loop]->Append("30 - Tame Animals");	// Selection 31
				// Fambaa Shield Generators
				// Wild Fambaa
				// Kaadu
				// Tauntaun
				// Cu-pa
				// Womp Rat
			}
			for(short loop2=0; loop2 < ExtraCount; loop2++)
			{
				Customs->Read("Names/"+lexical_cast<string>(loop2+31), &MoveHolder, lexical_cast<string>(loop2+31)+" - Extra Class");
				Attacks_ComboBox_Class[loop]->Append(MoveHolder);
			}
			Attacks_ComboBox_Class[loop]->SetSelection(0);
		}
		delete Customs;

		for(short loop=0; loop < 2; loop++)
		{
			if(Units_ComboBox_Class[loop]->GetCount() > 0) Units_ComboBox_Class[loop]->Clear();
			Units_ComboBox_Class[loop]->Append("No Class/Invalid Class");	// Selection 0
			if(GameVersion < 4)
			{
				Units_ComboBox_Class[loop]->Append(AoEClasses);
			}
			else
			{
				Units_ComboBox_Class[loop]->Append(SWGBClasses);
			}
			Units_ComboBox_Class[loop]->SetSelection(0);

			if(Units_Units_SearchFilters[loop]->GetCount() > 0)
			{
				Units_Units_SearchFilters[loop]->Clear();
			}
			Units_Units_SearchFilters[loop]->Append("Lang DLL Name");	// 0
			Units_Units_SearchFilters[loop]->Append("Internal Name");
			Units_Units_SearchFilters[loop]->Append("Type");
			Units_Units_SearchFilters[loop]->Append("Class");
			Units_Units_SearchFilters[loop]->Append("Terrain Restriction");
			Units_Units_SearchFilters[loop]->Append("Max Range");
			Units_Units_SearchFilters[loop]->Append("Train Location");
			Units_Units_SearchFilters[loop]->Append("Attacks");
			Units_Units_SearchFilters[loop]->Append("Armors");
			Units_Units_SearchFilters[loop]->Append("Commands");
			Units_Units_SearchFilters[loop]->Append("Pointer");
		/*	Units_Units_SearchFilters[loop]->Append("Garrison Type");
			Units_Units_SearchFilters[loop]->Append("Projectile Unit");
			Units_Units_SearchFilters[loop]->Append("Enabled");
			Units_Units_SearchFilters[loop]->Append("Hidden In Editor");
			Units_Units_SearchFilters[loop]->Append("Visible In Fog");
			Units_Units_SearchFilters[loop]->Append("Death Mode");
			Units_Units_SearchFilters[loop]->Append("Hero Mode");
			Units_Units_SearchFilters[loop]->Append("Air Mode");
			Units_Units_SearchFilters[loop]->Append("Fly Mode");
			Units_Units_SearchFilters[loop]->Append("Building Mode");
			Units_Units_SearchFilters[loop]->Append("Placement Mode");
			Units_Units_SearchFilters[loop]->Append("Interaction Mode");
			Units_Units_SearchFilters[loop]->Append("Minimap Mode");
			Units_Units_SearchFilters[loop]->Append("Sheep Conversion");
			Units_Units_SearchFilters[loop]->Append("Villager Mode");
			Units_Units_SearchFilters[loop]->Append("Unseletable");
			Units_Units_SearchFilters[loop]->Append("Selection Mask");
			Units_Units_SearchFilters[loop]->Append("Selection Shape Type");
			Units_Units_SearchFilters[loop]->Append("Selection Shape");
			Units_Units_SearchFilters[loop]->Append("Selection Effect");
			Units_Units_SearchFilters[loop]->Append("Editor Selection Color");
			Units_Units_SearchFilters[loop]->Append("Unitline");
			Units_Units_SearchFilters[loop]->Append("Tracking Unit Used");
			Units_Units_SearchFilters[loop]->Append("Command Attribute");
			Units_Units_SearchFilters[loop]->Append("Stack Unit");
			Units_Units_SearchFilters[loop]->Append("Terrain");
			Units_Units_SearchFilters[loop]->Append("Research");
		*/	Units_Units_SearchFilters[loop]->SetSelection(0);

			if(Research_Research_SearchFilters[loop]->GetCount() > 0)
			{
				Research_Research_SearchFilters[loop]->Clear();
			}
			Research_Research_SearchFilters[loop]->Append("Lang DLL Name");	// 0
			Research_Research_SearchFilters[loop]->Append("Internal Name");
			Research_Research_SearchFilters[loop]->Append("Required Researches");
			Research_Research_SearchFilters[loop]->Append("Min. Req. Researches");
			Research_Research_SearchFilters[loop]->Append("Research Location");
			Research_Research_SearchFilters[loop]->Append("Research Time");
			Research_Research_SearchFilters[loop]->Append("Technology");
			Research_Research_SearchFilters[loop]->Append("Type");
			Research_Research_SearchFilters[loop]->Append("Icon");
			Research_Research_SearchFilters[loop]->Append("Button");
			Research_Research_SearchFilters[loop]->Append("Lang DLL Pointer 1");
			Research_Research_SearchFilters[loop]->Append("Lang DLL Pointer 2");
			Research_Research_SearchFilters[loop]->Append("Pointer 3");
			Research_Research_SearchFilters[loop]->Append("Cost Types");
			Research_Research_SearchFilters[loop]->Append("Cost Amounts");
			Research_Research_SearchFilters[loop]->Append("Cost Uses");
			if(GameVersion >= 2)
			{
				Research_Research_SearchFilters[loop]->Append("Civilization");
				Research_Research_SearchFilters[loop]->Append("Full Tech. Mode");
				if(GameVersion >= 4)
				Research_Research_SearchFilters[loop]->Append("Internal Name 2");
			}
			Research_Research_SearchFilters[loop]->SetSelection(0);

			if(Sounds_Items_SearchFilters[loop]->GetCount() > 0)
			{
				Sounds_Items_SearchFilters[loop]->Clear();
			}
			Sounds_Items_SearchFilters[loop]->Append("Filename");	// 0
			Sounds_Items_SearchFilters[loop]->Append("DRS");
			Sounds_Items_SearchFilters[loop]->Append("Probability");
			if(GameVersion >= 2)
			{
				Sounds_Items_SearchFilters[loop]->Append("Civilization");
				Sounds_Items_SearchFilters[loop]->Append("Unknown");
			}
			Sounds_Items_SearchFilters[loop]->SetSelection(0);
		}

		Items.Add(0);
		//wxMessageBox("Loaded!");
		if(GameVersion >= 2)
		{
			ListTTAgess();
			ListTTBuildings();
			ListTTUnits();
			ListTTResearches();
		}
		else
		{
			if(TechTrees_MainList_Ages_List->GetCount() > 0)
			TechTrees_MainList_Ages_List->Clear();
			if(TechTrees_DataList_Ages_List_Buildings->GetCount() > 0)
			TechTrees_DataList_Ages_List_Buildings->Clear();
			if(TechTrees_DataList_Ages_List_Units->GetCount() > 0)
			TechTrees_DataList_Ages_List_Units->Clear();
			if(TechTrees_DataList_Ages_List_Researches->GetCount() > 0)
			TechTrees_DataList_Ages_List_Researches->Clear();
			if(TechTrees_MainList_Buildings_List->GetCount() > 0)
			TechTrees_MainList_Buildings_List->Clear();
			if(TechTrees_DataList_Buildings_List_Buildings->GetCount() > 0)
			TechTrees_DataList_Buildings_List_Buildings->Clear();
			if(TechTrees_DataList_Buildings_List_Units->GetCount() > 0)
			TechTrees_DataList_Buildings_List_Units->Clear();
			if(TechTrees_DataList_Buildings_List_Researches->GetCount() > 0)
			TechTrees_DataList_Buildings_List_Researches->Clear();
			if(TechTrees_MainList_Units_List->GetCount() > 0)
			TechTrees_MainList_Units_List->Clear();
			if(TechTrees_DataList_Units_List_Units->GetCount() > 0)
			TechTrees_DataList_Units_List_Units->Clear();
			if(TechTrees_MainList_Researches_List->GetCount() > 0)
			TechTrees_MainList_Researches_List->Clear();
			if(TechTrees_DataList_Researches_List_Buildings->GetCount() > 0)
			TechTrees_DataList_Researches_List_Buildings->Clear();
			if(TechTrees_DataList_Researches_List_Units->GetCount() > 0)
			TechTrees_DataList_Researches_List_Units->Clear();
			if(TechTrees_DataList_Researches_List_Researches->GetCount() > 0)
			TechTrees_DataList_Researches_List_Researches->Clear();
		}
		OnCivCountChange();
		ListCivs();
		ListUnits(0);
		if(GameVersion >= 4)
		{
			ListUnitLines();
		}
		else
		{
			if(UnitLines_UnitLines_List->GetCount() > 0)
			UnitLines_UnitLines_List->Clear();
			if(UnitLines_UnitLineUnits_List->GetCount() > 0)
			UnitLines_UnitLineUnits_List->Clear();
		}
		ListResearches();
		ListTechs();
		ListGraphics();
		ListSounds();
		ListTerrainNumbers();
		ListTerrainRestrictions();
		ListTerrains();
		ListPlayerColors();
		ListTerrainBorders();
		ListGeneral();

		if(Effects_ComboBox_AttributesC->GetCount() > 0)
		{
			Effects_ComboBox_AttributesC->Clear();
		}
		Effects_ComboBox_AttributesC->Append("No Attribute/Invalid Attribute");		// Selection 0
		Effects_ComboBox_AttributesC->Append("0 - Hit Points");		// Selection 1
		Effects_ComboBox_AttributesC->Append("1 - Line of Sight");
		Effects_ComboBox_AttributesC->Append("2 - Garrison Capacity");
		Effects_ComboBox_AttributesC->Append("3 - Unit Size Radius 1");
		Effects_ComboBox_AttributesC->Append("4 - Unit Size Radius 2");
		Effects_ComboBox_AttributesC->Append("5 - Movement Speed");
		Effects_ComboBox_AttributesC->Append("6 - Garrison Recovery Rate");
		Effects_ComboBox_AttributesC->Append("7 - Unknown?");
		Effects_ComboBox_AttributesC->Append("8 - Armor");
		Effects_ComboBox_AttributesC->Append("9 - Attack");
		Effects_ComboBox_AttributesC->Append("10 - Attack Reloading Time");
		Effects_ComboBox_AttributesC->Append("11 - Accuracy Percent");
		Effects_ComboBox_AttributesC->Append("12 - Range");
		Effects_ComboBox_AttributesC->Append("13 - Working Rate");
		Effects_ComboBox_AttributesC->Append("14 - Resource Carriage");
		Effects_ComboBox_AttributesC->Append("15 - Unknown?");
		Effects_ComboBox_AttributesC->Append("16 - New Projectile Unit");
		Effects_ComboBox_AttributesC->Append("17 - Upgrade Graphic");
		Effects_ComboBox_AttributesC->Append("18 - Unknown?");
		Effects_ComboBox_AttributesC->Append("19 - Projectile Intelligent Accuracy");
		Effects_ComboBox_AttributesC->Append("20 - Minimum Range");
		if(GameVersion < 2)
		Effects_ComboBox_AttributesC->Append("21 - Unknown?");
		else
		Effects_ComboBox_AttributesC->Append("21 - Population Support");
		Effects_ComboBox_AttributesC->Append("22 - Blast Radius");
		Effects_ComboBox_AttributesC->Append("23 - Search Radius");
		Effects_ComboBox_AttributesC->Append("24 - None");
		Effects_ComboBox_AttributesC->Append("25 - None");
		Effects_ComboBox_AttributesC->Append("26 - None");
		Effects_ComboBox_AttributesC->Append("27 - None");
		Effects_ComboBox_AttributesC->Append("28 - None");
		Effects_ComboBox_AttributesC->Append("29 - None");
		Effects_ComboBox_AttributesC->Append("30 - None");
		Effects_ComboBox_AttributesC->Append("31 - None");
		Effects_ComboBox_AttributesC->Append("32 - None");
		Effects_ComboBox_AttributesC->Append("33 - None");
		Effects_ComboBox_AttributesC->Append("34 - None");
		Effects_ComboBox_AttributesC->Append("35 - None");
		Effects_ComboBox_AttributesC->Append("36 - None");
		Effects_ComboBox_AttributesC->Append("37 - None");
		Effects_ComboBox_AttributesC->Append("38 - None");
		Effects_ComboBox_AttributesC->Append("39 - None");
		Effects_ComboBox_AttributesC->Append("40 - None");
		Effects_ComboBox_AttributesC->Append("41 - None");
		Effects_ComboBox_AttributesC->Append("42 - None");
		Effects_ComboBox_AttributesC->Append("43 - None");
		Effects_ComboBox_AttributesC->Append("44 - None");
		Effects_ComboBox_AttributesC->Append("45 - None");
		Effects_ComboBox_AttributesC->Append("46 - None");
		Effects_ComboBox_AttributesC->Append("47 - None");
		Effects_ComboBox_AttributesC->Append("48 - None");
		Effects_ComboBox_AttributesC->Append("49 - None");
		Effects_ComboBox_AttributesC->Append("50 - None");
		Effects_ComboBox_AttributesC->Append("51 - None");
		Effects_ComboBox_AttributesC->Append("52 - None");
		Effects_ComboBox_AttributesC->Append("53 - None");
		Effects_ComboBox_AttributesC->Append("54 - None");
		Effects_ComboBox_AttributesC->Append("55 - None");
		Effects_ComboBox_AttributesC->Append("56 - None");
		Effects_ComboBox_AttributesC->Append("57 - None");
		Effects_ComboBox_AttributesC->Append("58 - None");
		Effects_ComboBox_AttributesC->Append("59 - None");
		Effects_ComboBox_AttributesC->Append("60 - None");
		Effects_ComboBox_AttributesC->Append("61 - None");
		Effects_ComboBox_AttributesC->Append("62 - None");
		Effects_ComboBox_AttributesC->Append("63 - None");
		Effects_ComboBox_AttributesC->Append("64 - None");
		Effects_ComboBox_AttributesC->Append("65 - None");
		Effects_ComboBox_AttributesC->Append("66 - None");
		Effects_ComboBox_AttributesC->Append("67 - None");
		Effects_ComboBox_AttributesC->Append("68 - None");
		Effects_ComboBox_AttributesC->Append("69 - None");
		Effects_ComboBox_AttributesC->Append("70 - None");
		Effects_ComboBox_AttributesC->Append("71 - None");
		Effects_ComboBox_AttributesC->Append("72 - None");
		Effects_ComboBox_AttributesC->Append("73 - None");
		Effects_ComboBox_AttributesC->Append("74 - None");
		Effects_ComboBox_AttributesC->Append("75 - None");
		Effects_ComboBox_AttributesC->Append("76 - None");
		Effects_ComboBox_AttributesC->Append("77 - None");
		Effects_ComboBox_AttributesC->Append("78 - None");
		Effects_ComboBox_AttributesC->Append("79 - None");
		Effects_ComboBox_AttributesC->Append("80 - Boarding Energy Reload Speed");
		Effects_ComboBox_AttributesC->Append("81 - None");
		Effects_ComboBox_AttributesC->Append("82 - None");
		Effects_ComboBox_AttributesC->Append("83 - None");
		Effects_ComboBox_AttributesC->Append("84 - None");
		Effects_ComboBox_AttributesC->Append("85 - None");
		Effects_ComboBox_AttributesC->Append("86 - None");
		Effects_ComboBox_AttributesC->Append("87 - None");
		Effects_ComboBox_AttributesC->Append("88 - None");
		Effects_ComboBox_AttributesC->Append("89 - None");
		Effects_ComboBox_AttributesC->Append("90 - None");
		Effects_ComboBox_AttributesC->Append("91 - None");
		Effects_ComboBox_AttributesC->Append("92 - None");
		Effects_ComboBox_AttributesC->Append("93 - None");
		Effects_ComboBox_AttributesC->Append("94 - None");
		Effects_ComboBox_AttributesC->Append("95 - None");
		Effects_ComboBox_AttributesC->Append("96 - None");
		Effects_ComboBox_AttributesC->Append("97 - None");
		Effects_ComboBox_AttributesC->Append("98 - None");
		Effects_ComboBox_AttributesC->Append("99 - None");
		Effects_ComboBox_AttributesC->Append("100 - Resource Cost");
		if(GameVersion < 2)
		Effects_ComboBox_AttributesC->Append("101 - Population Support");
		else
		Effects_ComboBox_AttributesC->Append("101 - Creation Time");
		Effects_ComboBox_AttributesC->Append("102 - Number of Garrison Arrows");
		Effects_ComboBox_AttributesC->Append("103 - Food Cost");
		if(GameVersion < 4)
		{
			Effects_ComboBox_AttributesC->Append("104 - Wood Cost");
			Effects_ComboBox_AttributesC->Append("105 - Gold Cost");
			Effects_ComboBox_AttributesC->Append("106 - Stone Cost");
		}
		else
		{
			Effects_ComboBox_AttributesC->Append("104 - Carbon Cost");
			Effects_ComboBox_AttributesC->Append("105 - Nova Cost");
			Effects_ComboBox_AttributesC->Append("106 - Ore Cost");
		}
		Effects_ComboBox_AttributesC->Append("107 - Max Dup. Missiles");
		Effects_ComboBox_AttributesC->Append("108 - Healing Rate");	// Selection 109
		Effects_ComboBox_AttributesC->SetSelection(0);

		if(Units_GraphicSet->GetCount() > 0)
		{
			Units_GraphicSet->Clear();
		}
		if(GameVersion < 2)
		{
			// AoE
			Units_GraphicSet->Append("0 West Middle Easterns");
			Units_GraphicSet->Append("1 Greeks");
			Units_GraphicSet->Append("2 East Middle Easterns");
			Units_GraphicSet->Append("3 Asians");
			// + RoR
			Units_GraphicSet->Append("4 Romans");
			Units_GraphicSet->Append("5 Unused");
			Units_GraphicSet->Append("6 Unused");
			Units_GraphicSet->Append("7 Unused");
			Units_GraphicSet->Append("8 Unused");
		}
		else if(GameVersion < 4)
		{
			Units_GraphicSet->Append("0 Unused");
			// AoK
			Units_GraphicSet->Append("1 East Europeans");
			Units_GraphicSet->Append("2 West Europeans");
			Units_GraphicSet->Append("3 Asians");
			Units_GraphicSet->Append("4 Arabs");
			// + TC
			Units_GraphicSet->Append("5 Americans");
			Units_GraphicSet->Append("6 Unused");
			Units_GraphicSet->Append("7 Unused");
			Units_GraphicSet->Append("8 Unused");
		}
		else
		{
			// SWGB
			Units_GraphicSet->Append("0 Unused");
			Units_GraphicSet->Append("1 Galactic Empire");
			Units_GraphicSet->Append("2 Gungans");
			Units_GraphicSet->Append("3 Rebels");
			Units_GraphicSet->Append("4 Royal Naboo");
			Units_GraphicSet->Append("5 Trade Federation");
			Units_GraphicSet->Append("6 Wookiees");
			Units_GraphicSet->Append("7 Republic");
			Units_GraphicSet->Append("8 Confederacy");
		}
		Units_GraphicSet->Append("9 Ask me for more!");
		Units_GraphicSet->SetSelection(0);

		DataOpened = true;
		OnGameVersionChange();
	}
	SetStatusText("", 0);

	NeedDat = false;
	SkipOpenDialog = false;
}

void AGE_Frame::OnGameVersionChange()
{
	if(DataOpened)	// Hiding stuff according to game version should be here.
	{
		for(short loop = 32;loop < TERRAINBORDERSMAX; loop++)
		{
			if(GameVersion >= 3)
			{
				Terrains_TerrainBorderID[loop]->Show(true);
				Terrains_ComboBox_TerrainBorderID[loop]->Show(true);
			}
			else
			{
				Terrains_TerrainBorderID[loop]->Show(false);
				Terrains_ComboBox_TerrainBorderID[loop]->Show(false);
			}
		}
		for(short loop = 4;loop < 6; loop++)
		{
			if(GameVersion >= 2)
			{
				Research_RequiredTechs[loop]->Show(true);
				Research_ComboBox_RequiredTechs[loop]->Show(true);
			}
			else
			{
				Research_RequiredTechs[loop]->Show(false);
				Research_ComboBox_RequiredTechs[loop]->Show(false);
			}
		}

		if(GameVersion >= 2) // AoK ->
		{
			Research_Holder_Civ->Show(true);
			Research_Holder_FullTechMode->Show(true);
			Civs_Holder_TeamBonus->Show(true);
			Terrains_Holder_BlendPriority->Show(true);
			Terrains_Holder_BlendType->Show(true);
			TerRestrict_Holder_Unknown1->Show(true);
			TerRestrict_Holder_Graphics->Show(true);
			TerRestrict_Holder_Amount->Show(true);
			SoundItems_Holder_Civ->Show(true);
			Colors_Holder_Name->Show(false);
			Colors_Holder_Palette->Show(true);
			Colors_Holder_MinimapColor->Show(true);
			Units_StandingGraphic[1]->Show(true);
			Units_ComboBox_StandingGraphic[1]->Show(true);
			Units_TrainSound[1]->Show(true);
			Units_ComboBox_TrainSound[1]->Show(true);
			Units_Holder_Unknown3a->Show(false);
			Units_Holder_SelectionShapeType->Show(true);
			Units_Holder_ID3->Show(true);
			Units_Holder_GarrisonRecoveryRate->Show(true);
			Units_Holder_MissileGraphicDelay->Show(true);
			Units_Holder_HeroMode->Show(true);
			Units_Holder_GarrisonGraphic->Show(true);
			Units_Holder_AttackMissileDuplicationAmount1->Show(true);
			Units_Holder_AttackMissileDuplicationAmount2->Show(true);
			Units_Holder_AttackMissileDuplicationSpawning->Show(true);
			Units_Holder_AttackMissileDuplicationUnit->Show(true);
			Units_Holder_AttackMissileDuplicationGraphic->Show(true);
			Units_Holder_AnnexUnit1->Show(true);
			Units_Holder_AnnexUnitMisplacement1->Show(true);
			Units_Holder_HeadUnit->Show(true);
			Units_Holder_HeadUnit->Show(true);
			Units_Holder_TransformUnit->Show(true);
			Units_Holder_GarrisonType->Show(true);
			Units_Holder_GarrisonHealRate->Show(true);
			TechTrees_Main->Show(true);
			if(ShowUnknowns)
			{
				SoundItems_Holder_Unknown->Show(true);
				Colors_Holder_UnknownArea->Show(true);
				Units_Holder_Unknown7->Show(true);
				Units_Holder_Unknown8->Show(true);
				Units_Holder_Unknown12->Show(true);
				for(short loop=1; loop < 17; loop++)
				Units_Unknown16[loop]->Show(true);
				Units_Holder_Unknown26->Show(true);
				Units_Holder_Unknown27->Show(true);
				Units_Holder_Unknown28->Show(true);
				Units_Holder_Unknown29->Show(true);
				Units_Holder_Unknown33->Show(true);
				Units_Holder_Unknown34->Show(true);
				Units_Holder_Unknown35->Show(true);
				Units_Holder_Unknown36->Show(true);
				Units_Holder_Unknown37->Show(true);
				for(short loop=1; loop < General_AfterBorders.size(); loop++)
				General_AfterBorders[loop]->Show(true);
				General_Holder_Variables2->Show(true);
			}
		}
		else // <- RoR
		{
			Research_Holder_Civ->Show(false);
			Research_Holder_FullTechMode->Show(false);
			Civs_Holder_TeamBonus->Show(false);
			Terrains_Holder_BlendPriority->Show(false);
			Terrains_Holder_BlendType->Show(false);
			TerRestrict_Holder_Unknown1->Show(false);
			TerRestrict_Holder_Graphics->Show(false);
			TerRestrict_Holder_Amount->Show(false);
			SoundItems_Holder_Civ->Show(false);
			SoundItems_Holder_Unknown->Show(false);
			Colors_Holder_Name->Show(true);
			Colors_Holder_Palette->Show(false);
			Colors_Holder_MinimapColor->Show(false);
			Colors_Holder_UnknownArea->Show(false);
			Units_StandingGraphic[1]->Show(false);
			Units_ComboBox_StandingGraphic[1]->Show(false);
			Units_TrainSound[1]->Show(false);
			Units_ComboBox_TrainSound[1]->Show(false);
			Units_Holder_Unknown7->Show(false);
			Units_Holder_Unknown8->Show(false);
			Units_Holder_SelectionShapeType->Show(false);
			Units_Holder_ID3->Show(false);
			Units_Holder_Unknown12->Show(false);
			for(short loop=1; loop < 17; loop++)
			Units_Unknown16[loop]->Show(false);
			Units_Holder_GarrisonRecoveryRate->Show(false);
			Units_Holder_Unknown26->Show(false);
			Units_Holder_Unknown27->Show(false);
			Units_Holder_Unknown28->Show(false);
			Units_Holder_MissileGraphicDelay->Show(false);
			Units_Holder_HeroMode->Show(false);
			Units_Holder_GarrisonGraphic->Show(false);
			Units_Holder_AttackMissileDuplicationAmount1->Show(false);
			Units_Holder_AttackMissileDuplicationAmount2->Show(false);
			Units_Holder_AttackMissileDuplicationSpawning->Show(false);
			Units_Holder_AttackMissileDuplicationUnit->Show(false);
			Units_Holder_AttackMissileDuplicationGraphic->Show(false);
			Units_Holder_Unknown29->Show(false);
			Units_Holder_Unknown33->Show(false);
			Units_Holder_AnnexUnit1->Show(false);
			Units_Holder_AnnexUnitMisplacement1->Show(false);
			Units_Holder_HeadUnit->Show(false);
			Units_Holder_TransformUnit->Show(false);
			Units_Holder_Unknown34->Show(false);
			Units_Holder_GarrisonType->Show(false);
			Units_Holder_GarrisonHealRate->Show(false);
			Units_Holder_Unknown35->Show(false);
			Units_Holder_Unknown36->Show(false);
			Units_Holder_Unknown37->Show(false);
			TechTrees_Main->Show(false);
			for(short loop=1; loop < General_AfterBorders.size(); loop++)
			General_AfterBorders[loop]->Show(false);
			General_Holder_Variables2->Show(false);
			if(ShowUnknowns) Units_Holder_Unknown3a->Show(true);
		}
		if(GameVersion >= 4) // SWGB ->
		{
			Graphics_Name->SetMaxSize(25);
			Graphics_Name2->SetMaxSize(25);
			SoundItems_Name->SetMaxSize(27);
			Terrains_Name->SetMaxSize(17);
			Terrains_Name2->SetMaxSize(17);

			Research_Holder_Name[1]->Show(true);
			Civs_Holder_Name[1]->Show(true);
			Units_Holder_Name2->Show(true);
			Units_Holder_Unitline->Show(true);
			Units_Holder_MinTechLevel->Show(true);
			UnitLines_Main->Show(true);
			if(ShowUnknowns)
			{
				Civs_Holder_SUnknown1->Show(true);
				Terrains_Holder_SUnknown1->Show(true);
				General_Holder_Variables1->Show(true);
				TechTrees_Ages_Grid_Zeroes2->Show(true);
				TechTrees_Buildings_Grid_Unknown2a2->Show(true);
				TechTrees_Buildings_Grid_Unknown2b2->Show(true);
				TechTrees_Units_Grid_Unknown2a2->Show(true);
				TechTrees_Units_Grid_Unknown2b2->Show(true);
				TechTrees_Researches_Grid_Unknown2a2->Show(true);
				TechTrees_Researches_Grid_Unknown2b2->Show(true);
			}
		}
		else // <- TC
		{
			Graphics_Name->SetMaxSize(21);
			Graphics_Name2->SetMaxSize(13);
			SoundItems_Name->SetMaxSize(13);
			Terrains_Name->SetMaxSize(13);
			Terrains_Name2->SetMaxSize(13);

			Research_Holder_Name[1]->Show(false);
			Civs_Holder_Name[1]->Show(false);
			Civs_Holder_SUnknown1->Show(false);
			Terrains_Holder_SUnknown1->Show(false);
			Units_Holder_Name2->Show(false);
			Units_Holder_Unitline->Show(false);
			Units_Holder_MinTechLevel->Show(false);
			General_Holder_Variables1->Show(false);
			UnitLines_Main->Show(false);
			TechTrees_Ages_Grid_Zeroes2->Show(false);
			TechTrees_Buildings_Grid_Unknown2a2->Show(false);
			TechTrees_Buildings_Grid_Unknown2b2->Show(false);
			TechTrees_Units_Grid_Unknown2a2->Show(false);
			TechTrees_Units_Grid_Unknown2b2->Show(false);
			TechTrees_Researches_Grid_Unknown2a2->Show(false);
			TechTrees_Researches_Grid_Unknown2b2->Show(false);
		}
		if(GameVersion >= 3) // TC ->
		{
			Units_Holder_SnowGraphicID->Show(true);
			Units_Holder_Attribute->Show(true);
			Units_Holder_Civ->Show(true);
			if(ShowUnknowns)
			{
				Units_Holder_Unknown9->Show(true);
				Units_Unknown20[1]->Show(true);
			}
		}
		else // <- AoK
		{
			Units_Holder_Attribute->Show(false);
			Units_Holder_Civ->Show(false);
			Units_Holder_Unknown9->Show(false);
			Units_Unknown20[1]->Show(false);
			Units_Holder_SnowGraphicID->Show(false);
		}
	}

//	Every data area should be layouted.
	Civs_Main->Layout();
	Units_Main->Layout();
	Research_Main->Layout();
	Techs_Main->Layout();
	TechTrees_Main->Layout();
	Graphics_Main->Layout();
	Terrains_Main->Layout();
	TerRestrict_Main->Layout();
	Sounds_Main->Layout();
	Colors_Main->Layout();
	UnitLines_Main->Layout();
	Borders_Main->Layout();
	General_Main->Layout();
	Units_Scroller->GetSizer()->FitInside(Units_Scroller);
	Research_Scroller->GetSizer()->FitInside(Research_Scroller);
	TechTrees_Scroller->GetSizer()->FitInside(TechTrees_Scroller);
	Graphics_Scroller->GetSizer()->FitInside(Graphics_Scroller);
	Terrains_Scroller->GetSizer()->FitInside(Terrains_Scroller);
	General_Scroller->GetSizer()->FitInside(General_Scroller);
	Refresh(); // Does this refresh non-visible tabs?
}

void AGE_Frame::OnSave(wxCommandEvent &Event)
{
	wxCommandEvent Selected;

	AGE_SaveDialog SaveBox(this);
	SaveBox.Path_DatFileLocation->SetFocus();

	SaveBox.DriveLetterBox->ChangeValue(DriveLetter);
	SaveBox.CheckBox_GenieVer->SetSelection(SaveGameVersion);

	SaveBox.CheckBox_DatFileLocation->SetValue(SaveDat);
	Selected.SetEventType(wxEVT_COMMAND_CHECKBOX_CLICKED);
	Selected.SetId(SaveBox.CheckBox_DatFileLocation->GetId());
	Selected.SetInt(SaveDat);
	SaveBox.GetEventHandler()->ProcessEvent(Selected);

	SaveBox.Path_DatFileLocation->SetPath(SaveDatFileName);

	SaveBox.CheckBox_ApfFileLocation->SetValue(SaveApf);
	Selected.SetEventType(wxEVT_COMMAND_CHECKBOX_CLICKED);
	Selected.SetId(SaveBox.CheckBox_ApfFileLocation->GetId());
	Selected.SetInt(SaveApf);
	SaveBox.GetEventHandler()->ProcessEvent(Selected);

	SaveBox.Path_ApfFileLocation->SetPath(SaveApfFileName);

	SaveBox.CheckBox_LangFileLocation->SetValue(LangsUsed & 1);
	Selected.SetEventType(wxEVT_COMMAND_CHECKBOX_CLICKED);
	Selected.SetId(SaveBox.CheckBox_LangFileLocation->GetId());
	Selected.SetInt(LangsUsed & 1);
	SaveBox.GetEventHandler()->ProcessEvent(Selected);

	SaveBox.CheckBox_LangX1FileLocation->SetValue(LangsUsed & 2);
	Selected.SetEventType(wxEVT_COMMAND_CHECKBOX_CLICKED);
	Selected.SetId(SaveBox.CheckBox_LangX1FileLocation->GetId());
	Selected.SetInt(LangsUsed & 2);
	SaveBox.GetEventHandler()->ProcessEvent(Selected);

	SaveBox.CheckBox_LangX1P1FileLocation->SetValue(LangsUsed & 4);
	Selected.SetEventType(wxEVT_COMMAND_CHECKBOX_CLICKED);
	Selected.SetId(SaveBox.CheckBox_LangX1P1FileLocation->GetId());
	Selected.SetInt(LangsUsed & 4);
	SaveBox.GetEventHandler()->ProcessEvent(Selected);

	SaveBox.Path_LangFileLocation->SetPath(SaveLangFileName);
	SaveBox.Path_LangX1FileLocation->SetPath(SaveLangX1FileName);
	SaveBox.Path_LangX1P1FileLocation->SetPath(SaveLangX1P1FileName);

	if(SaveBox.ShowModal() != wxID_OK) return;

	SaveGameVersion = SaveBox.CheckBox_GenieVer->GetSelection();
	SaveDat = SaveBox.CheckBox_DatFileLocation->IsChecked();
	SaveApf = SaveBox.CheckBox_ApfFileLocation->IsChecked();
	SaveLangs = SaveBox.CheckBox_LangWrite->IsChecked();

	SaveDatFileName = SaveBox.Path_DatFileLocation->GetPath();
	SaveApfFileName = SaveBox.Path_ApfFileLocation->GetPath();
	SaveLangFileName = SaveBox.Path_LangFileLocation->GetPath();
	SaveLangX1FileName = SaveBox.Path_LangX1FileLocation->GetPath();
	SaveLangX1P1FileName = SaveBox.Path_LangX1P1FileLocation->GetPath();

	if(SaveDat)
	{
		SetStatusText("Saving dat file...", 0);
		wxBusyCursor WaitCursor;

		try
		{
			GenieFile->saveAs(SaveDatFileName.c_str());
		}
		catch(std::ios_base::failure e)
		{
			wxMessageBox("Unable to save the file!");
			return;
		}
	}
	if(SaveApf)
	{
		//	 Not Implemented Yet = Nothing Happens
	}

	if(SaveLangs)
	{
		SetStatusText("Saving language files...", 0);
		wxBusyCursor WaitCursor;
		if(LangsUsed & 1)
		{
			try
			{
				Lang->saveAs(SaveLangFileName.c_str());
			}
			catch(std::ios_base::failure e)
			{
				wxMessageBox("Unable to save language file!");
				return;
			}
		}
		if(LangsUsed & 2)
		{
			try
			{
				LangX->saveAs(SaveLangX1FileName.c_str());
			}
			catch(std::ios_base::failure e)
			{
				wxMessageBox("Unable to save language expansion file!");
				return;
			}
		}
		if(LangsUsed & 4)
		{
			try
			{
				LangXP->saveAs(SaveLangX1P1FileName.c_str());
			}
			catch(std::ios_base::failure e)
			{
				wxMessageBox("Unable to save language expansion patch file!");
				return;
			}
		}
	}

	SetStatusText("", 0);
}

void AGE_Frame::OnMenuOption(wxCommandEvent &Event)
{
	switch(Event.GetId())
	{
		case MenuOption_Prompt:
		{
			PromptForFilesOnOpen = Event.IsChecked();
		}
		break;
		case ToolBar_Show:
		{
			ShowUnknowns = Event.IsChecked();

			Units_Holder_UnknownArea->Show(ShowUnknowns);
			Units_CommandHolder_Data2->Show(ShowUnknowns);
			TechTrees_Ages_Holder_Zeroes->Show(ShowUnknowns);
			TechTrees_Buildings_Holder_Unknown2a->Show(ShowUnknowns);
			TechTrees_Buildings_Holder_Unknown2b->Show(ShowUnknowns);
			TechTrees_Units_Holder_Unknown2a->Show(ShowUnknowns);
			TechTrees_Units_Holder_Unknown2b->Show(ShowUnknowns);
			TechTrees_Researches_Holder_Unknown2a->Show(ShowUnknowns);
			TechTrees_Researches_Holder_Unknown2b->Show(ShowUnknowns);
			Graphics_Holder_Unknowns->Show(ShowUnknowns);
			Graphics_Grid_Deltas_Data2->Show(ShowUnknowns);
			Terrains_Holder_UnknownArea->Show(ShowUnknowns);
			Borders_Unknonws->Show(ShowUnknowns);
			Sounds_Holder_Unknown->Show(ShowUnknowns);
			SoundItems_Holder_Unknown->Show(ShowUnknowns);
			Colors_Holder_UnknownArea->Show(ShowUnknowns);
			General_TopRow->Show(ShowUnknowns);
			General_ScrollerWindowsSpace->Show(ShowUnknowns);

			OnGameVersionChange(); // Does layouting and refreshing and ... check it out.
		}
		break;
		case MenuOption_Buttons:
		{
			ShowButtons = Event.IsChecked();

			Terrains_Add->Enable(ShowButtons);
			Terrains_Insert->Enable(ShowButtons);
			Terrains_Delete->Enable(ShowButtons);
			Terrains_PasteInsert->Enable(ShowButtons);
		}
		break;
		case wxID_EXIT:
		{
			Close(true);
		}
		break;
		case MenuOption_Tips:
		{
			wxString TipText = "Tips\n\n";
			TipText.Append("You can have multiple search entries separated with \"|\" letter.\n");
			TipText.Append("Upper search boxes are inclusive and lower ones exclusive.\n");
			TipText.Append("Example: \"tower|ship|ram\"\n");
			TipText.Append("You can switch from or finding to and finding with check boxes.\n");
			TipText.Append("Meaning that every search entry has to match.\n");
			wxMessageBox(TipText);
		}
		break;
		case MenuOption_About:
		{
			AGE_AboutDialog AGEAbout(this);
			AGEAbout.ShowModal();
		}
		break;
		case MenuOption_IDFix:
		{
			EnableIDFix = Event.IsChecked();
			wxMessageBox("Please restart this program.\nI do not recommend disabling index fixes!");
		}
		break;
		case ToolBar_CustomNames:
		{
			Customs = new wxFileConfig(wxEmptyString, "Tapsa", "age2lists.ini", wxEmptyString, wxCONFIG_USE_LOCAL_FILE | wxCONFIG_USE_RELATIVE_PATH);
			long ExtraCount = Attacks_ComboBox_Class[0]->GetCount()-32;
			Customs->Write("Count/ExtraCount", ExtraCount);
			wxString MoveHolder = "";
			for(short loop=0; loop < ExtraCount; loop++)
			{
				MoveHolder = Attacks_ComboBox_Class[0]->GetString(loop+32);
				Customs->Write("Names/"+lexical_cast<string>(loop+31), MoveHolder);
			}
			delete Customs;
			//wxMessageBox("File extracted successfully!");
		}
		break;
		case ToolBar_Help:
		{
			wxString help = "ATTENTION!\nChanges to editing boxes affect all selected items!\n\n";
			help.Append("Short Guide to Advanced Editing\n");
			help.Append("Click \"Help\" from the toolbar to see this again.\n\n");
			help.Append("Here are examples which cover all the nice features.\n");
			help.Append("(This is assuming that you edit Age of Empires II: The Conquerors)\n\n");
			help.Append("Let's suppose that you want to edit buildings (type 80) of class 3,\n");
			help.Append("but not camps nor houses or yurts.\n");
			help.Append("See the two combo boxes having \"Lang DLL Name\" selected by default?\n");
			help.Append("Select \"Type\" to the upper one and \"Class\" to the lower one.\n");
			help.Append("Then put \"t 80|c 3,\" to the upper search box\n");
			help.Append("and \"camp|house|yurt\" to the lower (exclusive) search box.\n");
			help.Append("Select the upper \"And\" check box to make sure that the search returns\n");
			help.Append("only units to which ALL search terms in the upper search box match.\n\n");
			help.Append("Let's suppose that you want to multiply the garrison heal rate\n");
			help.Append("of all selected units (buildings) by 1.3.\n");
			help.Append("Simply select all the desired units and put into garrison heal rate box \"b*1.3\", ");
			help.Append("or just put the absolute value you want for all selected units.\n");
			help.Append("The other batch modifiers are b+, b- and b/.\n");
			help.Append("Note that overflows are not checked!\n\n");
			help.Append("Let's suppose that you want to copy all ships to another dat file.\n");
			help.Append("On the open dialog, you must put \"2\" into \"Windows to open\"!\n");
			help.Append("Now that you have a second window titled \"AGE window 2\" open with a dat file, ");
			help.Append("let's begin the copying.\n");
			help.Append("(You can copy to another game version if you dare.)\n");
			help.Append("On the first window, select all the ships (search for \"ship\").\n");
			help.Append("Then click copy and go to the second window.\n");
			help.Append("Then click paste or paste insert wherever you want the ships.\n");
			help.Append("That's it.");
			wxMessageBox(help);
		}
		break;
	}
}

/* Check if File Exists

bool AGE_Frame::FileExists(const char * value)
{
	FILE * handle;
	handle = fopen(value, "rb");
	if(handle)
	{
		fclose(handle);
	 	return true;
	}
	return false;
}*/

string AGE_Frame::LangDLLstring(int ID, int Letters)
{
	if(ID < 0) return "";
	string Result = "";
	if(WriteLangs)
	{
		if(LangsUsed & 4 && !(Result = LangXP->getString(ID)).empty()){}
		else if(LangsUsed & 2 && !(Result = LangX->getString(ID)).empty()){}
		else if(LangsUsed & 1 && !(Result = Lang->getString(ID)).empty()){}
	}
	else
	{
		char Buffer[Letters];
		if(LangsUsed & 4 && LoadStringA(LanguageDLL[2], ID, Buffer, Letters) && strlen(Buffer) > 0) Result = Buffer;
		else if(LangsUsed & 2 && LoadStringA(LanguageDLL[1], ID, Buffer, Letters) && strlen(Buffer) > 0) Result = Buffer;
		else if(LangsUsed & 1 && LoadStringA(LanguageDLL[0], ID, Buffer, Letters) && strlen(Buffer) > 0) Result = Buffer;
	}
	return Result;
}

void AGE_Frame::OnKillFocus_LangDLL(wxFocusEvent &Event)
{
	TextCtrl_DLL *control = (TextCtrl_DLL*)Event.GetEventObject();
	if(!control->IsModified() || !WriteLangs || control->index < 0) return;
	int ID = control->index;
	string Name = string(control->GetValue());
	//wxMessageBox(lexical_cast<string>(Name.size())+"\n"+lexical_cast<string>(LangDLLstring(ID).size()));
	if(LangWriteToLatest)
	{
		try
		{
			if(LangsUsed & 4) LangXP->setString(ID, Name);
			else if(LangsUsed & 2) LangX->setString(ID, Name);
			else if(LangsUsed & 1) Lang->setString(ID, Name);
		}
		catch(...)
		{
			wxMessageBox("An error occured while trying to save the string!");
		}
	}
	else
	{
		try
		{
			if(LangsUsed & 4 && !LangXP->getString(ID).empty()) LangXP->setString(ID, "");
			if(LangsUsed & 2 && !LangX->getString(ID).empty()) LangX->setString(ID, "");
			if(LangsUsed & 1) Lang->setString(ID, Name);
		}
		catch(...)
		{
			wxMessageBox("An error occured while trying to save the string!");
		}
	}
	SetStatusText("Wrote \""+Name+"\" to "+lexical_cast<string>(ID), 0);
	control->DiscardEdits();
}

bool AGE_Frame::SearchMatches(wxString itemText)
{
	// Make this so that no strings are altered! Easy ways?
	bool matches = false;

	// If there is no search text, list normally
	// If search text has a match
	if(searchText.empty() || (itemText.find(searchText) != string::npos))
	{
		matches = true;
	}
	else
	{
		size_t found = searchText.find("|");
		if(found != string::npos)
		{
			size_t pos = 0;
			if(UseAnd[0]) // All search parts must match
			{
				matches = true;
				while(1)
				{
					if(itemText.find(searchText.substr(pos, found-pos)) == string::npos)
					{
						matches = false;
						break;
					}
					if(found == string::npos) break;
					pos = found+1;
					found = searchText.find("|", pos);
				}
			}
			else // Only one match needed
			{
				while(1)
				{
					if(itemText.find(searchText.substr(pos, found-pos)) != string::npos)
					{
						matches = true;
						break;
					}
					if(found == string::npos) break;
					pos = found+1;
					found = searchText.find("|", pos);
				}
			}
		}
	}

	// We don't need to check for excluding if it's not going to be listed.
	// If there is no exclude text, list normally.
	// If exclude text has a match.
	if(matches && !excludeText.empty())
	if(itemText.find(excludeText) != string::npos)
	{
		matches = false;
	}
	else
	{
		size_t found = excludeText.find("|");
		if(found != string::npos)
		{
			size_t pos = 0;
			if(UseAnd[1]) // All search parts must match
			{
				matches = false;
				while(1)
				{
					if(itemText.find(excludeText.substr(pos, found-pos)) == string::npos)
					{
						matches = true;
						break;
					}
					if(found == string::npos) break;
					pos = found+1;
					found = excludeText.find("|", pos);
				}
			}
			else // Only one match needed
			{
				while(1)
				{
					if(itemText.find(excludeText.substr(pos, found-pos)) != string::npos)
					{
						matches = false;
						break;
					}
					if(found == string::npos) break;
					pos = found+1;
					found = excludeText.find("|", pos);
				}
			}
		}
	}

	return matches;
}

//	Following kill focuses are used to update lists in user interface

void AGE_Frame::OnSelection_SearchFilters(wxCommandEvent &Event)
{
	for(short loop=0; loop < 2; loop++) // Custom search filters
	{
		if(Event.GetId() == Units_Units_SearchFilters[loop]->GetId())
		{
			ListUnits(UnitCivID, false);
			Units_Units_Search->SetFocus();
		}
		else if(Event.GetId() == Graphics_Graphics_SearchFilters[loop]->GetId())
		{
			ListGraphics(false);
			Graphics_Graphics_Search->SetFocus();
		}
		else if(Event.GetId() == TechTrees_MainList_Units_SearchFilters[loop]->GetId())
		{
			ListTTUnits();
			TechTrees_MainList_Units_Search->SetFocus();
		}
		else if(Event.GetId() == TechTrees_MainList_Buildings_SearchFilters[loop]->GetId())
		{
			ListTTBuildings();
			TechTrees_MainList_Buildings_Search->SetFocus();
		}
		else if(Event.GetId() == TechTrees_MainList_Researches_SearchFilters[loop]->GetId())
		{
			ListTTResearches();
			TechTrees_MainList_Researches_Search->SetFocus();
		}
		else if(Event.GetId() == Research_Research_SearchFilters[loop]->GetId())
		{
			ListResearches(false);
			Research_Research_Search->SetFocus();
		}
		else if(Event.GetId() == Sounds_Items_SearchFilters[loop]->GetId())
		{
			ListSoundItems();
			Sounds_Items_Search->SetFocus();
		}
	}
}

void AGE_Frame::ListingFix(int Selections, wxListBox* &List)
{
	if(Selections == 0)
	{
		List->SetSelection(0);
		return;
	}
	if(Added || Items.Item(0) >= List->GetCount())
	{
		List->SetFirstItem(List->GetCount() - 1);
		List->SetSelection(List->GetCount() - 1);
		Added = false;
		return;
	}
	List->SetFirstItem(Items.Item(0) - 3);
	List->SetSelection(Items.Item(0));
}

void AGE_Frame::SearchAllSubVectors(wxListBox* &List, wxTextCtrl* &TopSearch, wxTextCtrl* &SubSearch)
{
	auto Selections = List->GetSelections(Items);
	if(Selections < 1) return;

	wxString TopText, SubText, Line;
	size_t found;
	for(int loop=0; loop < Selections; loop++)
	{
		Line = List->GetString(Items.Item(loop));
		found = Line.find(" ", 3);
		if(loop == 0)
		{
			TopText = " "+Line.substr(2, found-1); // Cutting the tech number. (for example)
			SubText = " "+Line.substr(found+2, Line.find(" ", found+3)-found-1); // Cutting the effect number.
		}
		else
		{
			TopText += "| "+Line.substr(2, found-1); // Cutting the sound number.
			SubText += "| "+Line.substr(found+2, Line.find(" ", found+3)-found-1); // Cutting the filename.
		}
		TopSearch->SetValue(TopText);
		SubSearch->SetValue(SubText);
	}
}

void AGE_Frame::OnExit(wxCloseEvent &Event)
{
	Config = new wxFileConfig(wxEmptyString, "Tapsa", "age2configw"+lexical_cast<string>(AGEwindow)+".ini", wxEmptyString, wxCONFIG_USE_LOCAL_FILE | wxCONFIG_USE_RELATIVE_PATH);
	Config->Write("Interaction/PromptForFilesOnOpen", PromptForFilesOnOpen);
	Config->Write("Interaction/AutoCopy", AutoCopy);
	Config->Write("Interaction/CopyGraphics", CopyGraphics);
	Config->Write("Interaction/AllCivs", Units_SpecialCopy_Civs->GetValue());
	Config->Write("Interaction/EnableIDFix", EnableIDFix);
	Config->Write("Interaction/ShowHelpOnStart", ShowHelpOnStart);
	Config->Write("Interface/ShowUnknowns", ShowUnknowns);
	Config->Write("Interface/ShowButtons", ShowButtons);
	if(AGEwindow == 1) Config->Write("DefaultFiles/SimultaneousFiles", SimultaneousFiles);
	Config->Write("DefaultFiles/DriveLetter", DriveLetter);
	Config->Write("DefaultFiles/Version", GameVersion);
	Config->Write("DefaultFiles/SaveVersion", SaveGameVersion);
	Config->Write("DefaultFiles/DatUsed", DatUsed);
	Config->Write("DefaultFiles/DatFilename", DatFileName);
	Config->Write("DefaultFiles/ApfFilename", ApfFileName);
	Config->Write("DefaultFiles/SaveDatFilename", SaveDatFileName);
	Config->Write("DefaultFiles/SaveApfFilename", SaveApfFileName);
	Config->Write("DefaultFiles/LangsUsed", LangsUsed);
	Config->Write("DefaultFiles/WriteLangs", WriteLangs);
	Config->Write("DefaultFiles/SaveLangs", SaveLangs);
	Config->Write("DefaultFiles/LangWriteToLatest", LangWriteToLatest);
	Config->Write("DefaultFiles/LangFilename", LangFileName);
	Config->Write("DefaultFiles/LangX1Filename", LangX1FileName);
	Config->Write("DefaultFiles/LangX1P1Filename", LangX1P1FileName);
	Config->Write("DefaultFiles/SaveLangFilename", SaveLangFileName);
	Config->Write("DefaultFiles/SaveLangX1Filename", SaveLangX1FileName);
	Config->Write("DefaultFiles/SaveLangX1P1Filename", SaveLangX1P1FileName);
	Config->Write("DefaultFiles/SaveDat", SaveDat);
	Config->Write("DefaultFiles/SaveApf", SaveApf);
	delete Config;

	//TabBar_Main->Show(false);
	//TabBar_Main->Destroy();
	Destroy();

	delete GenieFile;
	if(WriteLangs)
	{
		delete Lang;
		delete LangX;
		delete LangXP;
	}
}