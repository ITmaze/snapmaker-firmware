import sys
from datetime import datetime
from shutil import move
from os.path import join,isfile,getsize
Import("env", "projenv")


print("============================== CPPDEFINES start ================================")
print(projenv.get("CPPDEFINES", [])[:])
print("============================== CPPDEFINES end ==================================")
print()

print("============================== CPPPATH start ===================================")
cpp_path = projenv.get("CPPPATH", [])[:]
for path in cpp_path:
  print(path)
print("============================== CPPPATH end =====================================")
print()

print("============================== CCFLAGS start ===================================")
print(projenv.get("CCFLAGS", [])[:])
print("============================== CCFLAGS end =====================================")
print()

print("============================== CXXFLAGS start ==================================")
print(projenv.get("CXXFLAGS", [])[:])
print("============================== CXXFLAGS end ====================================")
print()

print("============================== LINKFLAGS start =================================")
print(projenv.get("LINKFLAGS", [])[:])
print("LDSCRIPT_PATH: " + projenv.get("LDSCRIPT_PATH"))
print("============================== LINKFLAGS end ===================================")
print()

print("============================== LIBPATH start ===================================")
lib_path = projenv.get("LIBPATH", [])[:]
for path in lib_path:
  print(path)
print("============================== LIBPATH end =====================================")
print()

print("============================== LIBSOURCE_DIRS start ============================")
lib_src = projenv.get("LIBSOURCE_DIRS", [])[:]
for path in lib_src:
  print(path)
print("============================== LIBSOURCE_DIRS end ==============================")
print()

print("============================== record env start ================================")

def save_env():
  cwd = sys.path[0]
  print("current dir: " + cwd)

  # get file path and see if it is too large
  f_path = join(cwd, "env.txt")
  if isfile(f_path):
    # about 85 kB / time, we git it 500kB
    # if it is over 500kB, jsut rename to env_old.txt
    if getsize(f_path) > 500 * 1024:
      move(f_path, join(cwd, "env_old.txt"))

  f = open(f_path, "a+")

  # build timestamp
  f.write("==============================\r\n")
  f.write(datetime.now().strftime('%Y-%m-%d %H:%M:%S'))
  f.write("\r\n" * 2)

  # project env, we need to focus on it
  f.write("=============== projenv start ===============\r\n")
  f.write(str(projenv.Dump()))
  f.write("\r\n")
  f.write("=============== projenv end =================\r\n")
  f.write("\r\n")

  # globle env
  f.write("=============== env start ===================\r\n")
  f.write(str(env.Dump()))
  f.write("\r\n")
  f.write("=============== env end =====================\r\n")
  f.write("\r\n")

  f.close()


save_env()

print("============================== record env end ==================================")
print()
