<?php
/**
 * @file
 * openlucius_ldap.features.inc
 */

/**
 * Implements hook_ctools_plugin_api().
 */
function openlucius_ldap_ctools_plugin_api($module = NULL, $api = NULL) {
  if ($module == "strongarm" && $api == "strongarm") {
    return array("version" => "1");
  }
}
