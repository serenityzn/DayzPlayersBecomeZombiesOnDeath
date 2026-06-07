class CfgPatches
{
	class PlayersBecameZombiesOnDeath
	{
		units[] = {};
		weapons[] = {};
		requiredAddons[] =
		{
			"DZ_Characters",
			"DZ_Characters_Backpacks",
			"DZ_Characters_Pants",
			"DZ_Characters_Vests",
			"DZ_Characters_Tops",
			"DZ_Characters_Masks",
			"DZ_Characters_Headgear",
			"DZ_Characters_Zombies",
			"DZ_Gear_Camping",
			"DZ_Gear_Crafting",
			"DZ_Gear_Tools",
			"DZ_Gear_Containers",
			"DZ_Weapons_Melee",
			"DZ_Weapons_Firearms",
			"DZ_Pistols",
			"DZ_Data"
		};
	};
};

class CfgMods
{
	class PlayersBecameZombiesOnDeath
	{
		type = "mod";
		author = "Serenity.Zn";
		dir    = "PlayersBecameZombiesOnDeath";

		class defs
		{
			class worldScriptModule
			{
				value = "";
				files[] = { "PlayersBecameZombiesOnDeath/4_World" };
			};
			class missionScriptModule
			{
				value = "";
				files[] = { "PlayersBecameZombiesOnDeath/5_Mission" };
			};
		};
	};
};

class CfgVehicles
{
	class DZ_LightAI;
	class DayZInfected;
	class ZombieBase;
	class ZombieFemaleBase;
	class ZombieMaleBase;
	class LowTierMaleZombieBase;

	class ZmbF_DoctorSkinny_Base;
	class ZmbF_CitizenANormal_Base;
	class ZmbF_CitizenANormal_Blue;
	class ZmbM_CitizenASkinny_LT_Base;
	class ZmbM_CitizenASkinny_Brown;

	class PBZ_Zombie_Female: ZmbF_CitizenANormal_Blue
	{
		scope=2;
		storageCategory=9;
		attachments[]=
		{
			"Body",
			"Legs",
			"Feet",
			"Hands",
			"Vest",
			"Back",
			"Headgear",
			"Eyewear",
			"Mask"
		};
		class Cargo
		{
			itemsCargoSize[]={10,20};
			allowOwnedCargoManipulation=1;
			openable=0;
		};
		class InventoryEquipment
		{
			playerSlots[]=
			{
				"Slot_Body",
				"Slot_Legs",
				"Slot_Feet",
				"Slot_Hands",
				"Slot_Vest",
				"Slot_Back",
				"Slot_Headgear",
				"Slot_Eyewear",
				"Slot_Mask",
				"Slot_Armband"
			};
		};
	};

	class PBZ_Zombie_Male: ZmbM_CitizenASkinny_Brown
	{
		scope=2;
		storageCategory=9;
		attachments[]=
		{
			"Body",
			"Legs",
			"Feet",
			"Hands",
			"Vest",
			"Back",
			"Headgear",
			"Eyewear",
			"Mask"
		};
		class Cargo
		{
			itemsCargoSize[]={10,20};
			allowOwnedCargoManipulation=1;
			openable=0;
		};
		class InventoryEquipment
		{
			playerSlots[]=
			{
				"Slot_Body",
				"Slot_Legs",
				"Slot_Feet",
				"Slot_Hands",
				"Slot_Vest",
				"Slot_Back",
				"Slot_Headgear",
				"Slot_Eyewear",
				"Slot_Mask",
				"Slot_Armband"
			};
		};
	};
};
