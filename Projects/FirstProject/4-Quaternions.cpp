/////////////////////////////////////////////////////////////////////////
////
//// Using quaternions to rotate in 3D.
////
//// Assignment: 1. Create a class for Quaternions.
////             2. Create a scene with a camera rotating around an 
////                object at the origin and pointing towards it. 
////                Do NOT use "gluLookAt" to create the ViewMatrix, 
////                use rotation matrices!
////             3. Gimbal lock mode ON: use Translation + Rotation 
////                matrices (e.g. Euler angles, Rodrigues’ formula).
////             4. Gimbal lock mode OFF: use Quaternions to produce a 
////                transformation matrix and avoid gimbal lock.
////             5. Switch between modes by pressing the 'g' key.
////
//// (c) 2013-18 by Carlos Martinho
////
/////////////////////////////////////////////////////////////////////////
//
//#include <iostream>
//#include <iomanip>
//#include <string>
//#include <cassert>
//
//#include "quaternion.h"
//#include "vector.h"
//#include "matrix.h"
/////////////////////////////////////////////////////////////////////////
//
//#define HEADER(X) std::cout<<std::endl<<(X)<<std::endl<<std::endl;
//using namespace engine;
//
//void qtest1()
//{
//	HEADER("TEST 1 : Rotation of 90 degrees about the y-axis")
//
//	vec4 axis = { 0.0f, 1.0f, 0.0f, 1.0f };
//	std::cout << axis << std::endl;
//	qtrn q = qtrn(90.0f, axis);
//	std::cout <<" q " <<  q;
//
//	qtrn vi = {0.0f, 7.0f, 0.0f, 0.0f};
//	std::cout << " vi " << vi;
//
//	qtrn qe = {0.0f, 0.0f, 0.0f, -7.0f};
//	std::cout << " qe " << qe;
//
//	qtrn qinv = inverse(q);
//	qinv.clean();
//	std::cout << " qinv " << qinv;
//
//	qtrn qf = (q * vi) * qinv;
//	std::cout << "  qf" << qf;
//
//	assert(qf == qe);
//}
//
//void qtest2()
//{
//	HEADER("TEST 2 : Rotation of 90 degrees about the y-axis with matrix")
//
//	vec4 axis = {0.0f, 1.0f, 0.0f, 1.0f};
//	qtrn q = qtrn(90.0f, axis);
//	std::cout << " q " << q;
//
//	vec4 vi = {7.0f, 0.0f, 0.0f, 1.0f};
//	std::cout << " vi " << vi << std::endl;
//
//	vec4 ve = {0.0f, 0.0f, -7.0f, 1.0f};
//	std::cout << " ve " << ve << std::endl;
//
//	mat4 m = q.qToMatrix();
//	std::cout <<  m;
//
//	vec4 vf = m * vi;
//	std::cout <<  "vf" << vf << std::endl;
//
//	assert(vf == ve);
//}
//
//void qtest3()
//{
//	HEADER("TEST 3 : Yaw 900 = Roll 180 + Pitch 180")
//
//	vec4 axis_x = {1.0f, 0.0f, 0.0f, 1.0f};
//	vec4 axis_y = {0.0f, 1.0f, 0.0f, 1.0f};
//	vec4 axis_z = {0.0f, 0.0f, 1.0f, 1.0f};
//
//	qtrn qyaw900 = qtrn(900.0f, axis_y);
//	std::cout << "  qyaw900" << qyaw900 ;
//
//	qtrn qroll180 = qtrn(180.0f, axis_x);
//	std::cout << " qroll180" << qroll180;
//	qtrn qpitch180 = qtrn(180.0f, axis_z);
//	std::cout <<"qpitch180" << qpitch180;
//	qtrn qrp = qpitch180 * qroll180;
//	std::cout <<"      qrp" << qrp;
//	qtrn qpr =qroll180 * qpitch180;
//	std::cout <<"      qpr"<< qpr ;
//
//	qtrn qi = { 0.0f, 1.0f, 0.0f, 0.0f }; // x-axis
//	std::cout <<"       qi" << qi;
//	qtrn qe = { 0.0f, -1.0f, 0.0f, 0.0f };
//	std::cout <<"       qe" << qe;
//
//	qtrn qyaw900inv = inverse(qyaw900);
//	qtrn qfy = qyaw900 * qi * qyaw900inv;
//	std::cout <<"       qy" << qfy;
//	assert(qe == qfy);
//
//	qtrn qrpinv = inverse(qrp);
//	qtrn qfrp = qrp * qi * qrpinv;
//	std::cout <<"     qfrp" << qfrp ;
//	assert(qe == qfrp);
//
//	qtrn qprinv = inverse(qpr);
//	qtrn qfpr = qpr * qi * qprinv;
//	std::cout << "     qfpr" << qfpr;
//	assert(qe == qfpr);
//}
//
//void qtest4()
//{
//	HEADER("TEST 4: Q <-> (angle, axis)")
//
//	float thetai = 45.0f;
//	vec4 axis_i = {0.0f, 1.0f, 0.0f, 1.0f};
//	qtrn q = qtrn(thetai, axis_i);
//	std::cout << thetai << " " << std::endl;
//	std::cout << "axis_i" << axis_i << std::endl;
//
//	float thetaf;
//	vec4 axis_f;
//	q.qToAngleAxis( thetaf, axis_f);
//	std::cout << thetaf << " " << std::endl;
//	std::cout << "axis_f" << axis_f << std::endl;
//	std::cout <<"Theta: " << fcmp(thetai, thetaf) << " axis" << (axis_i == axis_f) << std::endl;
//	assert(fcmp(thetai , thetaf) && (axis_i == axis_f));
//}
//
//void qtest5()
//{
//	HEADER("TEST 5: LERP")
//
//	vec4 axis = {0.0f, 1.0f, 0.0f, 1.0f};
//	qtrn q0 = qtrn(0.0f, axis);
//	std::cout <<"       q0" << q0 ;
//	qtrn q1 = qtrn(90.0f, axis);
//	std::cout <<"       q1" << q1 ;
//	qtrn qe = qtrn(30.0f, axis);
//	std::cout <<"       qe" << qe ;
//
//	qtrn qLerp0 = lerp(q0, q1, 0.0f);
//	std::cout <<"  lerp(0)" << qLerp0;
//	assert(qLerp0 == q0);
//
//	qtrn qLerp1 = lerp(q0, q1, 1.0f);
//	std::cout <<"  lerp(1)" << qLerp1;
//	assert(qLerp1 == q1);
//
//	qtrn qlerp = lerp(q0, q1, 1/3.0f);
//	std::cout <<"lerp(1/3)" << qlerp ;
//	printAngleAxis("lerp(1/3)", qlerp);
//
//	assert((qlerp == qe) == 0 );
//}
//
//void qtest6()
//{
//	HEADER("TEST 6: SLERP")
//
//	vec4 axis = {0.0f, 1.0f, 0.0f, 1.0f};
//	qtrn q0 = qtrn( 0.0f, axis);
//	std::cout << "        q0" << q0 ;
//	qtrn q1 = qtrn(90.0f, axis);
//	std::cout <<"        q1" << q1 ;
//	qtrn qe = qtrn(30.0f, axis);
//	std::cout <<"        qe" << qe ;
//
//	qtrn qSlerp0 = slerp(q0, q1, 0.0f);
//	std::cout <<"  slerp(0)" << qSlerp0;
//	assert(qSlerp0 == q0);
//
//	qtrn qSlerp1 = slerp(q0, q1, 1.0f);
//	std::cout <<"  slerp(1)" << qSlerp1;
//	assert(qSlerp1 == q1);
//
//	qtrn qslerp = slerp(q0, q1, 1/3.0f);
//	std::cout <<"slerp(1/3)" << qslerp ;
//	printAngleAxis("slerp(1/3)", qslerp);
//
//	assert(qslerp == qe);
//}
//
/////////////////////////////////////////////////////////////////////////
//
//int main(int argc, char* argv[])
//{
//	qtest1();
//	qtest2();
//	qtest3();
//	qtest4();
//	qtest5();
//	qtest6();
//
//	std::cout << std::endl << "Press <return>.";
//	std::cin.ignore(1);
//	exit(EXIT_SUCCESS);
//}
//
/////////////////////////////////////////////////////////////////////////