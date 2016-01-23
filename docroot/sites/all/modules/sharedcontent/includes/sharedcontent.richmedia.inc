<?php

/**
 * @file
 * RichMedia plugin interface.
 */
interface SharedContentRichMedia {
  /**
   * Get JS files to be added when displaying an embedded Shared Content.
   *
   * @param $index
   *   The index record.
   * @param $view_mode
   *   The view mode the entity was rendered in.
   * @param $langcode
   *   The language the enetity was rendered for.
   * @return
   *  Array of JS files.
   */
  function getJsFiles($index, $view_mode, $langcode);

  /**
   * Get JS settings to be added when displaying an embedded Shared Content.
   *
   * @param $index
   *   The index record.
   * @param $view_mode
   *   The view mode the entity was rendered in.
   * @param $langcode
   *   The language the enetity was rendered for.
   * @return
   *  Array of JS settings.
   */
  function getJsSettings($index, $view_mode, $langcode);

  /**
   * Get inline JS.
   *
   * @param $index
   *   The index record.
   * @param $view_mode
   *   The view mode the entity was rendered in.
   * @param $langcode
   *   The language the enetity was rendered for.
   * @return
   *  Array of JS settings.
   */
  function getInlineJs($index, $view_mode, $langcode);

  /**
   * Get CSS files to be added when displaying an embedded Shared Content.
   *
   * @param $index
   *   The index record.
   * @param $view_mode
   *   The view mode the entity was rendered in.
   * @param $langcode
   *   The language the enetity was rendered for.
   * @return
   *  Array of CSS files.
   */
  function getCssFiles($index, $view_mode, $langcode);
}

class SharedContentRichMediaBase implements SharedContentRichMedia {

  /**
   * Internal cache for loaded indexes.
   *
   * @var array
   */
  protected $indexes = array();

  /**
   * Implements getJsFiles().
   */
  function getJsFiles($index, $view_mode, $langcode) {
    return array();
  }

  /**
   * Implements getJsSettings().
   */
  function getJsSettings($index, $view_mode, $langcode) {
    return array();
  }

  /**
   * Implements getInlineJs().
   */
  function getInlineJs($index, $view_mode, $langcode) {
    return array();
  }

  /**
   * Implements getCssFiles().
   */
  function getCssFiles($index, $view_mode, $langcode) {
    return array();
  }

  /**
   * Returns an array of loaded indexes, by default including parents.
   *
   * @todo: Pass in the $index and change this to getParents().
   *
   * @param $index
   *   The index record.
   * @param bool $parents
   *   (optional) FALSE if no parents should be loaded, defaults to TRUE.
   *
   * @return array
   *   Array with at least the index entity for the given UUID.
   */
  protected function getIndexes($index, $parents = TRUE) {
    if (empty($indexes[$index->uuid])) {
      $indexes[$index->uuid][] = $index;
      while ($parents && !empty($indexes[$index->uuid][count($indexes[$index->uuid]) - 1]->parent_uuid)) {
        $indexes[$index->uuid][] = sharedcontent_index_load_by_uuid($indexes[$index->uuid][count($indexes) - 1]->parent_uuid);
      }
    }
    return $indexes[$index->uuid];
  }
}
