//----------------------------------------------------------------------------------------------------
// fighter-enemy.js
//----------------------------------------------------------------------------------------------------

//----------------------------------------------------------------------------------------------------
import {BotFighterInputComponent} from "../../components/input/bot-fighter-input-component.js";
import {VerticalMovementComponent} from "../../components/movement/vertical-movement-component.js";
import {WeaponComponent} from "../../components/weapon/weapon-component.js";
import {HealthComponent} from "../../components/health/health-component.js";
import {ColliderComponent} from "../../components/collider/collider-component.js";
import {CUSTOM_EVENTS} from "../../components/events/event-bus-component.js";
import * as CONFIG from '../../config.js';

//----------------------------------------------------------------------------------------------------
export class FighterEnemy extends Phaser.GameObjects.Container
{
    #shipSprite;
    #shipEngineSprite;
    #inputComponent;
    #verticalInputComponent;
    #weaponComponent;
    #healthComponent;
    #colliderComponent;
    #eventBusComponent;
    #isInitialized;

    //------------------------------------------------------------------------------------------------
    constructor(scene, x, y)
    {
        super(scene, x, y, []);

        this.scene.add.existing(this);
        this.scene.physics.add.existing(this);
        this.body.setSize(24, 24);
        this.body.setOffset(-12, -12);
        this.setDepth(2);
        this.#isInitialized = false;

        this.#shipSprite = scene.add.sprite(0, 0, 'fighter', 0);
        this.#shipEngineSprite = scene.add.sprite(0, 0, 'fighter_engine').setFlipY(true);
        this.#shipEngineSprite.play('fighter_engine');
        this.add([this.#shipEngineSprite, this.#shipSprite]);

        this.scene.events.on(Phaser.Scenes.Events.UPDATE, this.update, this);
        this.once(Phaser.GameObjects.Events.DESTROY, () =>
        {
            this.scene.events.off(Phaser.Scenes.Events.UPDATE, this.update, this);
        }, this);
    }

    //------------------------------------------------------------------------------------------------
    get weaponGameObjectGroup()
    {
        return this.#weaponComponent.bulletGroup;
    }

    //------------------------------------------------------------------------------------------------
    get weaponComponent()
    {
        return this.#weaponComponent;
    }

    //------------------------------------------------------------------------------------------------
    get colliderComponent()
    {
        return this.#colliderComponent;
    }

    //------------------------------------------------------------------------------------------------
    get healthComponent()
    {
        return this.#healthComponent;
    }

    //------------------------------------------------------------------------------------------------
    get shipAssetKey()
    {
        return 'fighter';
    }

    //------------------------------------------------------------------------------------------------
    get shipDestroyedAnimationKey()
    {
        return 'fighter_destroy';
    }

    //------------------------------------------------------------------------------------------------
    init(eventBusComponent)
    {
        this.#eventBusComponent = eventBusComponent;
        this.#inputComponent = new BotFighterInputComponent();
        this.#verticalInputComponent = new VerticalMovementComponent(this, this.#inputComponent, CONFIG.ENEMY_FIGHTER_MOVEMENT_VERTICAL_VELOCITY)
        this.#weaponComponent = new WeaponComponent(this, this.#inputComponent, {
            maxCount: CONFIG.ENEMY_FIGHTER_BULLET_MAX_COUNT,
            yOffset: 10,
            interval: CONFIG.ENEMY_FIGHTER_BULLET_INTERVAL,
            speed: CONFIG.ENEMY_FIGHTER_BULLET_SPEED,
            lifespan: CONFIG.ENEMY_FIGHTER_BULLET_LIFESPAN,
            flipY: true,
        });
        this.#healthComponent = new HealthComponent(CONFIG.ENEMY_FIGHTER_HEALTH);
        this.#colliderComponent = new ColliderComponent(this.#healthComponent);
        this.#eventBusComponent.emit(CUSTOM_EVENTS.ENEMY_INIT, this);

        this.#isInitialized = true;
    }

    //------------------------------------------------------------------------------------------------
    reset()
    {
        this.setActive(true);
        this.setVisible(true);
        this.#healthComponent.reset();
        this.#verticalInputComponent.reset();
    }

    //------------------------------------------------------------------------------------------------
    update(ts, dt)
    {
        if(!this.#isInitialized)
        {
            return;
        }

        if (!this.active)
        {
            return;
        }

        if (this.#healthComponent.isDead)
        {
            this.setActive(false);
            this.setVisible(false);
            this.#eventBusComponent.emit(CUSTOM_EVENTS.ENEMY_DESTROY, this);
        }

        this.#inputComponent.update();
        this.#verticalInputComponent.update();
        this.#weaponComponent.update(dt);
    }
}