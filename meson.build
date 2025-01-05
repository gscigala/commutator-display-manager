project('commutator-display-manager', 'cpp', default_options : ['cpp_std=c++20'])

boost_dep = dependency('boost', modules : ['log', 'program_options', 'system'])
sdbus_cpp_dep = dependency('sdbus-c++', version : '>= 0.6.0')
pugixml_dep = dependency('pugixml', version : '>= 1.0')
src_files = files('src/main.cpp',
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
                  )

executable('commutator-display-manager',
           src_files,
           dependencies: [boost_dep, sdbus_cpp_dep, pugixml_dep],
           cpp_args : ['-Werror'])
