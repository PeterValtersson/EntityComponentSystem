#include "stdafx.h"
//#include "CppUnitTest.h"
//#include <EntityManager_Interface.h>
//using namespace Microsoft::VisualStudio::CppUnitTestFramework;
//using namespace ECS;
//#include <Windows.h>
//#include <Graphics\Renderer_Interface.h>
//#include <Managers\TransformManager_Interface.h>
//#include <Managers\MeshManager_Interface.h>
//#include <ResourceHandler\ResourceHandler_Interface.h>
//#include <ThreadPool.h>
//#include "../OBJParser/OBJPassthrough.h"
//#include <filesystem>
//namespace fs = std::experimental::filesystem;
//
//namespace OBJS
//{
//	std::string test12(R"(
//v 1.0 2.0 3.0
//v 4.0 5.0 6.0
//vt 0 0
//vt 1 1
//vn 0 1 0
//vn 1 0 0
//o First
//f 1/1/1 1/2/1 1/1/2
//f 2/1/1 1/2/2 2/2/2
//o Sec
//f 1//1 1//2 2//1
//)");
//
//
//	std::string tree1(R"(
//# 3ds Max Wavefront OBJ Exporter v0.97b - (c)2007 guruware
//# File Created: 01.07.2017 22:41:07
//
//mtllib Tree low.mtl
//
//#
//# object Hedra001
//#
//
//v  -7.8541 121.3862 17.1093
//v  7.1431 113.8654 23.1824
//v  15.3955 112.2824 13.7628
//v  13.1239 113.9954 -0.7797
//v  3.5989 114.8814 -2.4757
//v  -10.4425 120.6379 5.9937
//v  22.8577 106.7685 -11.6212
//v  21.9717 97.5478 -18.1949
//v  14.7447 95.6226 -21.0780
//v  5.5241 105.1476 -15.9234
//v  10.7313 81.5813 -25.8106
//v  -0.3842 84.1696 -28.3517
//v  -10.9133 99.1668 -24.3868
//v  -9.0183 107.4192 -18.2374
//v  -27.2165 97.6283 -16.2752
//v  -33.2003 101.3003 -6.4332
//v  -28.3916 115.2407 -2.9244
//v  -19.9313 118.1055 -5.8959
//v  -26.1201 114.2536 11.6181
//v  -16.3871 120.3799 19.7621
//v  -29.9762 95.9312 17.0970
//v  -25.9713 95.5127 25.2404
//v  -16.3814 107.9399 31.6712
//v  -8.9379 115.5364 30.9508
//v  -18.2024 95.0759 36.6940
//v  -7.0869 92.0138 39.2351
//v  9.2766 98.9971 34.9800
//v  5.6046 108.8397 33.2648
//v  23.2170 96.1637 23.3207
//v  26.0818 105.1410 11.9090
//v  28.6141 102.7551 -3.8491
//v  29.3625 91.6395 -2.3836
//v  21.8416 81.1104 -9.9508
//v  20.2587 83.0054 -16.7807
//v  22.2299 81.6212 24.7348
//v  28.3561 79.4830 13.5794
//v  -21.7506 84.4098 29.8955
//v  -11.0534 74.0208 25.0309
//v  3.4890 67.3241 31.1889
//v  3.9074 75.4670 36.2789
//v  15.9162 57.8074 19.3533
//v  23.5126 63.0942 14.6086
//v  16.8159 64.8072 0.0661
//v  6.9734 58.8234 -6.3048
//v  4.1399 63.6321 -17.9993
//v  13.1173 72.0924 -19.7791
//v  3.0521 56.3405 5.7597
//v  -0.0099 55.5921 -5.3559
//v  -26.9583 80.9703 27.6242
//v  -27.7019 70.2731 22.7151
//v  -18.0029 65.0654 8.3409
//v  -7.6140 64.3219 10.6634
//v  -24.6996 66.0525 -6.2015
//v  -16.5568 62.0475 -14.9946
//v  -10.4025 65.9037 -24.1574
//v  -12.5408 75.6366 -26.1625
//v  -34.2164 75.6423 -0.8151
//v  -28.9295 83.0858 -13.8913
//v  -36.4316 84.9369 12.2765
//v  -35.6832 73.8213 16.4996
//# 60 vertices
//
//vn 0.3139 0.9492 -0.0204
//vn 0.1225 0.7408 -0.6604
//vn 0.2003 0.4396 -0.8756
//vn -0.4552 0.5219 -0.7214
//vn -0.3300 0.9440 -0.0028
//vn -0.7507 0.4201 0.5099
//vn -0.0359 0.3147 0.9485
//vn 0.6340 0.5615 0.5318
//vn 0.8945 -0.0702 -0.4416
//vn 0.9644 0.0781 0.2527
//vn -0.3885 -0.4956 0.7768
//vn 0.6301 -0.3308 0.7026
//vn 0.7323 -0.5777 -0.3606
//vn 0.4421 -0.8746 -0.1992
//vn -0.0063 -0.7476 0.6642
//vn -0.3705 -0.9112 0.1800
//vn 0.1918 -0.3801 -0.9049
//vn -0.7110 -0.3794 -0.5920
//vn -0.8163 0.0976 0.5693
//vn -0.9574 -0.0302 -0.2871
//vn 0.1938 0.8514 -0.4873
//vn 0.2309 0.8786 0.4179
//vn 0.5762 0.8172 -0.0137
//vn 0.4853 -0.0290 0.8739
//vn 0.8659 -0.3894 -0.3140
//vn -0.4175 -0.8255 0.3797
//vn 0.0558 -0.8785 -0.4745
//vn -0.5225 -0.8523 0.0224
//vn -0.4501 0.0486 -0.8917
//vn -0.9278 0.3189 0.1937
//vn -0.7618 0.1525 0.6297
//vn 0.6215 -0.1014 -0.7768
//# 32 vertex normals
//
//vt 0.0000 -0.0854 0.5000
//vt 0.1809 -0.1972 0.4309
//vt 0.3191 -0.1118 0.3781
//vt 0.3191 0.1118 0.3781
//vt 0.1809 0.1972 0.4309
//vt 0.0000 0.0854 0.5000
//vt 0.1118 0.3781 0.3191
//vt 0.1771 0.1741 0.4578
//vt 0.4309 0.1809 0.1972
//vt 0.3973 0.3657 0.0460
//vt 0.2060 0.4870 0.1160
//vt -0.0000 0.0854 0.5000
//vt -0.0000 -0.0854 0.5000
//vt 0.4309 0.1809 -0.1972
//vt 0.3191 0.1118 -0.3781
//vt 0.1809 0.1972 -0.4309
//vt 0.1118 0.3781 -0.3191
//vt 0.1972 0.4309 -0.1809
//vt 0.3781 0.3191 -0.1118
//vt -0.3191 0.1118 0.3781
//vt -0.3191 -0.1118 0.3781
//vt -0.1809 -0.1972 0.4309
//vt -0.1809 0.1972 0.4309
//vt -0.1118 0.3781 -0.3191
//vt -0.1941 0.1721 -0.4429
//vt -0.3191 0.1118 -0.3781
//vt -0.4309 0.1809 -0.1972
//vt -0.4536 0.2547 -0.1007
//vt -0.2583 0.3720 -0.1788
//vt 0.1809 -0.1972 -0.4309
//vt 0.0000 -0.0854 -0.5000
//vt 0.0000 0.0854 -0.5000
//vt 0.3191 -0.1118 -0.3781
//vt -0.4309 0.1809 0.1972
//vt -0.1118 0.3781 0.3191
//vt -0.1972 0.4309 0.1809
//vt -0.3781 0.3191 0.1118
//vt -0.1809 -0.1972 -0.4309
//vt -0.3191 -0.1118 -0.3781
//vt -0.1809 0.1972 -0.4309
//vt 0.1118 -0.3781 -0.3191
//vt 0.2179 -0.1576 -0.4335
//vt 0.4309 -0.1809 -0.1972
//vt 0.3411 -0.3067 -0.1612
//vt 0.1242 -0.4486 -0.2362
//vt 0.1118 -0.3781 0.3191
//vt 0.1972 -0.4309 0.1809
//vt 0.3781 -0.3191 0.1118
//vt 0.4309 -0.1809 0.1972
//vt -0.1972 -0.4309 -0.1809
//vt -0.3052 -0.3423 -0.1615
//vt -0.4309 -0.1809 -0.1972
//vt -0.1891 -0.1835 -0.4364
//vt -0.1249 -0.3695 -0.3146
//vt 0.3781 -0.3191 -0.1118
//vt 0.2795 -0.3566 -0.1730
//vt 0.2089 -0.3107 -0.2894
//vt 0.4249 -0.2130 -0.1712
//vt -0.1118 -0.3781 0.3191
//vt -0.1118 -0.3781 -0.3191
//# 60 texture coords
//
//g Hedra001
//usemtl wire_154185229
//s off
//f 1/1/1 2/2/1 3/3/1 4/4/1 5/5/1 6/6/1 
//f 4/7/2 7/8/2 8/4/2 9/9/2 10/10/2 5/11/2 
//f 9/5/3 11/12/3 12/13/3 13/2/3 14/3/3 10/4/3 
//f 13/14/4 15/15/4 16/16/4 17/17/4 18/18/4 14/19/4 
//f 17/20/5 19/21/5 20/22/5 1/1/5 6/6/5 18/23/5 
//f 19/24/6 21/25/6 22/26/6 23/27/6 24/28/6 20/29/6 
//f 23/30/7 25/31/7 26/32/7 27/16/7 28/15/7 24/33/7 
//f 27/34/8 29/20/8 30/23/8 3/35/8 2/36/8 28/37/8 
//f 7/5/9 31/12/9 32/13/9 33/2/9 34/3/9 8/4/9 
//f 31/12/10 30/23/10 29/20/10 35/21/10 36/22/10 32/13/10 
//f 25/31/11 37/38/11 38/39/11 39/26/11 40/40/11 26/32/11 
//f 39/41/12 41/42/12 42/33/12 36/43/12 35/44/12 40/45/12 
//f 33/2/13 43/46/13 44/47/13 45/48/13 46/49/13 34/3/13 
//f 43/15/14 42/33/14 41/30/14 47/31/14 48/32/14 44/16/14 
//f 37/50/15 49/51/15 50/52/15 51/39/15 52/53/15 38/54/15 
//f 51/39/16 53/26/16 54/40/16 48/32/16 47/31/16 52/38/16 
//f 45/20/17 55/21/17 56/22/17 12/13/17 11/12/17 46/23/17 
//f 55/55/18 54/56/18 53/57/18 57/30/18 58/33/18 56/58/18 
//f 49/39/19 22/26/19 21/40/19 59/32/19 60/31/19 50/38/19 
//f 59/32/20 16/16/20 15/15/20 58/33/20 57/30/20 60/31/20 
//f 5/5/21 10/7/21 14/35/21 18/23/21 6/6/21 
//f 20/22/22 24/59/22 28/46/22 2/2/22 1/1/22 
//f 3/35/23 30/23/23 31/12/23 7/5/23 4/7/23 
//f 27/16/24 26/32/24 40/40/24 35/24/24 29/17/24 
//f 36/22/25 42/59/25 43/46/25 33/2/25 32/13/25 
//f 39/41/26 38/60/26 52/38/26 47/31/26 41/30/26 
//f 48/32/27 54/40/27 55/24/27 45/17/27 44/16/27 
//f 51/60/28 50/38/28 60/31/28 57/30/28 53/41/28 
//f 58/59/29 15/46/29 13/2/29 12/13/29 56/22/29 
//f 59/32/30 21/40/30 19/24/30 17/17/30 16/16/30 
//f 22/41/31 49/60/31 37/38/31 25/31/31 23/30/31 
//f 9/5/32 8/7/32 34/35/32 46/23/32 11/12/32 
//# 32 polygons
//
//#
//# object Cylinder002
//#
//
//v  5.8107 0.0000 2.5305
//v  1.3320 0.0000 -3.6338
//v  -4.3269 25.5184 -3.6338
//v  -0.4267 23.3169 2.5305
//v  -5.9147 0.0000 -1.2792
//v  -10.6377 29.0805 -1.2792
//v  -5.9147 0.0000 6.3403
//v  -10.6377 29.0805 6.3403
//v  1.3320 0.0000 8.6949
//v  -4.3269 25.5184 8.6949
//v  1.9548 71.7094 2.5306
//v  -0.2241 70.1154 2.3016
//v  -3.7498 67.5364 2.3891
//v  -3.7498 67.5364 2.6720
//v  -0.2241 70.1154 2.7595
//v  2.1547 38.8537 0.0607
//v  3.7174 36.6522 2.5305
//v  -0.3738 42.4157 1.0041
//v  -0.3738 42.4157 4.0570
//v  2.1547 38.8537 5.0004
//v  9.6451 49.6451 0.0607
//v  12.2492 50.3572 2.5306
//v  5.4316 48.4928 1.0041
//v  5.4316 48.4928 4.0570
//v  9.6451 49.6451 5.0004
//v  6.4660 60.9696 -0.4828
//v  8.6450 62.5636 2.5305
//v  2.9403 58.3906 0.6682
//v  2.9403 58.3906 4.3929
//v  6.4660 60.9696 5.5439
//# 30 vertices
//
//vn 0.9704 0.2413 0.0095
//vn 0.3723 0.0913 -0.9236
//vn 0.0389 0.0219 -0.9990
//vn 0.9704 0.2408 0.0177
//vn -0.7798 -0.1266 -0.6131
//vn -0.8990 -0.1536 -0.4102
//vn -0.7824 -0.1372 0.6075
//vn -0.9011 -0.1464 0.4081
//vn 0.3861 0.1064 0.9163
//vn 0.0625 0.0305 0.9976
//vn 0.0000 -1.0000 -0.0000
//vn -0.5904 0.8071 0.0000
//vn 0.7684 -0.2230 -0.5999
//vn -0.1214 0.2890 -0.9496
//vn -0.7925 0.6099 0.0000
//vn -0.1214 0.2890 0.9496
//vn 0.7684 -0.2230 0.5999
//vn 0.6311 -0.4128 -0.6568
//vn -0.2104 0.1658 -0.9635
//vn -0.7231 0.6908 0.0000
//vn -0.2104 0.1658 0.9635
//vn 0.6311 -0.4128 0.6568
//vn 0.7004 0.1860 -0.6891
//vn -0.2157 -0.0979 -0.9715
//vn -0.9698 -0.2441 0.0000
//vn -0.2157 -0.0979 0.9715
//vn 0.7004 0.1860 0.6891
//vn 0.3514 0.3868 -0.8526
//vn -0.2252 0.0746 -0.9715
//vn -0.8071 -0.5904 0.0000
//vn -0.2252 0.0746 0.9715
//vn 0.3514 0.3868 0.8526
//# 32 vertex normals
//
//vt 0.7500 0.0000 1.0000
//vt 0.9500 0.0000 1.0000
//vt 0.9500 1.0000 1.0000
//vt 0.7500 1.0000 1.0000
//vt 1.1500 0.0000 1.0000
//vt 1.1500 1.0000 1.0000
//vt 0.1500 0.0000 1.0000
//vt 0.3500 0.0000 1.0000
//vt 0.3500 1.0000 1.0000
//vt 0.1500 1.0000 1.0000
//vt 0.5500 0.0000 1.0000
//vt 0.5500 1.0000 1.0000
//vt 0.9755 0.6545 -0.2500
//vt 0.7939 0.0955 -0.2500
//vt 0.2061 0.0955 -0.2500
//vt 0.0245 0.6545 -0.2500
//vt 0.5000 1.0000 -0.2500
//vt 0.5000 1.0000 0.2500
//vt 0.0245 0.6545 0.2500
//vt 0.2061 0.0955 0.2500
//vt 0.7939 0.0955 0.2500
//vt 0.9755 0.6545 0.2500
//# 22 texture coords
//
//g Cylinder002
//usemtl wire_224086086
//s 8
//f 61/61/33 62/62/34 63/63/35 64/64/36 
//f 62/62/34 65/65/37 66/66/38 63/63/35 
//f 65/67/37 67/68/39 68/69/40 66/70/38 
//f 67/68/39 69/71/41 70/72/42 68/69/40 
//f 69/71/41 61/61/33 64/64/36 70/72/42 
//s 1
//f 69/73/43 67/74/43 65/75/43 62/76/43 61/77/43 
//usemtl Material__49
//f 71/78/44 72/79/44 73/80/44 74/81/44 75/82/44 
//usemtl wire_224086086
//s off
//f 64/78/45 63/79/45 76/79/45 77/78/45 
//f 63/79/46 66/80/46 78/80/46 76/79/46 
//f 66/80/47 68/81/47 79/81/47 78/80/47 
//f 68/81/48 70/82/48 80/82/48 79/81/48 
//f 70/82/49 64/78/49 77/78/49 80/82/49 
//f 77/78/50 76/79/50 81/79/50 82/78/50 
//f 76/79/51 78/80/51 83/80/51 81/79/51 
//f 78/80/52 79/81/52 84/81/52 83/80/52 
//f 79/81/53 80/82/53 85/82/53 84/81/53 
//f 80/82/54 77/78/54 82/78/54 85/82/54 
//f 82/78/55 81/79/55 86/79/55 87/78/55 
//f 81/79/56 83/80/56 88/80/56 86/79/56 
//f 83/80/57 84/81/57 89/81/57 88/80/57 
//f 84/81/58 85/82/58 90/82/58 89/81/58 
//f 85/82/59 82/78/59 87/78/59 90/82/59 
//usemtl Material__49
//f 87/78/60 86/79/60 72/79/60 71/78/60 
//f 86/79/61 88/80/61 73/80/61 72/79/61 
//f 88/80/62 89/81/62 74/81/62 73/80/62 
//f 89/81/63 90/82/63 75/82/63 74/81/63 
//f 90/82/64 87/78/64 71/78/64 75/82/64 
//# 27 polygons
//
//
//)");
//
//}
//
//namespace Tests
//{
//
//
//	LRESULT CALLBACK WndProc(HWND hwnd, UINT umessage, WPARAM wparam, LPARAM lparam)
//	{
//		switch (umessage)
//		{
//			// Check if the window is being destroyed.
//		case WM_DESTROY:
//		{
//			PostQuitMessage(0);
//			break;
//		}
//
//		// Check if the window is being closed.
//		case WM_CLOSE:
//		{
//			PostQuitMessage(0);
//			break;
//		}
//
//		// All other messages pass to the message handler in the system class.
//		default:
//		{
//			return DefWindowProc(hwnd, umessage, wparam, lparam);
//		}
//		}
//		return 0;
//	}
//
//	void InitWindow(HWND& _hWnd)
//	{
//		// Setup the windows class
//		WNDCLASSEX wc;
//
//		auto _hInst = GetModuleHandle(NULL);
//		wc.style = CS_HREDRAW | CS_VREDRAW;
//		wc.lpfnWndProc = WndProc;
//		wc.cbClsExtra = 0;
//		wc.cbWndExtra = 0;
//		wc.hInstance = _hInst;
//		wc.hIcon = LoadIcon(NULL, IDI_WINLOGO);
//		wc.hIconSm = wc.hIcon;
//		wc.hCursor = LoadCursor(NULL, IDC_ARROW);
//		wc.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
//		wc.lpszMenuName = NULL;
//		wc.lpszClassName = L"TEST";
//		wc.cbSize = sizeof(WNDCLASSEX);
//
//		LONG windowWidth = 300;
//		LONG windowHeight = 300;
//
//
//		// Register the window class.
//		//Place the window in the middle of the Window.
//		auto _windowPosX = (GetSystemMetrics(SM_CXSCREEN) - (int)windowWidth) / 2;
//		auto _windowPosY = (GetSystemMetrics(SM_CYSCREEN) - (int)windowHeight) / 2;
//
//		RegisterClassEx(&wc);
//		RECT rc = { 0, 0, (LONG)windowWidth, (LONG)windowHeight };
//
//
//		// Create the window with the Window settings and get the handle to it.
//		_hWnd = CreateWindowEx(
//			WS_EX_APPWINDOW,
//			L"TEST",
//			L"TEST",
//			0,
//			_windowPosX,
//			_windowPosY,
//			rc.right - rc.left,
//			rc.bottom - rc.top,
//			NULL,
//			NULL,
//			_hInst,
//			NULL);
//
//		// Bring the window up on the Window and set it as main focus.
//		ShowWindow(_hWnd, SW_SHOW);
//		SetForegroundWindow(_hWnd);
//		SetFocus(_hWnd);
//	}
//
//
//
//	TEST_CLASS(MeshManagerTest)
//	{
//	public:
//
//		TEST_METHOD(MeshManagerTest_OBJPassthrough_DLL)
//		{
//			std::error_code err;
//			fs::remove("data.dat", err);
//			{
//
//				auto bl = CreateFileSystem(ResourceHandler::FileSystemType::Binary);
//				Assert::IsTrue(bl);
//
//				auto result = InitLoader_C(bl, "data.dat", ResourceHandler::Mode::EDIT);
//				Assert::AreEqual("Success"_hash, result.hash);
//				Utilities::ThreadPool tp(4);
//				auto rh = CreateResourceHandler(bl, &tp);
//				Assert::IsTrue(rh);
//
//				result = rh->Initialize();
//				Assert::AreEqual("Success"_hash, result.hash, L"Init fail");
//
//				ResourceHandler::ResourceDataVoid data;
//				data.data = (void*)OBJS::tree1.c_str();
//				data.size = OBJS::tree1.size();
//				result = bl->Create("Tree", "StaticMesh", data);
//				Assert::AreEqual("Success"_hash, result.hash);
//
//				result = ResourceHandler_CreateType(rh, "StaticMesh", "../../Release/x64/OBJParser.dll");
//				Assert::AreEqual("Success"_hash, result.hash);
//
//				
//				
//				HWND w;
//				InitWindow(w);
//				auto r = CreateRenderer(Renderer_Backend::DIRECTX11, { w });
//				auto re = Renderer_Initialize_C(r);
//				Assert::AreEqual("Success"_hash, re.hash);
//
//				re = Renderer_Start_C(r);
//				Assert::AreEqual("Success"_hash, re.hash);
//
//
//				Assert::IsNotNull(Graphics::Get());
//
//
//				struct MeshInfo
//				{
//					uint8_t numSubmeshes;
//					ArfData::SubMesh* subMeshes;
//				};
//
//
//				ResourceHandler::ResourceData<MeshInfo> mesh("Tree", "StaticMesh");
//				Assert::AreEqual(2ui8, mesh->numSubmeshes);
//				Assert::AreEqual(32u, mesh->subMeshes[0].faceCount);
//				Assert::AreEqual(52u, mesh->subMeshes[1].faceCount);
//				Assert::AreEqual("Hedra001", mesh->subMeshes[0].name);
//				Assert::AreEqual("Cylinder002", mesh->subMeshes[1].name);
//				Assert::AreEqual(0u, mesh->subMeshes[0].faceStart);
//				Assert::AreEqual(32u, mesh->subMeshes[1].faceStart);
//
//
//				Renderer_Shutdown_C(r);
//				DestroyResourceHandler(rh);
//				DestroyLoader(bl);
//			}
//			fs::remove("data.dat", err);
//		}
//		TEST_METHOD(MeshManagerTest_OBJPassthrough_Static)
//		{
//			std::error_code err;
//			fs::remove("data.dat", err);
//			{
//
//				auto bl = CreateFileSystem(ResourceHandler::FileSystemType::Binary);
//				Assert::IsTrue(bl);
//
//				auto result = InitLoader_C(bl, "data.dat", ResourceHandler::Mode::EDIT);
//				Assert::AreEqual("Success"_hash, result.hash);
//				Utilities::ThreadPool tp(4);
//				auto rh = CreateResourceHandler(bl, &tp);
//				Assert::IsTrue(rh);
//
//				result = rh->Initialize();
//				Assert::AreEqual("Success"_hash, result.hash, L"Init fail");
//
//				ResourceHandler::ResourceDataVoid data;
//				data.data = (void*)OBJS::tree1.c_str();
//				data.size = OBJS::tree1.size();
//				result = bl->Create("Tree2", "StaticMesh2", data);
//				Assert::AreEqual("Success"_hash, result.hash);
//				
//				ResourceHandler::Type_Info ti;
//				using namespace std::placeholders;
//				ti.passthrough.Load = std::bind(Load, _1, _2, _3, _4, _5, _6);
//				ti.passthrough.Unload = std::bind(Unload, _1, _2, _3, _4);
//				ti.passthrough.Parse = std::bind(Parse, _1, _2, _3, _4, _5);
//				ti.passthrough.DestroyParsedData = std::bind(DestroyParsedData, _1, _2, _3);
//				Assert::AreEqual("Success"_hash, rh->AddType("StaticMesh2", ti).hash);
//				
//
//				HWND w;
//				InitWindow(w);
//				auto r = CreateRenderer(Renderer_Backend::DIRECTX11, { w });
//				auto re = Renderer_Initialize_C(r);
//				Assert::AreEqual("Success"_hash, re.hash);
//
//				re = Renderer_Start_C(r);
//				Assert::AreEqual("Success"_hash, re.hash);
//
//
//				Assert::IsNotNull(Graphics::Get());
//
//
//				struct MeshInfo
//				{
//					uint8_t numSubmeshes;
//					ArfData::SubMesh* subMeshes;
//				};
//
//
//
//
//				ResourceHandler::ResourceData<MeshInfo> mesh("Tree2", "StaticMesh2");
//				Assert::AreEqual(2ui8, mesh->numSubmeshes);
//				Assert::AreEqual(32u, mesh->subMeshes[0].faceCount);
//				Assert::AreEqual(52u, mesh->subMeshes[1].faceCount);
//				Assert::AreEqual("Hedra001", mesh->subMeshes[0].name);
//				Assert::AreEqual("Cylinder002", mesh->subMeshes[1].name);
//				Assert::AreEqual(0u, mesh->subMeshes[0].faceStart);
//				Assert::AreEqual(32u, mesh->subMeshes[1].faceStart);
//
//
//				Renderer_Shutdown_C(r);
//				DestroyResourceHandler(rh);
//				DestroyLoader(bl);
//			}
//			fs::remove("data.dat", err);
//		}
//		TEST_METHOD(MeshManagerTest_CreateDestroy)
//		{
//			std::error_code err;
//			fs::remove("data.dat", err);
//			{
//
//				auto bl = CreateFileSystem(ResourceHandler::FileSystemType::Binary);
//				Assert::IsTrue(bl);
//
//				auto result = InitLoader_C(bl, "data.dat", ResourceHandler::Mode::EDIT);
//				Assert::AreEqual("Success"_hash, result.hash);
//				Utilities::ThreadPool tp(4);
//				auto rh = CreateResourceHandler(bl, &tp);
//				Assert::IsTrue(rh);
//
//				result = rh->Initialize();
//				Assert::AreEqual("Success"_hash, result.hash, L"Init fail");
//
//				ResourceHandler::ResourceDataVoid data;
//				data.data = (void*)OBJS::tree1.c_str();
//				data.size = OBJS::tree1.size();
//				result = bl->Create("Tree2", "StaticMesh2", data);
//				Assert::AreEqual("Success"_hash, result.hash);
//
//				ResourceHandler::Type_Info ti;
//				using namespace std::placeholders;
//				ti.passthrough.Load = std::bind(Load, _1, _2, _3, _4, _5, _6);
//				ti.passthrough.Unload = std::bind(Unload, _1, _2, _3, _4);
//				ti.passthrough.Parse = std::bind(Parse, _1, _2, _3, _4, _5);
//				ti.passthrough.DestroyParsedData = std::bind(DestroyParsedData, _1, _2, _3);
//				Assert::AreEqual("Success"_hash, rh->AddType("StaticMesh2", ti).hash);
//
//
//				HWND w;
//				InitWindow(w);
//				auto r = CreateRenderer(Renderer_Backend::DIRECTX11, { w });
//				auto re = Renderer_Initialize_C(r);
//				Assert::AreEqual("Success"_hash, re.hash);
//
//				re = Renderer_Start_C(r);
//				Assert::AreEqual("Success"_hash, re.hash);
//
//
//				Assert::IsNotNull(Graphics::Get());
//
//
//				struct MeshInfo
//				{
//					uint8_t numSubmeshes;
//					ArfData::SubMesh* subMeshes;
//				};
//
//
//
//
//				ResourceHandler::ResourceData<MeshInfo> mesh("Tree2", "StaticMesh2");
//				Assert::AreEqual(2ui8, mesh->numSubmeshes);
//				Assert::AreEqual(32u, mesh->subMeshes[0].faceCount);
//				Assert::AreEqual(52u, mesh->subMeshes[1].faceCount);
//				Assert::AreEqual("Hedra001", mesh->subMeshes[0].name);
//				Assert::AreEqual("Cylinder002", mesh->subMeshes[1].name);
//				Assert::AreEqual(0u, mesh->subMeshes[0].faceStart);
//				Assert::AreEqual(32u, mesh->subMeshes[1].faceStart);
//
//
//
//				auto em = EntityManager_CreateEntityManager_C();
//
//				TransformManager_Init_Info tmii;
//				tmii.entityManager = em;
//				auto tm = TransformManager_CreateTransformManager_C(tmii);
//
//				MeshManager_InitializationInfo rmii;
//				rmii.entityManager = em;
//				rmii.renderer = r;
//				rmii.transformManager = tm;
//				auto rm = MeshManager_CreateMeshManager(rmii);
//				//OBJS::tree
//
//
//
//
//
//
//				Delete_C(rm);
//				Delete_C(tm);
//				Delete_C(em);
//				Renderer_Shutdown_C(r);
//				DestroyResourceHandler(rh);
//				DestroyLoader(bl);
//			}
//			fs::remove("data.dat", err);
//		}
//	};
//}