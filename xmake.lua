add_rules("mode.debug", "mode.release")

target("car")
    set_kind("binary")
    add_files("src/*.cpp")
    add_syslinks("z","pthread")
    add_links("serial")
