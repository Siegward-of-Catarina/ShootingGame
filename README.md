# 🧬 BioArmada (C++20 / SDL3 / EnTT)

縦スクロール型の非弾幕シューティング。  
自作軽量エンジン上で ECS(EnTT) によりゲームロジックを構築。アセット/シーン/ウェーブを JSON でデータ駆動化。

- 役割: 個人開発（設計/実装/アセットパイプライン）
- 主要技術: C++20, SDL3, SDL_image, SDL_ttf, EnTT, nlohmann::json
- 開発環境: Visual Studio 2022 / Windows x64
- リポジトリ: https://github.com/Siegward-of-Catarina/ShootingGame

---

## ハイライト

- ECS設計の実装経験：エンジン層とゲーム層を分離し、責務と依存を明確化
- データ駆動設計：JSONでアセット/シーン/ウェーブ/エンティティを定義、`EntityFactory`で生成
- 当たり判定の効率化：100px/セル × 6x8 の空間分割で近傍 3×3 セルのみ探索
- フェーズ設計：イベント駆動で被弾→ダメージ→撃破演出→削除を非同期処理
- 実装の見どころ: フェード演出、被ダメ赤点滅、波状出現、蛇行移動など

---

## 画面と操作

- タイトル: 上下で選択、Spaceで決定（Start/Exit）
- ゲーム: 矢印キーで移動、Zでショット
- ウィンドウ: 600×800 固定

---

## ビルドと実行（Visual Studio 2022）

1) レポジトリを開く（`Game/Game.vcxproj` またはフォルダーを開く）
2) 構成: x64 / Debug または Release
3) スタートアップを `Game` に設定（__Set as Startup Project__）
4) ビルド（__Build Solution__ / Ctrl+Shift+B）
5) 実行（__Start Debugging__ / F5 または __Start Without Debugging__ / Ctrl+F5）

依存は同梱済み（追加インストール不要）。

---

## 実装構成（抜粋）

- エンジン層（OOP）
  - `EngineContext`: Renderer/Resource/Input/Scene/System を初期化・保持
  - システム: `MovementSystem`, `RotateSystem`, `ScaleSystem`, `FadeSystem`
  - フェード: 専用エンティティ + `FadeSystem` でシーン遷移演出
- ゲーム層（ECS）
  - シーン: `title_scene`, `test_scene`（`Scene`基底に依存注入）
  - ファクトリ: `EntityFactory`（Player/Enemy/Bullet/UI/Text/背景などを JSON から生成）
  - システム:
    - 入力: `InputSystem` → `PlayerInput`/`TitleInput` を更新
    - 射撃: `ShootSystem` → `ShootEvent` → 弾生成（`EntityFactory::createBullet`）
    - 移動: `PlayerMovementSystem` / `EnemyMovementSystem`（蛇行 `SerpentineMovement`）
    - 当たり判定: `CollisionSystem`（円形近似 + 6×8グリッドで近傍探索）
    - ヒット解決: `HitResolutionSystem`（HP/ATK減算、`AppedDeadEffectEvent` 発火）
    - 演出: `SpriteBrinkSystem`（被ダメ赤点滅、点滅UI）
    - 画面外/寿命: `OutOfScreenSystem` ほか
- データ駆動
  - アセット: `assets/test_assets.json`（スプライト/アニメ/フォント）
  - シーン: `game_data/scene_data/game_scene_data.json`
  - ウェーブ: `waves`（クラス名 + データパスでロード）
  - 例: `en_wanderer_array` による一括生成、`movement.serpentine` パラメトリック移動

---

## 技術的課題と解決

- 当たり判定の低コスト化
  - 問題: 全探索は弾×敵で O(NM) に増大
  - 解法: 100px グリッドで空間分割し、弾は近傍 3×3 セルのみ探索（9セル）
  - 実装: `CollisionSystem`（セルIDを線形化し `unordered_map` でバケット化）
- 被弾から撃破までの責務分離
  - `HitEvent`（衝突検出）→ `HitResolutionSystem`（HP処理/演出付与）→ `DeadEvent`（破棄/エフェクト）
  - イベント駆動で結合度を抑制し、演出の差し替え容易化

---

## コード参照（導線）

- エンティティ生成: `Game/source/app/entity_factory.cpp`
- 衝突とヒット解決: `Game/source/app/systems/collision_system.cpp`, `hit_resolution_system.cpp`
- 被ダメ演出: `Game/source/app/systems/sprite_brink_system.cpp`, `components/damage.hpp`
- エントリ/起動: `Game/source/app/game.cpp`, `main.cpp`

---

## 今後の拡張案

- 弾幕/パターンエディタ、敵AIのスクリプト化（Lua/JSON強化）
- ECSシステムの並列化、プロファイリングに基づくホットスポット最適化
- 当たり判定のAABB/OBB併用、広域空間分割（Quadtree）検証
- ツールチェーン（Wave/Stage 可視編集ツール）

---

## メディア

- トレーラー: <リンクを挿入>
- スクリーンショット: <リンクを挿入>

---

## ライセンス

同梱ライブラリは各ライセンスに従います（`/LICENSES`）。
- SDL3, SDL_image, SDL_ttf（Zlib）
- nlohmann/json（Apache-2.0）
- EnTT（配布形態に準拠）

本体/自作エンジン部のライセンスはリポジトリの LICENSE を参照（未設定の場合は追記してください）。
# その他

* 作成者 倉本　星玖
* 所属　ポリテクセンター関西　Iotシステム科
* E-mail kuramoto.riku.zzz@gmail.com

<p align="right">(<a href="#top">トップへ</a>)</p>
