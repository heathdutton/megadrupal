<?php

// @todo only search for subdomain + hostname, because currently the original
// path will only have its hostname portion replaced with the configured
// replacement url.
define('HAP_ROUTER_URL', '
  /^                                          # Anchor to beginning of string
    (?P<url>                                  # Capture entire URL @see RFC 3986
      (?P<protocol>(?:[\w][\w\d+\-.]*)):\/\/  # Capture the protocol protocol
      (?P<subdomains>(?:[\w\d\-_]+\.)*)       # Capture any subdomains
      (?P<hostname>[\w\d\-_]+)\/              # Capture the hostname
      (?P<uri>[\S]*)                          # Capture any remaining end of the URL as the URI.
    )$                                        # Anchor to end of string
  /x');


/******************************************************************************
 * Drupal Hooks
 *****************************************************************************/

/**
 * Implements hook_hap_info().
 */
function hap_router_hap_info() {
  return array(
    'name' => 'Router',
    'description' => 'Routes alternatives destinations to incoming/outgoing paths',
    'configure' => 'admin/config/hap/router',
    'hooks' => array('menu', 'url_outbound_alter'),
  );
}


/**
 * Implements hook_menu().
 */
function hap_router_menu() {
  $items['admin/config/hap/router'] = array(
    'title' => '★ Highly Attractive Router',
    'description' => 'something something',
    'page callback' => 'drupal_get_form',
    'page arguments' => array('hap_router_settings'),
    'access arguments' => array('administer site settings'),
    'file path' => hap_path(),
    'file' => 'router.inc',
  );

  return $items;
}


/**
 * Implements hook_url_outbound_alter().
 */
function hap_router_url_outbound_alter(&$path, &$options, $original_path) {
  // If the routing files configuration is not enabled, do nothing.
  if (variable_get('hap_router_file_enabled', FALSE)) {

    // Using regex.inc to parse the extension out of the path and determine if
    // it is a routable file-type.
    if (_hap_router_is_routable($path)) {
      if (preg_match(HAP_ROUTER_URL, $path, $matches)) {
        $path = str_replace($matches['hostname'], variable_get('hap_router_host', '') , $path);
      }
    }
  }
}


/******************************************************************************
 * Form Builders/Validators/Submission Handlers
 *****************************************************************************/

/**
 * Form builder for hap_router_settings().
 */
function hap_router_settings($form, &$form_state) {

  $form['files'] = array(
    '#type' => 'fieldset',
    '#title' => t('Files'),
  );

  $form['files']['hap_router_file_enabled'] = array(
    '#type' => 'checkbox',
    '#title' => 'Enable file router',
    '#default_value' => variable_get('hap_router_file_enabled', 0),
  );

  $form['files']['hap_router_host'] = array(
    '#type' => 'textfield',
    '#title' => 'Host',
    '#description' => t('Type in the fully qualified URL to source assets from. ex. http://dev.example.com'),
    '#default_value' => variable_get('hap_router_host', array()),
    '#states' => array(
      'visible' => array(
        ':input[files[hap_router_file_enabled]]' => array('checked' => TRUE),
      ),
    ),
  );

  $form['files']['hap_router_file_types'] = array(
    '#type' => 'checkboxes',
    '#title' => t('Allowable file-types'),
    '#options' => drupal_map_assoc(array('png', 'jpg', 'gif', 'pdf', 'swf', 'mp3', 'txt', 'rtf', 'doc')),
    '#default_value' => variable_get('hap_router_file_types', array()),
    '#states' => array(
      'visible' => array(
        ':input[files[hap_router_file_enabled]]' => array('checked' => TRUE),
      ),
    ),
  );
  return system_settings_form($form);
}


/******************************************************************************
 * Helper Functions
 *****************************************************************************/

/**
 * Helper function that checks if a path has a qualified extention for hap
 * to re-route.
 */
function _hap_router_is_routable($path) {
  return in_array(substr(strrchr($path, '.'), 1), _hap_router_get_file_types());
}


/**
 * Helper function to generate a clean array of compatible file extensions to
 * file router.
 */
function _hap_router_get_file_types() {
  static $file_types = array();

  if (!$file_types) {
    $types = variable_get('hap_router_file_types', array());
    foreach ($types as $key => $value) {
      if ($value) {
        $file_types[$key] = $key;
      }
    }
  }

  return $file_types;
}
