/* -*- mode: c++; c-basic-offset: 4; indent-tabs-mode: nil -*-

   this file is part of rcssserver3D
   Fri May 9 2003
   Copyright (C) 2002,2003 Koblenz University
   Copyright (C) 2003 RoboCup Soccer Server 3D Maintenance Group
   $Id: gamecontrolserver.cpp,v 1.1.2.11 2003/12/21 10:21:55 rollmark Exp $

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
#include <sstream>
#include "gamecontrolserver.h"
#include "baseparser.h"
#include <oxygen/agentaspect/agentaspect.h>
#include <zeitgeist/logserver/logserver.h>
#include <zeitgeist/scriptserver/scriptserver.h>
#include <zeitgeist/corecontext.h>

using namespace oxygen;
using namespace zeitgeist;
using namespace boost;
using namespace std;

GameControlServer::GameControlServer() : zeitgeist::Node()
{
}

GameControlServer::~GameControlServer()
{
}

bool
GameControlServer::InitParser(const std::string& parserName)
{
    mParser = shared_dynamic_cast<BaseParser>(GetCore()->New(parserName));

    if (mParser.get() == 0)
    {
        GetLog()->Error() << "ERROR: (GameControlServer::InitParser) Unable to create "
                          << parserName << std::endl;
        return false;
    }

    return true;
}

shared_ptr<BaseParser>
GameControlServer::GetParser()
{
    return mParser;
}


bool
GameControlServer::AgentConnect(int id)
{
    // for map::insert(Elem), the test here is not required. map::insert does not
    // overwrite existing elements. The test is required to report if the agent
    // was already connected or not.
    if (mAgentMap.find(id) != mAgentMap.end())
    {
        return false;
    }

    GetLog()->Debug()
        << "(GameControlServer) A new agent connected (id: " << id << ")\n";

    // create a new AgentAspect for the new ID in the scene
    stringstream name;
    name << "/usr/scene/_AgentAspect" << id;

    shared_ptr<AgentAspect> aspect = shared_dynamic_cast<AgentAspect>
        (GetCore()->GetScriptServer()->GetContext()->New("kerosin/AgentAspect",name.str()));

    if (aspect.get() == 0)
        {
            GetLog()->Error()
                << "ERROR: (GameControlServer) cannot create new AgentAspect\n";
            return false;
        }

    mAgentMap[id] = aspect;

    return aspect->Init();
}

bool GameControlServer::AgentDisappear(int id)
{
    TAgentMap::iterator iter = mAgentMap.find(id);

    if (iter == mAgentMap.end())
        {
            GetLog()->Error()
                << "ERROR: (GameControlServer) AgentDisappear called for unknown agent id "
                << id << "\n";
            return false;
        }

    // remove the AgentAspect from the set of children and from our
    // private map. The AgentAspect does all the necessary cleanup
    RemoveChildReference((*iter).second);
    mAgentMap.erase(id);

    GetLog()->Debug() << "(GameControlServer) An agent disconnected (id: " << id << ")\n";

    return true;
}

float GameControlServer::GetSenseInterval(int /*id*/)
{
    // the real thing should query the AgentAspect corresponding to
    // the agent.
    return 2;
}

float GameControlServer::GetSenseLatency(int /*id*/)
{
    // the real thing should query the AgentAspect corresponding to
    // the agent
    return 2;
}

shared_ptr<ActionObject::TList> GameControlServer::Parse(int id, string str) const
{
    TAgentMap::const_iterator iter = mAgentMap.find(id);

    if (iter == mAgentMap.end())
        {
            GetLog()->Error()
                << "ERROR: (GameControlServer::Parse) Parse called with unknown agent id "
                << id << "\n";
            return shared_ptr<ActionObject::TList>(new ActionObject::TList());
        }

    if (mParser.get() == 0)
        {
            GetLog()->Error()
                << "ERROR: (GameControlServer::Parse) No parser registered.\n";
            return shared_ptr<ActionObject::TList>(new ActionObject::TList());
        }

    // use the parser to create a TPredicateList
    shared_ptr<BaseParser::TPredicateList> predicates(mParser->Parse(str));

    // construct an ActionList using the registered effectors
    shared_ptr<ActionObject::TList> actionList(new ActionObject::TList());

    // update the map of effectors below the agentaspect
    shared_ptr<AgentAspect> aspect = (*iter).second;
    aspect->UpdateEffectorMap();

    for
        (
         BaseParser::TPredicateList::iterator iter = predicates->begin();
         iter != predicates->end();
         ++iter
        )
        {
            BaseParser::TPredicate& predicate = (*iter);

            shared_ptr<Effector> effector = aspect->GetEffector(predicate.name);
            if (effector.get() == 0)
                {
                    GetLog()->Warning()
                        << "(GameControlServer::Parse) No effector registered for predicate "
                        << predicate.name << "\n";
                    continue;
                }

            shared_ptr<ActionObject> action(effector->GetActionObject(predicate));

            if (action.get() == 0)
                {
                    continue;
                }

            actionList->push_back(action);
        }

    return actionList;
}

shared_ptr<AgentAspect> GameControlServer::GetAgentAspect(int id)
{
    TAgentMap::iterator iter = mAgentMap.find(id);
    if (iter == mAgentMap.end())
        {
            return shared_ptr<AgentAspect>();
        } else
            {
                return (*iter).second;
            }
}



std::string
GameControlServer::TmpGenerate(const BaseParser::TPredicate& pred)
{
    if (mParser != 0)
    {
        shared_ptr<BaseParser::TPredicateList>
            plist(new BaseParser::TPredicateList());
        plist->push_back(pred);
        return mParser->Generate(plist);
    }
    return string();
}
