/* -*- mode: c++; c-basic-offset: 4; indent-tabs-mode: nil -*-
   this file is part of rcssserver3D
   Thu Nov 8 2005
   Copyright (C) 2005 Koblenz University

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
#include "soccerbotbehavior.h"
#include <iostream>
#include <sstream>
#include <cmath>

using namespace oxygen;
using namespace zeitgeist;
using namespace std;
using namespace boost;
using namespace salt;

SoccerbotBehavior::SoccerbotBehavior() : mZG("." PACKAGE_NAME)
{
}

void SoccerbotBehavior::SetupJointIDMap()
{
    mJointIDMap.clear();
    mJointIDMap["head_joint_2"]   = JID_HEAD_2;
    mJointIDMap["head_joint_1"]   = JID_HEAD_1;
    mJointIDMap["lleg_joint_1"]   = JID_LLEG_1;
    mJointIDMap["rleg_joint_1"]   = JID_RLEG_1;
    mJointIDMap["lleg_joint_2_3"] = JID_LLEG_2_3;
    mJointIDMap["rleg_joint_2_3"] = JID_RLEG_2_3;
    mJointIDMap["lleg_joint_4"]   = JID_LLEG_4;
    mJointIDMap["rleg_joint_4"]   = JID_RLEG_4;
    mJointIDMap["lleg_joint_5_6"] = JID_LLEG_5_6;
    mJointIDMap["rleg_joint_5_6"] = JID_RLEG_5_6;
    mJointIDMap["larm_joint_1_2"] = JID_LARM_1_2;
    mJointIDMap["rarm_joint_1_2"] = JID_RARM_1_2;
    mJointIDMap["larm_joint_3"]   = JID_LARM_3;
    mJointIDMap["rarm_joint_3"]   = JID_RARM_3;
    mJointIDMap["larm_joint_4"]   = JID_LARM_4;
    mJointIDMap["rarm_joint_4"]   = JID_RARM_4;
    mJointIDMap["larm_joint_5"]   = JID_LARM_5;
    mJointIDMap["rarm_joint_5"]   = JID_RARM_5;
}

string SoccerbotBehavior::Init()
{
    mZG.GetCore()->ImportBundle("sexpparser");

    mParser = shared_static_cast<BaseParser>
        (mZG.GetCore()->New("SexpParser"));

    if (mParser.get() == 0)
    {
        cerr << "unable to create SexpParser instance." << endl;
    }

    SetupJointIDMap();
    // use the scene effector to build the agent and beam to a
    // position near the center of the playing field
    return
        "(scene rsg/agent/soccerbot.rsg)";
}

void SoccerbotBehavior::ParseHingeJointInfo(const oxygen::Predicate& predicate)
{
    //cout << "(SoccerbotBehavior) parsing HJ info" << endl;

    // read the object name
    string name;
    Predicate::Iterator iter(predicate);

    if (! predicate.GetValue(iter, "name", name))
    {
        return;
    }

    // try to lookup the joint id
    TJointIDMap::iterator idIter = mJointIDMap.find(name);
    if (idIter == mJointIDMap.end())
    {
        cerr << "(SoccerbotBehavior) unknown joint id!" << endl;
        return;
    }

    JointID jid = (*idIter).second;

    // read the angle value
    HingeJointSense sense;
    if (! predicate.GetValue(iter,"ax", sense.angle))
    {
        return;
    }

    // update the map
    mHingeJointSenseMap[jid] = sense;
}

void SoccerbotBehavior::ParseUniversalJointInfo(const oxygen::Predicate& predicate)
{
    // read the object name
    string name;
    Predicate::Iterator iter(predicate);

    if (! predicate.GetValue(iter, "name", name))
    {
        return;
    }

    // try to lookup the joint id
    TJointIDMap::iterator idIter = mJointIDMap.find(name);
    if (idIter == mJointIDMap.end())
    {
        cerr << "(SoccerbotBehavior) unknown joint id!" << endl;
        return;
    }

    JointID jid = (*idIter).second;

    // record the angle and rate values
    UniversalJointSense sense;

    // try to read axis1 angle
    if (! predicate.GetValue(iter,"ax1", sense.angle1))
    {
        cerr << "(SoccerbotBehavior) could not parse universal joint angle1!" << endl;
        return;
    }
    // try to read axis2 angle
    if (! predicate.GetValue(iter,"ax2", sense.angle2))
    {
        cerr << "(SoccerbotBehavior) could not parse universal joint angle2!" << endl;
        return;
    }   
    // try to read axis2 rate

    // update the map
    mUniversalJointSenseMap[jid] = sense;
}



string SoccerbotBehavior::Think(const std::string& message)
{
    //sleep(1);

    //bow before me
//    return "(rleg_eff_2_3 -1 0)(lleg_eff_2_3 2 0)";

    static const float gain = 0.1;
    static BehaviorState leftstate = ARM_UP;
    static BehaviorState rightstate = ARM_UP;

    // parse message and extract joint angles
    //cout << "(SoccerbotBehavior) received message " << message << endl;

    shared_ptr<PredicateList> predList =
        mParser->Parse(message);

    if (predList.get() != 0)
    {
        PredicateList& list = *predList;

        for (
             PredicateList::TList::const_iterator iter = list.begin();
             iter != list.end();
             ++iter
             )
        {
            const Predicate& predicate = (*iter);

            // check for a joint percept
            switch(predicate.name[0])
            { 
            case 'H': // hinge joint (HJ)
                ParseHingeJointInfo(predicate);
                break;
            case 'U': // universal joint (UJ)
                ParseUniversalJointInfo(predicate);
                break;
            default:
                break;
            }
        }
    }

    float curAngle = 0;
    float newAngle = 0;
 
    // string stream for the server commands
    stringstream ss("");

//    curAngle = mUniversalJointSenseMap[JID_LLEG_5_6].angle1;

//    if (curAngle < 40.0)
//    {
//        newAngle = gain * (40.0 - curAngle);
//        ss << "(lleg_eff_5_6 0.0 " << newAngle << ")";
//    }

#if 1
    switch(rightstate)
    {
        case ARM_UP:
            curAngle = mUniversalJointSenseMap[JID_RARM_1_2].angle2;
            if (curAngle < 90.0)
            {
                newAngle = gain * (90.0 - curAngle);
                ss << "(rarm_eff_1_2 0.0 " << newAngle << ")";
            }
            else
            {
                rightstate = ARM_ROTATE;
            }
            break;
        case ARM_ROTATE:
            curAngle = mHingeJointSenseMap[JID_RARM_3].angle;
            //cout << curAngle << endl;
            
            if (curAngle > -90.0)
            {
                newAngle = gain * (-90.0 - curAngle);
                ss << "(rarm_eff_3 " << newAngle << ")";
            }
            else    
            {
                rightstate = ARM_WAVE_1;
            }
            break;
        case ARM_WAVE_1:
            curAngle = mHingeJointSenseMap[JID_RARM_4].angle;
            if (curAngle < 90.0)
            {
                newAngle = gain * (90.0 - curAngle);
                ss << "(rarm_eff_4 " << newAngle << ")"; 
            }
            else
            {
                rightstate = ARM_WAVE_2;
            }
            break;
        case ARM_WAVE_2:
            curAngle = mHingeJointSenseMap[JID_RARM_4].angle;
            if (curAngle > 45.0 || curAngle <= 44.5)
            {
                newAngle = gain * (45.0 - curAngle);
                ss << "(rarm_eff_4 " << newAngle << ")";
            }
            else
            {
                rightstate = ARM_WAVE_1;
            }
            break;
        default:   
            break;
    }
#endif
    
#if 1
    switch(leftstate)
    {
        case ARM_UP:
            curAngle = mUniversalJointSenseMap[JID_LARM_1_2].angle2;
            if (curAngle < 90.0)
            {
                newAngle = gain * (90.0 - curAngle);
                ss << "(larm_eff_1_2 0.0 " << newAngle << ")";
            }
            else
            {
                leftstate = ARM_ROTATE;
            }
            break;
        case ARM_ROTATE:
            curAngle = mHingeJointSenseMap[JID_LARM_3].angle;
            //cout << curAngle << endl;
            
            if (curAngle < 90.0)
            {
                newAngle = gain * (90.0 - curAngle);
                ss << "(larm_eff_3 " << newAngle << ")";
            }
            else    
            {
                leftstate = ARM_WAVE_1;
            }
            break;
        case ARM_WAVE_1:
            curAngle = mHingeJointSenseMap[JID_LARM_4].angle;
            if (curAngle < 90.0)
            {
                newAngle = gain * (90.0 - curAngle);
                ss << "(larm_eff_4 " << newAngle << ")"; 
            }
            else
            {
                leftstate = ARM_WAVE_2;
            }
            break;
        case ARM_WAVE_2:
            curAngle = mHingeJointSenseMap[JID_LARM_4].angle;
            if (curAngle > 45.0 || curAngle <= 44.5)
            {
                newAngle = gain * (45.0 - curAngle);
                ss << "(larm_eff_4 " << newAngle << ")";
            }
            else
            {
                leftstate = ARM_WAVE_1;
            }
            break;
        default:   
            break;
    }

#endif

//    cout << "+++" << endl;
//    cout << "current angle: " << curAngle  << endl;
//    cout << "desired angle: " << newAngle  << endl;
//    cout << "(Behavior) sending string " << ss.str() << " to server" << endl;
//    cout << "State is " << state << endl;
//    cout << "---" << endl;

    return ss.str();
    //return string("");
}
