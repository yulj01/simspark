/* -*- mode: c++; c-basic-offset: 4; indent-tabs-mode: nil -*-

   this file is part of rcssserver3D
   Fri May 9 2003
   Copyright (C) 2002,2003 Koblenz University
   Copyright (C) 2003 RoboCup Soccer Server 3D Maintenance Group
   $Id: space.h 102 2009-11-18 07:24:29Z a-held $

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

#ifndef OXYGEN_PHYSICSOBJECTINT_H
#define OXYGEN_PHYSICSOBJECTINT_H

#include <boost/smart_ptr/shared_ptr.hpp>
#include <salt/matrix.h>
#include <oxygen/oxygen_defines.h>
#include <oxygen/physicsserver/genericphysicsobjects.h>

namespace oxygen
{

class OXYGEN_API PhysicsObjectInt
{

public:    
    virtual void ConvertRotationMatrix(const salt::Matrix& rot, GenericPhysicsMatrix& matrix) = 0;
    virtual void ConvertRotationMatrix(const GenericPhysicsMatrix* matrix, salt::Matrix& rot) const = 0;
};

} //namespace oxygen

#endif //OXYGEN_PHYSICSOBJECTINT_H