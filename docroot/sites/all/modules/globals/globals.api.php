<?php

/**
 * @file
 * API information for Globals
 *
 * @license GPL v2 http://www.fsf.org/licensing/licenses/gpl.html
 * @author Chris Skene chris at xtfer dot com
 * @copyright Copyright(c) 2015 Christopher Skene
 */

/**
 * Define global properties.
 */
function hook_globals() {
  $globals = array();

  $globals['site_name'] = array(
    'name' => t('Site name'),
    'description' => t('The Site Name.'),
    'type' => GLOBALS_TYPE_VARIABLE,
    'key' => 'site_name',
    'form element' => array(
      '#type' => 'textfield',
      '#required' => TRUE,
    ),
  );

  return $globals;
}

/**
 * Alter global properties.
 */
function hook_globals_alter(&$globals) {
  $globals['site_name']['default'] = t('Foo');
}
