modded class PlayerBase
{
	bool BecameZombie = false;
	string m_PBZ_PlayerID = "";

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
			PlayerIdentity identity = GetIdentity();
			if (identity)
				m_PBZ_PlayerID = identity.GetId();
			GetGame().GetCallQueue(CALL_CATEGORY_GAMEPLAY).CallLater( SpawnZombie, PBZ_Config.GetInstance().SpawnDelayMs, false, GetPosition(), GetInventory(), m_PBZ_PlayerID );
		}
	}

	private void PBZ_LogRegistryStatus()
	{
		array<EntityAI> reg = PBZ_Config.GetZombieRegistry();
		if (!reg) return;

		int total = reg.Count();
		int activeTracking = 0;

		map<string, int> perPlayer = new map<string, int>();

		foreach (EntityAI ze : reg)
		{
			string pid = PBZ_GetZombiePID(ze);
			if (pid == "") continue;

			perPlayer.Set(pid, perPlayer.Get(pid) + 1);

			// Count as "actively following" if it has a target and is not already in aggro
			ZombieBase zb = ZombieBase.Cast(ze);
			if (zb)
			{
				int ms = zb.GetMindStateSynced();
				bool chasing = (ms == DayZInfectedConstants.MINDSTATE_CHASE || ms == DayZInfectedConstants.MINDSTATE_FIGHT);
				if (!chasing) activeTracking++;
			}
		}

		Print("[PBZ] === Registry Status ===");
		Print("[PBZ]   Total PBZ zombies : " + total + " / " + PBZ_Config.GetInstance().MaxCustomZombies);
		Print("[PBZ]   Following (noise) : " + activeTracking);
		Print("[PBZ]   In aggro (vanilla) : " + (total - activeTracking));
		Print("[PBZ]   Per player:");
		for (int pi = 0; pi < perPlayer.Count(); pi++)
		{
			string ppid = perPlayer.GetKeyByIndex(pi);
			Print("[PBZ]     " + ppid + " : " + perPlayer.Get(ppid) + " zombie(s)");
		}
		Print("[PBZ] ========================");
	}

	private string PBZ_GetZombiePID(EntityAI z)
	{
		PBZ_Zombie_Female zF = PBZ_Zombie_Female.Cast(z);
		if (zF) return zF.GetTargetPlayerID();
		PBZ_Zombie_Male zM = PBZ_Zombie_Male.Cast(z);
		if (zM) return zM.GetTargetPlayerID();
		return "";
	}

	private void PBZ_DeleteZombie(EntityAI z)
	{
		PBZ_Zombie_Female zF = PBZ_Zombie_Female.Cast(z);
		if (zF) zF.ClearTargetPlayerID();
		PBZ_Zombie_Male zM = PBZ_Zombie_Male.Cast(z);
		if (zM) zM.ClearTargetPlayerID();
		PBZ_Config.UnregisterZombie(z);
		GetGame().ObjectDelete(z);
	}

	void SpawnZombie(vector pos, GameInventory playerInventory, string playerID)
	{
		array<EntityAI> registry = PBZ_Config.GetZombieRegistry();

		// Purge dead/invalid entries
		if (registry)
		{
			for (int ri = registry.Count() - 1; ri >= 0; ri--)
			{
				if (!registry[ri] || !registry[ri].IsAlive())
					registry.Remove(ri);
			}
		}

		// Enforce global cap only when we've reached it
		registry = PBZ_Config.GetZombieRegistry();
		if (registry && registry.Count() >= PBZ_Config.GetInstance().MaxCustomZombies)
		{
			PBZ_Config cfg = PBZ_Config.GetInstance();

			// Find the latest registry index for each player (highest index = most recent zombie)
			map<string, int> latestIdxPerPlayer = new map<string, int>();
			for (int li = 0; li < registry.Count(); li++)
			{
				string lpid = PBZ_GetZombiePID(registry[li]);
				if (lpid != "") latestIdxPerPlayer.Set(lpid, li);
			}

			// Collect "old" zombies — all but the most recent one per player, oldest first
			array<EntityAI> oldZombies = new array<EntityAI>();
			for (int oi = 0; oi < registry.Count(); oi++)
			{
				string opid = PBZ_GetZombiePID(registry[oi]);
				if (opid != "" && latestIdxPerPlayer.Get(opid) != oi)
					oldZombies.Insert(registry[oi]);
			}

			// Pass 1: delete a batch of oldest "old" zombies (min 1, max all available old)
			if (oldZombies.Count() > 0)
			{
				int toDelete = Math.Clamp(cfg.ZombieDeleteBatchSize, 1, oldZombies.Count());
				for (int di = 0; di < toDelete; di++)
					PBZ_DeleteZombie(oldZombies[di]);
			}

			// Pass 2: still at cap and no more "old" zombies — delete oldest overall
			while (registry.Count() >= cfg.MaxCustomZombies && registry.Count() > 0)
				PBZ_DeleteZombie(registry[0]);
		}

		// Stop the existing zombie chasing this player (1-per-player enforcement)
		registry = PBZ_Config.GetZombieRegistry();
		if (registry)
		{
			foreach (EntityAI zEntity : registry)
			{
				if (!zEntity || !zEntity.IsAlive()) continue;
				PBZ_Zombie_Female zF = PBZ_Zombie_Female.Cast(zEntity);
				if (zF && zF.GetTargetPlayerID() == playerID)
				{
					zF.ClearTargetPlayerID();
					continue;
				}
				PBZ_Zombie_Male zM = PBZ_Zombie_Male.Cast(zEntity);
				if (zM && zM.GetTargetPlayerID() == playerID)
					zM.ClearTargetPlayerID();
			}
		}

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

		// Pass the dead player's ID so the zombie hunts them when they respawn
		if (ZombieSkin == "PBZ_Zombie_Female")
		{
			PBZ_Zombie_Female pbzF = PBZ_Zombie_Female.Cast(zombie);
			if (pbzF) pbzF.SetTargetPlayerID(playerID);
		}
		else
		{
			PBZ_Zombie_Male pbzM = PBZ_Zombie_Male.Cast(zombie);
			if (pbzM) pbzM.SetTargetPlayerID(playerID);
		}

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

		if (PBZ_Config.GetInstance().DebugEnabled)
			PBZ_LogRegistryStatus();

		Delete();
	}
}