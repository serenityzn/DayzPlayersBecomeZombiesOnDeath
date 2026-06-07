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
