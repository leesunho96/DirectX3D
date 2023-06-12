#pragma once

struct ModelKeyframeData
{
	float Time;

	Vector3 Scale;
	Quaternion Rotation;
	Vector3 Translation;
};

struct ModelKeyframe
{
	wstring BoneName;
	vector<ModelKeyframeData> Transforms;
};

class ModelClip
{
public:
	friend class Model;

private:
	ModelClip();
	~ModelClip();

public:
	float Duration() { return duration; }
	float FrameRate() { return frameRate; }
	UINT FrameCount() { return frameCount; }

	ModelKeyframe* Keyframe(wstring name);

private:
	wstring name;

	float duration;
	float frameRate;
	UINT frameCount;


	/*
	 * map : RB Tree : �˻� �ӵ� ����
	 * unordered_Tree : Hash : �ؽ� ��ȯ�� �Ͼ�� �ӵ��� �� ����.
	 */
	unordered_map<wstring, ModelKeyframe*> keyframeMap;
};