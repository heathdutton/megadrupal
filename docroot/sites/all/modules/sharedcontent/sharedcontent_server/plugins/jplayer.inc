<?php

/**
 * @file
 * RichMedia plugin for jPlayer.
 *
 * See project:
 * drupal.org/project/jplayer
 */
class SharedContentRichMediaJPlayer extends SharedContentRichMediaBase {

  /**
   * Overrides getCssFiles().
   */
  function getCssFiles($index, $view_mode, $langcode) {
    return array(
      drupal_get_path('module', 'jplayer') . '/theme/jplayer.css',
    );
  }

  /**
   * Overrides getJsFiles().
   */
  function getJsFiles($index, $view_mode, $langcode) {
    $library = drupal_get_library('jplayer', 'jplayer');
    if ($library) {
      $result = array_keys($library['js']);
    }
    $result[] = drupal_get_path('module', 'jplayer') . '/theme/jplayer.js';
    return $result;
  }

  /**
   * Overrides getJsSettings().
   */
  function getJsSettings($index, $view_mode, $langcode) {
    $js = drupal_add_js();
    $result = array();
    foreach ($js['settings']['data'] as $data) {
      if (isset($data['jPlayer'])) {
        $result['jPlayer'] = $data['jPlayer'];
      }
      elseif (isset($data['jplayerInstances'])) {
        foreach ($this->getIndexes($index) as $index_all) {
          foreach ($data['jplayerInstances'] as $key => $value) {
            if (strpos($key, $index_all->entity_type . '-' . $index_all->entity_id)) {
              $result['jplayerInstances'][$key] = $value;
            }
          }
        }
      }
    }
    return $result;
  }
}
