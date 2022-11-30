
# TownCount Remake
Towncount is a 2D Metroidvania game, the main character can use his unique ability to explore the area and fight with monsters.
![](figure/init.jpg)

# Implemented Systems
1. 2D animation system that supports loops and different frame rates.
2. Data loader that reads JSON files and generates stage and creature classes.
3. A Collision Detection system that can return which side is collided.
4. Sprite renderer using Texture Mapping with OpenGL, which splits each frame into sprites for animations (changing UV buffer).
5. Stage manager that the player can interact with different connected stages.
6. Finite-State Machine for controlling player states, which also can easily extend new states.
7. Sound manager using bass.h
8. Deferred Key system for adding tolerance to player inputs.
9. Debug system for visualizing the collision range.
10. Camera that chases the player and clamps the position depending on the stage size. 
11. Direct light change over time in the background.
12. Dissolve shader applies when the enemy has died, and it dissolves over time.
13. Unique ability: He can use his chain to drag himself closer to the terrain.

# 1.2D Sprites and Animations
I make a Finite-State Machine for controlling player animations and states.

## Idle

<p align="center">
<img  height="300" src="figure/idle.gif">
</p>

## Run

<p align="center">
<img height="300" src="figure/run.gif">
</p>

## 3 Combo Attack

<p align="center">
<img height="300" src="figure/attack.gif">
</p>

## Jump

<p align="center">
<img  height="300" src="figure/jump.gif">
</p>

## Jump Attack

<p align="center">
<img  height="300" src="figure/jumpattack.gif">
</p>

## Damaged

<p align="center">
<img  height="300" src="figure/damage.gif">
</p>

# 2. Collision Applies to Game Mechanics
It can detect whether multiple rectangles are collied, and return which side has collided: left, right, top, or bottom.
It applies to player movements and, creature hitboxes, the trigger box to the next stage, and the unique ability that the chain interacts with the tube and wall.

## Stage Explore
<p align="center">
<img  height="300" src="figure/explore.gif">
</p>


## Attack Monsters
<p align="center">
<img  height="300" src="figure/combat.gif">
</p>

## Chain
<p align="center">
<img  height="300" src="figure/chain.gif">
</p>

# 3. 2D Light and Shadow
I only mange to make one type of light, which is direct light and it changes over time.

## Direct Light Over Time
<p align="center">
<img  height="300" src="figure/light.gif">
</p>

# 4. 2D Shaders
There are many shaders in this game, like drawing sprites and lines, light shader, and one can dissolve the sprites over time.  

## Dissolve Shader
<p align="center">
<img  height="300" src="figure/dissolve.gif">
</p>

# Extra tasks
The unique ability can interact with tubes and walls, which allows the player to explore the stage, without this, we won't able to go back to the beginning point. The most important machinic in the Metroidvania games is the player can go back and forth the area.

## Chain
<p align="center">
<img  height="300" src="figure/chain.gif">
</p>




