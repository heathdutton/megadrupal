<?php

/**
 * @file
 * Mapping functions of Domain ThemeKey.
 */

/**
 * ThemeKey mapping function.
 *
 * Set a ThemeKey property's value (destination)
 * with the aid of another ThemeKey property (source).
 *
 * src: system:dummy
 * dst: domain:site_name
 *
 * @param string $dummy
 *   String containing current value of ThemeKey property system:dummy.
 *
 * @return string
 *   Current domain site_name.
 */
function domain_themekey_dummy2domain_site_name($dummy) {
  return get_current_domain_info('sitename');
}

/**
 * ThemeKey mapping function.
 *
 * Set a ThemeKey property's value (destination)
 * with the aid of another ThemeKey property (source).
 *
 * src: system:dummy
 * dst: domain:id
 *
 * @param string $dummy
 *   String containing current value of ThemeKey property system:dummy.
 *
 * @return string
 *   Current domain id.
 */
function domain_themekey_dummy2domain_id($dummy) {
  return get_current_domain_info('domain_id');
}

/**
 * ThemeKey mapping function.
 *
 * Set a ThemeKey property's value (destination)
 * with the aid of another ThemeKey property (source).
 *
 * src: system:dummy
 * dst: domain:machine_name
 *
 * @param string $dummy
 *   String containing current value of ThemeKey property system:dummy.
 *
 * @return string
 *   Current domain machine_name.
 */
function domain_themekey_dummy2domain_machine_name($dummy) {
  return get_current_domain_info('machine_name');
}

/**
 * ThemeKey mapping function.
 *
 * Set a ThemeKey property's value (destination)
 * with the aid of another ThemeKey property (source).
 *
 * src: system:dummy
 * dst: domain:is_default
 *
 * @param string $dummy
 *   String containing current value of ThemeKey property system:dummy.
 *
 * @return string
 *   Default domain setting.
 */
function domain_themekey_dummy2domain_is_default($dummy) {
  return get_current_domain_info('is_default');
}

/**
 * ThemeKey mapping function.
 *
 * Set a ThemeKey property's value (destination)
 * with the aid of another ThemeKey property (source).
 *
 * src: system:host
 * dst: domain:is_alias
 *
 * @param string $host
 *   String containing current value of ThemeKey property system:host.
 *
 * @return bool
 *   If host is alias for current domain then returned value will be TRUE
 *   else FALSE.
 */
function domain_themekey_host2domain_is_alias($host) {
  $is_alias = FALSE;
  $aliases = get_current_domain_info('aliases');

  foreach ($aliases as $alias_info) {
    if ($host === $alias_info['pattern']) {
      $is_alias = TRUE;
      break;
    }
  }

  return $is_alias;
}

/**
 * Helper function to get info about current domain.
 *
 * @param string $domain_property
 *   String containing domain property name.
 *
 * @return array
 *   An array with data about current domain.
 */
function get_current_domain_info($domain_property) {
  $current_domain = domain_get_domain();
  $domains = domain_domains();

  return $domains[$current_domain['domain_id']][$domain_property];
}
