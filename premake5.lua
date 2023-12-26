-- premake5.lua
workspace "App"
   architecture "x64"
   configurations { "Debug", "Release", "Dist" }
   startproject "ToffeeApp"

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

include "External.lua"
include "ToffeeApp"