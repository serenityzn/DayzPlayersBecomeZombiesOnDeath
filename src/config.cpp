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
