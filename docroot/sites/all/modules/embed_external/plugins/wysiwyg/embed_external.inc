<?php

/**
 * Implements hook_INCLUDE_plugin().
 */
function embed_external_embed_external_plugin() {
  $plugins['embed_external'] = array(
    'title' => t('Embed External'),
    'vendor url' => 'https://www.drupal.org/project/embed_external',
    'icon file' => 'embed_external.png',
    'icon title' => t('Embed external media'),
    'settings' => array(),
  );
  return $plugins;
}
