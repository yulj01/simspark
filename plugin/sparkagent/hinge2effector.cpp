/* -*- mode: c++; c-basic-offset: 4; indent-tabs-mode: nil -*-

   this file is part of rcssserver3D
   Fri May 9 2003
   Copyright (C) 2002,2003 Koblenz University
   Copyright (C) 2003 RoboCup Soccer Server 3D Maintenance Group
   $Id: hinge2effector.cpp,v 1.2 2004/05/17 09:13:30 rollmark Exp $

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
#include "hinge2effector.h"
#include "hinge2action.h"

using namespace oxygen;
using namespace zeitgeist;
using namespace boost;
using namespace std;

Hinge2Effector::Hinge2Effector() : Effector()
{
    SetName("hinge2");
}

Hinge2Effector::~Hinge2Effector()
{
}

bool Hinge2Effector::Realize(boost::shared_ptr<ActionObject> action)
{
    if (mJoint.get() == 0)
        {
            return false;
        }

    shared_ptr<Hinge2Action> hinge2Action =
        shared_dynamic_cast<Hinge2Action>(action);

    if (hinge2Action.get() == 0)
    {
        GetLog()->Error()
            << "ERROR: (Hinge2Effector) cannot realize an "
            << "unknown ActionObject\n";
        return false;
    }

    mJoint->SetAngularMotorVelocity
        (Joint::AI_SECOND, hinge2Action->GetMotorVelocity());

    return true;
}

shared_ptr<ActionObject> Hinge2Effector::GetActionObject(const Predicate& predicate)
{
    for(;;)
        {
            if (mJoint.get() == 0)
                {
                    break;
                }

            if (predicate.name != GetPredicate())
                {
                    GetLog()->Error()
                        << "ERROR: (KickEffector) invalid predicate"
                        << predicate.name << "\n";
                    break;
                }

            Predicate::Iterator iter = predicate.begin();

            float velocity;
            if (! predicate.AdvanceValue(iter, velocity))
                {
                    GetLog()->Error()
                        << "ERROR: (KickEffector) motor velocity expected\n";
                    break;
                }

            return shared_ptr<Hinge2Action>(new Hinge2Action(GetPredicate(),velocity));
        }

    return shared_ptr<ActionObject>();
}

void Hinge2Effector::OnLink()
{
    mJoint = make_shared(FindParentSupportingClass<Hinge2Joint>());

    if (mJoint.get() == 0)
        {
            GetLog()->Error()
                << "(Hinge2Effector) ERROR: found no Hinge2Joint parent\n";
        }

}

void Hinge2Effector::OnUnlink()
{
    mJoint.reset();
}