[Root Directory](../../CLAUDE.md) > **src**

# Source Code Module

## Module Responsibilities
Core game source code containing scenes, framework integration, type definitions, and game logic implementation.

## Entry and Startup
- **Main Entry**: `main.js` - Game initialization, Phaser configuration, scene registration
- **Scene Entry**: `scenes/game-scene.js` - Primary game scene with preload/create/update lifecycle

## External Interfaces
- **Game Configuration**: Phaser game instance with canvas rendering, arcade physics
- **Asset Loading**: Integration with `assets/data/assets.json` pack loading
- **Browser DOM**: Game container mounting to `#game-container` element

## Key Dependencies and Configuration
- **Phaser 3**: Game engine accessed via `lib/phaser.js` (window.Phaser wrapper)
- **ES6 Modules**: Native browser module system
- **Physics**: Arcade physics with gravity disabled for space environment
- **Rendering**: Canvas renderer with pixel art settings, 450x640 resolution

## Data Models
Defined in `types/typedef.js`:
- **AnimationConfig**: Animation definition structure
- **AssetJson**: Asset pack configuration format
- **ColliderComponent**: Entity collision handling interface
- **HealthComponent**: Entity health/damage system interface
- **BaseEnemy/Enemy**: Enemy ship component composition

## File Structure
```
src/
├── main.js              # Game initialization and configuration
├── lib/
│   └── phaser.js        # Phaser 3 framework wrapper
├── scenes/
│   └── game-scene.js    # Main game scene implementation
└── types/
    ├── typedef.js       # JSDoc type definitions
    └── phaser.d.ts     # TypeScript declarations for Phaser
```

## Testing and Quality
- **Debug Mode**: Arcade physics debugging enabled in main.js
- **Type Safety**: JSDoc annotations with checkJs enabled in jsconfig.json
- **Development**: Live reload support via local web server

## Current Implementation Status
- ✅ Game initialization and scene setup
- ✅ Asset pack loading system
- ✅ Basic scene structure (preload/create)
- 🔄 Player ship display (recently added ship sprite)
- ⏳ Player input handling
- ⏳ Enemy spawning and behavior
- ⏳ Projectile system
- ⏳ Collision detection
- ⏳ Animation system integration

## Key Configuration Points
- **Canvas Size**: 450x640 pixels (portrait orientation)
- **Physics**: Arcade physics with zero gravity
- **Scale Mode**: HEIGHT_CONTROLS_WIDTH with auto-centering
- **Asset Loading**: Pack-based loading from JSON configuration

## FAQ
**Q: Why is physics debugging enabled?**
A: Development mode for collision detection visualization during game development.

**Q: How are assets loaded?**
A: Via Phaser's pack loading system using `assets/data/assets.json` configuration.

**Q: What's the scene lifecycle?**
A: Standard Phaser pattern: preload() → create() → update() (game loop)

## Related File List
- `../index.html` - HTML entry point and DOM container
- `../assets/data/assets.json` - Asset pack configuration
- `../assets/data/animations.json` - Animation definitions
- `../jsconfig.json` - JavaScript/TypeScript compiler configuration

## Changelog
- **2025-09-24**: Initial documentation created, noted recent ship sprite addition to game-scene.js