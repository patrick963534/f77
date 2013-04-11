if not _PREMAKE_VERSION then
   os.execute('premake4 --file=premake4_vs2008_sl.lua clean')
   os.execute('premake4 --file=premake4_vs2008_sl.lua vs2008')
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
      libdirs { "../deps/win32/lib" }
      links { "freetype" }
      includedirs { "platform/include", "include", "../deps/win32/include" }
      
      files { "include/**.h", 
              "platform/include/**.h",
			  "platform/src/win32/common/**.h", 
			  "platform/src/win32/common/**.c", 
			  "platform/src/win32/win32_system_sl.c", 
			  "platform/src/common/**.h", 
			  "platform/src/common/**.c", 
			  "platform/src/graphics/sl/**.h", 
			  "platform/src/graphics/sl/**.c", 
			  "src/**.h", "src/**.c" }
	  
	  prebuildcommands {
	                    '@xcopy /Y "$(InputDir)\\..\\..\\deps\\win32\\lib\\*.dll" "$(InputDir)\\..\\bin\\" 1>NUL 2>&1',
	                    '@xcopy /Y "$(InputDir)\\..\\..\\res\\*" "$(InputDir)\\..\\bin\\" 1>NUL 2>&1',
	                    }
 
      configuration "Debug"
         defines { "DEBUG", "_CRT_SECURE_NO_DEPRECATE", "_CRT_NONSTDC_NO_DEPRECATE" }
         flags { "Symbols", "ExtraWarnings" }
 
      configuration "Release"
         defines { "NDEBUG", "_CRT_SECURE_NO_DEPRECATE", "_CRT_NONSTDC_NO_DEPRECATE" }
         flags { "Optimize" }     
