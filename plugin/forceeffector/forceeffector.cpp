/* -*- mode: c++; c-basic-indent: 4; indent-tabs-mode: nil -*-

   this file is part of rcssserver3D
   Fri May 9 2003
   Copyright (C) 2002,2003 Koblenz University
   Copyright (C) 2003 RoboCup Soccer Server 3D Maintenance Group
   $Id: forceeffector.cpp,v 1.3.2.2 2004/01/25 12:22:28 rollmark Exp $

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

#include "forceeffector.h"
#include "forceaction.h"
#include <zeitgeist/logserver/logserver.h>
#include <oxygen/gamecontrolserver/actionobject.h>

using namespace boost;
using namespace oxygen;
using namespace salt;

ForceEffector::ForceEffector() : oxygen::Effector()
{
}

ForceEffector::~ForceEffector()
{
}

bool ForceEffector::Realize(boost::shared_ptr<ActionObject> action)
{
  if (mBody.get() == 0)
    {
      return false;
    }

  shared_ptr<ForceAction> forceAction =
    shared_dynamic_cast<ForceAction>(action);

  if (forceAction.get() == 0)
    {
      GetLog()->Error()
        << "ERROR: (ForceEffector) cannot realize an unknown ActionObject\n";
      return false;
    }

  const Vector3f& force = forceAction->GetForce();
  mBody->AddForce(force);

  return true;
}

shared_ptr<ActionObject>
ForceEffector::GetActionObject(const Predicate& predicate)
{
  if (predicate.name != GetPredicate())
    {
      GetLog()->Error() << "ERROR: (ForceEffector) invalid predicate"
                        << predicate.name << "\n";
      return shared_ptr<ActionObject>();
    }

  Predicate::Iterator iter = predicate.begin();
  Vector3f force;
  if (! predicate.GetValue(iter, force))
  {
      GetLog()->Error()
        << "ERROR: (ForceEffector) Vector3f parameter expected\n";
      return shared_ptr<ActionObject>();
  };

  return shared_ptr<ActionObject>(new ForceAction(GetPredicate(),force));
}

void ForceEffector::OnLink()
{
  shared_ptr<BaseNode> parent =
    shared_dynamic_cast<BaseNode>(make_shared(GetParent()));

  if (parent.get() == 0)
    {
      GetLog()->Error()
        << "ERROR: (ForceEffector) parent node is not derived from BaseNode\n";
      return;
    }

  // parent should be a transform, or some other node, which has a
  // Body-child
  mBody = shared_dynamic_cast<Body>(parent->GetChildOfClass("Body"));

  if (mBody.get() == 0)
    {
      GetLog()->Error()
        << "ERROR: (ForceEffector) parent node has no Body child;"
          "cannot apply force\n";
      return;
    }
}

void ForceEffector::OnUnlink()
{
    mBody.reset();
}


