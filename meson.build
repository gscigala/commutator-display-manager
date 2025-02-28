project('commutator-display-manager', ['c','cpp'], default_options : ['c_std=c17', 'cpp_std=c++20'])

boost_dep = dependency('boost', modules : ['log', 'program_options', 'system'])
sdbus_cpp_dep = dependency('sdbus-c++', version : '>= 0.6.0')
pugixml_dep = dependency('pugixml', version : '>= 1.0')
opencv_dep = dependency('opencv4', version : '>= 4.0.0')

include_dirs = include_directories(
    'lib/e-Paper/RaspberryPi_JetsonNano/c/lib/Config',
    'lib/e-Paper/RaspberryPi_JetsonNano/c/lib/e-Paper',
    'lib/e-Paper/RaspberryPi_JetsonNano/c/lib/GUI',
)

src_files = files('src/main.cpp',
                  'src/Display.cpp',
                  'src/Supervisor.cpp',
                  'src/Widget.cpp',
                  'src/WidgetSytadin.cpp',
                  'src/WidgetVigicrues.cpp',
                  'src/WidgetIdfmLineReports.cpp',
                  'src/CommutatorSytadin.cpp',
                  'src/CommutatorVigicrues.cpp',
                  'src/CommutatorIdfmLineReports.cpp',
                  'src/IdfmLineStatus.cpp',
                  'src/IdfmTransportMode.cpp',
                  'lib/e-Paper/RaspberryPi_JetsonNano/c/lib/Config/DEV_Config.c',
                  'lib/e-Paper/RaspberryPi_JetsonNano/c/lib/e-Paper/EPD_2in15g.c',
                  'lib/e-Paper/RaspberryPi_JetsonNano/c/lib/Fonts/font24.c',
                  )

executable('commutator-display-manager',
           src_files,
           dependencies: [boost_dep, sdbus_cpp_dep, pugixml_dep, opencv_dep],
           include_directories: include_dirs)

