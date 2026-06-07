modded class PlayerBase
{
	bool BecameZombie = false;

	override void EEHitBy(TotalDamageResult damageResult, int damageType, EntityAI source, int component, string dmgZone, string ammo, vector modelPos, float speedCoef)
	{
		super.EEHitBy(damageResult, damageType, source, component, dmgZone, ammo, modelPos, speedCoef);

		// Extra damage when hit by a player zombie
		if ( source && ( source.GetType() == "ZmbF_HikerSkinny_Blue" || source.GetType() == "ZmbM_HikerSkinny_Brown" ) )
		{
			AddHealth("", "Health", -15);
			AddHealth("", "Blood", -30);
			AddHealth("", "Shock", -10);
		}

		if ( BecameZombie )
			return;

		if ( GetHealth() <= 0 )
		{
			BecameZombie = true;
			GetGame().GetCallQueue(CALL_CATEGORY_GAMEPLAY).CallLater( SpawnZombie, 5000, false, GetPosition(), GetInventory() );
		}
	}

	void SpawnZombie(vector pos, GameInventory playerInventory)
	{
		string ZombieSkin;
		ZombieBase zombie;
		string playerType = GetType();

		if ( playerType.Contains("SurvivorF_") )
		{
			ZombieSkin = "ZmbF_HikerSkinny_Blue";
		}
		else
		{
			ZombieSkin = "ZmbM_HikerSkinny_Brown";
		}

		Object zombieObject = GetGame().CreateObject(ZombieSkin, pos, false, true);
		zombie = ZombieBase.Cast( zombieObject );

		if ( !zombie )
			return;

		// Make the player zombie stronger — double health
		zombie.SetHealth("", "", zombie.GetMaxHealth("", "") * 2);
		zombie.SetHealth("Blood", "", zombie.GetMaxHealth("Blood", ""));

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