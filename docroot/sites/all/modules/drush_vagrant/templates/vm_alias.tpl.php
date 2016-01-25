<?php
/**
 * @file
 * Template to generate a drush alias for a VM.
 *
 * Variables:
 * - $vm_alias: The Vagrant name of the VM.
 * - $fqdn: The full hostname of the VM.
 * - $ip_addr: The IP address of the VM.
 */
?>
$aliases['<?php print $vm_alias; ?>'] = array(
  'type' => 'vm',
  'vm-name' => '<?php print $vm_alias; ?>',
  'fqdn' => '<?php print $fqdn; ?>',
  'ip' => '<?php print $ip_addr; ?>',
);
