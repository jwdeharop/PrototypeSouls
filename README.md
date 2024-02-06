# PrototypeSouls

**Prototype Souls** is meant to be my way of improving my skills using Gameplay Ability System and Multiplayer with Unreal Engine (5.3). I have been kind of obsessed with Brandon Sanderson's books lately and, while reading, I thought: "Bloody hell, wouldn't it be amazing if some of the magic systems of this book were implemented into a game?" So, that's me trying to do this.

# 1. Gameplay Ability System (GAS)

The official Unreal Engine's GAS documentations reads as follow: "The Gameplay Ability System is a framework for building attributes, abilities, and interactions that an Actor can own and trigger. The system is designed to be adapted to a wide variety of Gameplay-Driven projects such as Role-Playing Games(RPGs), Action-Adventure games, and Multiplayer Online Battle Arenas games(MOBA)."
Here you can find some useful sites with GAS examples:
- [GASDocumentation](https://github.com/tranek/GASDocumentation/tree/5.3)
- [QuodSoler](https://www.quodsoler.com/)

# 2. Gameplay Mechanics

Before starting with the Cosmere's gameplay mechanics we need to have a strong base that, by itself, it's funny and robust. For this, I'll try to make something similar to God of War (2018).

## Sprint

I decided starting with the Sprint feature because it's the easiest to implement using GAS.
![](https://github.com/jwdeharop/PrototypeSouls/blob/main/ReadmeContent/gifs/2024-01-0720-07-25-ezgif.com-video-to-gif-converter.gif)

The Sprint Ability just takes the Sprint Gameplay Effect and applies the Gameplay Effect to its owner (Server side only). When the ability ends, we just remove that GameplayEffect from the player. (Server side only).
As for the Gamemplay Effect itself, it is just a Modifier that takes the MaxWalkSpeed from the PlayerAttributeSet and overrides it to be the new MaxWalkSpeed. This MaxSpeed is Replicated, so the clients knows when the server changes its value (only when changed).

## Dodge

All Dodges animations are configured in the same animation montage. This animation montage has different sections, depending on the type of dodge (forward, back, right...). Once the ability starts, we get the Dodge Section Name we need. We calculate the direction of the character and with that we return the type of direction.

![](https://github.com/jwdeharop/PrototypeSouls/blob/main/ReadmeContent/gifs/dodge.gif)

If that Section Name is correct, we Activate a Gamemplay Task that plays a montage and waits for some events to happen. And, in the server side, we launch the character in the correct direction. The dodge expects a GameplayTag event that matches the gameplay tag Ability.Dodge.CanRoll. This Tag is sended with an Anim Notification. Once that event is received, we activate another GameplayTask that waits for the input to be pressed again. If the input is pressed correctly, we will roll. 

![](https://github.com/jwdeharop/PrototypeSouls/blob/main/ReadmeContent/gifs/roll.gif)

## Light Attack

I have created a Data Asset to easily configure and create new weapons. With this DataAsset we can configure:
- The Weapon Type: for now just an Axe.
- The ComboSections: The prefix's of the combo section.
- The Montages to play.

When the LightAttackAbility is activated, we try to play the current combo. To do so, we get the information of the Current Weapon equipped by the player. From this information, we access the current montage that has to be played and we generate the ComboSection. (Example: Combo02_1, this means the first part of the combo 02). If that ComboSection is correct, we will activate a GameplayTask that will play the animation montage and wait for some events. At the same time, we activate another GameplayTask that will wait for the input to be pressed again. Once pressed again, we will check if we can play the next part of the combo.

![](https://github.com/jwdeharop/PrototypeSouls/blob/main/ReadmeContent/gifs/attack.gif)





  





