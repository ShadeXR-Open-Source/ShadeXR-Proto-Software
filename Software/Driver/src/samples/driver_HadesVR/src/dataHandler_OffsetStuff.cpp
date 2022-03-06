#include "dataHandler.h"

// + (char)'W'
// 

void setCalibOffset(float DataW, float DataY, Quaternion &Offset, const char* settingsKey)
{
	std::string key_y = settingsKey;
	std::string key_w = settingsKey;

	Offset.W = DataW;
	Offset.Y = -DataY;
	
	key_w += "W";
	key_y += "Y";

	const char* keyW = key_w.c_str();
	const char* keyY = key_y.c_str();

	if (Offset.W == 0.f && Offset.Y == 0.f) {
		Offset.W = 1.f;
		Offset.Y = 0.f;
		vr::VRSettings()->SetFloat(k_pch_Calibration_Section, keyW, Offset.W);
		vr::VRSettings()->SetFloat(k_pch_Calibration_Section, keyY, Offset.Y);
	}
	else {
		vr::VRSettings()->SetFloat(k_pch_Calibration_Section, keyW, Offset.W);
		vr::VRSettings()->SetFloat(k_pch_Calibration_Section, keyY, Offset.Y);
	}
}

void CdataHandler::SetCentering()
{
	/// <summary>
	/// HMD
	/// </summary>

	setCalibOffset(HMDData.Rotation.W, HMDData.Rotation.Y, HMDOffset, k_pch_Calibration_HMD);

	/// <summary>
	///				Controllers
	/// </summary>

	setCalibOffset(RightCtrlData.Rotation.W, RightCtrlData.Rotation.Y, RightCtrlOffset, k_pch_Calibration_CONTRight);
	setCalibOffset(LeftCtrlData.Rotation.W, LeftCtrlData.Rotation.Y, LeftCtrlOffset, k_pch_Calibration_CONTLeft);

	/// <summary>
	///					Trackers
	/// </summary>

	setCalibOffset(TrackerWaistData.Rotation.W, TrackerWaistData.Rotation.Y, WaistTrackerOffset, k_pch_Calibration_TRKWaist);
	setCalibOffset(TrackerLeftData.Rotation.W, TrackerLeftData.Rotation.Y, LeftTrackerOffset, k_pch_Calibration_TRKLeft);
	setCalibOffset(TrackerRightData.Rotation.W, TrackerRightData.Rotation.Y, RightTrackerOffset, k_pch_Calibration_TRKRight);
}

void CdataHandler::ResetPos(bool hmdOnly) {
	if (!hmdOnly) {
		ctrlRightPosData.position = Vector3::Zero();
		ctrlRightPosData.oldPosition = Vector3::Zero();
		ctrlRightPosData.velocity = Vector3::Zero();
		ctrlRightPosData.oldVelocity = Vector3::Zero();

		ctrlLeftPosData.position = Vector3::Zero();
		ctrlLeftPosData.oldPosition = Vector3::Zero();
		ctrlLeftPosData.velocity = Vector3::Zero();
		ctrlLeftPosData.oldVelocity = Vector3::Zero();
	}
	hmdPosData.position = Vector3::Zero();
	hmdPosData.oldPosition = Vector3::Zero();
	hmdPosData.velocity = Vector3::Zero();
	hmdPosData.oldVelocity = Vector3::Zero();
}

Quaternion CdataHandler::SetOffsetQuat(Quaternion Input, Quaternion offsetQuat, Quaternion configOffset)
{
	if (offsetQuat.W == 0.f && offsetQuat.Y == 0.f) {  //Don't try to use an enpty offset quaternion 
		offsetQuat.W = 1.f;
		offsetQuat.Y = 0.f;
	}

	Quaternion inputCal = Quaternion::Normalized(offsetQuat * Input);

	Quaternion Output = Quaternion::Normalized(inputCal * configOffset);

	return Output;
}