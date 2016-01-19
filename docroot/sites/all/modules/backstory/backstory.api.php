<?php

/**
 * @file
 * Documents the hooks provided by the Backstory module.
 */

/**
 * Allows modules to alter the default determination of whether or not the
 * Backstory JavaScript should be included on a given page.
 *
 * @param &$active
 *   The current determination, can only be altered to TRUE or FALSE. Note that
 *   changing this value may override the settings specifically configured by
 *   an administrator.
 */
function hook_backstory_validate_display_alter(&$active) {
  // No example.
}

/**
 * Allows modules to alter the array of role IDs used to determine for which
 * roles Backstory should be disabled.
 *
 * @param &$exempt_roles
 *   The array of exempt role IDs originally derived from the Backstory settings
 *   form. Note that changing this array may override settings specifically
 *   configured by an administrator.
 */
function hook_backstory_exempt_roles_alter(&$exempt_roles) {
  // No example.
}

/**
 * Allows modules to alter the paths or path patterns used to specifically
 * enable or disable the Backstory JavaScript on certain pages of the site.
 *
 * @param &$paths
 *   A string with paths and path patterns separated by line breaks for paths
 *   that are designated for Backstory inclusion or exemption based on the
 *   path mode variable. Note that changing this string may override settings
 *   specifically configured by an administrator. New paths should be added
 *   following line breaks, e.g. "\r\n".
 */
function hook_backstory_paths_alter(&$paths) {
  // No example.
}

/**
 * Alerts modules that the Backstory JavaScript has been added to the current page.
 */
function hook_backstory_add_js() {
  // No example.
}
