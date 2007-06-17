/* -*- mode: c++; c-basic-offset: 4; indent-tabs-mode: nil -*-

   this file is part of rcssserver3D
   Fri May 9 2003
   Copyright (C) 2002,2003 Koblenz University
   Copyright (C) 2003 RoboCup Soccer Server 3D Maintenance Group
   $Id: staticmesh_c.cpp,v 1.8 2007/06/17 13:47:35 fruit Exp $

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
#include "staticmesh.h"

using namespace boost;
using namespace kerosin;
using namespace zeitgeist;
using namespace salt;
using namespace std;

FUNCTION(StaticMesh,load)
{
    // load <fileName> parameter ...
    string inFileName;

    if (
        (in.GetSize() == 0) ||
        (! in.GetValue(in[0],inFileName))
        )
        {
            return false;
        }

    ParameterList parameter(in);

    // remove filename argument
    parameter.Pop_Front();

    return obj->Load(inFileName, parameter);
}

FUNCTION(StaticMesh,setScale)
{
    Vector3f inScale;

    if (
        (in.GetSize() != 3) ||
        (! in.GetValue(in.begin(), inScale))
        )
        {
            return false;
        }

    obj->SetScale(inScale);
    return true;
}

void CLASS(StaticMesh)::DefineClass()
{
    DEFINE_BASECLASS(oxygen/BaseNode);
    DEFINE_FUNCTION(load);
    DEFINE_FUNCTION(setScale);
}
