/**
 * @file
 * Properly handle the use of jQuery through require.js and Drupal.
 */

/**
 * Override the require.js definition of jQuery.
 *
 * Drupal ships its own version of jQuery, so override the Component Installer
 * definition of jQuery with the one from Drupal.
 */
define('jquery', [], function () {
  return jQuery;
});

/**
 * Example of using jQuery through require.js.
 */
require(['jquery'], function ($) {
  // jQuery is now available.
  return $;
});
