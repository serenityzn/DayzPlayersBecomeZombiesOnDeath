class PBZ_Zombie_Female : ZombieBase
{
	private string m_TargetPlayerID;
	private ref NoiseParams m_NoiseParams;

	void PBZ_Zombie_Female()
	{
		m_NoiseParams = new NoiseParams();
		m_NoiseParams.LoadFromPath("CfgVehicles SurvivorBase NoiseShout");
	}

	void SetTargetPlayerID(string id)
	{
		m_TargetPlayerID = id;
		if (m_TargetPlayerID != "")
			GetGame().GetCallQueue(CALL_CATEGORY_GAMEPLAY).CallLater(ScanForTarget, PBZ_Config.GetInstance().RescanIntervalMs, true);
	}

	void ScanForTarget()
	{
		if (!IsAlive())
		{
			GetGame().GetCallQueue(CALL_CATEGORY_GAMEPLAY).Remove(ScanForTarget);
			return;
		}

		array<Man> players = new array<Man>;
		GetGame().GetPlayers(players);

		foreach (Man man : players)
		{
			PlayerBase player = PlayerBase.Cast(man);
			if (!player || !player.IsAlive())
				continue;

			PlayerIdentity identity = player.GetIdentity();
			if (!identity || identity.GetId() != m_TargetPlayerID)
				continue;

			// Target found — place noise waypoint ahead in player's direction
			PBZ_Config cfg = PBZ_Config.GetInstance();

			vector myPos     = GetPosition();
			vector targetPos = player.GetPosition();
			vector dir       = vector.Direction(myPos, targetPos);
			dir.Normalize();

			float dist      = vector.Distance(myPos, targetPos);
			float leadDist  = Math.Min(cfg.NoiseLeadDistance, dist);
			vector noisePos = myPos + dir * leadDist;

			float lifetime = cfg.RescanIntervalMs / 1000.0 + 1.0;

			// Place noise at lead point AND at player position for strong pull
			GetGame().GetNoiseSystem().AddNoiseTarget(noisePos, lifetime, m_NoiseParams, 100.0);
			GetGame().GetNoiseSystem().AddNoiseTarget(targetPos, lifetime, m_NoiseParams, 100.0);

			if (cfg.DebugEnabled)
			{
				Print("[PBZ] PBZ_Zombie_Female tracking player: " + m_TargetPlayerID);
				Print("[PBZ]   Zombie pos : " + myPos.ToString());
				Print("[PBZ]   Player pos : " + targetPos.ToString());
				Print("[PBZ]   Distance   : " + dist.ToString() + " m");
				Print("[PBZ]   Noise pos  : " + noisePos.ToString());
				Print("[PBZ]   Lead dist  : " + leadDist.ToString() + " m");
			}
			return;
		}

		if (PBZ_Config.GetInstance().DebugEnabled)
			Print("[PBZ] PBZ_Zombie_Female: target " + m_TargetPlayerID + " not found — roaming");
	}

	override void EEKilled(Object killer)
	{
		super.EEKilled(killer);
		GetGame().GetCallQueue(CALL_CATEGORY_GAMEPLAY).Remove(ScanForTarget);
	}
}
