class Hm < Vm                   # VM-specific overrides of default settings
  Count      = 1
  Basebox    = "debian-LAMP-2012-03-29"
  Box_url    = "http://ergonlogic.com/files/boxes/debian-LAMP-current.box"
  Shortname  = "drupal7"             # Vagrant name (used for manifest name, e.g., hm.pp)
  Longname   = "Drupal 7.x"     # VirtualBox name

end
