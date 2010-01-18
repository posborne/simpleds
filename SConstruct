import glob

env = Environment(CC='gcc',
                  CCFLAGS='-Wall -pedantic -g',
                  parse_flags='-lcheck')

# env.StaticLibrary('simpleds', source=glob.glob('src/*.c'))
env.Program('runtests', source=glob.glob('src/*.c') + glob.glob('tests/*.c'))
