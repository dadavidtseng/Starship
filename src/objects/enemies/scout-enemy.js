//----------------------------------------------------------------------------------------------------
// scout-enemy.js
//----------------------------------------------------------------------------------------------------

//----------------------------------------------------------------------------------------------------
import {BotScoutInputComponent} from "../../components/input/bot-scout-input-component.js";
import {HorizontalMovementComponent} from "../../components/movement/horizontal-movement-component.js";
import {VerticalMovementComponent} from "../../components/movement/vertical-movement-component.js";
import {HealthComponent} from "../../components/health/health-component.js";
import {ColliderComponent} from "../../components/collider/collider-component.js";
import {CUSTOM_EVENTS} from "../../components/events/event-bus-component.js";
import * as CONFIG from '../../config.js';

//----------------------------------------------------------------------------------------------------
export class ScoutEnemy extends Phaser.GameObjects.Container
{
    #shipSprite;
    #shipEngineSprite;
    #inputComponent;
    #horizontalInputComponent;
    #verticalInputComponent;
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

        this.#shipSprite = scene.add.sprite(0, 0, 'scout', 0);
        this.#shipEngineSprite = scene.add.sprite(0, 0, 'scout_engine').setFlipY(true);
        this.#shipEngineSprite.play('scout_engine');
        this.add([this.#shipEngineSprite, this.#shipSprite]);

        this.scene.events.on(Phaser.Scenes.Events.UPDATE, this.update, this);
        this.once(Phaser.GameObjects.Events.DESTROY, () =>
        {
            this.scene.events.off(Phaser.Scenes.Events.UPDATE, this.update, this);
        }, this);
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
    init(eventBusComponent)
    {
        this.#eventBusComponent = eventBusComponent;
        this.#inputComponent = new BotScoutInputComponent(this);
        this.#verticalInputComponent = new VerticalMovementComponent(this, this.#inputComponent, CONFIG.ENEMY_SCOUT_MOVEMENT_VERTICAL_VELOCITY)
        this.#horizontalInputComponent = new HorizontalMovementComponent(this, this.#inputComponent, CONFIG.ENEMY_SCOUT_MOVEMENT_HORIZONTAL_VELOCITY)
        this.#healthComponent = new HealthComponent(CONFIG.ENEMY_SCOUT_HEALTH);
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
        this.#horizontalInputComponent.reset();
        this.#verticalInputComponent.reset();
        this.#inputComponent.startX = this.x;
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
        }

        this.#inputComponent.update();
        this.#verticalInputComponent.update();
        this.#horizontalInputComponent.update();
    }
}