/* -*- mode: c++; c-basic-offset: 4; indent-tabs-mode: nil -*-

   this file is part of rcssserver3D
   Fri May 9 2003
   Copyright (C) 2002,2003 Koblenz University
   Copyright (C) 2003 RoboCup Soccer Server 3D Maintenance Group
   $Id: odeobject.cpp 3 2008-11-21 02:38:08Z hedayat $

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
#ifndef OXYGEN_ODESTATICBODY_H
#define OXYGEN_ODESTATICBODY_H

#include <oxygen/physicsserver/ode/odebody.h>
#include <oxygen/physicsserver/int/staticbodyint.h>

namespace oxygen
{

class OXYGEN_API ODEStaticBody : public StaticBodyInt, public ODEBody
{
public:
    ODEStaticBody();
};

} //namespace oxygen

#endif //OXYGEN_ODESTATICBODY_H
