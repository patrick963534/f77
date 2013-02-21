if not _PREMAKE_VERSION then
   os.execute('premake4 --file=premake4_vs2008.lua clean')
   os.execute('premake4 --file=premake4_vs2008.lua vs2008')
   return
end

solution "libpng"
   configurations { "Debug", "Release" }
 
   -- A project defines one build target
   project "libpng"
      kind "ConsoleApp"
      language "C"
      location ( "build" )
      targetdir ( "bin" )
      libdirs {  }
      links { }
      includedirs { "include" }
      
      files { "include/**.h", "src/**.h", "src/**.c" }
	  
	  prebuildcommands {}
 
      configuration "Debug"
         defines { "WINDOWS", "HAVE_VSNPRINTF", "DEBUG", "_CRT_SECURE_NO_DEPRECATE", "_CRT_NONSTDC_NO_DEPRECATE" }
         flags { "Symbols", "ExtraWarnings" }
 
      configuration "Release"
         defines { "WINDOWS", "NDEBUG" }
         flags { "Optimize" }     
