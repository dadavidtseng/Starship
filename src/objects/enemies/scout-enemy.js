//----------------------------------------------------------------------------------------------------
// scout-enemy.js
//----------------------------------------------------------------------------------------------------

//----------------------------------------------------------------------------------------------------
import {BotScoutInputComponent} from "../../components/input/bot-scout-input-component.js";
import {HorizontalMovementComponent} from "../../components/movement/horizontal-movement-component.js";
import {VerticalMovementComponent} from "../../components/movement/vertical-movement-component.js";
import * as CONFIG from '../../config.js';

//----------------------------------------------------------------------------------------------------
export class ScoutEnemy extends Phaser.GameObjects.Container
{
    #shipSprite;
    #shipEngineSprite;
    #inputComponent;
    #horizontalInputComponent;
    #verticalInputComponent;

    //------------------------------------------------------------------------------------------------
    constructor(scene, x, y)
    {
        super(scene, x, y, []);

        this.scene.add.existing(this);
        this.scene.physics.add.existing(this);
        this.body.setSize(24, 24);
        this.body.setOffset(-12, -12);
        this.setDepth(2);

        this.#shipSprite = scene.add.sprite(0, 0, 'scout', 0);
        this.#shipEngineSprite = scene.add.sprite(0, 0, 'scout_engine').setFlipY(true);
        this.#shipEngineSprite.play('scout_engine');
        this.add([this.#shipEngineSprite, this.#shipSprite]);

        this.#inputComponent = new BotScoutInputComponent(this);
        this.#verticalInputComponent = new VerticalMovementComponent(this,this.#inputComponent,CONFIG.ENEMY_SCOUT_MOVEMENT_VERTICAL_VELOCITY)
        this.#horizontalInputComponent = new HorizontalMovementComponent(this,this.#inputComponent,CONFIG.ENEMY_SCOUT_MOVEMENT_HORIZONTAL_VELOCITY)

        this.scene.events.on(Phaser.Scenes.Events.UPDATE, this.update, this);
        this.once(Phaser.GameObjects.Events.DESTROY, () =>
        {
            this.scene.events.off(Phaser.Scenes.Events.UPDATE, this.update, this);
        }, this);
    }

    //------------------------------------------------------------------------------------------------
    update(ts, dt)
    {
        this.#inputComponent.update();
        this.#verticalInputComponent.update();
        this.#horizontalInputComponent.update();
    }
}