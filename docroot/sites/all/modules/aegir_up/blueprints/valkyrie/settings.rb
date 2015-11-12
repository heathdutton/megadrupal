class Client < Vm                   # VM-specific overrides of default settings
  Count      = 1
  Basebox    = "debian-LAMP-i386-2012-03-29"
  Box_url    = "http://ergonlogic.com/files/boxes/debian-LAMP-i386-current.box"
  Shortname  = "client"             # Vagrant name (used for manifest name, e.g., hm.pp)
  Longname   = "Valkyrie Client"     # VirtualBox name
  Facts      = { "aegir_user"        => "aegir",
                 "aegir_root"        => "/var/aegir",
                 "aegir_user_exists" => "true",
                 "aegir_version"     => "6.x-1.x",
               }
  Script     = "repo_setup.sh"
#  Host_IP    = "192.168.222.11"
end
class Server < Vm                   # VM-specific overrides of default settings
  Count      = 0
  Basebox    = "debian-LAMP-i386-2012-03-29"
  Box_url    = "http://ergonlogic.com/files/boxes/debian-LAMP-i386-current.box"
  Shortname  = "server"             # Vagrant name (used for manifest name, e.g., hm.pp)
  Longname   = "Valkyrie Server"     # VirtualBox name
  Facts      = { "aegir_user"        => "aegir",
                 "aegir_root"        => "/var/aegir",
                 "aegir_user_exists" => "true",
                 "aegir_version"     => "6.x-1.x",
               }
  Host_IP    = "192.168.222.12"
end
