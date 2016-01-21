<?php
/**
 * @file Status filter for search api.
 */

/**
 * Search API data alteration callback that filters out hidden parties.
 */
class PartyAlterStatusFilter extends SearchApiAbstractAlterCallback {

  /**
   * {@inheritdoc}
   */
  public function supportsIndex(SearchApiIndex $index) {
    return $index->item_type == 'party';
  }

  /**
   * {@inheritdoc}
   */
  public function configurationForm() {
    $form = array();

    $form['archived'] = array(
      '#type' => 'checkbox',
      '#title' => t('Exclude archived parties'),
      '#default_value' => isset($this->options['archived']) ? $this->options['archived']: NULL,
    );

    return $form;
  }

  /**
   * {@inheritdoc}
   */
  public function alterItems(array &$items) {
    foreach ($items as $id => $item) {
      if (!$this->checkParty($item)) {
        unset($items[$id]);
      }
    }
  }

  /**
   * Run the appropriate checks on a party depending on the configuration.
   *
   * @param \Party $party
   *   The party we are checking.
   *
   * @return bool
   *   TRUE if this party passes the filter, FALSE if it doesn't.
   */
  protected function checkParty(Party $party) {
    // Hidden always gets filtered out.
    if ($party->hidden) {
      return FALSE;
    }

    // If we've chosen to filter out archived, check it.
    if (!empty($this->options['archived']) && $party->archived) {
      return FALSE;
    }

    // If we've got this far, we've passed the filter.
    return TRUE;
  }

}
