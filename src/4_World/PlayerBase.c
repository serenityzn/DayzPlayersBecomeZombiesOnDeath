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

	void SpawnZombie(vector pos, GameInventory playerInventory, string playerID)
	{
		// Cap enforcement — runs only when total reaches MaxCustomZombies
		PBZ_Config cfg = PBZ_Config.GetInstance();
		int femCount = 0;
		int malCount = 0;
		if (PBZ_Zombie_Female.s_Instances) femCount = PBZ_Zombie_Female.s_Instances.Count();
		if (PBZ_Zombie_Male.s_Instances)   malCount = PBZ_Zombie_Male.s_Instances.Count();

		if (femCount + malCount >= cfg.MaxCustomZombies)
		{
			// Collect old zombies (IsChasing == false) into temp arrays — never modify s_Instances during iteration
			array<PBZ_Zombie_Female> oldFem = new array<PBZ_Zombie_Female>();
			if (PBZ_Zombie_Female.s_Instances)
			{
				foreach (PBZ_Zombie_Female zF : PBZ_Zombie_Female.s_Instances)
				{
					if (zF && !zF.IsChasing()) oldFem.Insert(zF);
				}
			}
			array<PBZ_Zombie_Male> oldMal = new array<PBZ_Zombie_Male>();
			if (PBZ_Zombie_Male.s_Instances)
			{
				foreach (PBZ_Zombie_Male zM : PBZ_Zombie_Male.s_Instances)
				{
					if (zM && !zM.IsChasing()) oldMal.Insert(zM);
				}
			}

			// Pass 1: delete a batch of old zombies (oldest females first, then males)
			int oldCount = oldFem.Count() + oldMal.Count();
			int batchSize = cfg.ZombieDeleteBatchSize;
			if (batchSize < 1) batchSize = 1;
			if (batchSize > oldCount) batchSize = oldCount;

			int deleted = 0;
			for (int fi = 0; fi < oldFem.Count() && deleted < batchSize; fi++)
			{
				GetGame().ObjectDelete(oldFem[fi]);
				deleted++;
			}
			for (int mi = 0; mi < oldMal.Count() && deleted < batchSize; mi++)
			{
				GetGame().ObjectDelete(oldMal[mi]);
				deleted++;
			}

			if (cfg.DebugEnabled)
				Print("[PBZ] Cap cleanup: deleted " + deleted + " old zombie(s). Total was: " + (femCount + malCount));

			// Pass 2: still at cap — pre-calculate how many more to delete, collect then delete
			int totalAfterPass1 = (femCount + malCount) - deleted;
			int stillOver = totalAfterPass1 - cfg.MaxCustomZombies + 1;

			if (stillOver > 0)
			{
				array<PBZ_Zombie_Female> toDeleteF2 = new array<PBZ_Zombie_Female>();
				array<PBZ_Zombie_Male>   toDeleteM2 = new array<PBZ_Zombie_Male>();

				int fi2 = 0;
				while (stillOver > 0 && PBZ_Zombie_Female.s_Instances && fi2 < PBZ_Zombie_Female.s_Instances.Count())
				{
					toDeleteF2.Insert(PBZ_Zombie_Female.s_Instances[fi2]);
					fi2++;
					stillOver--;
				}
				int mi2 = 0;
				while (stillOver > 0 && PBZ_Zombie_Male.s_Instances && mi2 < PBZ_Zombie_Male.s_Instances.Count())
				{
					toDeleteM2.Insert(PBZ_Zombie_Male.s_Instances[mi2]);
					mi2++;
					stillOver--;
				}

				foreach (PBZ_Zombie_Female zFd : toDeleteF2)
					GetGame().ObjectDelete(zFd);
				foreach (PBZ_Zombie_Male zMd : toDeleteM2)
					GetGame().ObjectDelete(zMd);
			}
		}

		// Stop any existing zombie chasing this player
		if (PBZ_Zombie_Female.s_Instances)
		{
			foreach (PBZ_Zombie_Female zF : PBZ_Zombie_Female.s_Instances)
			{
				if (zF && zF.GetTargetPlayerID() == playerID)
					zF.SetChasing(false);
			}
		}

		if (PBZ_Zombie_Male.s_Instances)
		{
			foreach (PBZ_Zombie_Male zM : PBZ_Zombie_Male.s_Instances)
			{
				if (zM && zM.GetTargetPlayerID() == playerID)
					zM.SetChasing(false);
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
			PBZ_LogStatus();

		Delete();
	}

	void PBZ_LogStatus()
	{
		int totalF = 0;
		int totalM = 0;
		if (PBZ_Zombie_Female.s_Instances) totalF = PBZ_Zombie_Female.s_Instances.Count();
		if (PBZ_Zombie_Male.s_Instances)   totalM = PBZ_Zombie_Male.s_Instances.Count();
		int total  = totalF + totalM;

		// Build per-player counts using parallel arrays
		array<string> pids    = new array<string>();
		array<int>    counts  = new array<int>();
		array<int>    chasing = new array<int>();

		if (PBZ_Zombie_Female.s_Instances)
		{
			foreach (PBZ_Zombie_Female zF : PBZ_Zombie_Female.s_Instances)
			{
				if (!zF) continue;
				string pid = zF.GetTargetPlayerID();
				int idx = pids.Find(pid);
				if (idx == -1)
				{
					pids.Insert(pid);
					counts.Insert(1);
					int chasingValF = 0;
					if (zF.IsChasing()) chasingValF = 1;
					chasing.Insert(chasingValF);
				}
				else
				{
					counts[idx] = counts[idx] + 1;
					if (zF.IsChasing()) chasing[idx] = chasing[idx] + 1;
				}
			}
		}
		if (PBZ_Zombie_Male.s_Instances)
		{
			foreach (PBZ_Zombie_Male zM : PBZ_Zombie_Male.s_Instances)
			{
				if (!zM) continue;
				string pid = zM.GetTargetPlayerID();
				int idx = pids.Find(pid);
				if (idx == -1)
				{
					pids.Insert(pid);
					counts.Insert(1);
					int chasingValM = 0;
					if (zM.IsChasing()) chasingValM = 1;
					chasing.Insert(chasingValM);
				}
				else
				{
					counts[idx] = counts[idx] + 1;
					if (zM.IsChasing()) chasing[idx] = chasing[idx] + 1;
				}
			}
		}

		Print("[PBZ] === Zombie Status ===");
		Print("[PBZ]   Total : " + total + " / " + PBZ_Config.GetInstance().MaxCustomZombies + "  (F:" + totalF + " M:" + totalM + ")");
		for (int i = 0; i < pids.Count(); i++)
			Print("[PBZ]   Player " + pids[i] + " : " + counts[i] + " zombie(s), " + chasing[i] + " chasing");
		Print("[PBZ] ======================");
	}
}