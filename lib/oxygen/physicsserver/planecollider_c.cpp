/* -*- mode: c++ -*-

   this file is part of rcssserver3D
   Fri May 9 2003
   Copyright (C) 2003 Koblenz University
   $Id: planecollider_c.cpp,v 1.6 2004/02/26 21:14:06 fruit Exp $

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

#include "planecollider.h"

using namespace boost;
using namespace oxygen;

FUNCTION(setParams)
{
        if (in.size() == 4)
        {
                PlaneCollider *pc = static_cast<PlaneCollider*>(obj);
                pc->SetParams(any_cast<float>(in[0]), any_cast<float>(in[1]), any_cast<float>(in[2]), any_cast<float>(in[3]));
        }
        else if (in.size() == 6)
        {
                PlaneCollider *pc = static_cast<PlaneCollider*>(obj);
                pc->SetParams(salt::Vector3f(any_cast<float>(in[0]), any_cast<float>(in[1]), any_cast<float>(in[2])),
                              salt::Vector3f(any_cast<float>(in[3]), any_cast<float>(in[4]), any_cast<float>(in[5])));
        }
}

void CLASS(PlaneCollider)::DefineClass()
{
        DEFINE_BASECLASS(oxygen/Collider);
        DEFINE_FUNCTION(setParams);
}
