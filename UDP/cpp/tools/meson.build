eigen_dep = dependency('eigen3', required: true)
thread_dep = dependency('threads')
#tool_src = files('testBed.cpp')

sendStringUDP_exe = executable('sendStringUDP', 'sendStringUDP.cpp', src,
    include_directories: inc,       
    dependencies: thread_dep,
    cpp_args: cpp_args
)

sendDoubleUDP_exe = executable('sendDoubleUDP', 'sendDoubleUDP.cpp', src,
    include_directories: inc,       
    dependencies: thread_dep,
    cpp_args: cpp_args
)