include "Dependencies.lua"

workspace "AudioChat"
    architecture "x86_64"
    startproject "Server"
    configurations
    {
        "Debug",
        "Release"
    }

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

group "Dependencies"
	include "AudioChat/Dependencies/unp"
group ""

group "Core"
	include "AudioChat/Client"
	include "AudioChat/Server"
group ""