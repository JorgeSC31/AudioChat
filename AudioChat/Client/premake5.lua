project "Client"
	kind "ConsoleApp"
	language "C++"
	cppdialect "C++17"

	targetdir ("%{wks.location}/bin/" .. outputdir .. "/%{prj.name}")
	objdir ("%{wks.location}/obj/" .. outputdir .. "/%{prj.name}")

	files
	{
		"*.h",
		"*.cpp"
	}

	includedirs
	{
		"%{IncludeDir.unp}",
		"%{IncludeDir.epoll_api}",
		"%{IncludeDir.audio_api}",
    }

	links
	{
		"unp",
		"epoll-api",
		"audio-api"
	}

	filter "configurations:Debug"
		runtime "Debug"
		symbols "on"

	filter "configurations:Release"
		runtime "Release"
		optimize "on"