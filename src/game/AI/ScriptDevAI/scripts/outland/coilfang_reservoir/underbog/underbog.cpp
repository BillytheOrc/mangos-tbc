/* This file is part of the ScriptDev2 Project. See AUTHORS file for Copyright information
* This program is free software; you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation; either version 2 of the License, or
* (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program; if not, write to the Free Software
* Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
*/

/* ScriptData
SDName: Instance_Underbog
SD%Complete:
SDComment:
SDCategory: Coilfang Resevoir, The Underbog
EndScriptData */

#include "AI/ScriptDevAI/include/precompiled.h"
#include "underbog.h"

instance_underbog::instance_underbog(Map* map) : ScriptedInstance(map), m_bGhazanSurfaced(false)
{
    Initialize();
}

void instance_underbog::Initialize()
{

}

void instance_underbog::SetData(uint32 type, uint32 data)
{
    if (type == DATA_GHAZAN_SURFACE)
        m_bGhazanSurfaced = bool(data);
}

uint32 instance_underbog::GetData(uint32 type) const
{
    if (type == DATA_GHAZAN_SURFACE)
        return uint32(m_bGhazanSurfaced);

    return 0;
}

void instance_underbog::OnCreatureCreate(Creature* creature)
{
    switch (creature->GetEntry())
    {
    case NPC_GHAZAN:
    case NPC_GHAZAN_H:
        m_npcEntryGuidStore[creature->GetEntry()] = creature->GetObjectGuid();
        break;
    }
}

InstanceData* GetInstanceData_instance_underbog(Map* map)
{
    return new instance_underbog(map);
}

/*######
## at_ghazan_surface
######*/

bool AreaTrigger_at_ghazan_surface(Player* player, AreaTriggerEntry const* /*pAt*/)
{
    ScriptedInstance* instance = (ScriptedInstance*)player->GetMap()->GetInstanceData();
    if (instance->GetData(DATA_GHAZAN_SURFACE) == 0)
    {
        if (Unit* ghazan = instance->GetSingleCreatureFromStorage(player->GetMap()->IsRegularDifficulty() ? NPC_GHAZAN : NPC_GHAZAN_H))
        {
            ghazan->GetMotionMaster()->Clear(false, true);
            ghazan->GetMotionMaster()->MoveWaypoint(1);
        }
        instance->SetData(DATA_GHAZAN_SURFACE, 1);
    }
    return true;
}

void AddSC_instance_underbog()
{
    Script* pNewScript;

    pNewScript = new Script;
    pNewScript->Name = "instance_underbog";
    pNewScript->GetInstanceData = &GetInstanceData_instance_underbog;
    pNewScript->RegisterSelf();

    pNewScript = new Script;
    pNewScript->Name = "at_ghazan_surface";
    pNewScript->pAreaTrigger = &AreaTrigger_at_ghazan_surface;
    pNewScript->RegisterSelf();
}

