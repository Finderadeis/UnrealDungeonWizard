## What is it?
A Plugin for Unreal Engine that helps prototyping dungeons with semi-procedural Editor Modes

This is a WIP but im trying to ensure that whats on the repo is always stable.

## Engine / Languages
- Unreal Engine 5.3
- C++

> [!WARNING]
> Since this is written for UE 5.3 and the functionality/syntax of needed modules changed quite a bit during UE5 evolution, I can not guarantee for this to work in older versions.

## How to install

- Download the newest Release ZIP file

- Before the first use, there are some steps needed to get it up and running.

> [!TIP]
> If you want to use the plugin in other projects, including pure Blueprint projects, you can put a copy of the **compiled version** directly into the engines plugin folder. This makes it accessible from any projects plugin settings.

> [!WARNING]
> For the first compile this needs to be a **C++ project**

- If the project is currently open, close it before proceeding

- Put the “DW” folder into the plugin folder of the project where you want to use the plugin
  If you don’t have a plugin folder, just create one
  ![Screenshot 2024-05-01 110733 _sstep1png](https://github.com/Finderadeis/UnrealCleanup/assets/79834166/0b8f6466-2ca4-4edc-bca7-0ef76da1d087)

- Delete “Binaries” and “Intermediate” from your project folder
  (this is needed for initial compilation of the plugin)

  ![Screenshot 2024-05-01 110733 _sstep2](https://github.com/Finderadeis/UnrealCleanup/assets/79834166/f8e628e3-11ba-4da7-a6d4-70bab582dc61)

- Doubleclick the .uproject file to initiate a rebuild process of Unreal
  
  ![image](https://github.com/Finderadeis/UnrealDungeonWizard/assets/79834166/12f2cd06-bdc3-4032-85fd-de550cd5cd73)

> [!NOTE]
> Depending on your hardware **this may take a while** without any visual feedback!

- When it is finished, Unreal should automatically start the project.
  
- The plugin should be automatically activated. 
  If you want to double-check, it should be listed under Plugins-Installed-Other

  ![image](https://github.com/Finderadeis/UnrealDungeonWizard/assets/79834166/a1856f5a-811e-4b57-8259-73ca5bb71eb5)


## How to use

### Presets
If You want to test things out first, you can skip this step. The plugin comes with three predefined Presets Assets you can use. The included Assets are free to use (CC0).

If you want to control the parameters and add your own Assets, you can create a new Presets Assets.

- Right click inside your content folder and create a new Blueprint Class

  ![image](https://github.com/Finderadeis/UnrealDungeonWizard/assets/79834166/2ad479c3-33a3-4f37-8fde-fdd208f1d226)

- Click on “All Classes”, search for “DWDataAsset” and select it (see the highlighted entry in the screenshot)

  ![image](https://github.com/Finderadeis/UnrealDungeonWizard/assets/79834166/1c8d83b6-62d2-4c57-9099-20af42c146bd)

- Name your Blueprint and open it to set up your presets

### Presets Settings Explanation

| Setting | Description |
| :------------- | :------------- |
| Generation Mode  | Decides the type of generation (see below in Editor mode)  |
| Max Width / Max Depth  | Maximum dimension (X and Y) for the generated dungeon (does not apply for “Linear Rooms” Mode)  |
| Mesh Size | Edge length of the provided meshes. If you are not sure about this, compare with the standard Unreal Cube – this has a size of 100 |
| Floor Meshes | All Static Meshes to randomly choose from for floor tiles during dungeon spawn |
| Wall Meshes | All Static Meshes to randomly choose from for wall tiles during dungeon spawn |
| Wall Units | How many meshes to use for walls (Height) |
| Wall Connector Meshes | All Static Meshes to randomly choose from for wall connectors during dungeon spawn |
| Use Wall Connectors | If true, Connector Meshes will be spawned between walls for decoration or hiding potential seams |
| Ceiling Meshes | All Static Meshes to randomly choose from for ceiling tiles during dungeon spawn |
| Add Ceiling | If true, ceiling meshes are spawned, else its open on the top side |
| Wall Light Sources | All Actors to spawn as wall mounted light source |
| Wall Light Offset | Distance from wall pivot for the light spawn position, so they don’t stick inside the walls |
| Wall Light Chance | Average per wall propability of spawning wall light |
| Number Of Rooms | How many rooms to generate (“Linear Rooms” Mode only) |
| Min Room Size / Max Room Size | Room dimensions to generate (“Linear Rooms” Mode only) |
| Min Room Distance / Max Room Distance | Dimensions for corridors to generate between rooms (“Linear Rooms” Mode only) |


### How to prepare your assets

In order to spawn the assets at the the right position, they need to have their **pivot centered**, and have **uniform dimensions**. Meaning, floor and ceiling X/Y must be equal and wall X/Z accordingly.

If you don’t want to change and reimport the assets from your 3D Software or Assetpack, you can adjust the pivot point directly inside Unreal by using the Modeling Mode.

![image](https://github.com/Finderadeis/UnrealDungeonWizard/assets/79834166/88ac9b7b-dfea-47d2-90fa-8973956fd8e8)

- Place your asset in the viewport, and set its **Location and Rotation to Zero**

- Make sure the asset is selected and switch to Modling Mode

- Select XForm – Edit Pivot

  ![image](https://github.com/Finderadeis/UnrealDungeonWizard/assets/79834166/14b99636-20e2-4a5e-9549-0531760c0cf1)

- Click Center, then Accept

  ![image](https://github.com/Finderadeis/UnrealDungeonWizard/assets/79834166/9a28c67e-fc2d-4b4b-94dc-821b2d57e5ac)
![image](https://github.com/Finderadeis/UnrealDungeonWizard/assets/79834166/8c03ea6f-ce0e-4e15-a28e-96fae38c4e3e)

- **Save your asset** through the content browser

- It is now ready to be used and you can delete it from the viewport


## Dungeon Wizard Editor Mode

When the Plugin is active, a new Editor Mode gets added to the Mode Selection.

![image](https://github.com/Finderadeis/UnrealDungeonWizard/assets/79834166/e1d73548-77ae-455b-bd4b-9f6bcba402f2)

Click the “Dungeon” Button and select a Presets Asset.
These highlighted in the screenshot are the included Sample Assets – one for each generation mode.

If you created a new Presets Asset, it should be listed here too.

![image](https://github.com/Finderadeis/UnrealDungeonWizard/assets/79834166/cdde3c39-d2db-40ef-bd63-4b2555f84ca6)

Each Mode will have a spawned Dungeon as a result, which you can modify as you like.

Meshes and Actors get spawned separately and sorted into categorized folders in your Outliner like so:

![image](https://github.com/Finderadeis/UnrealDungeonWizard/assets/79834166/77d1dcdf-cb40-44dd-b384-0ba3a7bce3d0)

## "Pure Maze" Mode

![image](https://github.com/Finderadeis/UnrealDungeonWizard/assets/79834166/af6fc592-d9fd-4a70-87b6-c2a97a059eb8)

Generates a pure Maze, meaning no rooms, all tiles connected to each other (you can reach every point in the maze from every other point)

Parameters and used assets are determined by the Presets Asset.

Hold Ctrl + Shift and click on the BaseMesh to spawn the Maze.

## "Linear Rooms" Mode

![image](https://github.com/Finderadeis/UnrealDungeonWizard/assets/79834166/0045d87b-29fe-4b2a-8a0f-b88afcc808ee)

Generates a linear Dungeon, consisting of several rooms connected on after another.
 
Parameters and used assets are determined by the Presets Asset.

Hold Ctrl + Shift and click on the BaseMesh to spawn the Dungeon. 

## "Draw Layout" Mode

![image](https://github.com/Finderadeis/UnrealDungeonWizard/assets/79834166/8625e584-3d84-42f3-a0c0-4204536578a3)

Generates a grid layout, where you can draw your own dungeon layout.

Parameters and used assets are determined by the Presets Asset. 


- Click and drag on the grid to create rooms **(green outlines)** 

- Use Ctrl to remove from the dungeon **(orange outlines)**

- Use Shift to connect rooms **(purple outlines)**

If you are happy with your layout, hold Ctrl + Shift and click anywhere on the grid to spawn the Dungeon.

