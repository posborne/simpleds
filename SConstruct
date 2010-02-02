import glob

env = Environment(CC='gcc',
                  CCFLAGS='-Wall -pedantic -g',
                  parse_flags='-lcheck')

dslib = env.StaticLibrary('build/simpleds', source=glob.glob('src/*.c'))
env.Program('runtests', source=glob.glob('tests/*.c') + dslib)
