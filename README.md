# FreeRTOS_template
Basic structure of an STM32CubeMX generated project.
This is the basic skeleton of FreeRTOS project that I use for my embedded development. I've decided to create this repo because of numerous questions to me personally and on Reddit pertaining to exactly how to write large and complex embedded code based on FreeRTOS and CubeMX.

# How to adapt this template to your needs
This section will contain (hopefully, without errors) the process of adapting the template to your needs.
I used STM32L476RG chip because that's my goto Nucleo device that I do all my prototyping on. It's reasonably powerful, can be made low power, has sufficiently wide choice of peripherals and it's under £10, so when I blow it up because 24V rail got accidentally connected to a GPIO, it's no big deal. I have a drawer full of them.

## FreeRTOS code generation from CubeMX.
At this stage all that you need to do is choose a board or a processor and click FreeRTOS as required Middleware to be included in the build.
Make sure 
