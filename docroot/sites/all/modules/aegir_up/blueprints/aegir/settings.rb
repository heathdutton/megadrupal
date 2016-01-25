class Hm < Vm                   # VM-specific overrides of default settings
  Count      = 1
  Basebox    = "debian-LAMP-2012-03-29"
  Box_url    = "http://ergonlogic.com/files/boxes/debian-LAMP-current.box"
  Shortname  = "aegir-up"             # Vagrant name (used for manifest name, e.g., hm.pp)
  Longname   = "Aegir-Up"     # VirtualBox name
  Dir_shares = {
                 "aegir-up" => {
                   "host_path"  => "shared_platforms",
                   "guest_path" => "/var/aegir/shared_platforms",
                   "nfs" => { :nfs     => true,
                              :create  => true,
                              :remount => true,
                            }
                 }
               }
end
