modded class PlayerBase
{
	bool BecameZombie = false;

	override void EEHitBy(TotalDamageResult damageResult, int damageType, EntityAI source, int component, string dmgZone, string ammo, vector modelPos, float speedCoef)
	{
		super.EEHitBy(damageResult, damageType, source, component, dmgZone, ammo, modelPos, speedCoef);

		// Extra damage when hit by a player zombie
		if ( source && ( source.GetType() == "PBZ_Zombie_Female" || source.GetType() == "PBZ_Zombie_Male" ) )
		{
			PBZ_Config cfg = PBZ_Config.GetInstance();
			AddHealth("", "Health", -cfg.ExtraDamageHealth);
			AddHealth("", "Blood",  -cfg.ExtraDamageBlood);
			AddHealth("", "Shock",  -cfg.ExtraDamageShock);
		}

		if ( BecameZombie )
			return;

		if ( GetHealth() <= 0 )
		{
			BecameZombie = true;
			GetGame().GetCallQueue(CALL_CATEGORY_GAMEPLAY).CallLater( SpawnZombie, PBZ_Config.GetInstance().SpawnDelayMs, false, GetPosition(), GetInventory() );
		}
	}

	void SpawnZombie(vector pos, GameInventory playerInventory)
	{
		string ZombieSkin;
		ZombieBase zombie;
		string playerType = GetType();

		if ( playerType.Contains("SurvivorF_") )
		{
			ZombieSkin = "PBZ_Zombie_Female";
		}
		else
		{
			ZombieSkin = "PBZ_Zombie_Male";
		}

		Object zombieObject = GetGame().CreateObject(ZombieSkin, pos, false, true);
		zombie = ZombieBase.Cast( zombieObject );

		if ( !zombie )
			return;

		zombie.SetHealth("", "", zombie.GetMaxHealth("", "") * PBZ_Config.GetInstance().ZombieHealthMultiplier);

		int count = playerInventory.AttachmentCount();
		int cargoCount;

		EntityAI p2Item;
		EntityAI z2Item;

		for (int i = 0; i < count; i++)
		{
			p2Item = playerInventory.GetAttachmentFromIndex(i);
			if ( !p2Item )
				continue;
			z2Item = zombie.GetInventory().CreateInInventory(p2Item.GetType());

			if ( p2Item.HasAnyCargo() )
			{
				cargoCount = p2Item.GetInventory().CountInventory();

				for (int j = 0; j < cargoCount; j++)
				{
					EntityAI myItem = p2Item.GetInventory().GetCargo().GetItem(j);
					if ( myItem )
						zombie.GetInventory().CreateInInventory(myItem.GetType());
				}
			}

			if ( z2Item && p2Item.GetInventory().AttachmentCount() > 0 )
			{
				for (int k = 0; k < p2Item.GetInventory().AttachmentCount(); k++)
				{
					z2Item.GetInventory().CreateAttachment( p2Item.GetInventory().GetAttachmentFromIndex(k).GetType() );
				}
			}
		}

		Delete();
	}
}