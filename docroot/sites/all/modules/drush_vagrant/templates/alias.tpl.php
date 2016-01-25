<?php
/**
 * @file
 * Default implementation to output a drush alias.
 *
 * Variables:
 * - $alias: The name of the alias (user input.)
 * - $uri: The URI to reach the site (user input.)
 * - $docroot: The filesystem path to the site's docroot (user input.)
 * - $ssh_options: The parsed output of `vagrant ssh_config`.
 */
?>
<?php print "<?php\n\n" ?>
$aliases['<?php print $alias; ?>'] = array(
  'uri' => '<?php print $uri; ?>',
  'root' => '<?php print $docroot; ?>',
  'remote-host' => 'default',
  'remote-user' => 'vagrant',
  'ssh-options' => "<?php print $ssh_options; ?>",
);
