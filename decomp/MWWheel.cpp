void WheelMW::UpdateSurface(const Attrib::Collection* surface) {

}

bool WheelMW::InitPosition(ICollisionBody* cb, IRigidBody *rb, double maxcompression) {
	//FUNCTION_LOG("Wheel::InitPosition");
	auto mat = *rb->GetTransform();
	mat.p = *rb->GetPosition();
	UMath::Vector3 dim;
	rb->GetDimension(&dim);
	return UpdatePosition(*rb->GetAngularVelocity(), *rb->GetLinearVelocity(), mat, {0,0,0}, 0.0, maxcompression, false, cb->GetWCollider(), dim.y * 2.0);
}

bool WheelMW::UpdatePosition(const UMath::Vector3 &body_av, const UMath::Vector3 &body_lv,
						   const UMath::Matrix4 &body_matrix, const UMath::Vector3 &cog,
						   float dT, float wheel_radius, bool usecache, const WCollider *collider, float vehicle_height) {
	UMath::Rotate(mLocalArm, body_matrix, mWorldArm);
	UMath::Add(mWorldArm, UMath::Vector4To3(body_matrix.p), mPosition);

	UMath::Vector3 pVel;
	UMath::Vector3 pos;
	UMath::Sub(mWorldArm, cog, pos);
	UMath::Cross(body_av, pos, pVel);
	UMath::Add(pVel, body_lv, pVel);
	SetVelocity(pVel);

	UMath::Add(mWorldArm, UMath::Vector4To3(body_matrix.p), mPosition);

	float tolerance = UMath::Max(-pVel.y * dT, 0.0f) + wheel_radius + UMath::Lengthxz(pVel) * dT;
	float prev = vehicle_height * 0.5f;
	mWorldPos.SetTolerance(UMath::Min(tolerance, prev));

	bool result = WWorldPos::Update(&mWorldPos, &mPosition, &mNormal, IsOnGround() && usecache, collider, WWorldPos::kFail_KeepValid);
	mNormal.w = -mWorldPos.fHeight;
	UpdateSurface(mWorldPos.pSurface);
	return result;
}