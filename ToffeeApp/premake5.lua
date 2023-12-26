project "ToffeeApp"
   kind "ConsoleApp"
   language "C++"
   cppdialect "C++17"
   targetdir "bin/%{cfg.buildcfg}"
   staticruntime "off"

   files { "src/**.h", "src/**.cpp" }

   includedirs
   {
      "../vendor/imgui",
      "../vendor/glfw/include",

      "../Toffee/src",

      "%{IncludeDir.VulkanSDK}",
      "%{IncludeDir.glm}",
   }

    links
    {
        "Toffee"
    }

   targetdir ("../bin/" .. outputdir .. "/%{prj.name}")
   objdir ("../bin-int/" .. outputdir .. "/%{prj.name}")

   filter "system:windows"
      systemversion "latest"
      defines { "TF_PLATFORM_WINDOWS" }

   filter "configurations:Debug"
      defines { "TF_DEBUG" }
      runtime "Debug"
      symbols "On"

   filter "configurations:Release"
      defines { "TF_RELEASE" }
      runtime "Release"
      optimize "On"
      symbols "On"

   filter "configurations:Dist"
      kind "WindowedApp"
      defines { "TF_DIST" }
      runtime "Release"
      optimize "On"
      symbols "Off"