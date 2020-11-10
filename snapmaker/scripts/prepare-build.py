# -*- coding: UTF-8 -*-

import os
import sys
import shutil

Import("env")

platform = env.PioPlatform()  # ststm32

print("============================== Prepare env start ===============================")
print("Prepare env for \"{}\"...".format(env.get("BOARD")))

# Check framework
framework_list = env.get("PIOFRAMEWORK")
framework = framework_list[0]
print("framework =", framework)  # arduino
pkg = platform.frameworks[framework]["package"]
print("package:", pkg)
print()

# Check Board config
board = env.BoardConfig()
mcu = board.get('build.mcu')
core = board.get('build.core')
variant = board.get('build.variant')
print("build.mcu =", mcu)
print("build.core =", core)
print("build.variant =", variant)
print()

# Copy custom build script for our own MCU
if framework == "arduino" and core == "maple":
    build_script_name = "platformio-build-{}.py".format(mcu[0:7])

    local_script = os.path.join(sys.path[0], build_script_name)

    # See script location at `platforms/ststm32/builder/frameworks/arduino.py`
    # package_dir = platform.get_package_dir("framework-arduinoststm32-maple")
    package_dir = platform.get_package_dir(pkg)
    build_script = os.path.join(package_dir, "tools", build_script_name)

    print("Copying build script \n from {} \n to {}".format(local_script, build_script))
    shutil.copy(local_script, build_script)
else:
    print("Unsupported framework / build core")
    sys.exit(-1)

print("============================== Prepare env end ==============================")
