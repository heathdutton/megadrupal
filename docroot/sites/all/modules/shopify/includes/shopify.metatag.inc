<?php
/**
 * @file
 * Contains functions relating to metatags integration.
 */

/**
 * Implements hook_metatag_config_default().
 */
function shopify_metatag_config_default() {
  // @todo these produce errors
  // "The Page title is using the following invalid tokens: [shopify_product:title]."
  // "The Description is using the following invalid tokens: [shopify_product:body-html]."

  $configs = array();
  $config = new stdClass();
  $config->disabled = FALSE; /* Edit this to true to make a default config disabled initially */
  $config->api_version = 1;
  $config->instance = 'shopify_product';
  $config->config = array(
    'title' => array(
      'value' => '[shopify_product:title] | [site:name]',
    ),
    'description' => array(
      'value' => '[shopify_product:body-html]',
    ),
  );

  $configs[$config->instance] = $config;

  return $configs;
}