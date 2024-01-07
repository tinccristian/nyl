workspace "nyl"
	architecture "x64"

	configurations
	{
		"Debug",
		"Release",
		"Dist"
	}

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

project "Nyl"
	location "Nyl"
	kind "SharedLib"
	language "C++"
	
	targetdir("bin/" .. outputdir .. "/%{prj.name}")
	objdir("bin/" .. outputdir .. "/%{prj.name}/obj")

	files
	{
		"%{prj.name}/header/**.h",
		"%{prj.name}/source/**.cpp"
	}

	includedirs
	{
		"%{prj.name}/thirdparty/spdlog/include",
		"%{prj.name}/header"
	}
	
	filter "system:windows"
		cppdialect "C++20"
		staticruntime "On"
		systemversion "latest"

		defines
		{
			"NYL_PLATFORM_WINDOWS",
			"NYL_BUILD_DLL"
		}

		postbuildcommands
		{
			("{COPY} %{cfg.buildtarget.relpath} ../bin/" ..outputdir.. "/Antares")
		}

	filter "configurations:Debug"
		defines "NYL_DEBUG"
		symbols "On"

	filter "configurations:Debug"
		defines "NYL_RELEASE"
		optimize "On"

	filter "configurations:Debug"
		defines "NYL_DIST"
		optimize "On"

project "Antares"
	location "Antares"
	kind "ConsoleApp"
	language "C++"
	
	targetdir("bin/" .. outputdir .. "/%{prj.name}")
	objdir("bin/" .. outputdir .. "/%{prj.name}/obj")

	files
	{
		"%{prj.name}/header/**.h",
		"%{prj.name}/source/**.cpp"
	}

	includedirs
	{
		"Nyl/thirdparty/spdlog/include",
		"Nyl"
	}
	links
	{
		"Nyl"
	}
	
	filter "system:windows"
		cppdialect "C++20"
		staticruntime "On"
		systemversion "latest"

		defines
		{
			"NYL_PLATFORM_WINDOWS"
		}

	filter "configurations:Debug"
		defines "NYL_DEBUG"
		symbols "On"

	filter "configurations:Debug"
		defines "NYL_RELEASE"
		optimize "On"

	filter "configurations:Debug"
		defines "NYL_DIST"
		optimize "On"