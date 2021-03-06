#pragma once
#include <map>
#include <string>
#include <vector>
#include <Windows.h>
#include <iostream>

#include "util.h"
#include "json.hpp"
#include "Skeleton.h"
#include "AbstractGestureRecognizer.h"

#include <vrpn_Tracker.h>
#include <vrpn_Analog.h>
#include <vrpn_Button.h>


using json = nlohmann::json;

using namespace std::placeholders;

static const int SSIZE = 512;

const int VK_MOUSEMOVE				= 4000;
const int VK_LBUTTON_DOWN			= 4001;
const int VK_LBUTTON_UP				= 4002;
const int VK_RBUTTON_DOWN			= 4003;
const int VK_RBUTTON_UP				= 4004;
const int VK_MBUTTON_DOWN			= 4005;
const int VK_MBUTTON_UP				= 4006;

const int KINECT_STEP_UP			= 5001;
const int KINECT_STEP_DOWN			= 5002;
const int KINECT_STEP_NORMAL		= 5003;
const int KINECT_LEFT_HAND_TOP		= 5004;
const int KINECT_RIGHT_HAND_TOP		= 5005;
const int KINECT_LEFT_HAND_FAST		= 5006;
const int KINECT_RIGHT_HAND_FAST	= 5007;

const int KINECT_BODY_FRONT			= 5008;
const int KINECT_BODY_BACK			= 5009;
const int KINECT_BODY_RIGHT			= 5010;
const int KINECT_BODY_LEFT			= 5011;
const int KINECT_WALK				= 5012;

const int KINECT_TURN_RIGHT			= 5013;
const int KINECT_TURN_LEFT			= 5014;

const int KINECT_RIGHT_WRIST_UP		= 5015;
const int KINECT_RIGHT_WRIST_DOWN	= 5016;

const int KINECT_LEFT_WRIST_UP		= 5017;
const int KINECT_LEFT_WRIST_DOWN	= 5018;

const int KINECT_BALANCE_LEFT		= 5019;
const int KINECT_BALANCE_RIGHT		= 5020;
const int KINECT_SET_CENTER_POS		= 5021;

//kinect v2
const int KINECT_LEFT_HAND_CLOSED	= 5031;
const int KINECT_RIGHT_HAND_CLOSED	= 5032;
const int KINECT_LEFT_HAND_LASSO	= 5033;
const int KINECT_RIGHT_HAND_LASSO	= 5034;


const int JOINT_ANGLE				= 5600;


const int MESSAGE					= 6000;
const int ALERT						= 6001;


const int LEAP_CLOSED				= 7001;
const int LEAP_PINCH				= 7002;
const int LEAP_WRIST_UP				= 7003;
const int LEAP_WRIST_DOWN			= 7004;


const int NEDGLOVE_GRAB				= 7500;
const int NEDGLOVE_PINCH			= 7501;




const int TRACK_TYPE	= 1;
const int BUTTON_TYPE	= 2;
const int ANALOG_TYPE	= 3;


const int DEVTYPE_KINECT		= 0;
const int DEVTYPE_LEAPMOTION	= 1;
const int DEVTYPE_KEYBOARD		= 2;
const int DEVTYPE_MOUSE			= 3;
const int DEVTYPE_NEDGLOVE		= 4;
const int DEVTYPE_KINECT_V2		= 5;

const std::vector<std::string> DEVTYPE_STR = { "KINECT", "LEAP MOTION", "TECLADO", "MOUSE", "NEDGLOVE", "KINECT_V2" };


class KeyMap {
private:
	//Dispositivo que ser� lido
	std::string dev;//endereco
	int idDevType;//id
	std::string devTypeStr;//nome do dispositivo
	//Representa��o visual
	std::string keyRepr;
	std::string toKeyRepr;
	//comando que ser� convertido
	int key;
	//comando que ser� chamado
	int toKey;

	//Mensagem que ser� exibida
	std::string msg;

#ifdef ARDUINO_MODULE
	std::string sensors[3];
	int engine;

	int COM;
	int bauds;
#endif
	//Sera usado para enviar informacao de angulo para o arduino
	int angle;
	bool yAxis;

	//Caso o comando que ser� chamado seja uma constante
	bool toKeyIsConstant = false;

	//Caso seja para imprimir alguma informa��o no console
	bool print = false;

	bool isBtn = false;

	//O InputConverter tenta verificar primeiro os comandos que estao acionados
	//essa propriedade serve para n�o verificar duas vezes na mesma chamada
	bool verified = false;

	//Sera verdadeiro caso a tecla esteja acionada e esperando o desacionamento
	bool active = false;

	bool toKeyDown	= false;
	bool toKeyUp	= false;
	bool toKeyWhile = false;

	
	//Caso possua um evento para desacionar uma tecla
	//bool hasOnLeave = false;

	//Sinaliza que esse � o evento de desacionar
	bool keyUpEvent = false;

	//Guarda o evento para desacionar
	KeyMap *onLeave;

	int mouseX = -100;
	int mouseY = -100;

	//Possiveis propriedades
	int x = -100;
	int y = -100;
	//Como o x e y ser�o comparados
	int coordinateMod = 0;// 0 =, 1 >, -1 <

	//Medida em Metros por segundo para identificar um movimento brusco
	//Valor padrao, pode ser alterado nas configura��es
	double maxVelocityMs = 4;

	//NEDGlove e LeapMotion
	//strength � pra NEDGLOVE
	int strengthMin = 0;
	int strengthMax = 0;
	//Angle e distance min e max � para leapmotion
	float angleMin = 0;
	float angleMax = 0;
	float distanceMin = 0;
	float distanceMax = 0;
	int thumb = -1;
	int index = -1;
	
	//intervalo das linhas verticais para posicao da mao
	double handWidthInterval = 0.40;
	

	//sensibilidade do comando (para mudanca de altura)
	//step e marcha estacionaria
	double sensivity = 0.15;

	//delay atualmente usado somente para marcha estacionaria
	int delay = 400; //ms;

	std::string saveData = "";
	
	void setDevType(std::string devType);
	

	public:
	std::string getDevTypeStr() {
		return devTypeStr;
	}

	void setIdDevType(int type) {
		idDevType = type;
	}

	int getIdDevType() {
		return idDevType;
	}

	std::string getSaveData() {
		return saveData;
	}

	int scan2ascii(DWORD scancode, USHORT* result);

	std::string getKeyRepr() {
		return keyRepr;
	}

	std::string getToKeyRepr() {
		return toKeyRepr;
	}

	std::string toString();

	int getX() {
		return x;
	}

	int getY() {
		return y;
	}

	int getMouseX() {
		return mouseX;
	}

	int getMouseY() {
		return mouseY;
	}
	
	double getHandWidthInterval() {
		return handWidthInterval;
	}

	int getKey() {
		return key;
	}

	int getToKey() {
		return toKey;
	}

	std::string getDev() {
		return dev;
	}

	bool getToKeyIsConstant() {
		return toKeyIsConstant;
	}

	bool getIsBtn() {
		return isBtn;
	}

#ifdef ARDUINO_MODULE
	std::string getSensors(int num) {
		return sensors[num];
	}
	int getEngine() {
		return engine;
	}


	int getCOM() {
		return COM;
	}
	int getBauds() {
		return bauds;
	}
#endif

	int getAngle() {
		return angle;
	}
	int setAngle(int angle) {
		return this->angle = angle;
	}

	bool getXaxis() {
		return !yAxis;
	}

	bool getYaxis() {
		return yAxis;
	}


	/*bool getBtnDown() {
		return btnDown;
	}

	bool getBtnUp() {
		return btnUp;
	}*/

	bool getToKeyDown() {
		return toKeyDown;
	}

	bool getToKeyUp() {
		return toKeyUp;
	}

	bool getToKeyWhile() {
		return toKeyWhile;
	}

	bool getVerified() {
		return verified;
	}

	void setVerified(bool verif) {
		verified = verif;
	}

	/*bool getHasOnLeave() {
		return hasOnLeave;
	}*/

	bool isKeyUpEvent() {
		return keyUpEvent;
	}

	KeyMap* getOnLeave() {
		return onLeave;
	}

	bool getActive() {
		return active;
	}

	void setActive(bool wait) {
		active = wait;
	}

	int getCoordinateMod() {
		return coordinateMod;
	}

	double getSensivity() {
		return sensivity;
	}

	int getDelay() {
		return delay;
	}

	double getMaxVelocityMs() {
		return maxVelocityMs;
	}

	std::string getMsg() {
		return msg;
	}

	

	int getStrengthMax() {
		return strengthMax;
	}

	int getStrengthMin() {
		return strengthMin;
	}

	float getAngleMax() {
		return angleMax;
	}

	float getAngleMin() {
		return angleMin;
	}

	float getDistanceMax() {
		return distanceMax;
	}

	float getDistanceMin() {
		return distanceMin;
	}


	int getThumb() {
		return thumb;
	}

	int getIndex() {
		return index;
	}

	bool getPrint() {
		return print;
	}


	KeyMap(){}
	KeyMap(std::string dev, std::string toKeyUp);
	KeyMap(json config);

	void setToKey(std::string toKeyFind);
	void KeyMap::setKey(std::string key);


	static  std::map<std::string, int> configToCreatedConstants;
	static  std::map<std::string, int> configToAscii;
	static  std::map<std::string, int> configToScanCode;


	static std::map<std::string, int> create_configToCreatedConstants() {
		//Caso o mapa ja tenha sido criado, nao cria novamente
		if ( configToCreatedConstants.size() > 0 ) {
			return configToCreatedConstants;
		}

		std::map < std::string, int > m;

		m["VK_LBUTTON_DOWN"]		= VK_LBUTTON_DOWN;
		m["VK_LBUTTON_UP"]			= VK_LBUTTON_UP;
		m["VK_RBUTTON_DOWN"]		= VK_RBUTTON_DOWN;
		m["VK_RBUTTON_UP"]			= VK_RBUTTON_UP;
		m["VK_MBUTTON_DOWN"]		= VK_MBUTTON_DOWN;
		m["VK_MBUTTON_UP"]			= VK_MBUTTON_UP;
		m["VK_MOUSEMOVE"]			= VK_MOUSEMOVE;
		m["KINECT_SET_CENTER_POS"]	= KINECT_SET_CENTER_POS;
		

		return m;
	}

	
	static std::map<std::string, int> create_configToAscii() {
		//Caso o mapa ja tenha sido criado, nao cria novamente
		if ( configToAscii.size() > 0 ) {
			return configToAscii;
		}


		std::map < std::string, int > m;

		m["VK_ESCAPE"]			= VK_ESCAPE;
		m["VK_UP"]				= VK_UP;
		m["VK_DOWN"]			= VK_DOWN;
		m["VK_LEFT"]			= VK_LEFT;
		m["VK_RIGHT"]			= VK_RIGHT;
		m["VK_BACK"]			= VK_BACK;
		m["VK_TAB"]				= VK_TAB;
		m["VK_RETURN"]			= VK_RETURN;
		m["VK_CONTROL"]			= VK_CONTROL;

		m["VK_LSHIFT"]			= VK_LSHIFT;
		m["VK_RSHIFT"]			= VK_RSHIFT;
		m["VK_MENU"]			= VK_MENU;
		m["VK_SPACE"]			= VK_SPACE;
		m["VK_CAPITAL"]			= VK_CAPITAL;
		m["VK_F1"]				= VK_F1;
		m["VK_F2"]				= VK_F2;
		m["VK_F3"]				= VK_F3;
		m["VK_F4"]				= VK_F4;
		m["VK_F5"]				= VK_F5;
		m["VK_F6"]				= VK_F6;
		m["VK_F7"]				= VK_F7;
		m["VK_F8"]				= VK_F8;
		m["VK_F9"]				= VK_F9;
		m["VK_F10"]				= VK_F10;
		m["VK_F11"]				= VK_F11;
		m["VK_F12"]				= VK_F12;
		m["VK_NUMLOCK"]			= VK_NUMLOCK;
		m["VK_SCROLL"]			= VK_SCROLL;
		m["VK_HOME"]			= VK_HOME;
		m["VK_UP"]				= VK_UP;
		m["VK_PRIOR"]			= VK_PRIOR;
		m["VK_LEFT"]			= VK_LEFT;
		m["VK_CENTER"]			= 5;
		m["VK_RIGHT"]			= VK_RIGHT;
		m["VK_END"]				= VK_END;
		m["VK_DOWN"]			= VK_DOWN;
		m["VK_NEXT"]			= VK_NEXT;
		m["VK_INSERT"]			= VK_INSERT;
		m["VK_DELETE"]			= VK_DELETE;
		m["VK_SNAPSHOT"]		= VK_SNAPSHOT;
		m["VK_LWIN"]			= VK_LWIN;
		m["VK_RWIN"]			= VK_RWIN;
		m["VK_RMENU"]			= VK_RMENU;
		m["VK_LAUNCH_MAIL"]		= VK_LAUNCH_MAIL;


		m["VK_LBUTTON"] = VK_LBUTTON;
		m["VK_MBUTTON"] = VK_MBUTTON;
		m["VK_RBUTTON"] = VK_RBUTTON;
		
		
		return m;
	}

	static std::map<std::string, int> create_configToScanCode() {

		if ( configToScanCode.size() > 0 ) {
			return configToScanCode;
		}
		std::map < std::string, int > m;


		//MOUSE
		m["VK_LBUTTON"] = 0;
		m["VK_MBUTTON"] = 1;
		m["VK_RBUTTON"] = 2;

		//Kinect
		m["KINECT_STEP_UP"]				= KINECT_STEP_UP;
		m["KINECT_STEP_DOWN"]			= KINECT_STEP_DOWN;
		m["KINECT_STEP_NORMAL"]			= KINECT_STEP_NORMAL;
		m["KINECT_LEFT_HAND_TOP"]		= KINECT_LEFT_HAND_TOP;
		m["KINECT_RIGHT_HAND_TOP"]		= KINECT_RIGHT_HAND_TOP;
		m["KINECT_LEFT_HAND_FAST"]		= KINECT_LEFT_HAND_FAST;
		m["KINECT_RIGHT_HAND_FAST"]		= KINECT_RIGHT_HAND_FAST;

		m["KINECT_BODY_FRONT"]			= KINECT_BODY_FRONT;
		m["KINECT_BODY_BACK"]			= KINECT_BODY_BACK;
		m["KINECT_BODY_RIGHT"]			= KINECT_BODY_RIGHT;
		m["KINECT_BODY_LEFT"]			= KINECT_BODY_LEFT;
		m["KINECT_WALK"]				= KINECT_WALK;
		m["KINECT_TURN_RIGHT"]			= KINECT_TURN_RIGHT;
		m["KINECT_TURN_LEFT"]			= KINECT_TURN_LEFT;

		m["KINECT_RIGHT_WRIST_UP"]		= KINECT_RIGHT_WRIST_UP;
		m["KINECT_RIGHT_WRIST_DOWN"]	= KINECT_RIGHT_WRIST_DOWN;
		m["KINECT_LEFT_WRIST_UP"]		= KINECT_LEFT_WRIST_UP;
		m["KINECT_LEFT_WRIST_DOWN"]		= KINECT_LEFT_WRIST_DOWN;

		m["KINECT_BALANCE_LEFT"]		= KINECT_BALANCE_LEFT;
		m["KINECT_BALANCE_RIGHT"]		= KINECT_BALANCE_RIGHT;
		m["KINECT_SET_CENTER_POS"]		= KINECT_SET_CENTER_POS;

		m["KINECT_LEFT_HAND_CLOSED"]	= KINECT_LEFT_HAND_CLOSED;
		m["KINECT_RIGHT_HAND_CLOSED"]	= KINECT_RIGHT_HAND_CLOSED;
		m["KINECT_LEFT_HAND_LASSO"]		= KINECT_LEFT_HAND_LASSO;
		m["KINECT_RIGHT_HAND_LASSO"] = KINECT_RIGHT_HAND_LASSO;



		m["JOINT_ANGLE"]				= JOINT_ANGLE;
		

		//Leap Motion
		m["LEAP_CLOSED"]		= LEAP_CLOSED;
		m["LEAP_PINCH"]			= LEAP_PINCH;
		m["LEAP_WRIST_UP"]		= LEAP_WRIST_UP;
		m["LEAP_WRIST_DOWN"]	= LEAP_WRIST_DOWN;

		//NEDGlove
		m["NEDGLOVE_GRAB"]		= NEDGLOVE_GRAB;
		m["NEDGLOVE_PINCH"]		= NEDGLOVE_PINCH;



		//Mapeamento para ABNT2
		m["VK_ESCAPE"]	= 1;
		m["1"]			= 2;
		m["2"]			= 3;
		m["3"]			= 4;
		m["4"]			= 5;
		m["5"]			= 6;
		m["6"]			= 7;
		m["7"]			= 8;
		m["8"]			= 9;
		m["9"]			= 10;
		m["0"]			= 11;
		m["_"]			= 12;//BOTAO 12 E o unico que nao compreende o caracter natural, tem que usar _ em vez de -
		m["="]			= 13;
		m["VK_BACK"]	= 14;//CONFIRMAR SE E BACKSPACE
		m["VK_TAB"]		= 15;
		m["Q"]			= 16;
		m["W"]			= 17;
		m["E"]			= 18;
		m["R"]			= 19;
		m["T"]			= 20;
		m["Y"]			= 21;
		m["U"]			= 22;
		m["I"]			= 23;
		m["O"]			= 24;
		m["P"]			= 25;
		m["�"]			= 26;
		m["["]			= 27;
		m["VK_RETURN"]	= 28;//CONFIRMAR SE E ENTER
		m["VK_CONTROL"] = 29;
		m["A"]			= 30;
		m["S"]			= 31;
		m["D"]			= 32;
		m["F"]			= 33;
		m["G"]			= 34;
		m["H"]			= 35;
		m["J"]			= 36;
		m["K"]			= 37;
		m["L"]			= 38;
		m["�"]			= 39;
		m["~"]			= 40;
		m["'"]			= 41;
		m["VK_LSHIFT"]	= 42;
		m["]"]			= 43;
		m["Z"]			= 44;
		m["X"]			= 45;
		m["C"]			= 46;
		m["V"]			= 47;
		m["B"]			= 48;
		m["N"]			= 49;
		m["M"]			= 50;
		m[","]			= 51;
		m["."]			= 52;
		m[";"]			= 53;
		m["VK_RSHIFT"]	= 54;
		m["*"]			= 55;
		m["VK_MENU"]	= 56;//ALT
		m["VK_SPACE"]	= 57;
		m["VK_CAPITAL"] = 58;//CAPS

		m["VK_F1"]		= 59;
		m["VK_F2"]		= 60;
		m["VK_F3"]		= 61;
		m["VK_F4"]		= 62;
		m["VK_F5"]		= 63;
		m["VK_F6"]		= 64;
		m["VK_F7"]		= 65;
		m["VK_F8"]		= 66;
		m["VK_F9"]		= 67;
		m["VK_F10"]		= 68;
		
		m["VK_NUMLOCK"] = 69;
		m["VK_SCROLL"]	= 70;
		m["VK_HOME"]	= 71;
		m["VK_UP"]		= 72;
		m["VK_PRIOR"]	= 73;
		m["-"]			= 74;
		m["VK_LEFT"]	= 75;
		m["VK_CENTER"]	= 76; //[VK_CENTER] nao existe
		m["VK_RIGHT"]	= 77;
		m["+"]			= 78;
		m["VK_END"]		= 79;
		m["VK_DOWN"]	= 80;
		m["VK_NEXT"]	= 81;
		m["VK_INSERT"]	= 82;
		m["VK_DELETE"]	= 83;

		m["VK_SNAPSHOT"] = 84;
		m["\\"]			 = 86;

		m["VK_F11"]		= 87;
		m["VK_F12"]		= 88;
		m["VK_LWIN"]	= 91;
		m["VK_RWIN"]	= 92;
		m["VK_RMENU"]	= 93;//tecla menu do teclado numerico

		m["VK_LAUNCH_MAIL"] = 108;
		m["/"]				= 115;
		m["."]				= 126;

		return m;
	}
	

};

