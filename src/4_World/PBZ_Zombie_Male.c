class PBZ_Zombie_Male : ZombieBase
{
	private string m_TargetPlayerID;
	private bool   m_IsChasing;
	private int    m_ZombieID;
	private ref NoiseParams m_NoiseParams;

	static ref array<PBZ_Zombie_Male> s_Instances;
	static int s_NextID;

	void PBZ_Zombie_Male()
	{
		m_NoiseParams = new NoiseParams();
		m_NoiseParams.LoadFromPath("CfgVehicles SurvivorBase NoiseShout");

		m_ZombieID = s_NextID;
		s_NextID++;

		if (!s_Instances)
			s_Instances = new array<PBZ_Zombie_Male>();
		s_Instances.Insert(this);
	}

	string GetTargetPlayerID()
	{
		return m_TargetPlayerID;
	}

	void SetChasing(bool chasing)
	{
		m_IsChasing = chasing;
		if (!m_IsChasing)
		{
			GetGame().GetCallQueue(CALL_CATEGORY_GAMEPLAY).Remove(ScanForTarget);
			if (PBZ_Config.GetInstance().DebugEnabled)
				Print("[PBZ][M#" + m_ZombieID + "] chasing STOPPED for player " + m_TargetPlayerID);
		}
	}

	void SetTargetPlayerID(string id)
	{
		m_TargetPlayerID = id;
		m_IsChasing = true;
		if (m_TargetPlayerID != "")
		{
			if (PBZ_Config.GetInstance().DebugEnabled)
				Print("[PBZ][M#" + m_ZombieID + "] chasing STARTED for player " + m_TargetPlayerID);
			GetGame().GetCallQueue(CALL_CATEGORY_GAMEPLAY).CallLater(ScanForTarget, PBZ_Config.GetInstance().RescanIntervalMs, true);
		}
	}

	void ScanForTarget()
	{
		if (!IsAlive() || !m_IsChasing)
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

			GetGame().GetNoiseSystem().AddNoiseTarget(noisePos, lifetime, m_NoiseParams, 100.0);
			GetGame().GetNoiseSystem().AddNoiseTarget(targetPos, lifetime, m_NoiseParams, 100.0);

			if (cfg.DebugEnabled)
			{
				Print("[PBZ][M#" + m_ZombieID + "] tracking player: " + m_TargetPlayerID);
				Print("[PBZ][M#" + m_ZombieID + "]   Zombie pos : " + myPos.ToString());
				Print("[PBZ][M#" + m_ZombieID + "]   Player pos : " + targetPos.ToString());
				Print("[PBZ][M#" + m_ZombieID + "]   Distance   : " + dist.ToString() + " m");
				Print("[PBZ][M#" + m_ZombieID + "]   Noise pos  : " + noisePos.ToString());
				Print("[PBZ][M#" + m_ZombieID + "]   Lead dist  : " + leadDist.ToString() + " m");
			}
			return;
		}

		if (PBZ_Config.GetInstance().DebugEnabled)
			Print("[PBZ][M#" + m_ZombieID + "] target " + m_TargetPlayerID + " not found — roaming");
	}

	override void EEKilled(Object killer)
	{
		super.EEKilled(killer);
		GetGame().GetCallQueue(CALL_CATEGORY_GAMEPLAY).Remove(ScanForTarget);
		if (s_Instances)
			s_Instances.RemoveItem(this);
	}
}
