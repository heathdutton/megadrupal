<?php
/**
 * @file
 * Provides custom drush commands and callbacks.
 */

/**
 * Implements hook_drush_command().
 */
function bazaarvoice_productfeed_drush_command() {
  $items = array();

  $items['bazaarvoice-productfeed-generate'] = array(
    'callback' => 'drush_bazaarvoice_productfeed_generate',
    'description' => "Generate the Productfeed XML file.",
    'aliases' => array('bv-productfeed-generate', 'bvpfg'),
    'drupal dependencies' => array('bazaarvoice_productfeed'),
  );
  $items['bazaarvoice-productfeed-send'] = array(
    'callback' => 'drush_bazaarvoice_productfeed_send',
    'description' => "Send productfeed file to Bazaarvoice",
    'aliases' => array('bv-productfeed-send', 'bvpfs'),
    'drupal dependencies' => array('bazaarvoice_productfeed'),
  );

  return $items;
}

/**
 * Drush callback for regenerating productfeed.
 */
function drush_bazaarvoice_productfeed_generate() {
  module_load_include('inc', 'bazaarvoice_productfeed', 'includes/productfeed');
  // Trigger the productfeed to be generated.
  bazaarvoice_productfeed_trigger_rebuild();
  // Rebuild the productfeed.
  if (bazaarvoice_productfeed_rebuild_productfeed()) {
    drush_print(dt('Productfeed generated.'));
  }
  else {
    drush_set_error('BAZAARVOICE_PRODUCTFEED_ERROR', dt('Error generating productfeed.'));
  }
}

/**
 * Drush callback for sending productfeed to Bazaarvoice.
 */
function drush_bazaarvoice_productfeed_send() {
  module_load_include('inc', 'bazaarvoice_productfeed', 'includes/productfeed');
  // Get productfeed.
  if ($product_feed = bazaarvoice_productfeed_get_feed()) {
    // Attempt to FTP.
    if (bazaarvoice_productfeed_ftp_feed($product_feed)) {
      drush_print(dt('Productfeed sent to Bazaarvoice'));
    }
    else {
      drush_set_error('BAZAARVOICE_PRODUCTFEED_ERROR', dt('Error sending productfeed to Bazaarvoice'));
    }
  }
  else {
    drush_set_error('BAZAARVOICE_PRODUCTFEED_ERROR', dt('No productfeed found, unable to send to Bazaarvoice'));
  }
}
