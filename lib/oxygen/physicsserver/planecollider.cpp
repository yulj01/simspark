/* -*- mode: c++ -*-

   this file is part of rcssserver3D
   Fri May 9 2003
   Copyright (C) 2003 Koblenz University
   $Id: planecollider.cpp,v 1.5 2004/02/26 21:14:05 fruit Exp $

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

using namespace oxygen;

PlaneCollider::PlaneCollider() : Collider()
{
}

void PlaneCollider::SetParams(float a, float b, float c, float d)
{
  dGeomPlaneSetParams(mODEGeom, a, b, c, d);
}

bool PlaneCollider::ConstructInternal()
{
    if (! Collider::ConstructInternal())
      {
        return false;
      }

    // a plane with normal pointing up, going through the origin
    mODEGeom = dCreatePlane(0, 0, 1, 0, 0);

    return (mODEGeom != 0);
}

void
PlaneCollider::SetParams(const salt::Vector3f& pos, salt::Vector3f normal)
{
    normal.Normalize();
    float d = pos.Dot(normal);
    dGeomPlaneSetParams(mODEGeom, normal.x(), normal.y(), normal.z(), d);
}

