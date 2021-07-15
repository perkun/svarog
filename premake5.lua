workspace "Svarog"
	architecture "x64"

	configurations
	{
		"Debug",
		"Release",
		"Dist"
	}

--outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"
outputdir = "%{cfg.buildcfg}"


include "Svarog"
include "rarog"
include "meshlc"
include "meshview"


