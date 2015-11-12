<?php

/**
 * @file
 * Specialised hook to register plugin presence with the WYSIWYG module.
 */

/**
 * WYSIWYG module integration for map token builder.
 */
function wysiwyg_map_wysiwyg_map_tokenbuilder_plugin() {
  $plugins['wysiwyg_map_tokenbuilder'] = array(
    'title' => t('WYSIWYG Map Token Builder'),
    'icon file' => 'wysiwyg_map.toolbar_icon.png',
    'icon title' => t('WYSIWYG Map Token Builder'),
    'settings' => array(),
  );
  return $plugins;
}
