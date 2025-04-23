project('commutator-display-manager', ['c','cpp'], default_options : ['c_std=c17', 'cpp_std=c++20'])

boost_dep = dependency('boost', modules : ['log', 'program_options', 'system'])
sdbus_cpp_dep = dependency('sdbus-c++', version : '>= 2.0.0')
pugixml_dep = dependency('pugixml', version : '>= 1.0')
opencv_dep = dependency('opencv4', version : '>= 4.0.0')
nlohmann_json_dep = dependency('nlohmann_json', version : '>= 3.11.3')

include_dirs = include_directories(
    'lib/e-Paper/E-paper_Separate_Program/2in15_e-Paper_G/RaspberryPi_JetsonNano/c/lib/Config',
    'lib/e-Paper/E-paper_Separate_Program/2in15_e-Paper_G/RaspberryPi_JetsonNano/c/lib/e-Paper',
    'lib/e-Paper/E-paper_Separate_Program/2in15_e-Paper_G/RaspberryPi_JetsonNano/c/lib/GUI',
)

src_files = files('src/main.cpp',
                  'src/Display.cpp',
                  'src/Supervisor.cpp',
                  'src/EPaper.cpp',
                  'src/EPaperFake.cpp',
                  'src/EPaperWaveshare.cpp',
                  'src/Widget.cpp',
                  'src/WidgetSytadin.cpp',
                  'src/WidgetVigicrues.cpp',
                  'src/WidgetIdfmLineReports.cpp',
                  'src/CommutatorSytadin.cpp',
                  'src/CommutatorVigicrues.cpp',
                  'src/CommutatorIdfmLineReports.cpp',
                  'src/IdfmLineStatus.cpp',
                  'src/IdfmTransportMode.cpp',
                  'lib/e-Paper/E-paper_Separate_Program/2in15_e-Paper_G/RaspberryPi_JetsonNano/c/lib/Config/DEV_Config.c',
                  'lib/e-Paper/E-paper_Separate_Program/2in15_e-Paper_G/RaspberryPi_JetsonNano/c/lib/e-Paper/EPD_2in15g.c',
                  'lib/e-Paper/E-paper_Separate_Program/2in15_e-Paper_G/RaspberryPi_JetsonNano/c/lib/Fonts/font24.c',
                  'lib/e-Paper/E-paper_Separate_Program/2in15_e-Paper_G/RaspberryPi_JetsonNano/c/lib/GUI/GUI_Paint.c',
)

executable('commutator-display-manager',
           src_files,
           dependencies: [boost_dep, sdbus_cpp_dep, pugixml_dep, opencv_dep, nlohmann_json_dep],
           include_directories: include_dirs,
           c_args: ['-D USE_BCM2835_LIB', '-D RPI'],
           cpp_args : ['-Werror','-Wno-deprecated-enum-enum-conversion'],
           link_args: ['-Wl,--gc-sections','-lbcm2835','-lm'],
           install: true,
           install_dir: '/usr/bin'
)

install_data('cfg/config.json',
             install_dir: '/etc/commutator-display-manager',
             rename: 'config.json'
)

png_files = files(
  'res/arrow-bottom.png',
  'res/arrow-bottom-right.png',
  'res/arrow-right.png',
  'res/arrow-top-right.png',
  'res/arrow-top.png',
  'res/cross.png',
  'res/warning.png',
  'res/car.png',
  'res/wave.png',
  'res/water-flow-symbol.png',
  'res/water-level-symbol.png',
  'res/metro.png',
  'res/metro-1.png',
  'res/metro-2.png',
  'res/metro-3.png',
  'res/metro-3b.png',
  'res/metro-4.png',
  'res/metro-5.png',
  'res/metro-6.png',
  'res/metro-7.png',
  'res/metro-7b.png',
  'res/metro-8.png',
  'res/metro-9.png',
  'res/metro-10.png',
  'res/rer.png',
  'res/transilien.png',
  'res/rer-c.png',
  'res/transilien-n.png'
)

foreach png_file : png_files
  install_data(png_file, install_dir: '/etc/commutator-display-manager/ressources')
endforeach
