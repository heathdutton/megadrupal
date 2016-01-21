<?php
/**
 * @file
 * Helper functions for settings-related functionality.
 */

/**
 * Returns default values for all commerce_bpc settings.
 *
 * @return array
 *   An array keyed by settings-group labels, whose values are arrays whose
 *   key/value pairs specify default values for the settings in this group.
 */
function commerce_bpc_setting_defaults() {
  $settings = array();

  // Defaults for patterns.
  $settings['default'] = array(
    'sku_pattern' => '[bulk_defaults:entered_sku]--[bulk_defaults:combination_values]',
    'sku_separator' => '-',
    'title_pattern' => '[bulk_defaults:entered_title] ([bulk_defaults:combination_labels])',
    'title_separator' => ', ',
  );

  // Defaults for display node creation.
  $settings['display'] = array(
    'creation_method' => 'wizard',
    'auto_content_type' => NULL,
    'auto_node_title_pattern' => '[bulk_defaults:entered_title]',
    'auto_redirect' => 'product_listing',
    'auto_redirect_custom' => NULL,
  );

  return $settings;
}
/**
 * Retrieves a value form the variable table.
 *
 * @param string $group
 *   The settings group of the setting to be retrieved.
 * @param string $varname
 *   The name of the variable to retrieve.
 * @param string $type
 *   (optional) if set to the machine name of a product type, the function
 *   will check if a product type specific version of the value exists (i.e.
 *   a variable with the name '$varname . '_' . $product_type).
 *
 * @return mixed
 *   If $product_type is set and a product type specific value exists, it
 *   it will be returned. Otherwise, the non-specific value will be returned.
 *   failing that, a default value will be returned.
 */
function commerce_bpc_setting($group, $varname, $type = NULL) {
  // We short-circuit the look-up of defaults and global settings for 'override'
  // settings, for in this case, they do not make sense.
  if ($varname == 'override') {
    return variable_get('commerce_bpc_' . $group . '_override_' . $type, FALSE);
  }
  $defaults = commerce_bpc_setting_defaults();
  $value = isset($defaults[$group][$varname]) ? $defaults[$group][$varname] : NULL;
  $varname = 'commerce_bpc_' . $group . '_' . $varname;
  $value = variable_get($varname, $value);
  if (!empty($type) && commerce_bpc_setting($group, 'override', $type)) {
    $value = variable_get($varname . '_' . $type, $value);
  }
  return $value;
}
/**
 * Sets a variable value.
 *
 * @param string $group
 *   String specifying the settings group this variable belongs to.
 * @param string $varname
 *   The name of the variable.
 * @param mixed $value
 *   The value of the variable.
 * @param string $type
 *   (optional) The product type for which the value is being set. Pass NULL in
 *   order to set the global value.
 */
function commerce_bpc_setting_set($group, $varname, $value, $type = NULL) {
  $varname = 'commerce_bpc_' . $group . '_' . $varname;
  if (!empty($type)) {
    $varname .= '_' . $type;
  }
  variable_set($varname, $value);
}
