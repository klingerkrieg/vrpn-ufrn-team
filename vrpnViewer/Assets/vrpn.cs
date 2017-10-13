﻿using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using System.Runtime.InteropServices;
using System;


public class vrpn : MonoBehaviour {

	public bool kinect;//caso leapmotion setar false;
	
	int channels;
	int centerChannel;
	float size;
	float cyWidth;


	Vector3 sum = new Vector3 ();
	Vector3 lastSum = new Vector3 (0,0,0);
	bool freezed = false; 
	int lastFreezeUpdate = -1;
	bool unfreezing =  true;
	bool unfreezingRoutine = false;

	List<Bone> bones = new List<Bone> ();


	List<GameObject> spheres = new List<GameObject>();

	[DllImport ("unityVrpn")]
	private static extern double vrpnTrackerExtern(string address, int channel, int component, int frameCount);

	public static Vector3 vrpnTrackerPos(string address, int channel){
		return new Vector3(
			(float) vrpnTrackerExtern(address, channel, 0, Time.frameCount),
			(float) vrpnTrackerExtern(address, channel, 1, Time.frameCount),
			(float) vrpnTrackerExtern(address, channel, 2, Time.frameCount));
	}

	GameObject CreateCylinderBetweenPoints(Vector3 start, Vector3 end, float width) {
		GameObject cylinder = GameObject.CreatePrimitive (PrimitiveType.Cylinder);
		alignCylinderBetweenPoints (cylinder, start, end, width);
		return cylinder;
	}

	void alignCylinderBetweenPoints(GameObject cylinder,Vector3 start, Vector3 end, float width){
		Vector3 offset = end - start;
		Vector3 scale = new Vector3(width, (float)(offset.magnitude / 2.0), width);
		Vector3 position = start + new Vector3((float)(offset.x / 2.0), (float)(offset.y / 2.0), (float)(offset.z / 2.0));

		//Instantiate(Cylin, position, Quaternion.identity);
		cylinder.transform.position = position;
		cylinder.transform.up = offset;
		cylinder.transform.localScale = scale;

	}


	public IEnumerator moveCylinderOverTime(GameObject objectToMove, Vector3 end, Vector3 up, float seconds){

		float elapsedTime = 0;
		Vector3 startingPos = objectToMove.transform.position;
		Vector3 startingUpPos = objectToMove.transform.up;

		while (elapsedTime < seconds) {
			Vector3 newStart = Vector3.Lerp (startingPos, end, (elapsedTime / seconds));
			Vector3 newEnd = Vector3.Lerp (startingUpPos, up, (elapsedTime / seconds));

			alignCylinderBetweenPoints (objectToMove, newStart, newEnd, cyWidth);

			elapsedTime += Time.deltaTime;
			yield return new WaitForEndOfFrame();
		}

		objectToMove.transform.position = end;
		unfreezingRoutine = false;

	}


	public IEnumerator moveObjectOverTime(GameObject objectToMove, Vector3 end, float seconds){

		float elapsedTime = 0;
		Vector3 startingPos = objectToMove.transform.position;

		while (elapsedTime < seconds) {
			objectToMove.transform.position = Vector3.Lerp (startingPos, end, (elapsedTime / seconds));
			elapsedTime += Time.deltaTime;
			yield return new WaitForEndOfFrame();
		}

		objectToMove.transform.position = end;
		unfreezingRoutine = false;

	}


	// Use this for initialization
	void Start () {


		if (kinect) {
			//kinect
			centerChannel = 2;
			channels = 20;
			size = 0.15f;
			cyWidth = 0.06f;
			Camera.main.transform.position = new Vector3 (0f, 0f, -0.5f);

			//Kinect bones
			bones.Add (new Bone(0,1));
			bones.Add (new Bone(1,2));
			bones.Add (new Bone(2,3));
			//left arm
			bones.Add (new Bone(1,8));
			bones.Add (new Bone(8,9));
			bones.Add (new Bone(9,10));
			bones.Add (new Bone(10, 11));
			//right arm
			bones.Add (new Bone(1,4));
			bones.Add (new Bone(4,5));
			bones.Add (new Bone(5,6));
			bones.Add (new Bone(6, 7));
			//left leg
			bones.Add (new Bone(3,16));
			bones.Add (new Bone(16,17));
			bones.Add (new Bone(17,18));
			bones.Add (new Bone(18, 19));
			//right leg
			bones.Add (new Bone(3,12));
			bones.Add (new Bone(12,13));
			bones.Add (new Bone(13,14));
			bones.Add (new Bone(14, 15));

		} else {

			//Leap bones
			//mao 1
			bones.Add (new Bone(0,2));
			bones.Add (new Bone(1,2));
			//polegar
			bones.Add (new Bone(2,3));
			bones.Add (new Bone(3,4));
			bones.Add (new Bone(4,5));
			bones.Add (new Bone(5,6));
			//indicador
			bones.Add (new Bone(2,7));
			bones.Add (new Bone(7,8));
			bones.Add (new Bone(8,9));
			bones.Add (new Bone(9,10));
			//meio
			bones.Add (new Bone(2,11));
			bones.Add (new Bone(11,12));
			bones.Add (new Bone(12,13));
			bones.Add (new Bone(13,14));
			//medio
			bones.Add (new Bone(2,15));
			bones.Add (new Bone(15,16));
			bones.Add (new Bone(16,17));
			bones.Add (new Bone(17,18));
			//mindinho
			bones.Add (new Bone(2,19));
			bones.Add (new Bone(19,20));
			bones.Add (new Bone(20,21));
			bones.Add (new Bone(21,22));

			//Mao 2
			bones.Add (new Bone(23,25));
			bones.Add (new Bone(24,25));
			//polegar
			bones.Add (new Bone(25,26));
			bones.Add (new Bone(26,27));
			bones.Add (new Bone(27,28));
			bones.Add (new Bone(28,29));
			//indicador
			bones.Add (new Bone(25,30));
			bones.Add (new Bone(30,31));
			bones.Add (new Bone(31,32));
			bones.Add (new Bone(32,33));
			//meio
			bones.Add (new Bone(25,34));
			bones.Add (new Bone(34,35));
			bones.Add (new Bone(35,36));
			bones.Add (new Bone(36,37));
			//medio
			bones.Add (new Bone(25,38));
			bones.Add (new Bone(38,39));
			bones.Add (new Bone(39,40));
			bones.Add (new Bone(40,41));
			//mindinho
			bones.Add (new Bone(25,42));
			bones.Add (new Bone(42,43));
			bones.Add (new Bone(43,44));
			bones.Add (new Bone(44,45));

			//leapmotion
			centerChannel = 0;
			channels = 46;
			size = 15f;
			cyWidth = 5f;
			Camera.main.transform.position = new Vector3 (0f, 15f, -349.2f);
		}

		Material newMat = Resources.Load("esfera", typeof(Material)) as Material;

		//Create bones
		foreach (Bone bone in bones) {
			Vector3 start = vrpnTrackerPos ("Tracker0@localhost", bone.sensStart);
			Vector3 end = vrpnTrackerPos ("Tracker0@localhost", bone.sensEnd);
			bone.cylinder = CreateCylinderBetweenPoints (start,end,cyWidth);
		}

		for (int i = 0; i < channels; i++) {
			GameObject sphere = GameObject.CreatePrimitive (PrimitiveType.Sphere);
			sphere.name = i.ToString();
			sphere.transform.localScale = new Vector3 (size,size,size);
			MeshRenderer mesh = sphere.GetComponent<MeshRenderer>();
			mesh.material = newMat;

			spheres.Add (sphere);
		}


	}

	void OnGUI(){
		GUI.color = Color.red;
		Vector3 screenPos;

		//Caso esteja montando
		if (unfreezingRoutine) {
			return;
		}

		foreach (Bone bone in bones) {
			if (freezed == false) {
				Vector3 start = vrpnTrackerPos ("Tracker0@localhost", bone.sensStart);
				Vector3 end = vrpnTrackerPos ("Tracker0@localhost", bone.sensEnd);

				if (unfreezing) {
					//se estiver montando fara isso em tempo nao de uma unica vez
					StartCoroutine(moveCylinderOverTime(bone.cylinder, start, end, 0.5f));
					unfreezingRoutine = true;
				} else {
					alignCylinderBetweenPoints (bone.cylinder, start, end, cyWidth);
				}

				Destroy(bone.cylinder.GetComponent<Rigidbody>());
			} else {
				if (bone.cylinder.GetComponent<Rigidbody>() == null)
					bone.cylinder.AddComponent<Rigidbody>();
			}
		}

		//Contator para verificar se parou de receber dados
		sum = new Vector3 (0, 0, 0);

		int i = 0;
		GameObject central = null;
		foreach (GameObject sphere in spheres){
			
			Vector3 pos = vrpnTrackerPos ("Tracker0@localhost", i++);
			sum += pos;

			if (freezed == false) {
				Destroy(sphere.GetComponent<Rigidbody>());

				if (unfreezing) {
					StartCoroutine(moveObjectOverTime(sphere, pos, 0.5f));
					unfreezingRoutine = true;
				} else {
					sphere.transform.position = pos;
				}

			} else {
				if (sphere.GetComponent<Rigidbody>() == null)
					sphere.AddComponent<Rigidbody>();
			}


			screenPos = Camera.main.WorldToScreenPoint(sphere.transform.position);
			//nao sei porque o x se comporta corretamente e o y comeca de baixo pra cima
			GUI.Label (new Rect(screenPos.x, Camera.main.pixelHeight - screenPos.y,20,20), sphere.name);

			if (i == centerChannel)
				central = sphere;
		}

		//se estava descongelando
		if (unfreezing) {
			//nao esta mais
			unfreezing = false;
		}

		//Verifica se parou de receber do kinect
		System.DateTime epochStart = new System.DateTime(1970, 1, 1, 0, 0, 0, System.DateTimeKind.Utc);
		int now = (int)(System.DateTime.UtcNow - epochStart).TotalSeconds;

		if (lastFreezeUpdate == -1 || now - lastFreezeUpdate > 0.5f) {
			lastFreezeUpdate = now;
			if (sum == lastSum) {
				freezed = true;
			} else {
				if (freezed)
					unfreezing = true;
				freezed = false;
			}
			lastSum = sum;

		}



		if (freezed == false) {
			if (central != null)
				Camera.main.transform.LookAt (central.transform);
		}


	}
	
	// Update is called once per frame
	void Update () {
		

	}
}
