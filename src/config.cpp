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
		};
	};

};

class CfgAIBehaviours
{
	class Infected
	{
		class BehaviourHLZombie
		{
			class MovementWalk;
			class MovementRun;
			class MovementSprint;
			class SlotCalm
			{
				class BehaviourZombieCalm;
			};
			class SlotAlerted
			{
				class BehaviourZombieAlerted;
			};
			class AlertSystem
			{
				class Calm;
				class Alerted;
			};
		};
		class TargetSystemDZBase;
		class NoiseSystemParams;
	};
	class Custom_Zombie: Infected
	{
		name = "Custom_Zombie";
		class BehaviourHLZombie: BehaviourHLZombie
		{
			class MovementWalk: MovementWalk
			{
				maxSpeed = 100.0;
				minSpeed = 100.0;
			};
			class MovementRun: MovementRun
			{
				maxSpeed = 150.0;
				minSpeed = 150.0;
			};
			class MovementSprint: MovementSprint
			{
				maxSpeed = 200.0;
				minSpeed = 190.0;
			};
			class SlotCalm: SlotCalm
			{
				class BehaviourZombieCalm: BehaviourZombieCalm
				{
					StandingDurationMin = 0;
					StandingDurationMax = 0;
				};
			};
			class AlertSystem: AlertSystem
			{
				visionToAlertMultiplier = 40;
				noiseToAlertMultiplier = 3.0;
				damageToAlertMultiplier = 10000;
				noiseShotToAlertMultiplier = 5.0;
			};
		};
		class TargetSystemDZBase: TargetSystemDZBase
		{
			visionNightMinMult = 1;
			visionNightMaxMult = 1;
			visionRainMinMult = 1;
			visionRainMaxMult = 1;
			visionFogMinMult = 1;
			visionFogMaxMult = 1;
		};
	};
};

class CfgVehicles
{
	class Clothing;
	class hikingjacket_colorbase;
	class LTS_Admin_JAcket: hikingjacket_colorbase
	{
		displayName="Admin Jacket";
		descriptionShort="Admin JAcket";
		scope=2;
		itemsCargoSize[]={10,15};
		inventorySlot[]={
			"Body"
		};
		attachments[]=
		{
			"WalkieTalkie"
		};
		itemSize[]={3,3};
		hiddenSelectionsTextures[]=
		{
			"\dz\characters\tops\data\PCU5Jacket_ground_black_co.paa",
			"\dz\characters\tops\data\PCU5Jacket_black_co.paa",
			"\dz\characters\tops\data\PCU5Jacket_black_co.paa"
		};
		
	};
	class DZ_LightAI;
	class DayZInfected;
	class ZombieBase;
	class ZombieFemaleBase;
	class ZmbF_DoctorSkinny_Base;
	class ZmbF_Linda: ZmbF_DoctorSkinny_Base
	{
		htMin = 126000;
		htMax = 128800;
		afMax = 12400;
		mfMax = 12333;
		mFact = 12333;
		tBody = 12573;
		aiAgentTemplate = "Custom_Zombie";

		scope=2;
		hiddenSelectionsMaterials[]=
		{
			"PlayersBecameZombiesOnDeath\data\linda_zombie.rvmat"
		};
		hiddenSelectionsTextures[]=
		{
			"PlayersBecameZombiesOnDeath\data\linda_zombie_co.paa"
		};
		storageCategory=9;

		hiddenSelections[]=
		{
			"camo"
		};
		
		attachments[]=
		{
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
				"Slot_Vest",
				"Slot_Back",
				"Slot_Headgear",
				"Slot_Eyewear",
				"Slot_Mask",
				"Slot_Armband"
			};
		};
	};

	class ZmbF_CitizenANormal_Base;
	class ZmbF_CitizenANormal_Blue;
	class ZmbF_Baty: ZmbF_CitizenANormal_Blue
	{
		htMin = 126000;
		htMax = 128800;
		afMax = 12400;
		mfMax = 12333;
		mFact = 12333;
		tBody = 12573;
		aiAgentTemplate = "Custom_Zombie";
		scope=2;
		hiddenSelectionsMaterials[]=
		{
			"PlayersBecameZombiesOnDeath\data\baty_zombie.rvmat"
		};
		hiddenSelectionsTextures[]=
		{
			"PlayersBecameZombiesOnDeath\data\baty_zombie_co.paa"
		};
		storageCategory=9;

		hiddenSelections[]=
		{
			"camo"
		};
		
		attachments[]=
		{
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
				"Slot_Vest",
				"Slot_Back",
				"Slot_Headgear",
				"Slot_Eyewear",
				"Slot_Mask",
				"Slot_Armband"
			};
		};
	};

	class ZombieMaleBase;
	class LowTierMaleZombieBase;
	class ZmbM_CitizenASkinny_LT_Base;
	class ZmbM_CitizenASkinny_Brown;
	class ZmbF_Frida: ZmbM_CitizenASkinny_Brown 
	{
		htMin = 126000;
		htMax = 128800;
		afMax = 12400;
		mfMax = 12333;
		mFact = 12333;
		tBody = 12573;
		aiAgentTemplate = "Custom_Zombie";
		scope=2;
		hiddenSelectionsMaterials[]=
		{
			"PlayersBecameZombiesOnDeath\data\frida_zombie.rvmat"
		};
		hiddenSelectionsTextures[]=
		{
			"PlayersBecameZombiesOnDeath\data\frida_zombie_co.paa"
		};
		storageCategory=9;

		hiddenSelections[]=
		{
			"camo"
		};
		
		attachments[]=
		{
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
