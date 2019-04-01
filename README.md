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
```
    /* USER CODE BEGIN PFP */    
    extern void StartThreads(const void *);    
    /* USER CODE END PFP */    
```
* Find section `/* USER CODE BEGIN 2 */` and add the following code:
```
    osThreadDef(ThreadsStarter, StartThreads, osPriorityRealtime, 0, 128);
    osThreadCreate(osThread(ThreadsStarter), NULL);
```
* Copy file Threads.cpp to /src folder.

#### Libraries

* Create folder for Libraries. Use command File > New > Source Folder.    
**N.B. It is very important that you create a "Source folder", and not just "Folder". Source folder contents will be compiled, while contents of regular folders will not. You can tell the difference between source folders and regular folders by presence of blue square with letter 'c' inside next to source folders.**   
* Copy contents of folder Libraries to newly created folder. 
* This folder is basic abstractions for BooleanIO, FIFOBuffer, and SoftTimer. After I wrote the SoftTimer class I found out FreeRTOS already has the concept built in, so this class will be deprecated in the future. Try to avoid using it.

#### Threads (or Tasks as they are known in FreeRTOS)

* Follow the same process as you did for Libraries to create a source folder called Threads.
* Copy contents of folder Threads to newly created folder.
* This folder contains base class for all thread classes and two inherited classes.
* CUARTCommsController looks after UART channel. DebugController is general purpose controller that can be used to do anything. In this case it blinks LD2 to notify that kernel is running. I use DebugController when I need to create a backdoor to a GPIO port, or ADC, or perform something that overall architecture of the code would not allow or prove difficult to implement. Nothing that is supposed to remain in Release version should live in DebugController.

#### Enable c++ code compilation

* Add c++ nature to the project. File > New > Convert to a C/C++ Project (Adds C/C++ Nature).
* Add new folders to list of directories. Right click on project name and select Properties.
* In C/C++ Build > Settings > Tool Settings
    *  Copy everything from C Compiler > Directories  to C++ Compiler > Directories
* In C/C++ Build > Settings > Tool Settings > C++ Compiler > Directories create folders
    * ../Threads
    * ../Libraries

#### Minor improvements

If you have a modern PC it is likely running a multicore processor. To speed up compilation you can enable parallel build. In project properties go to > C/C++ Build > Behaviour and tick box Enable parallel build. Check Use optimal jobs to let Eclipse determine how many cores to use. This will cause eclipse to compile multiple files in parallel.

# Thread tracking
Below are step by step instructions on how to enable task performance tracking.
## Configuring hardware
First of all you'll need to configure a hardware timer that'll provide clock base for measuring task run time. For the purposes of this template I'll use TIM6, since that's what I normally use for my debugging. The process is easily adaptable to any other timer. TIM6 is pretty much only suitable for timekeeping, so it's unlikely to impact hardware functionality.
You might want to use 32 bit timer if better resolution is required, but for simple look-see of which task consumes most processor cycles 16 bit will suffice.
* Open FreeRTOS_template.ioc project file in CubeMX
* In Timer section select TIM6 and click to activate it
* Configuration parameters:
 *  Prescaler set to 79.
 *  Counter period set to 49.
At 80MHz base clock of my STM32L476, these settings trigger an overflow event every 50us.
80MHz/(79+1) = 1MHz, which is 1us
1us * (49+1) = 50us base timing unit.
This number is essentially the resolution at which you measure task time consumption. If you require higher resolution, either reduce the Prescaler or reduce Counter period.
* Click Generate code and wait until CubeMX is finished updating code.
* Open FreeRTOSConfig.h and add following code.
 * Enable stack high water mark tracking
    `#define configRECORD_STACK_HIGH_ADDRESS 1`
 * Enable trace facility (this will also be used to enable Tracealyzer)
    `#define configUSE_TRACE_FACILITY 1`
 * 

In order to enable collection of task performance data you'll need to do some code modifications. Also, some of the MCU hardware, specifically, a timer will be needed.

# Tracealyzer integration
In development
___
# Notes
This section contains various stuff that does not fit neatly into other sections.

### Stuff I don't know how to do
* Folder /.settings/ contains a file language.settings.xml that seems to change randomly depending on which machine I use to compile the code. I have what I think identical installations of toolchain on all my laptops, yet this file keeps creeping up in my source control. I eventually got annoyed and .gitignored it. Does not seem to break anything. Code compiles and works just fine. If any of you fine folk can explain how to deal with it properly, I'd be glad to hear it.
