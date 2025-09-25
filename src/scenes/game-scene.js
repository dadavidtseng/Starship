//----------------------------------------------------------------------------------------------------
// game-scene.js
//----------------------------------------------------------------------------------------------------

//----------------------------------------------------------------------------------------------------
import Phaser from '../lib/phaser.js';
import {Player} from "../objects/player.js";
import {ScoutEnemy} from "../objects/enemies/scout-enemy.js";
import {FighterEnemy} from "../objects/enemies/fighter-enemy.js";

//----------------------------------------------------------------------------------------------------
export class GameScene extends Phaser.Scene
{
    //------------------------------------------------------------------------------------------------
    constructor()
    {
        super({key: 'GameScene'});
    }

    //------------------------------------------------------------------------------------------------
    preload()
    {
    }

    //------------------------------------------------------------------------------------------------
    create()
    {
        const player = new Player(this);
        // const enemy = new ScoutEnemy(this, this.scale.width * 0.5, 20);
        const enemy = new FighterEnemy(this, this.scale.width * 0.5, 0);

        this.physics.add.overlap(player, enemy, (playerGameObject, enemyGameObject) =>
        {
            playerGameObject.colliderComponent.collideWithEnemyShip();
            enemyGameObject.colliderComponent.collideWithEnemyShip();
        });

        this.physics.add.overlap(player, enemy.weaponGameObjectGroup, (playerGameObject, projectileGameObject) =>
        {
            enemy.weaponComponent.destroyBullet(projectileGameObject);
            playerGameObject.colliderComponent.collideWithEnemyProjectile();

        })

        this.physics.add.overlap(enemy, player.weaponGameObjectGroup, (enemyGameObject, projectileGameObject) =>
        {
            player.weaponComponent.destroyBullet(projectileGameObject);
            enemyGameObject.colliderComponent.collideWithEnemyProjectile();

        });
    }
}
