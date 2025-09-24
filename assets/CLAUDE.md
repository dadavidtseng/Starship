[Root Directory](../../CLAUDE.md) > **assets**

# Assets Module

## Module Responsibilities
Game asset management including sprites, audio files, background images, and JSON configuration for asset loading and animations.

## Entry and Startup
- **Asset Pack**: `data/assets.json` - Primary asset loading configuration
- **Animation Config**: `data/animations.json` - Sprite animation definitions
- **Loading Integration**: Loaded via `this.load.pack()` in game scenes

## External Interfaces
- **Phaser Asset Loader**: JSON pack format for batch asset loading
- **Audio System**: WAV audio files for sound effects and background music
- **Sprite System**: PNG sprite sheets with frame configurations

## Key Dependencies and Configuration
- **Phaser Pack Format**: JSON structure for asset definitions
- **Image Formats**: PNG sprite sheets with defined frame dimensions
- **Audio Formats**: WAV audio files for cross-browser compatibility
- **Attribution**: License files for asset creators

## Data Models
### Asset Pack Structure
```javascript
{
  "path": "assets/images/foozle",
  "files": [
    {
      "type": "spritesheet",
      "key": "ship",
      "url": "ship.png",
      "frameConfig": { "frameWidth": 48, "frameHeight": 48 }
    }
  ]
}
```

### Animation Configuration
```javascript
{
  "key": "ship_engine_thruster",
  "frameRate": 12,
  "repeat": -1,
  "assetKey": "ship_engine_thruster"
}
```

## File Structure
```
assets/
├── data/
│   ├── assets.json      # Asset pack configuration
│   └── animations.json  # Animation definitions
├── images/
│   ├── foozle/         # Spacecraft and UI sprites
│   │   ├── ship.png    # Player ship (48x48 frames)
│   │   ├── scout.png   # Enemy scout ship
│   │   ├── fighter.png # Enemy fighter ship
│   │   ├── bullet.png  # Projectile sprites (32x32 frames)
│   │   └── bg*.png     # Animated backgrounds (640x360 frames)
│   └── piiixl/
│       └── bg.gif      # Static space background
├── audio/
│   └── ansimuz/
│       ├── explosion.wav    # Explosion sound effect
│       ├── hit.wav         # Impact sound
│       ├── shot_1.wav      # Weapon firing sound 1
│       ├── shot_2.wav      # Weapon firing sound 2
│       └── space_asteroids.wav # Background music
└── js/
    └── phaser.min.js   # Phaser 3 framework library
```

## Asset Categories

### Visual Assets
- **Player Ships**: Main ship with engine effects and thrusters
- **Enemy Ships**: Scout and fighter classes with destruction animations
- **Projectiles**: Animated bullet sprites
- **Backgrounds**: Multi-layer animated space backgrounds
- **Effects**: Engine trails, explosions, destruction sequences

### Audio Assets
- **Sound Effects**: Weapon firing, explosions, impacts
- **Background Music**: Space ambient soundtrack
- **Format**: WAV files for broad browser compatibility

## Animation Definitions
Configured animations include:
- **Engine Effects**: Looping thruster animations
- **Projectiles**: Spinning bullet animations
- **Destruction**: Non-repeating explosion sequences
- **Backgrounds**: Slow-scrolling space environments

## Asset Attribution and Licensing
- **Foozle**: Void series asset packs (ships, backgrounds, projectiles)
- **Ansimuz**: Warped Space Shooter audio pack
- **Piiixl**: Space background artwork
- **Licensing**: Individual license files included with asset groups

## Configuration Integration
Assets are loaded via Phaser's pack system:
1. `assets.json` defines asset locations and sprite sheet configurations
2. `animations.json` defines animation parameters and frame sequences
3. Game scenes reference assets by their configured keys
4. Frame dimensions and animation settings are automatically applied

## FAQ
**Q: How do I add new sprites?**
A: Add sprite files to appropriate subdirectory, update `assets.json` with file definition and frame configuration.

**Q: What's the difference between image and spritesheet types?**
A: Images are single static assets, spritesheets contain multiple frames with defined dimensions.

**Q: How are animations configured?**
A: Via `animations.json` with frameRate, repeat settings, and asset key references.

## Related File List
- `../src/scenes/game-scene.js` - Asset loading via pack system
- `../src/main.js` - Game configuration including asset paths
- `../index.html` - Static background image reference

## Changelog
- **2025-09-24**: Initial documentation created, catalogued all current assets and configurations