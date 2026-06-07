modded class ZombieBase
{
	override bool CanBeDestroyed()
	{
		// Only prevent despawn for player zombies
		string type = GetType();
		if ( type == "PBZ_Zombie_Linda" || type == "PBZ_Zombie_Baty" || type == "PBZ_Zombie_Female" || type == "PBZ_Zombie_Male" )
			return false;

		return super.CanBeDestroyed();
	}
}
