class Util < Vm             # VM-specific overrides of default settings
  Count      = 1
  Basebox    = "debian-LAMP-2012-03-29"
  Shortname  = "util"          # Vagrant name (used for manifest name, e.g., hm.pp)
  Longname   = "Utility"       # VirtualBox name
end
