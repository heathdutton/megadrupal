<?php
/**
 * @file
 * Provides documentation for the Share Selection Links API.
 */

/**
 * Obtains all available share selection links.
 *
 * @return
 *   An array containing all share selection links, keyed by name.
 */
function hook_share_selection_links() {
  $links = array();

  $links['myservice'] = array(
    // The name of the service.
    'name' => 'MyService',
    // A short description for the link.
    'description' => t('Share this post on MyService'),
    // The service icon. it will search on /images folder inside your module.
    'icon' => 'myservice.png',
    // JavaScript to add when this link is processed.
    'javascript' => drupal_get_path('module', 'myservice') .'/myservice.js',
    // External javaScript to add when this link is processed like services APIs.
    'external_js' => 'https://apis.google.com/js/client:plusone.js',
    // CSS to add when this link is processed, can be a string or an array.
    'css' => drupal_get_path('module', 'myservice') .'/myservice.css',
    // Custom options to use in javascript by the service.
    // Array formatted like this:
    // 'custom_options' => array(
    //   'option_machine_name' => Option field title,
    // ),
    // These options could be "set in admin/config/services/share-selection/services" and Drupal tokens can be used.
    // And will be return camelCased to js with service as prefix
    // Example: option_machine_name will be Drupal.settings.shareSelection.myserviceOptionMachineName
    'custom_options' => array(
      'my_custom_option' => t('My custom option'),
      'other_custom_option' => t('Other custom option'),
    ),
  );

  return $links;
}

/**
 * Allows alteration of the share selection Links.
 *
 * @param $links
 *   The constructed array of share selection links.
 */
function hook_share_selection_links_alter(&$links) {
  if (isset($links['myservice'])) {
    // Change the icon of MyService.
    $links['myservice']['icon'] = 'other-icon.png';
  }
}
