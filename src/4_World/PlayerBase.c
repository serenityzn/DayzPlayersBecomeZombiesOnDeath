modded class PlayerBase	// modded keyword for modding an existing class
{
	bool BecameZombie = false;
	
	
	
	override void OnJumpStart()	// overriding an existing method
	{
		super.OnJumpStart();			// call the original jump callback method so we don't break stuff
		Print("JUMP !@#!@#!@!");	// our modded print
    
	}
	
	override void EEHitBy(TotalDamageResult damageResult, int damageType, EntityAI source, int component, string dmgZone, string ammo, vector modelPos, float speedCoef)
	{
		super.EEHitBy(damageResult, damageType, source, component, dmgZone, ammo, modelPos, speedCoef);
		// TODO skip furhter actions if already a zombie
//		if ( BecameZombie )
//		{
//		    return
//		}
		
		if ( source.GetType() == "Sample_Zombie" )
		{
			SetHealth("", "", GetHealth() - 10);
		}

		Print("HIT by " + source.ToString() + " damageType "  +damageType.ToString() + " component is " + component.ToString() + " Ammo " + ammo + "dmg zone" + dmgZone +  " Health is " + GetHealth().ToString() );
		if ( GetHealth() <= 0 && BecameZombie == false )
		{
			Print("Player Dead!!!! Location is " + modelPos.ToString());
			//SpawnZombie(GetPosition(), GetInventory());
			BecameZombie = true;	
			GetGame().GetCallQueue(CALL_CATEGORY_GAMEPLAY).CallLater( SpawnZombie, 5000, false, GetPosition(), GetInventory() );
		}
		
	}
	
	
	void SpawnZombie(vector pos, GameInventory playerInventory)
	{
		string ZombieSkin;
		ZombieBase zombie;
		string playerType = GetType();
		Print("Player type is [" + playerType +"]");

		if ( playerType == "SurvivorF_Linda" )
		{
			ZombieSkin = "ZmbF_Linda";
		} 
		else if  ( playerType == "SurvivorF_Baty" )
		{
			ZombieSkin = "ZmbF_Baty";
		}
		else if  ( playerType == "SurvivorF_Eva" )
		{
			ZombieSkin = "ZmbF_Baty";
		}
		else if  ( playerType == "SurvivorF_Frida" )
		{
			ZombieSkin = "ZmbF_Frida";
		}
		else if  ( playerType == "SurvivorF_Maria" )
		{
			ZombieSkin = "ZmbF_Frida";
		}
		else if  ( playerType == "SurvivorF_Naomi" )
		{
			ZombieSkin = "ZmbF_Frida";
		}
		else if  ( playerType == "SurvivorF_Gabi" )
		{
			ZombieSkin = "ZmbF_Frida";
		}
		else if  ( playerType == "SurvivorF_Helga" )
		{
			ZombieSkin = "ZmbF_Frida";
		}
		else if  ( playerType == "SurvivorF_Irena" )
		{
			ZombieSkin = "ZmbF_Frida";
		}
		else if  ( playerType == "SurvivorF_Judy" )
		{
			ZombieSkin = "ZmbF_Frida";
		}
		else if  ( playerType == "SurvivorF_Kleiko" )
		{
			ZombieSkin = "ZmbF_Frida";
		}
		else if  ( playerType == "SurvivorM_Boris" )
		{
			ZombieSkin = "ZmbF_Frida";
		}
		else if  ( playerType == "SurvivorM_Cyril" )
		{
			ZombieSkin = "ZmbF_Frida";
		}
		else if  ( playerType == "SurvivorM_Denis" )
		{
			ZombieSkin = "ZmbF_Frida";
		}
     	else if  ( playerType == "SurvivorM_Elias" )
		{
			ZombieSkin = "ZmbF_Frida";
		}
     	else if  ( playerType == "SurvivorM_Francis" )
		{
			ZombieSkin = "ZmbF_Frida";
		}
     	else if  ( playerType == "SurvivorM_Guo" )
		{
			ZombieSkin = "ZmbF_Frida";
		}
     	else if  ( playerType == "SurvivorM_Hassan" )
		{
			ZombieSkin = "ZmbF_Frida";
		}
     	else if  ( playerType == "SurvivorM_Indar" )
		{
			ZombieSkin = "ZmbF_Frida";
		}
     	else if  ( playerType == "SurvivorM_Jose" )
		{
			ZombieSkin = "ZmbF_Frida";
		}
     	else if  ( playerType == "SurvivorM_Kaito" )
		{
			ZombieSkin = "ZmbF_Frida";
		}
     	else if  ( playerType == "SurvivorM_Lewis" )
		{
			ZombieSkin = "ZmbF_Frida";
		}
     	else if  ( playerType == "SurvivorM_Manua" )
		{
			ZombieSkin = "ZmbF_Frida";
		}
		else {
			ZombieSkin = "ZmbF_Frida";
		}

		Print("Zombie type is ["+ZombieSkin+"]");

		Object zombieObject = GetGame().CreateObject(ZombieSkin, pos, false, true);
		zombie = ZombieBase.Cast( zombieObject );
		
		int count = playerInventory.AttachmentCount();
		int cargoCount;
		
		EntityAI p2Item;
		EntityAI z2Item;		
	
	
		for (int i = 0; i < count; i++) {
			p2Item = playerInventory.GetAttachmentFromIndex(i);
			z2Item = zombie.GetInventory().CreateInInventory(p2Item.GetType());
			
			if ( p2Item.HasAnyCargo() == true ) {
				cargoCount = p2Item.GetInventory().CountInventory();
				cargoCount--;
				
				for (int j = 0; j < cargoCount; j++) {
					EntityAI myItem = p2Item.GetInventory().GetCargo().GetItem(j);					
					zombie.GetInventory().CreateInInventory(myItem.GetType());
				}
			}
			
			if (p2Item.GetInventory().AttachmentCount() > 0) {
				for (int k =0; k < p2Item.GetInventory().AttachmentCount(); k++ ) {
					z2Item.GetInventory().CreateAttachment( p2Item.GetInventory().GetAttachmentFromIndex(k).GetType() );
				}
			}
	}
		Print("Zombie Spawned!!!");
		Delete();
	}
}