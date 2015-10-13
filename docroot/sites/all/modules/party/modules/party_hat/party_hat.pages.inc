<?php
/**
 * @file Page and Page title callbacks
 */

/**
 * Get the add page title.
 *
 * This is only called if no title was explicitly set in the add page config.
 *
 * @param string $hats
 *   The hats the party should have initially, in a string formatted as
 *   "hat_name_1 hat_name_2"
 *
 * @return
 *   The page title.
 */
function party_hat_party_add_title($hats = '') {
  $hats = explode(' ', $hats);

  $title = 'Add ';

  // Generate a page title using the hats.
  // If there's only one hat set, the title should be 'Add $hat->label'
  if (count($hats) == 1) {
    $title .= party_hat_load($hats[0])->label;
  }
  // Otherwise we'll default to 'Add Party [$hat_names]'
  else {
    $title .= 'Party';

    $hats = entity_load_multiple_by_name('party_hat', $hats);
    if (count($hats)) {
      $title .= ' [';
      foreach ($hats as $name => $hat) {
        $hats[$name] = $hat->label;
      }
      $title .= implode(', ', $hats);
      $title .= ']';
    }
  }

  return $title;
}

/**
 * Get the Party Add form with the correct hats pre set on the party.
 *
 * @param string $hats
 *   The hats the party should have initially, in a string formatted as
 *   "hat_name_1 hat_name_2"
 *
 * @return
 *   The page render array
 */
function party_hat_party_add_form_wrapper($hats = '') {
  $hats = explode(' ', $hats);

  $party = entity_create('party', array(
    'label' => '',
  ));

  $items = array();

  // Build the hats field values array.
  foreach ($hats as $hat_name) {
    $items[] = array(
      'hat_name' => $hat_name,
    );
  }

  $party->party_hat['und'] = $items;

  module_load_include('inc', 'party', 'party.pages');
  return drupal_get_form('party_form', $party);
}
