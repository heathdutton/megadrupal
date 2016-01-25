<?php
/**
 * @file
 * Default implementation to output a Drush Vagrant config file.
 *
 * Variables:
 * - $global_path: The filesystem path to global.rb config file.
 * - $project_name: The name of the Vagrant project.
 * - $subnet: The /24 subnet the project will use.
 * - $facts: An array of Facter facts.
 * - $modules: An array of directories to look for Puppet modules in.
 */
?>
require "<?php print $global_path; ?>"
require "./settings.rb"

class Conf < Global
  Project   = "<?php print $project_name; ?>"
  Subnet    = "<?php print $subnet; ?>"                  # 192.168.###.0/24 subnet for this network
  <?php
    if (isset($facts)) {
      print "Facts     = {\n";
      foreach ($facts as $name => $fact) {
        print '                "' . $name . '" => ' . $fact . ",\n";
      }
      print "              }\n";
    }
    if (isset($modules)) {
      print "  Modules   = {\n";
      foreach ($modules as $name => $dir) {
        print '                "' . $name . '" => "' . $dir . "\",\n";
      }
      print "              }\n";
    }
  ?>
end
