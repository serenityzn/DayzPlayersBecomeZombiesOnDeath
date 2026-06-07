class PBZ_Config
{
	static ref PBZ_Config m_Instance;

	// How many times the zombie's base health is multiplied on spawn
	float ZombieHealthMultiplier = 2.0;

	// Extra damage dealt to a player on each hit from a PBZ zombie
	float ExtraDamageHealth = 15.0;
	float ExtraDamageBlood  = 30.0;
	float ExtraDamageShock  = 10.0;

	// Delay in milliseconds before the zombie spawns after player death
	int SpawnDelayMs = 5000;

	// How often (ms) the zombie scans for its target player after they respawn
	int RescanIntervalMs = 5000;

	// How far ahead (meters) to place the noise waypoint toward the target player
	float NoiseLeadDistance = 90.0;

	// Maximum total PBZ zombies allowed on the server at once
	int MaxCustomZombies = 20;

	// How many old zombies to delete per cleanup pass when cap is reached (min 1)
	int ZombieDeleteBatchSize = 2;

	// Print debug info to server console and logs
	bool DebugEnabled = false;

	static PBZ_Config GetInstance()
	{
		if ( !m_Instance )
		{
			m_Instance = new PBZ_Config();
			m_Instance.Load();
		}
		return m_Instance;
	}

	void Load()
	{
		string dir        = "$profile:PlayersBecameZombies";
		string configPath = dir + "/config.json";

		if ( FileExist(configPath) )
		{
			JsonFileLoader<PBZ_Config>.JsonLoadFile(configPath, this);
			Print("[PBZ] Config loaded from " + configPath);
		}
		else
		{
			MakeDirectory(dir);
			JsonFileLoader<PBZ_Config>.JsonSaveFile(configPath, this);
			Print("[PBZ] Default config created at " + configPath);
		}
	}
}
