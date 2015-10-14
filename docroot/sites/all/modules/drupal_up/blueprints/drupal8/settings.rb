class Hm < Vm                   # VM-specific overrides of default settings
  Count      = 1
  Basebox    = "debian7-LAMP-i386-2012-12-18"
  Box_url    = "http://ergonlogic.com/files/boxes/debian7-LAMP-i386-current.php"
  Shortname  = "drupal8"             # Vagrant name (used for manifest name, e.g., hm.pp)
  Longname   = "Drupal 8.x"     # VirtualBox name

end
