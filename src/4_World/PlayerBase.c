modded class PlayerBase
{
	bool BecameZombie = false;

	override void EEHitBy(TotalDamageResult damageResult, int damageType, EntityAI source, int component, string dmgZone, string ammo, vector modelPos, float speedCoef)
	{
		super.EEHitBy(damageResult, damageType, source, component, dmgZone, ammo, modelPos, speedCoef);

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

		if ( playerType == "SurvivorF_Linda" )
		{
			ZombieSkin = "PBZ_Zombie_Linda";
		}
		else if ( playerType == "SurvivorF_Baty" || playerType == "SurvivorF_Eva" )
		{
			ZombieSkin = "PBZ_Zombie_Baty";
		}
		else if ( playerType.Contains("SurvivorF_") )
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

		int count = playerInventory.AttachmentCount();
		int cargoCount;

		EntityAI p2Item;
		EntityAI z2Item;

		for (int i = 0; i < count; i++)
		{
			p2Item = playerInventory.GetAttachmentFromIndex(i);
			z2Item = zombie.GetInventory().CreateInInventory(p2Item.GetType());

			if ( p2Item.HasAnyCargo() )
			{
				cargoCount = p2Item.GetInventory().CountInventory();

				for (int j = 0; j < cargoCount; j++)
				{
					EntityAI myItem = p2Item.GetInventory().GetCargo().GetItem(j);
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