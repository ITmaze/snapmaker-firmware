/**
 * Marlin 3D Printer Firmware
 * Copyright (C) 2016 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
 *
 * Based on Sprinter and grbl.
 * Copyright (C) 2011 Camiel Gubbels / Erik van der Zalm
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

/**
 * M101 Task Free Memory Watcher
 *
 * This code watches the free memory defined for each task in the FreeRTOS environment
 * It is useful to determine how much memory each task has available.
 * 
 * Initial version...  More Marlin-specific information to be added.
 */

#include "../../inc/MarlinConfig.h"
#include "../gcode.h"
#include "../../Marlin.h"
#include "MapleFreeRTOS1030.h"
#include "../../libs/hex_print_routines.h"

#define MAX_TASKS 10

void GcodeSuite::M101() {
  TaskStatus_t TaskStatArray[MAX_TASKS];

  unsigned n_tasks = uxTaskGetNumberOfTasks();
  if (n_tasks > MAX_TASKS) {
    SERIAL_ECHOLNPAIR("?Too many tasks: ", n_tasks);
    SERIAL_EOL();
    return;
  }

  SERIAL_ECHOLNPAIR("M101 RTOS Task Info:");
  /* Generate raw status information about each task. */
  n_tasks = uxTaskGetSystemState( TaskStatArray, MAX_TASKS, NULL);

  SERIAL_ECHOLNPAIR("n_tasks: ", n_tasks);

  /* For each populated position in the TaskStatArray array,
     format the raw data as human readable ASCII data. */
  for (unsigned x = 0; x < n_tasks; x++) {
    char t_name[configMAX_TASK_NAME_LEN + 1];         // Pad out the task name so everything lines up nicely
    strcpy(t_name, TaskStatArray[x].pcTaskName);
    while (strlen(t_name) < configMAX_TASK_NAME_LEN)
      strcat(t_name, "_");

    SERIAL_ECHO(x);
    SERIAL_ECHOPAIR(":", t_name);
    SERIAL_ECHOPAIR(" Task #: ", TaskStatArray[x].xTaskNumber);
    SERIAL_ECHOPAIR(" Current_Priority: ", TaskStatArray[x].uxCurrentPriority);
    SERIAL_ECHOPAIR(" Base_Priority: ", TaskStatArray[x].uxBasePriority);

    SERIAL_ECHOPAIR(" Stack: ");
    print_hex_address((const void * const) TaskStatArray[x].pxStackBase);
    SERIAL_ECHOPAIR(" Free_Mem: ", (unsigned int) TaskStatArray[x].usStackHighWaterMark);
    SERIAL_ECHOPAIR(" State: ");
    switch( TaskStatArray[x].eCurrentState ) {
      case eRunning:   SERIAL_ECHOLNPGM("Running");   break;
      case eReady:     SERIAL_ECHOLNPGM("Ready");     break;
      case eBlocked:   SERIAL_ECHOLNPGM("Blocked");   break;
      case eSuspended: SERIAL_ECHOLNPGM("Suspended"); break;
      case eDeleted:   SERIAL_ECHOLNPGM("Deleted");   break;
      default:         SERIAL_ECHOLNPAIR("Corrupted:", TaskStatArray[x].eCurrentState);
                       break;
    }
  }
}
