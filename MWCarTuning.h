#pragma once
#ifndef MWCARTUNING_H
#define MWCARTUNING_H

#ifdef _MSC_VER
#pragma warning( push )
#pragma warning(disable: 4244) // conversion from 'const double' to 'T', possible loss of data
#endif

#include "framework.h"
#include <filesystem>
#include <toml++/toml.hpp>
//#include "decomp/SuspensionRacer.h"

class SuspensionRacer;


struct MWCarTuning {
	std::string carName;

	struct TempTable {
		float value;

		TempTable() {}
		TempTable(float f) { value = f; }
		float GetValue(float) { return value; }
	};

	struct TempAxlePairTable {
		TempTable Front;
		TempTable Rear;

		TempTable& At(int i) { return (&Front)[i]; }
	};

	// chassis
	TempTable AERO_CG;
	TempTable AERO_COEFFICIENT;
	float DRAG_COEFFICIENT;
	float FRONT_WEIGHT_BIAS;
	float RENDER_MOTION;
	AxlePair RIDE_HEIGHT;
	TempTable ROLL_CENTER;
	float SHOCK_BLOWOUT;
	AxlePair SHOCK_DIGRESSION;
	TempAxlePairTable SHOCK_EXT_STIFFNESS;
	TempAxlePairTable SHOCK_STIFFNESS;
	AxlePair SHOCK_VALVING;
	AxlePair SPRING_PROGRESSION;
	AxlePair SPRING_STIFFNESS;
	TempAxlePairTable SWAYBAR_STIFFNESS;
	AxlePair TRAVEL;

	// brakes
	AxlePair BRAKE_LOCK;
	TempAxlePairTable BRAKES;
	TempTable EBRAKE;

	// tires
	AxlePair DYNAMIC_GRIP;
	AxlePair GRIP_SCALE;
	TempAxlePairTable STATIC_GRIP;
	TempTable STEERING;
	std::vector<float> YAW_CONTROL;
	float YAW_SPEED;

	// transmission
	float DIFFERENTIAL[3];
	float TORQUE_SPLIT;

	// pvehicle
	float TENSOR_SCALE[3];
};
inline std::vector<MWCarTuning> aCarTunings;
inline MWCarTuning* LoadCarTuningFromFile(std::string configCarName) {
	DLLDirSetter _setdir;

	if (configCarName.ends_with(".conf")) {
		for (int i = 0; i < 5; i++) {
			configCarName.pop_back();
		}
	}

	auto fileName = std::format("CarDataDump/{}.conf", configCarName);
	if (!std::filesystem::exists(fileName)) return nullptr;
	auto config = toml::parse_file(fileName);

	MWCarTuning tmp;
	tmp.carName = configCarName;

	// brakes
	tmp.BRAKE_LOCK.Front = config["brakes"]["BRAKE_LOCK"][0].value_or(0.0);
	tmp.BRAKE_LOCK.Rear = config["brakes"]["BRAKE_LOCK"][1].value_or(0.0);
	tmp.BRAKES.Front = config["brakes"]["BRAKES"][0].value_or(0.0);
	tmp.BRAKES.Rear = config["brakes"]["BRAKES"][1].value_or(0.0);
	tmp.EBRAKE = config["brakes"]["EBRAKE"].value_or(0.0);

	// chassis
	tmp.AERO_CG = config["chassis"]["AERO_CG"].value_or(50.0);
	tmp.AERO_COEFFICIENT = config["chassis"]["AERO_COEFFICIENT"].value_or(0.0);
	tmp.DRAG_COEFFICIENT = config["chassis"]["DRAG_COEFFICIENT"].value_or(0.0);
	tmp.FRONT_WEIGHT_BIAS = config["chassis"]["FRONT_WEIGHT_BIAS"].value_or(50.0);
	tmp.RENDER_MOTION = config["chassis"]["RENDER_MOTION"].value_or(1.0);
	tmp.RIDE_HEIGHT.Front = config["chassis"]["RIDE_HEIGHT"][0].value_or(10.0);
	tmp.RIDE_HEIGHT.Rear = config["chassis"]["RIDE_HEIGHT"][1].value_or(10.0);
	tmp.ROLL_CENTER = config["chassis"]["ROLL_CENTER"].value_or(0.0);
	tmp.SHOCK_BLOWOUT = config["chassis"]["SHOCK_BLOWOUT"].value_or(0.0);
	tmp.SHOCK_DIGRESSION.Front = config["chassis"]["SHOCK_DIGRESSION"][0].value_or(0.0);
	tmp.SHOCK_DIGRESSION.Rear = config["chassis"]["SHOCK_DIGRESSION"][1].value_or(0.0);
	tmp.SHOCK_EXT_STIFFNESS.Front = config["chassis"]["SHOCK_EXT_STIFFNESS"][0].value_or(0.0);
	tmp.SHOCK_EXT_STIFFNESS.Rear = config["chassis"]["SHOCK_EXT_STIFFNESS"][1].value_or(0.0);
	tmp.SHOCK_STIFFNESS.Front = config["chassis"]["SHOCK_STIFFNESS"][0].value_or(0.0);
	tmp.SHOCK_STIFFNESS.Rear = config["chassis"]["SHOCK_STIFFNESS"][1].value_or(0.0);
	tmp.SHOCK_VALVING.Front = config["chassis"]["SHOCK_VALVING"][0].value_or(0.0);
	tmp.SHOCK_VALVING.Rear = config["chassis"]["SHOCK_VALVING"][1].value_or(0.0);
	tmp.SPRING_PROGRESSION.Front = config["chassis"]["SPRING_PROGRESSION"][0].value_or(0.0);
	tmp.SPRING_PROGRESSION.Rear = config["chassis"]["SPRING_PROGRESSION"][1].value_or(0.0);
	tmp.SPRING_STIFFNESS.Front = config["chassis"]["SPRING_STIFFNESS"][0].value_or(0.0);
	tmp.SPRING_STIFFNESS.Rear = config["chassis"]["SPRING_STIFFNESS"][1].value_or(0.0);
	tmp.SWAYBAR_STIFFNESS.Front = config["chassis"]["SWAYBAR_STIFFNESS"][0].value_or(0.0);
	tmp.SWAYBAR_STIFFNESS.Rear = config["chassis"]["SWAYBAR_STIFFNESS"][1].value_or(0.0);
	tmp.TRAVEL.Front = config["chassis"]["TRAVEL"][0].value_or(5.0);
	tmp.TRAVEL.Rear = config["chassis"]["TRAVEL"][1].value_or(5.0);

	// tires
	tmp.DYNAMIC_GRIP.Front = config["tires"]["DYNAMIC_GRIP"][0].value_or(1.0);
	tmp.DYNAMIC_GRIP.Rear = config["tires"]["DYNAMIC_GRIP"][1].value_or(1.0);
	tmp.GRIP_SCALE.Front = config["tires"]["GRIP_SCALE"][0].value_or(1.0);
	tmp.GRIP_SCALE.Rear = config["tires"]["GRIP_SCALE"][1].value_or(1.0);
	tmp.STATIC_GRIP.Front = config["tires"]["STATIC_GRIP"][0].value_or(2.0);
	tmp.STATIC_GRIP.Rear = config["tires"]["STATIC_GRIP"][1].value_or(2.0);
	tmp.STEERING = config["tires"]["STEERING"].value_or(1.0);
	tmp.YAW_CONTROL.clear();
	tmp.YAW_CONTROL.push_back(config["tires"]["YAW_CONTROL"][0].value_or(1.0));
	tmp.YAW_CONTROL.push_back(config["tires"]["YAW_CONTROL"][1].value_or(1.0));
	tmp.YAW_CONTROL.push_back(config["tires"]["YAW_CONTROL"][2].value_or(1.0));
	tmp.YAW_CONTROL.push_back(config["tires"]["YAW_CONTROL"][3].value_or(1.0));
	tmp.YAW_SPEED = config["tires"]["YAW_SPEED"].value_or(1.0);

	// transmission
	tmp.DIFFERENTIAL[0] = config["transmission"]["DIFFERENTIAL"][0].value_or(1.0);
	tmp.DIFFERENTIAL[1] = config["transmission"]["DIFFERENTIAL"][1].value_or(1.0);
	tmp.DIFFERENTIAL[2] = config["transmission"]["DIFFERENTIAL"][2].value_or(1.0);
	tmp.TORQUE_SPLIT = config["transmission"]["TORQUE_SPLIT"].value_or(0.5);

	// pvehicle
	tmp.TENSOR_SCALE[0] = config["pvehicle"]["TENSOR_SCALE"][0].value_or(-0.011f);
	tmp.TENSOR_SCALE[1] = config["pvehicle"]["TENSOR_SCALE"][1].value_or(3.5);
	tmp.TENSOR_SCALE[2] = config["pvehicle"]["TENSOR_SCALE"][2].value_or(1.0);
	if (tmp.TENSOR_SCALE[0] == -0.011f) {
		tmp.TENSOR_SCALE[0] = 1.0;
		WriteLog(std::format("TENSOR_SCALE missing for {}", configCarName));
	}

	aCarTunings.push_back(tmp);
	return &aCarTunings[aCarTunings.size()-1];
}

inline MWCarTuning* GetCarTuning(const char* model) {
	for (auto& tuning : aCarTunings) {
		if (tuning.carName == model) return &tuning;
	}
	if (auto tuning = LoadCarTuningFromFile(model)) {
		return tuning;
	}
	WriteLog(std::format("Failed to find tunings for {}", model));
	return &aCarTunings[0];
}

// tunings:
// aud_r8_stk_08 - NFSC darius_top
// aud_r8_stk_08_speedtest - NFSC darius_top
// aud_rs4_stk_08 - a4_top
// aud_s5_stk_08 - a4_top
// bmw_m3_e92_08 - bmwm3gtr_top
// bmw_m6_stk_08 - NFSC bmwm3gtr_crosschase
// bmw_z4_m_07 - NFSC 350z_top
// bug_vey_164_08 - NFSC darius_top
// che_cam_con_07 - NFSC camaron_top
// che_vet_sti_67 - corvette_top
// che_vet_z06_06 - corvettec6r_top
// dod_chr_bee_07 - NFSC charger06_top
// for_esc_rs_96 - imprezawrx_top
// for_foc_st_07 - punto_top
// for_shl_gt_08 - NFSC mustangshlbyn_top
// for_shl_ter_08 - NFSC mustangshlbyn_top
// lex_is_f_08 - is300
// mcl_f1_stk_94 - corvettec6r
// mer_cls_55_08 - clk500
// mer_cls_63_08 - clk500
// mit_evo_ix_06 - lancerevo8
// mit_evo_x_08 - lancerevo8
// nis_370_z_09 - NFSC 350z_top
// nis_gtr_r35_08 - NFSC skyline_top
// nis_syl_aer_99 - NFSC skyline_top
// player_cop_car_mid_05 - viper_top
// player_cop_car_mus_08 - mustanggt_top
// player_cop_car_suv_04 - cts_top
// pon_sol_gxp_06 - gto_top
// por_gt3_rs_08 - 911gt2_top
// ren_meg_cou_08 - gti_top
// vol_gol_r32_06 - gti_top
// vol_sci_stk_08 - cobaltss_top

inline void InitMWCarTunings() {
	{
		MWCarTuning tmp;
		tmp.carName = "elise";
		// chassis
		tmp.AERO_CG = 50.0;
		tmp.AERO_COEFFICIENT = 0.14;
		tmp.DRAG_COEFFICIENT = 0.22;
		//tmp.FRONT_AXLE = 1.13;
		tmp.FRONT_WEIGHT_BIAS = 53;
		tmp.RENDER_MOTION = 1.0;
		tmp.RIDE_HEIGHT = {8, 8};
		tmp.ROLL_CENTER = 11;
		tmp.SHOCK_BLOWOUT = 5;
		tmp.SHOCK_DIGRESSION = {0.1, 0.1};
		tmp.SHOCK_EXT_STIFFNESS = {35, 35};
		tmp.SHOCK_STIFFNESS = {30, 30};
		tmp.SHOCK_VALVING = {18, 18};
		tmp.SPRING_PROGRESSION = {6.1, 6.1};
		tmp.SPRING_STIFFNESS = {450, 400};
		tmp.SWAYBAR_STIFFNESS = {200, 200};
		//tmp.TRACK_WIDTH = {1.465, 1.522};
		tmp.TRAVEL = {6.5, 6.5};
		//tmp.WHEEL_BASE = 2.35;

		// brakes
		tmp.BRAKE_LOCK = {1.0, 2.75};
		tmp.BRAKES = {280, 350};
		tmp.EBRAKE = 400;

		// tires
		tmp.DYNAMIC_GRIP = {1.4, 1.5};
		tmp.GRIP_SCALE = {1.0, 1.0};
		//tmp.SECTION_WIDTH = {225, 225};
		tmp.STATIC_GRIP = {1.6, 1.7};
		tmp.STEERING = 1.0;
		tmp.YAW_CONTROL = {0.0, 0.1, 0.3, 1.0};
		tmp.YAW_SPEED = 0.4;

		// transmission
		tmp.DIFFERENTIAL[0] = 0.85;
		tmp.DIFFERENTIAL[1] = 0.85;
		tmp.DIFFERENTIAL[2] = 0.0;
		tmp.TORQUE_SPLIT = 0.0;
		aCarTunings.push_back(tmp);
	}
	{
		MWCarTuning tmp;
		tmp.carName = "gti_top";
		// chassis
		tmp.AERO_CG = 47.0;
		tmp.AERO_COEFFICIENT = 0.24;
		tmp.DRAG_COEFFICIENT = 0.35;
		//tmp.FRONT_AXLE = 1.17;
		tmp.FRONT_WEIGHT_BIAS = 53.25;
		tmp.RENDER_MOTION = 0.5;
		tmp.RIDE_HEIGHT = {8, 8};
		tmp.ROLL_CENTER = 9;
		tmp.SHOCK_BLOWOUT = 5;
		tmp.SHOCK_DIGRESSION = {0.1, 0.1};
		tmp.SHOCK_EXT_STIFFNESS = {65, 65};
		tmp.SHOCK_STIFFNESS = {60, 55};
		tmp.SHOCK_VALVING = {20, 20};
		tmp.SPRING_PROGRESSION = {7, 7};
		tmp.SPRING_STIFFNESS = {450, 400};
		tmp.SWAYBAR_STIFFNESS = {500, 500};
		//tmp.TRACK_WIDTH = {1.539, 1.538};
		tmp.TRAVEL = {6.5, 6.5};
		//tmp.WHEEL_BASE = 2.578;

		// brakes
		tmp.BRAKE_LOCK = {1.0, 3.5};
		tmp.BRAKES = {390, 475};
		tmp.EBRAKE = 750;

		// tires
		tmp.DYNAMIC_GRIP = {1.85, 1.9};
		tmp.GRIP_SCALE = {1.05, 1.1};
		//tmp.SECTION_WIDTH = {235, 235};
		tmp.STATIC_GRIP = {2.1, 2.1};
		tmp.STEERING = 1.0;
		tmp.YAW_CONTROL = {0.1, 0.6, 0.8, 1.25};
		tmp.YAW_SPEED = 0.35;

		// transmission
		tmp.DIFFERENTIAL[0] = 0.7;
		tmp.DIFFERENTIAL[1] = 0.7;
		tmp.DIFFERENTIAL[2] = 0.0;
		tmp.TORQUE_SPLIT = 1.0;
		aCarTunings.push_back(tmp);
	}
}

#ifdef SUSPENSIONRACER_ELISE_TEST
MWCarTuning* GetMWCarData(const SuspensionRacer* pThis);
MWCarTuning* GetMWCarDataFromTire(const void* pThis);
#else
Attrib::Gen::car_tuning::_LayoutStruct* GetMWCarData(const SuspensionRacer* pThis);
Attrib::Gen::car_tuning::_LayoutStruct* GetMWCarData(const SuspensionRacer::Tire* pThis);
#endif

#ifdef _MSC_VER
#pragma warning( pop )
#endif

#endif