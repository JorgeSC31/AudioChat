project "audio-api"
	kind "ConsoleApp"
	language "C++"
	-- cppdialect "C++17"

	targetdir ("%{wks.location}/bin/" .. outputdir .. "/%{prj.name}")
	objdir ("%{wks.location}/obj/" .. outputdir .. "/%{prj.name}")

	files
	{
		"*.h",
		"*.c",
		"*.cpp",
	}

	includedirs
	{
    }

	links
	{
        "asound",
        "m",
		"portaudio"
	}

	filter "configurations:Debug"
		runtime "Debug"
		symbols "on"

	filter "configurations:Release"
		runtime "Release"
		optimize "on"