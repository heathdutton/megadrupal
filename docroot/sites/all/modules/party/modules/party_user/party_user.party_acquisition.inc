<?php
/**
 * Hat related party acquisition hook implementations.
 */

/**
 * Implements hook_party_acquisition_values_alter().
 *
 * Allow callers to indicate special user related behavior:
 * - $context['party_user']['values_reference']: Set to TRUE to indicate that
 *   the value in $context['party_user']['values'] are references to the main
 *   acquire values.
 */
function party_user_party_acquisition_values_alter(&$values, array &$context) {
  if (!empty($context['party_user']['values_reference'])) {
    // Loop over the user values and set them. If they don't exist remove them.
    foreach ($context['party_user']['values'] as $property => &$value) {
      if (isset($values[$value])) {
        $value = $values[$value];
      }
      else {
        unset($context['party_user']['values_reference'][$property]);
      }
    }
  }
  if (!empty($context['party_user']['fallback'])) {
    $context['original_behavior'] = $context['behavior'];
    $context['behavior'] = PartyAcquisitionInterface::BEHAVIOR_NOTHING;
  }
}

/**
 * Implements hook_party_acquisition_query_alter().
 *
 * Allow callers to indicate special user related behavior:
 * - $context['party_user']['has_user']: TRUE or FALSE to filter on whether the
 *   party has a user. NULL will not affect behaviour.
 * - $context['party_user']['values']: Optional array of values to filter on
 *   the user table.
 * - $context['party_user']['values_operator']: 'AND' or 'OR' to indicate what
 *   operator to use for the above values. Defaults to 'AND'.
 */
function party_user_party_acquisition_query_alter(&$query, array &$context) {
  if (isset($context['party_user']['has_user']) || !empty($context['party_user']['values'])) {
    // Add our join to the user table, assume delta 0. PartyQuery can't handle
    // this without selecting other rows as well.
    $pae_alias = $query->leftJoin('party_attached_entity', 'pae_user',
      '%alias.pid = party.pid AND %alias.data_set = :user AND %alias.delta = :delta',
      array(':user' => 'user', ':delta' => 0));
    $alias = $query->leftJoin('users', 'user',
      "%alias.uid = {$pae_alias}.eid");

    if (isset($context['party_user']['has_user'])) {
      if ($context['party_user']['has_user']) {
        $query->isNotNull("{$alias}.uid");
      }
      else {
        $query->isNull("{$alias}.uid");
      }
    }

    if (!empty($context['party_user']['values'])) {
      // Add our matching conditions.
      $match = isset($context['party_user']['values_operator']) && $context['party_user']['values_operator'] == 'OR' ? db_or(): db_and();
      foreach ($context['party_user']['values'] as $property => $value) {
        $match->condition("{$alias}.{$property}", $value);
      }
      $query->condition($match);
    }
  }
}

/**
 * Implements hook_party_acquisition_post_acquisition().
 *
 * Allow callers to indicate special user related behavior:
 * - $context['party_user']['fallback']: Set to TRUE to re-run the acquire
 *   without the user conditions if we were unable to get a result.
 */
function party_user_party_acquisition_post_acquisition(&$party, &$method, array &$values, array &$context) {
  // Check we failed to get a party and want to re-run.
  if (!$party && !empty($context['party_user']['fallback'])) {
    // We want to re-run the whole thing without the user settings. Clone our
    // values and context so we don't alter them.
    $new_values = $values;
    $new_context = $context;
    $new_context['behavior'] = $context['original_behavior'];

    // Clear out the user settings and set a specific name.
    unset($new_context['party_user']);
    $new_context['name'] = 'party_user_rerun';

    // Run our new process, overriding the values.
    $party = party_acquire($new_values, $new_context, $method);
  }
}
