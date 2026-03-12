#ifndef MW_H
#define MW_H

struct UMath::Vector3 {
	float x;
	float y;
	float z;
};

struct UMath::Vector4 {
	float x;
	float y;
	float z;
	float w;
};

struct UMath::Matrix4 {
	UMath::Vector4 x;
	UMath::Vector4 y;
	UMath::Vector4 z;
	UMath::Vector4 p;
};

struct Attrib::Instance {
	void* mOwner;
	void* mCollection;
	void* mLayoutPtr;
	unsigned int mMsgPort;
	unsigned short mFlags;
	unsigned short mLocks;
};

struct Chassis::State {
	UMath::Matrix4 matrix;
	UMath::Vector3 local_vel;
	float gas_input;
	UMath::Vector3 linear_vel;
	float brake_input;
	UMath::Vector3 angular_vel;
	float ground_effect;
	UMath::Vector3 cog;
	float ebrake_input;
	UMath::Vector3 dimension;
	float steer_input;
	UMath::Vector3 local_angular_vel;
	float slipangle;
	UMath::Vector3 inertia;
	float mass;
	UMath::Vector3 world_cog;
	float speed;
	float time;
	int flags;
	short driver_style;
	short driver_class;
	short gear;
	short blown_tires;
	float nos_boost;
	float shift_boost;
	void* collider;
};

struct WSurface {
	unsigned char fSurface;
	unsigned char fFlags;
};

struct WCollisionTri {
	UMath::Vector3 fPt0;
	void* fSurfaceRef;
	UMath::Vector3 fPt1;
	unsigned int fFlags;
	UMath::Vector3 fPt2;
	WSurface fSurface;
	unsigned short PAD;
};

struct WWorldPos {
	WCollisionTri fFace;
	unsigned int fFaceValid:1;
	unsigned int fMissCount:15;
	unsigned int fUsageCount:16;
	float fYOffset;
	void* fSurface;
};

struct Wheel {
	WWorldPos mWorldPos;
	UMath::Vector4 mNormal;
	UMath::Vector3 mPosition;
	unsigned int mFlags;
	UMath::Vector3 mForce;
	float mAirTime;
	UMath::Vector3 mLocalArm;
	float mCompression;
	UMath::Vector3 mWorldArm;
	int pad;
	UMath::Vector3 mVelocity;
	int pad2;
	Attrib::Instance mSurface;
	float mSurfaceStick;
	UMath::Vector4 mIntegral;
};

#endif