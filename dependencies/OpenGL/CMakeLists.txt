add_library(GL_Deps STATIC
	include/glad/glad.h
	include/KHR/khrplatform.h
	include/wglext.h
	src/glad.c
)

target_include_directories(GL_Deps PUBLIC include)