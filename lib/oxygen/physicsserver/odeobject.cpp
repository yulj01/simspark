/* -*- mode: c++; c-basic-offset: 4; indent-tabs-mode: nil -*-

   this file is part of rcssserver3D
   Fri May 9 2003
   Copyright (C) 2002,2003 Koblenz University
   Copyright (C) 2003 RoboCup Soccer Server 3D Maintenance Group
   $Id: odeobject.cpp,v 1.5 2004/04/07 11:37:59 rollmark Exp $

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; version 2 of the License.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
*/
#include "odeobject.h"
#include "space.h"
#include "world.h"
#include <oxygen/sceneserver/scene.h>
#include <zeitgeist/logserver/logserver.h>

using namespace oxygen;
using namespace boost;

/** returns the world node */
shared_ptr<World> ODEObject::GetWorld()
{
    shared_ptr<Scene> scene = GetScene();
    if (scene.get() == 0)
        {
            GetLog()->Error() << "(ODEObject) ERROR: found no Scene node\n";
            return shared_ptr<World>();
        }

    shared_ptr<World> worldNode = shared_dynamic_cast<World>
        (scene->GetChildOfClass("World"));
    if (worldNode.get() == 0)
        {
            GetLog()->Error() << "(ODEObject) ERROR: found no World node\n";
        }

    return worldNode;
}

shared_ptr<Space> ODEObject::GetSpace()
{
    shared_ptr<Scene> scene = GetScene();
    if (scene.get() == 0)
        {
            GetLog()->Error() << "(ODEObject) ERROR: found no Scene node\n";
            return shared_ptr<Space>();
        }

    shared_ptr<Space> spaceNode = shared_dynamic_cast<Space>
        (scene->GetChildOfClass("Space"));
    if (spaceNode.get() == 0)
        {
            GetLog()->Error() << "(ODEObject) ERROR: found no Space node\n";
        }

    return spaceNode;
}

dWorldID ODEObject::GetWorldID()
{
    shared_ptr<World> world = GetWorld();
    if (world.get() == 0)
        {
            return 0;
        }

    dWorldID worldId = world->GetODEWorld();
    if (worldId == 0)
        {
            GetLog()->Error()
                << "(ODEObject) ERROR: World returned empty ODE handle\n";
        }

    return worldId;
}

dSpaceID ODEObject::GetSpaceID()
{
    shared_ptr<Space> space = GetSpace();
    if (space.get() == 0)
        {
            return 0;
        }

    dSpaceID spaceId = space->GetODESpace();

    if (spaceId == 0)
        {
            GetLog()->Error()
                << "(ODEObject) ERROR: Space returned empty ODE handle\n";
        }

    return spaceId;
}
