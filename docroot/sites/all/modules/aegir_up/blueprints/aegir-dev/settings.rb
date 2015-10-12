class Hm < Vm                   # VM-specific overrides of default settings
  Count      = 1
  Basebox    = "debian-LAMP-2012-03-29"
  Box_url    = "http://ergonlogic.com/files/boxes/debian-LAMP-current.box"
  Shortname  = "aegir-dev"             # Vagrant name (used for manifest name, e.g., hm.pp)
  Longname   = "Aegir Dev"     # VirtualBox name
 # NFS_shares = { "aegir_root" => "/var/aegir",
 #              }
  Facts      = { "aegir_user"        => "aegir",
                 "aegir_root"        => "/var/aegir",
                 "aegir_user_exists" => "true",
                 "aegir_version"     => "6.x-1.9",
               }
end
