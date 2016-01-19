<?php
/**
 * @file
 * Define variable registry for Legal Generator.
 *
 * Follows convention of variables.inc used in Media:Youtube, and others.
 */

/**
 * This is the variable namespace, automatically prepended to module variables.
 */
define('LEGAL_GEN_NAMESPACE', 'legal_gen__');

/**
 * Wrapper for variable_get() using the Legal Generator variable registry.
 *
 * @param string $name
 *   The variable name to retrieve. Note that it will be namespaced by
 *   pre-pending LEGAL_GEN_NAMESPACE, as to avoid variable collisions
 *   with other modules.
 * @param unknown $default
 *   An optional default variable to return if the variable hasn't been set
 *   yet. Note that within this module, all variables should already be set
 *   in the legal_gen_variable_default() function.
 *
 * @return unknown
 *   Returns the stored variable or its default.
 *
 * @see legal_gen_variable_set()
 * @see legal_gen_variable_del()
 * @see legal_gen_variable_default()
 */
function legal_gen_variable_get($name, $default = NULL) {
  // Allow for an override of the default.
  // Useful when a variable is required (like $path), but namespacing is still
  // desired.
  if (!isset($default)) {
    $default = legal_gen_variable_default($name);
  }
  // Namespace all variables.
  $variable_name = LEGAL_GEN_NAMESPACE . $name;
  return variable_get($variable_name, $default);
}

/**
 * Wrapper for variable_set() using the Legal Generator variable registry.
 *
 * @param string $name
 *   The variable name to set. Note that it will be namespaced by
 *   pre-pending LEGAL_GEN_NAMESPACE, as to avoid variable collisions with
 *   other modules.
 * @param unknown $value
 *   The value for which to set the variable.
 *
 * @return unknown
 *   Returns the stored variable after setting.
 *
 * @see legal_gen_variable_get()
 * @see legal_gen_variable_del()
 * @see legal_gen_variable_default()
 */
function legal_gen_variable_set($name, $value) {
  $variable_name = LEGAL_GEN_NAMESPACE . $name;
  return variable_set($variable_name, $value);
}

/**
 * Wrapper for variable_del() using the Legal Generator variable registry.
 *
 * @param string $name
 *   The variable name to delete. Note that it will be namespaced by
 *   pre-pending LEGAL_GEN_NAMESPACE, as to avoid variable collisions with
 *   other modules.
 *
 * @see legal_gen_variable_get()
 * @see legal_gen_variable_set()
 * @see legal_gen_variable_default()
 */
function legal_gen_variable_del($name) {
  $variable_name = LEGAL_GEN_NAMESPACE . $name;
  variable_del($variable_name);
}

/**
 * The default variables within the Legal Generator namespace.
 *
 * @param string $name
 *   Optional variable name to retrieve the default. Note that it has not yet
 *   been pre-pended with the LEGAL_GEN_NAMESPACE namespace at this time.
 *
 * @return unknown
 *   The default value of this variable, if it's been set, or NULL, unless
 *   $name is NULL, in which case we return an array of all default values.
 *
 * @see legal_gen_variable_get()
 * @see legal_gen_variable_set()
 * @see legal_gen_variable_del()
 */
function legal_gen_variable_default($name = NULL) {
  static $defaults;
  global $base_url;

  if (!isset($defaults)) {
    $defaults = array(
      'company_full' => 'YourCompany, Inc.',
      'company_short' => 'YourCompany',
      'company_county' => 'Your County',
      'company_state' => 'Your State',
      'company_country' => 'U.S.A.',
      'site_url' => substr($base_url, 7),
    );
  }

  if (!isset($name)) {
    return $defaults;
  }

  if (isset($defaults[$name])) {
    return $defaults[$name];
  }
}

/**
 * Return the fully namespace variable name.
 *
 * @param string $name
 *   The variable name to retrieve the namespaced name.
 *
 * @return string
 *   The fully namespace variable name, prepended with
 *   LEGAL_GEN_NAMESPACE.
 */
function legal_gen_variable_name($name) {
  return LEGAL_GEN_NAMESPACE . $name;
}
