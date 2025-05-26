# Real-Time Systems - Application 1

**Name**: Sean Willie  
**UCFID**: 5583422
**Theme**: Space Systems

## Overview

This project implements two FreeRTOS tasks on the ESP32 using the ESP-IDF framework:

- A blink task that toggles an LED at a frequency of 2 Hz (250 ms on, 250 ms off)
- A print task that logs a status message to the serial console every 10 seconds

The project runs successfully on a physical ESP32 board. I successfully set up VScode with PlatformIO for the ESP32. The LED is wired to GPIO2, which also controls the onboard LED.

---

## Theme Integration

In the Space Systems theme, the blinking LED simulates a satellite status beacon, and the print task simulates periodic telemetry confirming that the system is still alive and functioning. In a real-world satellite, timely blinking and regular telemetry are critical for ensuring the system is operational and communicating.

---

## Task Timing Verification

- **LED blink task period**: 500 ms (2 Hz total cycle)
- **Console print task period**: 10,000 ms (10 seconds)

### Did the tasks meet their timing requirements?
Yes. The LED blinks visibly at 2 Hz and the console prints every 10 seconds. This was verified using the serial monitor and a timer on my watch.

### How were the results checked?:
- The LED timing was verified by observing it blink consistently twice per second.
- The console output timestamp was printed using `xTaskGetTickCount()`from the application notes and confirmed regular intervals.

---

## Super-loop vs. Multitasking

If both blink and print functionality were written inside a single infinite loop (super-loop), delays in one task (e.g., `printf`) would affect the LED blink rate. FreeRTOS multitasking allows each task to operate on its own schedule, ensuring timing independence and deterministic behavior.
Super-loops are simple but unreliable for real-time systems, while an RTOS provides deterministic scheduling. This would be essential for space applications where missed deadlines can be catastrophic.


Example sequential super-loop: 
void main() {
    while(1) {
        task1();  // e.g., LED blink
        task2();  // e.g., serial print
        task3();  // e.g., sensor read
    }
}

Example Multi-Task RTOS:
void task1(void *pvParam) {  // High-priority task (e.g., LED blink)
    while(1) {
        toggle_led();
        vTaskDelay(250 / portTICK_PERIOD_MS);  // Non-blocking delay
    }
}

void task2(void *pvParam) {  // Low-priority task (e.g., serial print)
    while(1) {
        printf("Telemetry: OK\n");
        vTaskDelay(10000 / portTICK_PERIOD_MS);
    }
}

void main() {
    xTaskCreate(task1, "blink", 1024, NULL, 2, NULL);  // Higher priority
    xTaskCreate(task2, "print", 1024, NULL, 1, NULL);  // Lower priority
    vTaskStartScheduler();
}
---

## Experimentation

- When the `print_task` was modified to run without `vTaskDelay()`, the LED stopped blinking properly. This confirms that a task which never yields can cause other tasks to not function properly, demonstrating the importance of cooperative multitasking.

---

## AI Usage
ChatGPT was used to help interpret the assignment, generate sample code, and structure this README. I also used ChatGPT to generate the examples of the difference between a sequential super-loop and a multi-task real-time system.

---

## Wokwi Project 
https://wokwi.com/projects/431981172267850753
