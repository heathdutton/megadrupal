class Hm < Vm                   # VM-specific overrides of default settings
  Count      = 1
  Basebox    = "wheezy64"
  Box_url    = "http://dl.dropbox.com/u/937870/VMs/wheezy64.box"
  Shortname  = "aegir3-dev"          # Vagrant name (used for manifest name, e.g., hm.pp)
  Longname   = "Aegir 3 (dev)"     # VirtualBox name
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
