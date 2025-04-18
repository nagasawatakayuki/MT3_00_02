#include <Novice.h>
#include <stdio.h>
#include <math.h>

const char kWindowTitle[] = "LD2B_04_ナガサワタカユキ_行列計算";

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

// 平行移動行列の作成
Matrix4x4 MakeTranslateMatrix(const Vector3& translate) {
	Matrix4x4 matrix;
	matrix.m[0][0] = 1.0f; matrix.m[0][1] = 0.0f; matrix.m[0][2] = 0.0f; matrix.m[0][3] = 0.0f;
	matrix.m[1][0] = 0.0f; matrix.m[1][1] = 1.0f; matrix.m[1][2] = 0.0f; matrix.m[1][3] = 0.0f;
	matrix.m[2][0] = 0.0f; matrix.m[2][1] = 0.0f; matrix.m[2][2] = 1.0f; matrix.m[2][3] = 0.0f;
	matrix.m[3][0] = translate.x; matrix.m[3][1] = translate.y; matrix.m[3][2] = translate.z; matrix.m[3][3] = 1.0f;
	return matrix;
}

// 拡大縮小行列の作成
Matrix4x4 MakeScaleMatrix(const Vector3& scale) {
	Matrix4x4 matrix;
	matrix.m[0][0] = scale.x; matrix.m[0][1] = 0.0f; matrix.m[0][2] = 0.0f; matrix.m[0][3] = 0.0f;
	matrix.m[1][0] = 0.0f; matrix.m[1][1] = scale.y; matrix.m[1][2] = 0.0f; matrix.m[1][3] = 0.0f;
	matrix.m[2][0] = 0.0f; matrix.m[2][1] = 0.0f; matrix.m[2][2] = scale.z; matrix.m[2][3] = 0.0f;
	matrix.m[3][0] = 0.0f; matrix.m[3][1] = 0.0f; matrix.m[3][2] = 0.0f; matrix.m[3][3] = 1.0f;
	return matrix;
}

//==================================
// 変換関数
//==================================

// ベクトルと行列の積 (座標変換)
Vector3 Transform(const Vector3& vector, const Matrix4x4& matrix) {
	Vector3 result;
	result.x = vector.x * matrix.m[0][0] + vector.y * matrix.m[1][0] + vector.z * matrix.m[2][0] + 1.0f * matrix.m[3][0];
	result.y = vector.x * matrix.m[0][1] + vector.y * matrix.m[1][1] + vector.z * matrix.m[2][1] + 1.0f * matrix.m[3][1];
	result.z = vector.x * matrix.m[0][2] + vector.y * matrix.m[1][2] + vector.z * matrix.m[2][2] + 1.0f * matrix.m[3][2];
	return result;
}

//==================================
// 描画関数
//==================================

void MatrixScreenPrint(int x, int y, const Matrix4x4& matrix, const char* label) {
	Novice::ScreenPrintf(x, y, "%s", label);
	for (int i = 0; i < 4; ++i) {
		Novice::ScreenPrintf(x, y + (i + 1) * 20, "[%.2f, %.2f, %.2f, %.2f]",
			matrix.m[i][0], matrix.m[i][1], matrix.m[i][2], matrix.m[i][3]);
	}
}

void VectorScreenPrint(int x, int y, const Vector3& vector, const char* label) {
	Novice::ScreenPrintf(x, y, "%s [%.2f, %.2f, %.2f]", label, vector.x, vector.y, vector.z);
}

const int kRowHeight = 20;

// Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {

	// ライブラリの初期化
	Novice::Initialize(kWindowTitle, 1280, 720);

	// キー入力結果を受け取る箱
	char keys[256] = { 0 };
	char preKeys[256] = { 0 };

	// 一枚目の写真の初期設定の数値
	Vector3 translate = { 0.71f, 0.67f, 0.60f };
	Vector3 scale = { 1.00f, 1.00f, 1.00f }; // ← scale の初期値も写真に合わせてみた
	Vector3 point = { 0.00f, 0.00f, 0.00f }; // ← point の初期値も写真に合わせてみた

	Matrix4x4 translateMatrix = MakeTranslateMatrix(translate);
	Matrix4x4 scaleMatrix = MakeScaleMatrix(scale);
	Matrix4x4 transformMatrix;

	// 完成イメージの順序に合わせて スケール -> 移動 で合成
	for (int i = 0; i < 4; ++i) {
		for (int j = 0; j < 4; ++j) {
			transformMatrix.m[i][j] = 0.0f;
			for (int k = 0; k < 4; ++k) {
				transformMatrix.m[i][j] += scaleMatrix.m[i][k] * translateMatrix.m[k][j];
			}
		}
	}

	Vector3 transformed = Transform(point, transformMatrix);

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
		// キー入力による数値操作処理はカット
		///
		/// ↑更新処理ここまで
		///

		///
		/// ↓描画処理ここから
		///
		
		VectorScreenPrint(0, 0, transformed, "transformed");
		MatrixScreenPrint(0, kRowHeight * 2, translateMatrix, "translateMatrix");
		MatrixScreenPrint(0, kRowHeight * 7, scaleMatrix, "scaleMatrix");
		MatrixScreenPrint(0, kRowHeight * 12, transformMatrix, "transformMatrix");

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