# 🧬 BioArmada

**BioArmade** は、非弾幕系の縦スクロールシューティングゲームです。  
自作エンジンを用いて、基本的なSTG（シューティングゲーム）の遊びを再現しつつ、最新のECS設計や軽量エンジン構築手法を検証することを目的としています。

> 🛠️ **約1ヶ月半**でゲーム本体とエンジンを開発。

---

## ⚙️ 使用技術

- **言語**：C++20  
- **ライブラリ**：SDL3 / SDL3_image / SDL3_mixer / EnTT / nlohmann::json  
- **開発環境**：Visual Studio  
- **バージョン管理**：Git  
- **開発目的**：ECSベースの軽量エンジン構築とゲーム開発手法の検証  

---

## 🏗️ リポジトリ構成

プロジェクトには以下の主要ディレクトリが含まれます：

### 📂 Game

- **assets**: ゲーム内で利用するサウンドやスプライト画像ファイル、およびテスト用のアセット定義ファイルが含まれます。
  - `sound/`
  - `sprite/`
  - `test_assets.json`
- **game_data**: ステージやウェーブデータを管理するシーンデータを格納。
  - `scene_data/`
  - `wave_data/`
- **source**: ゲームロジックとエンジンのコード。
  - `app/`: アプリケーションコード
  - `engine/`: エンジンコード

### 📂 LICENSES

- 各ライブラリやアセットのライセンス情報を格納。
  - SDL関連ライセンス（`SDL3`, `SDL3_image`, `SDL3_mixer`, `SDL_ttf`）
  - nlohmann::json関連ライセンス（Apache, BSD, MIT, GPL-3.0など）
  - サウンドアセットのライセンス情報（`sound_LICENSES.txt`）

---

## 🧩 エンジン設計

本プロジェクトでは、**EngineContext** を中核としたモジュラー構造を採用しています。  
エンジン層とゲームロジック層を明確に分離し、**OOP**と**ECS**の利点を活用しています。

### エンジン層

- **EngineContext**：エンジンの中核モジュール  
- **Renderer**：描画処理を統括  
- **ResourceManager**：アセット管理（JSON定義）  
- **SceneManager**：シーン遷移と演出管理  

### ゲームロジック層

- `MovementSystem`：移動管理  
- `ShooterSystem`：弾生成・発射ロジック  
- `CollisionSystem`：当たり判定処理  
- `SpawnSystem`：敵出現管理  

---

## 🎮 ゲーム内容

- プレイヤーの操作：移動、ショット、敵撃破  
- ステージ進行：JSONデータによる外部制御  
- 柔軟な演出：システムの拡張を容易にする設計  

---

## 📜 ライセンス

本プロジェクトでは以下のライセンスを使用しています：

- ゲームエンジンおよびコード：MITライセンス（ライセンス詳細は `LICENSES/` を参照）  
- 使用ライブラリのライセンス情報：
  - SDL関連ライブラリ（`SDL3`, `SDL3_image`, `SDL3_mixer`, `SDL_ttf`）
  - nlohmann::json（Apache, BSD, MIT, GPL-3.0）

---

## 📩 コンタクト情報

- **作成者**: 倉本 星玖  
- **所属**: ポリテクセンター関西 IoTシステム科  
- **E-mail**: kuramoto.riku.zzz@gmail.com  

---

<p align="right">(<a href="#top">トップに戻る</a>)</p>  

---