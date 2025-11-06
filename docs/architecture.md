# BioArmada アーキテクチャ図

このドキュメントでは、BioArmadaゲームのアーキテクチャを説明します。

## 凡例

- **実線矢印 (→)**: 保持 (所有権を持つ)
- **点線矢印 (-.->)**: 使用 (一時的な参照)
- **太線矢印 (==>)**: 参照 (永続的な参照)

---

## システム全体構成

BioArmadaは、カスタム2Dゲームエンジンの上に構築された縦スクロールシューティングゲームです。

```mermaid
graph TB
    subgraph "Application Layer"
        Scenes[Scenes]
        AppSystems[Game Systems]
    end
    
    subgraph "Engine Layer"
        EC[EngineContext]
        Managers[Managers]
        EngineSystems[Engine Systems]
    end
    
    subgraph "External Libraries"
        SDL[SDL3]
        EnTT[EnTT]
    end
    
    Scenes ==refers==> Managers
    Scenes -.uses.-> AppSystems
    
    EC -->|owns| Managers
    EC -->|owns| SDL
    EC -->|owns| EnTT
    
    Managers -->|owns| EngineSystems
    Managers -->|owns| AppSystems
```

---

## エンジン層アーキテクチャ

エンジン層は、ゲームの基盤となる汎用的な機能を提供します。

```mermaid
graph TB
    subgraph "EngineContext"
        EC[EngineContext<br/>コンテキスト管理]
    end
    
    subgraph "Managers"
        SM[SceneManager]
        SYM[SystemManager]
        RM[ResourceManager]
        IM[InputManager]
    end
    
    subgraph "Core Components"
        R[Renderer]
        SMX[SoundMixer]
        GT[GameTimer]
    end
    
    subgraph "Loaders"
        SL[SpriteLoader]
        SAL[SpriteAnimLoader]
        FL[FontLoader]
        SndL[SoundLoader]
    end
    
    EC -->|owns| SM
    EC -->|owns| SYM
    EC -->|owns| RM
    EC -->|owns| IM
    EC -->|owns| R
    EC -->|owns| SMX
    EC -->|owns| GT
    
    RM -.uses.-> SL
    RM -.uses.-> SAL
    RM -.uses.-> FL
    RM -.uses.-> SndL
    
    SL -.uses.-> R
    SndL -.uses.-> SMX
```

---

## アプリケーション層アーキテクチャ

アプリケーション層は、ゲーム固有のロジックとコンテンツを実装します。

```mermaid
graph TB
    subgraph "Scenes"
        TS[TitleScene]
        GS[GameScene]
        GOS[GameOverScene]
        GCS[GameClearScene]
    end
    
    subgraph "Game Systems"
        IS[InputSystem]
        PMS[PlayerMovementSystem]
        EMS[EnemyMovementSystem]
        SS[ShootSystem]
        CS[CollisionSystem]
        HRS[HitResolutionSystem]
    end
    
    subgraph "Wave Management"
        WM[WaveManager]
        EF[EntityFactory]
    end
    
    GS -.uses.-> WM
    GS -.uses.-> PMS
    GS -.uses.-> SS
    GS -.uses.-> CS
    
    WM -.uses.-> EF
```

---

## ECS アーキテクチャ

BioArmadaは、EnTTライブラリを使用したEntity-Component-System（ECS）パターンを採用しています。

```mermaid
graph LR
    subgraph "ECS Core"
        E[Entity<br/>ID]
        C[Component<br/>Data]
        S[System<br/>Logic]
    end
    
    subgraph "EnTT"
        R[registry]
        D[dispatcher]
    end
    
    E --> R
    C --> R
    R -.uses.-> S
    S -.triggers.-> D
```

---

## データ駆動設計

JSONファイルによるデータ駆動設計を採用しています。

```mermaid
graph TB
    subgraph "Data Files"
        SD[scene_data/*.json]
        WD[wave_data/*.json]
        AD[assets/test_assets.json]
    end
    
    subgraph "Engine"
        RM[ResourceManager]
        SM[SceneManager]
    end
    
    subgraph "Game"
        Scene[Scene]
        WM[WaveManager]
        EF[EntityFactory]
    end
    
    AD -.loads.-> RM
    SD -.loads.-> Scene
    WD -.loads.-> WM
    
    Scene -.uses.-> EF
    WM -.uses.-> EF
    EF -.uses.-> RM
```

---

## まとめ

### 設計原則

1. **レイヤード・アーキテクチャ**: Engine層とApp層の明確な分離
2. **ECS パターン**: EnTTによる効率的なエンティティ管理
3. **イベント駆動**: 疎結合なシステム間通信
4. **データ駆動設計**: JSONによるコンテンツの外部化

### 技術スタック

- **言語**: C++20
- **ECS**: EnTT
- **グラフィックス**: SDL3 / SDL3_image
- **オーディオ**: SDL3_mixer
- **データ**: nlohmann::json

### ディレクトリ構成

```
Game/
├── assets/              # 画像、音声、フォント
├── game_data/           # シーン、ウェーブデータ
└── source/
    ├── app/             # ゲーム固有実装
    │   ├── components/  # ゲームコンポーネント
    │   ├── events/      # ゲームイベント
    │   ├── scene/       # シーン実装
    │   ├── systems/     # ゲームシステム
    │   └── waves/       # ウェーブ管理
    └── engine/          # エンジン実装
        ├── components/  # エンジンコンポーネント
        ├── core/        # コアシステム
        ├── managers/    # 各種マネージャ
        ├── rendering/   # 描画システム
        ├── sound/       # サウンドシステム
        └── systems/     # エンジンシステム
```
