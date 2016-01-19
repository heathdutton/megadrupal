<?php

/**
 * @file
 * RichMedia plugin for JW Player.
 *
 * See project:
 * drupal.org/project/jw_player
 */
class SharedContentRichMediaJwPlayer extends SharedContentRichMediaBase {

  /**
   * Overrides getJsFiles().
   */
  function getJsFiles($index, $view_mode, $langcode) {
    $js_files = array();
    if ($this->showJWPlayer($index, $view_mode)) {
      $js_files[] = libraries_get_path('jwplayer') . '/jwplayer.js';
      if (!variable_get('jw_player_inline_js', FALSE)) {
        $js_files[] = drupal_get_path('module', 'jw_player') . '/jw_player.js';
      }
      // @todo: Using default value of 6 to avoid issues with old players. This
      //   requires an explicit save of this variable.
      if (variable_get('jw_player_version', NULL) == 6) {
        $js_files[] = libraries_get_path('jwplayer') . '/jwplayer.html5.js';
      }
    }
    return $js_files;
  }

  /**
   * Overrides getJsSettings().
   */
  function getJsSettings($index, $view_mode, $langcode) {
    $settings = array();
    if ($this->showJWPlayer($index, $view_mode)) {
      $js = drupal_add_js();
      foreach ($js['settings']['data'] as $data) {
        if (isset($data['jw_player'])) {
          foreach ($data['jw_player'] as $key => $value) {
            $settings['jw_player'][$key] = $value;
          }
        }
      }
    }
    return $settings;
  }

  /**
   * Overrides getInlineJs().
   *
   * Note: We're delivering the player paid key here to other domains.
   * This might be a licensing issue. Check with longtail to either have separate licences for all domains or get confirmation this is allowed.
   */
  function getInlineJs($index, $view_mode, $langcode) {
    $js = array();
    $show = $this->showJWPlayer($index, $view_mode);
    // Add key if available.
    if ($show && $key = variable_get('jw_player_key', NULL)) {
      $js[] = 'jwplayer.key="' . $key . '"';
    }
    return $js;
  }

  /**
   * Tests if a JW Player will be rendered.
   *
   * @param $index
   *   The index record.
   * @param $view_mode
   *   The view mode.
   * @return bool
   *   TRUE if a Carousel Gallery will be rendered, FALSE otherwise.
   */
  function showJWPlayer($index, $view_mode) {
    if (module_exists('sharedcontent_server')) {
      foreach ($this->getIndexes($index) as $index_all) {
        $fields = field_info_instances($index_all->entity_type, $index_all->entity_bundle);
        foreach ($fields as $key => $field) {
          if (isset($field['display'][$view_mode]['module'])
            && $field['display'][$view_mode]['module'] == 'jw_player'
          ) {
            return TRUE;
          }
        }
      }
    }
    return FALSE;
  }
}
