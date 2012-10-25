if not _PREMAKE_VERSION then
   os.execute('premake4 --file=premake4_linux.lua clean')
   os.execute('premake4 --file=premake4_linux.lua gmake')
   return
end

solution "zlib"
   configurations { "Debug", "Release" }
 
   -- A project defines one build target
   project "zlib"
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
         defines { "DEBUG" }
         flags { "Symbols", "ExtraWarnings" }
 
      configuration "Release"
         defines { "NDEBUG" }
         flags { "Optimize" }     
