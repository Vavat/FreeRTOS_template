# FreeRTOS_template
Basic structure of an STM32CubeMX generated project.

This is the basic skeleton of FreeRTOS project that I use for my embedded development. I've decided to create this repo because of numerous questions to me personally and on Reddit pertaining to exactly how to write large and complex embedded code based on FreeRTOS and CubeMX.

## How to adapt this template to your needs
This section will contain (hopefully, without errors) the process of adapting the template to your needs.

I used STM32L476RG chip because that's my goto Nucleo device that I do all my prototyping on. It's reasonably powerful, can be made low power, has sufficiently wide choice of peripherals and it's under £10, so when I blow it up because 24V rail got accidentally connected to a GPIO, it's no big deal. I have a drawer full of them.

### FreeRTOS code generation from CubeMX.
At this stage all that you need to do is choose a board or a processor and click FreeRTOS as required Middleware to be included in the build. Pay attention to SysTick selection. FreeRTOS uses SysTick by default, so if you choose to use HAL pick a different timebase.    

### Import into TrueStudio
Now that the code is generated you can open it in TrueStudio (this is what I use, for your choice of SDK, process might be different). In project pane right click and select "Import..." In a pop up window select >General>Existing Project into Workspace. In Select root directory: browse to the folder where code was generated. I'd recommend generating code directly into TrueStudio workspace, but preference is entirely yours. You can keep code where it is or choose to let TrueStudio copy it into Workspace. Keep in mind that if you choose to copy code into workspace subsequent CubeMX updates will not be copied automatically.

Once code is imported, build it to make sure everything is in order.

### Modify code
Here are the steps you need to perform in order to allow you to write your various controllers in c++ instead of c.

#### C++ entry point
* Find section `/* USER CODE BEGIN PFP */` in `main.c` and add:
```c++
    /* USER CODE BEGIN PFP */    
    extern void StartThreads(const void *);    
    /* USER CODE END PFP */    
```
* Find section `/* USER CODE BEGIN 2 */` and add the following code:
```c++

```
* Copy file Threads.cpp to /src folder.
___
# Notes
This section contains various stuff that does not fit neatly into other sections.

### Stuff I don't know how to do
* Folder /.settings/ contains various files that seem to change randomly depending on which machine I use to compile the code. I have what I think identical installations of toolchain on all my laptops, yet this folder keeps creeping up in my source control. I eventually got annoyed and .gitignored it. Does not seem to break anything. Code compiles and works just fine. If any of you fine folk can explain how to deal with it properly, I'd be glad to hear it.
