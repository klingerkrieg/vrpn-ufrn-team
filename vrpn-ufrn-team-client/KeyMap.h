#pragma once
#include <map>
#include <string>
#include <Windows.h>

static const int SSIZE = 512;

const int KINECT_TOP_ADD = 5001;
const int KINECT_TOP_DEC = 5002;


class KeyMap {
private:
	std::string dev;
	int key;
	char toKey;
	bool toKeyIsConstant = false;

	//sensibilidade para mudanca de altura
	double heightSens = 0.15;

	void init(std::string dev, char key[SSIZE], char toKey[SSIZE]);

public:
	int getKey() {
		return key;
	}

	char getToKey() {
		return toKey;
	}

	std::string getDev() {
		return dev;
	}

	bool getToKeyIsConstant() {
		return toKeyIsConstant;
	}

	double getHeightSens() {
		return heightSens;
	}

	KeyMap(){}
	KeyMap(std::string dev, int key, char toKey);
	KeyMap(std::string dev, char key[SSIZE], char toKey[SSIZE]);
	KeyMap(std::string dev, char key[SSIZE], char toKey[SSIZE], char config[SSIZE]);
	
	static std::map<std::string, int> create_map() {
		//Caso o mapa ja tenha sido criado, nao cria novamente
		if ( constantMap.size() > 0 ) {
			return constantMap;
		}

		std::map < std::string, int > m;
		m["WM_LBUTTONDOWN"] = WM_LBUTTONDOWN;
		m["WM_RBUTTONDOWN"] = WM_RBUTTONDOWN;
		m["WM_MBUTTONDOWN"] = WM_MBUTTONDOWN;
		m["VK_ESCAPE"]		= VK_ESCAPE;
		m["VK_RETURN"]		= VK_RETURN;
		m["VK_UP"]			= VK_UP;
		m["VK_DOWN"]		= VK_DOWN;
		m["VK_LEFT"]		= VK_LEFT;
		m["VK_RIGHT"]		= VK_RIGHT;
		m["KINECT_TOP_ADD"] = KINECT_TOP_ADD;
		m["KINECT_TOP_DEC"] = KINECT_TOP_DEC;
		
		return m;
	}
	static  std::map<std::string, int> constantMap;

};


std::map<std::string, int> KeyMap::constantMap = KeyMap::create_map();


KeyMap::KeyMap(std::string dev, int key, char toKey) {

	this->dev = dev;
	this->key = key;
	this->toKey = toKey;
}

KeyMap::KeyMap(std::string dev, char key[SSIZE], char toKey[SSIZE], char config[SSIZE]) {
	
	//Cria normalmente
	init(dev, key, toKey);
	//Caso seja top no kinect a configuracao extra sera a sensibilidade
	if ( this->key == KINECT_TOP_ADD || this->key == KINECT_TOP_DEC ) {
		heightSens = atof(config);
	}

}

KeyMap::KeyMap(std::string dev, char key[SSIZE], char toKey[SSIZE]) {
	init(dev,key,toKey);
}


void KeyMap::init(std::string dev, char key[SSIZE], char toKey[SSIZE]) {
	
	this->dev = dev;

	//Tenta localizar a constante em key
	std::map<std::string, int>::iterator it = KeyMap::constantMap.find(key);

	if ( it == KeyMap::constantMap.end() ) {
		//Quando sao enviados botoes via VRPN um inteiro sera esperado
		this->key = atoi(key);
	} else {
		//Entretanto com o Kinect espera-se uma constante
		this->key = it->second;
	}


	//Tenta localizar a constante para toKey
	it = KeyMap::constantMap.find(toKey);

	if ( it == KeyMap::constantMap.end() ) {
		//A saida sera um botao representado por um char quando uma constante nao for encontrada
		this->toKey = toKey[0];
	} else {
		this->toKey = it->second;
		this->toKeyIsConstant = true;
	}

}