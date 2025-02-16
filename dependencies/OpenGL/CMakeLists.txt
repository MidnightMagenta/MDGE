add_library(GL_Dependencies STATIC
	include/glad/glad.h
	include/KHR/khrplatform.h
	include/wglext.h
	src/glad.c
)

target_include_directories(GL_Dependencies PUBLIC include)