<?php
/**
 * @file Status filter for search api.
 */

/**
 * Search API data alteration callback that filters out hidden parties.
 */
class PartyAlterStatusFilter extends SearchApiAbstractAlterCallback {

  public function supportsIndex(SearchApiIndex $index) {
    return $index->item_type == 'party';
  }

  public function alterItems(array &$items) {
    if ($this->index->item_type == 'party') {
      foreach ($items as $id => $item) {
        if ($item->hidden == TRUE) {
          unset($items[$id]);
        }
      }
    }
  }

}
