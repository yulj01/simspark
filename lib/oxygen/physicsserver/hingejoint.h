/* -*- mode: c++; c-basic-offset: 4; indent-tabs-mode: nil -*-
   this file is part of rcssserver3D
   Fri May 9 2003
   Copyright (C) 2003 Koblenz University
   $Id: hingejoint.h,v 1.1 2004/04/12 19:54:15 rollmark Exp $

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
#ifndef OXYGEN_HINGEJOINT_H
#define OXYGEN_HINGEJOINT_H

#include "joint.h"

namespace oxygen
{
class Body;

class HingeJoint : public Joint
{
public:
    HingeJoint();
    virtual ~HingeJoint();

    /** Set the joint anchor point. The joint will try to keep this
        point on each body together. The input is specified in local
        coordinates.
    */
    void SetHingeAnchor(const salt::Vector3f& anchor);

    /** Returns the joint anchor point in world coordinates on one of
        the two bodies. If the joint is perfectly satisfied, the joint
        anchor point will be the same for both bodies.
    */
    salt::Vector3f GetHingeAnchor (EBodyIndex idx);

protected:
    /** creates a new contact joint */
    virtual void OnLink();
};

DECLARE_CLASS(HingeJoint);

} // namespace oxygen

#endif // OXYGEN_HINGEJOINT_H