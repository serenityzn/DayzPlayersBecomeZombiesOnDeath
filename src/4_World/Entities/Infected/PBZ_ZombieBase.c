class PBZ_ZombieBase: ZombieBase
{
	// Prevent the server's proximity-based despawn from removing this zombie.
	// Does NOT affect death — killed zombies still go through normal death flow.
	override bool CanBeDestroyed()
	{
		return false;
	}
}

class PBZ_Zombie_Linda: PBZ_ZombieBase {}
class PBZ_Zombie_Baty: PBZ_ZombieBase {}
class PBZ_Zombie_Female: PBZ_ZombieBase {}
class PBZ_Zombie_Male: PBZ_ZombieBase {}
