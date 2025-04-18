#include <Novice.h>
#include <stdio.h>
#include <math.h>

const char kWindowTitle[] = "LD2B_04_ナガサワタカユキ_回転行列";

//==================================
// 構造体
//==================================

struct Vector3 {
	float x;
	float y;
	float z;
};

struct Matrix4x4 {
	float m[4][4];
};

//==================================
// 行列関数
//==================================

// X軸回転行列の作成
Matrix4x4 MakeRotateXMatrix(float radian) {
	Matrix4x4 matrix;
	matrix.m[0][0] = 1.0f; matrix.m[0][1] = 0.0f;         matrix.m[0][2] = 0.0f;          matrix.m[0][3] = 0.0f;
	matrix.m[1][0] = 0.0f; matrix.m[1][1] = cosf(radian);  matrix.m[1][2] = sinf(radian);  matrix.m[1][3] = 0.0f;
	matrix.m[2][0] = 0.0f; matrix.m[2][1] = -sinf(radian); matrix.m[2][2] = cosf(radian);  matrix.m[2][3] = 0.0f;
	matrix.m[3][0] = 0.0f; matrix.m[3][1] = 0.0f;         matrix.m[3][2] = 0.0f;          matrix.m[3][3] = 1.0f;
	return matrix;
}

// Y軸回転行列の作成
Matrix4x4 MakeRotateYMatrix(float radian) {
	Matrix4x4 matrix;
	matrix.m[0][0] = cosf(radian);  matrix.m[0][1] = 0.0f; matrix.m[0][2] = -sinf(radian); matrix.m[0][3] = 0.0f;
	matrix.m[1][0] = 0.0f;         matrix.m[1][1] = 1.0f; matrix.m[1][2] = 0.0f;          matrix.m[1][3] = 0.0f;
	matrix.m[2][0] = sinf(radian);  matrix.m[2][1] = 0.0f; matrix.m[2][2] = cosf(radian);  matrix.m[2][3] = 0.0f;
	matrix.m[3][0] = 0.0f;         matrix.m[3][1] = 0.0f; matrix.m[3][2] = 0.0f;          matrix.m[3][3] = 1.0f;
	return matrix;
}

// Z軸回転行列の作成
Matrix4x4 MakeRotateZMatrix(float radian) {
	Matrix4x4 matrix;
	matrix.m[0][0] = cosf(radian);  matrix.m[0][1] = sinf(radian);  matrix.m[0][2] = 0.0f; matrix.m[0][3] = 0.0f;
	matrix.m[1][0] = -sinf(radian); matrix.m[1][1] = cosf(radian);  matrix.m[1][2] = 0.0f; matrix.m[1][3] = 0.0f;
	matrix.m[2][0] = 0.0f;         matrix.m[2][1] = 0.0f;          matrix.m[2][2] = 1.0f; matrix.m[2][3] = 0.0f;
	matrix.m[3][0] = 0.0f;         matrix.m[3][1] = 0.0f;          matrix.m[3][2] = 0.0f; matrix.m[3][3] = 1.0f;
	return matrix;
}

// 行列の積
Matrix4x4 Multiply(const Matrix4x4& m1, const Matrix4x4& m2) {
	Matrix4x4 result;
	for (int i = 0; i < 4; ++i) {
		for (int j = 0; j < 4; ++j) {
			result.m[i][j] = 0.0f;
			for (int k = 0; k < 4; ++k) {
				result.m[i][j] += m1.m[i][k] * m2.m[k][j];
			}
		}
	}
	return result;
}

//==================================
// 描画関数
//==================================

void MatrixScreenPrint(int x, int y, const Matrix4x4& matrix, const char* label) {
	Novice::ScreenPrintf(x, y, "%s", label);
	for (int i = 0; i < 4; ++i) {
		Novice::ScreenPrintf(x, y + (i + 1) * 20, "  %.2f  %.2f  %.2f  %.2f",
			matrix.m[i][0], matrix.m[i][1], matrix.m[i][2], matrix.m[i][3]);
	}
}

const int kRowHeight = 20;

// Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {

	// ライブラリの初期化
	Novice::Initialize(kWindowTitle, 1280, 720);

	// キー入力結果を受け取る箱
	char keys[256] = { 0 };
	char preKeys[256] = { 0 };

	// 回転角度の初期化
	Vector3 rotate = { 0.4f, 1.43f, -0.8f };

	// 各軸の回転行列
	Matrix4x4 rotateXMatrix = MakeRotateXMatrix(rotate.x);
	Matrix4x4 rotateYMatrix = MakeRotateYMatrix(rotate.y);
	Matrix4x4 rotateZMatrix = MakeRotateZMatrix(rotate.z);

	// 全体の回転行列
	Matrix4x4 rotateXYZMatrix = Multiply(rotateXMatrix, Multiply(rotateYMatrix, rotateZMatrix));

	// ウィンドウの×ボタンが押されるまでループ
	while (Novice::ProcessMessage() == 0) {
		// フレームの開始
		Novice::BeginFrame();

		// キー入力を受け取る
		memcpy(preKeys, keys, 256);
		Novice::GetHitKeyStateAll(keys);

		///
		/// ↓更新処理ここから
		///
		// 今回は初期値の表示なので更新処理は特にありません
		///
		/// ↑更新処理ここまで
		///

		///
		/// ↓描画処理ここから
		///

		MatrixScreenPrint(0, 0, rotateXMatrix, "rotateXMatrix");
		MatrixScreenPrint(0, kRowHeight * 5, rotateYMatrix, "rotateYMatrix");
		MatrixScreenPrint(0, kRowHeight * 10, rotateZMatrix, "rotateZMatrix"); // Z軸をY軸の上に配置
		MatrixScreenPrint(0, kRowHeight * 15, rotateXYZMatrix, "rotateXYZMatrix"); // 全体回転行列をY軸の下に配置

		///
		/// ↑描画処理ここまで
		///

		// フレームの終了
		Novice::EndFrame();

		// ESCキーが押されたらループを抜ける
		if (preKeys[DIK_ESCAPE] == 0 && keys[DIK_ESCAPE] != 0) {
			break;
		}
	}

	// ライブラリの終了
	Novice::Finalize();
	return 0;
}