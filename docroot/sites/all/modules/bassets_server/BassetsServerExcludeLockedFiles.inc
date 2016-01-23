<?php
/**
 * @file
 * Search API data alteration callback that filters out items which are locked.
 */

class BassetsServerExcludeLockedFiles extends SearchApiAbstractAlterCallback{

  /**
   * {@inheritdoc }
   */
  public function supportsIndex(SearchApiIndex $index) {
    if ($index->item_type == 'file') {
      return TRUE;
    }
    return FALSE;
  }

  /**
   * {@inheritdoc}
   */
  public function alterItems(array &$items) {
    foreach ($items as $id => $item) {
      if (_bassets_server_file_is_locked($item)) {
        unset($items[$id]);
      }
    }
  }
}
