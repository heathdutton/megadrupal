<?php
/**
 * @file Page Callbacks for party_extras module
 */

/**
 * Party autcomplete callback.
 *
 * @param $hats
 *   The hats to filter the search by.
 *
 * @return Array
 *   A list of party labels
 */
function party_extras_autocomplete($string = '', $hats = '') {
  if (!empty($string)) {
    $query = db_select('party')
      ->fields('party', array('label', 'pid'))
      ->condition('label', db_like($string) . '%', 'LIKE');

    if (!empty($hats)) {
      $hats = explode(' ', $hats);
      $query->extend('PartyQuery')
        ->fieldCondition('party', 'party_hat', 'hat_name', $hats, 'IN');
    }

    $result = $query->range(0, 10)
      ->execute();

    foreach ($result as $party) {
      $matches[$party->label . ' (' . $party->pid .')'] = check_plain($party->label . ' (' . $party->pid .')');
    }
  }

  drupal_json_output($matches);
}