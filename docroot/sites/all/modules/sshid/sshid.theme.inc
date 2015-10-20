<?php
/**
 * @file
 *  Theme hook implementations for SSH module
 */

/**
 * Default implementation for sshid_config theme hook.
 */
function theme_sshid_config($variables) {
  $config = '';

  foreach ($variables['hosts'] as $host => $host_config) {
    $config .= theme('sshid_host', array('host' => $host, 'config' => $host_config)) . "\n";
  }

  return $config;
}

/**
 * Default implementation for sshid_host theme hook.
 */
function theme_sshid_host($variables) {
  $host = $variables['host'];
  $host_config = $variables['config'];

  $config = t("Host @host\n", array('@host' => $host));
  foreach ($host_config as $name => $value) {
    $config .= "  {$name} = {$value}";
  }

  return $config;
}
