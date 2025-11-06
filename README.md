# 🧬 BioArmada

**BioArmada** は、非弾幕系の縦スクロール・シューティングゲームです。  
自作の軽量 2D エンジン上で、ECS 設計（Entity-Component-System）を用いた拡張しやすい構造を採用し、ゲームプレイとエンジン設計の両面を検証しています。

> 🛠️ 約1ヶ月半で、ゲーム本体とエンジンを開発

---

## ⚙️ 使用技術

- 言語: C++20
- ライブラリ: SDL3 / SDL3_image / SDL3_mixer / EnTT / nlohmann::json
- 開発環境: Visual Studio
- バージョン管理: Git
- 目的: ECS ベースの軽量エンジン構築と、データ駆動のゲーム開発手法の検証

---

## 🎮 操作方法

- 矢印キー: 移動
- Z キー: ショット
- Space: タイトル/ゲームオーバーのメニュー決定

---

## 🏗️ リポジトリ構成

プロジェクトは以下の主要ディレクトリで構成されています。

- Game/
  - assets/ … 画像/アニメ/サウンド等のアセットと定義ファイル
    - sprite/
    - sound/
    - test_assets.json
  - game_data/ … シーン・ウェーブ等のデータ駆動 JSON
    - scene_data/ … タイトル/ゲーム/ゲームオーバー
    - wave_data/ … 敵出現/演出のウェーブ定義
  - source/
    - app/ … ゲーム固有のロジック（シーン/システム/コンポーネント/イベント）
    - engine/ … エンジン層（コア/レンダリング/サウンド/マネージャ/共通システム/ユーティリティ）

- LICENSES/ … 使用ライブラリやアセットのライセンス情報

---

## 🧩 データ駆動の流れ

1) エンジン初期化時に [assets/test_assets.json](Game/assets/test_assets.json) を読み込み  
2) シーン切替時に scene_data の JSON をロード  
3) ゲーム中は wave_data に従って敵群/演出を生成  
4) EntityFactory が JSON の `entity_type` ごとに適切なエンティティを構築  
5) 各システムがコンポーネントに従って毎フレーム更新・描画・音声再生

代表的なデータ:
- [title_scene_data.json](Game/game_data/scene_data/title_scene_data.json)
- [game_scene_data.json](Game/game_data/scene_data/game_scene_data.json)
- [game_over_scene_data.json](Game/game_data/scene_data/game_over_scene_data.json)
- [first_wave_data.json](Game/game_data/wave_data/first_wave_data.json) / [second_wave_data.json](Game/game_data/wave_data/second_wave_data.json) / [third_wave_data.json](Game/game_data/wave_data/third_wave_data.json)
- [boss_battle_effect_wave_data.json](Game/game_data/wave_data/boss_battle_effect_wave_data.json) / [boss_wave_data.json](Game/game_data/wave_data/boss_wave_data.json)

---

## 🖥️ シーン構成（app/scene）

- TitleScene
  - タイトル UI のハイライト/決定と BGM 再生、ゲームへ遷移
- GameScene
  - Wave（敵出現/演出）の開始・更新・終了をステートマシンで制御
  - プレイヤ/敵/弾/スコア/UI/衝突/演出など主要システムを登録
- GameOverScene
  - リトライ（Continue）/タイトルへ戻るのメニューと遷移

それぞれのシーンは `engine/scene/Scene` を継承し、  
initialize() 内の流れ（データロード → エンティティ生成 → システム追加 → イベント購読）を共通化しています。

---

## 🛠️ ゲームロジック・システム（app/systems）

- Movement/向き/境界
  - PlayerMovementSystem: 滑らかな移動と加減速
  - FacingSystem: ターゲットや進行方向へ回頭
  - ScreenBoundsSystem: 画面内外の位置状態（BoundingBox）を更新
  - OutOfScreenSystem: 画面外オブジェクトの削除・位置補正
  - TransformLinkSystem: 親 Transform に子を追従、親喪失で子を破棄

- 攻撃/衝突/ダメージ
  - ShootSystem: プレイヤ/敵（バースト/ダブル/レーザー）の発射ロジック
  - CollisionSystem: グリッド分割による高速衝突判定、HitEvent を発火
  - HitResolutionSystem: ダメージ/死亡/無敵・演出/ライフUI更新/SE再生を統合処理

- UI/演出/スコア/ゲームオーバー
  - MenuSystem: タイトル/ゲームオーバーの選択・決定
  - HighlightSystem: 選択 UI のハイライト切替
  - SpriteBrinkSystem: 点滅/被ダメ時の色変化演出
  - LifeUISystem: ライフ UI（RepeatSprite）更新
  - ScoreSystem: 敵撃破スコア加算と表示更新
  - GameOverSystem: ゲームオーバー後にフェード遷移

- 入力
  - InputSystem: プレイヤ入力（矢印/Z）とメニュー入力（Up/Down/Space） → イベント発行

---

## 🧠 イベント（app/event, engine/events）

イベントは `entt::dispatcher` を EventListener でラップし、安全に connect/trigger/enqueue できます。

- ゲーム側（例）
  - ShootEvent / LaserShootEvent / HitEvent / DeadEvent / AppendDeadEffectEvent / AppendOverrayFadeEvent
  - AppendChargeEvent / ChargeEndEvent / GameOverEvent / PlayDamageSEEvent
  - HighlightEvent / KeyDownEvent / MenuButtonEvent / PlayerLifeChangedEvent

- エンジン側（例）
  - FadeOutStartEvent / FadeInEndEvent / FadeOutEndEvent / FadeSetAlphaEvent
  - FadeRenderLayerChangeEvent / StopBGMEvent / SpriteAnimEndEvent / QuitEvent

---

## 🧱 エンジン構成

- Core（engine/core）
  - EngineContext: SDL 初期化、各 Manager の生成、フレーム更新の順序管理
  - GameTimer: delta_time / FPS / game_speed 管理
  - 依存注入（SceneDependencies）により、Scene へ registry/dispatcher/resource/input/scene/system を提供

- Managers（engine/managers）
  - ResourceManager: Sprite/SpriteAnim/Sound/Font の読み込み・キャッシュ
  - SceneManager: フェードを介したシーン遷移（FadeSystem と連携）
  - SystemManager: priority 順でシステム更新、固定システム（Sound/SpriteAnim/Render）を後段で実行
  - InputManager: フレーム単位のキー状態管理（KeyDown/Press/Up、AnyKey）

- Rendering（engine/rendering）
  - Renderer: SDL_Renderer の RAII ラッパ、テクスチャ/文字列描画
  - Resource/Loader: Sprite/SpriteAnim/Font のリソース表現とローダ群

- Sound（engine/sound）
  - SoundMixer: SDL_mixer(3) の薄いラッパ。SE 複数トラック/BGM トラック、フェード停止時の次曲予約
  - Resource/Loader: MIX_Audio のロード・所有

- Components（engine/components）
  - Transform/Velocity/Sprite/SpriteAnim/Text/RepeatSprite
  - Fade/BackgroundMusic/SoundEffect
  - 各種 Render レイヤタグ / Update タグ / 直接制御タグ

- Common Systems（engine/systems）
  - Movement / Rotate / Scale / Fade / SpriteAnimation / Sound / SpriteRender

---

## 🧾 アセットとデータ（data & assets）

- アセット定義: [Game/assets/test_assets.json](Game/assets/test_assets.json)
  - Sprites: `name` → 画像ファイル
  - SpriteAnims: フレーム矩形/時間/タイプ（Loop/Once/Manual）
  - Sounds: `name` → 音声ファイル
  - FontDatas: テクスチャフォントの 1 文字矩形配列

- シーン・ウェーブ
  - scene_data: エンティティ群（背景/UI/テキスト/メニュー/プレイヤ/BGM など）を JSON で定義
  - wave_data: 敵の出現パターン（wanderer/shooter/boss）や演出（SE/点滅）を JSON で定義
  - EntityFactory が `entity_type` ごとに `createXxx()` を呼び分け、コンポーネント付与を行います

---

## 🚀 ビルドと実行（概要）

- 依存: SDL3 / SDL3_image / SDL3_mixer / EnTT / nlohmann::json
- Visual Studio でソリューションを開きビルド
- 実行すると EngineContext が初期化され、Title → Game → GameOver のフローで動作します

（開発環境や依存のセットアップは、ご利用の環境に合わせて調整してください）

---

## 📜 ライセンス

- コード: MIT（詳細は LICENSES/ を参照）
- ライブラリ:
  - SDL 関連（SDL3 / SDL3_image / SDL3_mixer / SDL_ttf）
  - nlohmann::json（Apache, BSD, MIT, GPL-3.0）
- サウンド/画像などアセットのライセンスは各ファイルに準拠

---

## 📩 コンタクト

- 作成者: 倉本 星玖
- 所属: ポリテクセンター関西 IoTシステム科
- E-mail: kuramoto.riku.zzz@gmail.com

<p align="right">(<a href="#top">トップに戻る</a>)</p>
