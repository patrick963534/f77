if not _PREMAKE_VERSION then
   os.execute('premake4 --file=premake4_vs2008.lua clean')
   os.execute('premake4 --file=premake4_vs2008.lua vs2008')
   return
end

solution "freetype_test"
   configurations { "Debug", "Release" }
 
   -- A project defines one build target
   project "freetype_test"
      kind "ConsoleApp"
      language "C"
      location ( "build" )
      targetdir ( "bin" )
      libdirs { "deps/lib" }
      links { "freetype" }
      includedirs { "include", "deps/include" }
      
      files { "src/**.h", "src/**.c" }
	  
      configuration "Debug"
         defines { "DEBUG", "_CRT_SECURE_NO_DEPRECATE", "_CRT_NONSTDC_NO_DEPRECATE" }
         flags { "Symbols", "ExtraWarnings" }
 
      configuration "Release"
         defines { "NDEBUG", "_CRT_SECURE_NO_DEPRECATE", "_CRT_NONSTDC_NO_DEPRECATE" }
         flags { "Optimize" }     
