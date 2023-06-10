include "Dependencies.lua"

workspace "AudioChat"
    architecture "x86_64"
    configurations
    {
        "Debug",
        "Release"
    }

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

group "Dependencies"
	include "AudioChat/Dependencies/unp"
	include "AudioChat/Dependencies/epoll-api"
	include "AudioChat/Dependencies/broadcast"
group ""

group "Core"
	include "AudioChat/Client"
	include "AudioChat/Server"
group ""