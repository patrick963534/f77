if not _PREMAKE_VERSION then
   os.execute('premake4 --file=premake4_linux_spark.lua clean')
   os.execute('premake4 --file=premake4_linux_spark.lua gmake')
   return
end

solution "libks"
   configurations { "Debug", "Release" }
 
   -- A project defines one build target
   project "libks"
      kind "ConsoleApp"
      language "C"
      location ( "build" )
      targetdir ( "bin" )
      libdirs { "/usr/lib" }
      links { "rt", "EGL", "X11", "GLESv2", "png" }
      includedirs { "include", "platform/include" }

	  files { "include/**.h", 
              "platform/include/**.h",
	          "platform/src/linux/common/**.h", 
			  "platform/src/linux/common/**.c", 
			  "platform/src/linux/linux_system_spark.c", 
			  "platform/src/common/**.h", 
			  "platform/src/common/**.c", 
			  "platform/src/graphics/gles2/**.h", 
			  "platform/src/graphics/gles2/**.c", 
			  "src/**.h", "src/**.c" }
	   
      configuration "Debug"
         defines { "DEBUG" }
         flags { "Symbols", "ExtraWarnings" }
 
      configuration "Release"
         defines { "NDEBUG" }
         flags { "Optimize" }     