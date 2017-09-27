#pragma once
#include <vector>
#include "KeyMap.h"
#include "DeviceInfo.h"
#include "GestureRecognizer.h"

#include "vrpn_Configure.h" // for VRPN_CALLBACK
#include "vrpn_Types.h"     // for vrpn_float64, vrpn_int32
#include "vrpn_BaseClass.h" // for vrpn_System_TextPrinter, etc
#include <vrpn_Tracker.h>        // for vrpn_TRACKERACCCB, etc
#include <vrpn_Analog.h>         // for vrpn_ANALOGCB, etc
#include <vrpn_Button.h>         // for vrpn_Button_Remote, etc
#include <Tchar.h>

class InputConverter {
private:
	 std::vector<KeyMap> map;
	 std::string app;

public:

	InputConverter() {

	}

	InputConverter(std::vector<KeyMap> map, std::string app) {
		this->map = map;
		this->app = app;
	}
	
	 void InputConverter::press(char key, bool isConstant);

	// void InputConverter::release(char key);

	 void InputConverter::checkTrack(TrackerUserCallback *userdata, const vrpn_TRACKERCB t);

	 void InputConverter::checkButton(const char * name, const vrpn_BUTTONCB b);

	 void InputConverter::checkAnalog();
	
};



void InputConverter::press(char key, bool isConstant) {
	printf("Press: %d ", key);
	if ( app != "" ) {

		HWND window = FindWindow(_T(app.c_str()), NULL);
		if ( window ) {
			HWND edit = FindWindowEx(window, NULL, _T("Edit"), NULL);
			printf(" em %s.\n", app.c_str());
			if ( isConstant ) {
				PostMessage(edit, WM_KEYDOWN, VK_LEFT, 0);
			} else {
				PostMessage(edit, WM_KEYDOWN, VkKeyScanEx(key, GetKeyboardLayout(0)), 0);
			}
			return;
		}

	}
	printf(" no Windows.\n");
	//Caso nenhum app tenha sido configurado ou encontrado lanca evento no windows
	keybd_event(key, 0, 0, 0);
}
/*
void InputConverter::release(char key) {
	printf("Release: %d\n", key);

	if ( app != "" ) {

		HWND window = FindWindow(_T(app.c_str()), NULL);
		HWND edit = FindWindowEx(window, NULL, _T("Edit"), NULL);

		if ( edit != NULL ) {
			printf(" em %s.\n", app.c_str());
			//PostMessage(edit, WM_KEYUP, VkKeyScanEx(key, GetKeyboardLayout(0)), 0);
			PostMessage(edit, WM_KEYUP,VK_LEFT, 0);
			return;
		}


	}
	printf(" no Windows.\n");
	keybd_event(key, 0, KEYEVENTF_KEYUP, 0);

}

*/
void InputConverter::checkTrack(TrackerUserCallback *userdata, const vrpn_TRACKERCB t) {
	
	int top = 0;
	bool topCalculated = false;

	for ( std::vector<KeyMap>::iterator keyMap = map.begin(); keyMap != map.end(); ++keyMap ) {
		
		//Caso seja reconhecimento de mudanca na altura ele calcula somente uma vez nesse metodo
		if ( topCalculated == false && (keyMap->getKey() == KINECT_TOP_ADD || keyMap->getKey() == KINECT_TOP_DEC) ) {
			top = GestureRecognizer::detectTopChange(userdata, t, keyMap->getHeightSens());
			topCalculated = true;
		}


		//se ja foi calculado durante esse reconhecimento nao calcula novamente para as demais configuracoes de teclas
		if ( keyMap->getKey() == KINECT_TOP_ADD && topCalculated == true && top == 1 ) {
			//Se houve uma mudanca para cima e isso e esperado
			press(keyMap->getToKey(), keyMap->getToKeyIsConstant());
		} else
		if ( keyMap->getKey() == KINECT_TOP_DEC && topCalculated == true && top == -1 ) {
			press(keyMap->getToKey(), keyMap->getToKeyIsConstant());
		}

	}
}

void InputConverter::checkButton(const char * name, const vrpn_BUTTONCB b) {

	for ( std::vector<KeyMap>::iterator keyMap = map.begin(); keyMap != map.end(); ++keyMap ) {
		//printf("%s == %s && %d == %d\n", name, keyMap->getDev().c_str(),  keyMap->getKey(), b.button);

		if ( !strcmp(name, keyMap->getDev().c_str()) && keyMap->getKey() == b.button ) {
			if ( b.state == 1 ) {
				press(keyMap->getToKey(), keyMap->getToKeyIsConstant());
			} else {
				//o release esta ocorrendo em duplicacao do evento
				//release(keyMap->toKey);
			}
		}
	}

}

void InputConverter::checkAnalog() {

}