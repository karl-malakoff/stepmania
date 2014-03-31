#include "global.h"
#include "ScreenMultiLobby.h"
#include "GameState.h"
#include "GameInput.h"
#include "RageLog.h"
#include "EnumHelper.h"
#include <vector>

REGISTER_SCREEN_CLASS( ScreenMultiLobby );

ScreenMultiLobby::ScreenMultiLobby()
{
	//this probably isn't where this should be put but will do for testing
	GAMESTATE->m_bMultiplayer = true;
}

ScreenMultiLobby::~ScreenMultiLobby()
{
	//really not sure if this is right...
	GAMESTATE->m_bMultiplayer = false;
}

void ScreenMultiLobby::Init()
{
	ScreenWithMenuElements::Init();
	info.LoadFromFont( THEME->GetPathF("ScreenMultiLobby", "text"));
	info.SetName("Info");
	info.SetShadowLength(0);
	info.SetHorizAlign( align_left);
	//LOAD_ALL_COMMANDS(info);
	//ON_COMMAND(info);
	this->AddChild(&info);
	info.SetX(50.0);
	info.SetY(200.0);

	active = "Joined multiplayers : ";
	info.SetText(active);
}

void ScreenMultiLobby::BeginScreen()
{
	ScreenWithMenuElements::BeginScreen();
}

void ScreenMultiLobby::Update( float fDeltaTime )
{
	ScreenWithMenuElements::Update( fDeltaTime );

	//check for joined players
	FOREACH_MultiPlayer( mp )
	{
		if(GAMESTATE->IsMultiPlayerEnabled(mp))
		{
			//LOG->Trace("MultiPlayer Enabled %d\n", mp);
		}
		
	}
}

//This is where I really need to do stuff, if a player does an input join them to the game
//First lets try and just detect what controllers are being pressed and display messages
bool ScreenMultiLobby::Input( const InputEventPlus &input )
{
	DeviceInput dev = input.DeviceI;
	int button = input.DeviceI.button;

	//Allow user to quit the game using the keyboard
	if(input.DeviceI.device == DEVICE_KEYBOARD)
	{
		switch(button)
		{
		case KEY_ESC:
			StartTransitioningScreen(SM_GoToNextScreen);
			return true;
			break;
		}
	}

	//Check for device input and activate players
	//Also need some way of exiting
	//LOG->Trace("#IN Input detected of type :");
	//LOG->Trace(InputDeviceToString(dev.device));
	if(dev.IsJoystick())
	{
		//work out which player the input belongs to
		//TODO double check this is the correct way to do this
		LOG->Trace("Joining Multiplayer %d", dev.device);
		MultiPlayer mp = (MultiPlayer)enum_add2(dev.device, -1);
		if(!GAMESTATE->IsMultiPlayerEnabled(mp))
		{
			GAMESTATE->JoinMultiPlayer(mp);
			active = active + " " + MultiPlayerToString(mp);
			info.SetText(active);
		}
	}
	
	return false;
}


/*
 * (c) 2014 Karl Malakoff
 * All rights reserved.
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the
 * "Software"), to deal in the Software without restriction, including
 * without limitation the rights to use, copy, modify, merge, publish,
 * distribute, and/or sell copies of the Software, and to permit persons to
 * whom the Software is furnished to do so, provided that the above
 * copyright notice(s) and this permission notice appear in all copies of
 * the Software and that both the above copyright notice(s) and this
 * permission notice appear in supporting documentation.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
 * OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT OF
 * THIRD PARTY RIGHTS. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR HOLDERS
 * INCLUDED IN THIS NOTICE BE LIABLE FOR ANY CLAIM, OR ANY SPECIAL INDIRECT
 * OR CONSEQUENTIAL DAMAGES, OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS
 * OF USE, DATA OR PROFITS, WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR
 * OTHER TORTIOUS ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE OR
 * PERFORMANCE OF THIS SOFTWARE.
 */
