add_rules("mode.debug", "mode.release")
set_languages("cxx14")

target("car")
    set_kind("binary")
    add_files("src/*.cpp")
    add_syslinks("z","pthread")
    add_links("serial")
