workspace "WastelandAdventuresWorkspace"
   configurations { "Debug", "Release" }
   platforms { "Windows" }
   location "build"

   -- Set common C flags and platform detection
   filter "system:windows"
      defines { "WINDOWS" }
      architecture "x64"

   -- Set common include paths
   includedirs {
      "vendor/raylib5/include",
      "vendor/cjson/include",
      "vendor/fmt/include"
   }

   -- Set common library paths
   libdirs {
      "vendor/raylib5/lib/windows", -- For Windows
      "vendor/cjson/lib/windows"    -- For Windows
   }

project "WastelandAdventures"
   kind "ConsoleApp"
   language "C"
   targetdir "bin/%{cfg.buildcfg}"
   objdir "bin-int/%{cfg.buildcfg}"

   -- Source files
   files { "src/**.c" }

   -- Output and intermediate directories
   targetname "main"

   -- Platform-specific configurations
   filter "system:windows"
      links { "raylib", "cjson", "gdi32", "winmm", "Ws2_32" }
      defines { "WINDOWS" }

   -- Debug configuration
   filter "configurations:Debug"
      defines { "DEBUG" }
      symbols "On"
      optimize "Off"
      buildoptions { "-g", "-fno-asynchronous-unwind-tables", "-std=gnu99" }

   -- Release configuration
   filter "configurations:Release"
      defines { "RELEASE" }
      optimize "On"
      buildoptions { "-s", "-std=gnu99" }
