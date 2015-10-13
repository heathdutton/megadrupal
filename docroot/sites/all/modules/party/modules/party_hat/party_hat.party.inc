<?php
/**
 * @file
 * Party general hook include.
 */

/**
 * Implements hook_party_access().
 */
function party_hat_party_access($op, $party = NULL, $data_set = NULL, $account = NULL) {
  // We say nothing about access to just the party.
  if (!isset($data_set)) {
    return NULL;
  }

  switch ($op) {
    // View, attach, and add ops are the same: you can't view a party's data set
    // if you can't have it attached either.
    case 'attach':
    case 'add':
    case 'view':
      $data_set_name = $data_set['set_name'];
      $sets = party_get_party_data_sets($party);

      // If any hat has the data set, grant access.
      if (!in_array($data_set_name, $sets)) {
        return FALSE;
      }
  }
}
