/* -*- mode: c++; c-basic-offset: 4; indent-tabs-mode: nil -*-

   this file is part of rcssserver3D
   Fri May 9 2003
   Copyright (C) 2002,2003 Koblenz University
   Copyright (C) 2003 RoboCup Soccer Server 3D Maintenance Group
   $Id: sceneimporter.h,v 1.1 2004/04/08 14:50:09 rollmark Exp $

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
#ifndef OXYGEN_SCENEIMPORTER
#define OXYGEN_SCENEIMPORTER

#include <zeitgeist/class.h>
#include <zeitgeist/leaf.h>
#include <salt/fileclasses.h>

namespace oxygen
{
class BaseNode;

class SceneImporter : public zeitgeist::Leaf
{
public:
    SceneImporter() : zeitgeist::Leaf() {}
    virtual ~SceneImporter() {};

    virtual bool ImportScene(const boost::shared_ptr<salt::RFile> file,
                             boost::shared_ptr<BaseNode> root) = 0;
};

DECLARE_ABSTRACTCLASS(SceneImporter);

} // namespace oxygen

#endif // OXYGEN_SCENEIMPORTER