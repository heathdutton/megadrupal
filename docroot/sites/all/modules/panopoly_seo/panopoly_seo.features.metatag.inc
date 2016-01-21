<?php
/**
 * @file
 * panopoly_seo.features.metatag.inc
 */

/**
 * Implements hook_metatag_export_default().
 */
function panopoly_seo_metatag_export_default() {
  $config = array();

  // Exported Metatag config instance: node.
  $config['node'] = array(
    'instance' => 'node',
    'config' => array(
      'title' => array(
        'value' => '[node:title] | [site:name]',
      ),
      'description' => array(
        'value' => '[node:summary]',
      ),
      'author' => array(
        'value' => '[node:author:field_google_url]',
      ),
      'og:title' => array(
        'value' => '[node:title]',
      ),
      'og:description' => array(
        'value' => '[node:summary]',
      ),
    ),
  );

  // Exported Metatag config instance: node:panopoly_page.
  $config['node:panopoly_page'] = array(
    'instance' => 'node:panopoly_page',
    'config' => array(
      'image_src' => array(
        'value' => '[node:field_featured_image]',
      ),
      'og:type' => array(
        'value' => 'website',
      ),
      'og:image' => array(
        'value' => '[node:field_featured_image]',
      ),
    ),
  );

  return $config;
}
