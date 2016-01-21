<?php
/**
 * Hat related party acquisition hook implementations.
 */

/**
 * Implements hook_party_acquisition_query_alter().
 *
 * Allow callers to indicate special hat related behavior:
 * - $context['party_hat']['filter']: An array of hat names to filter the
 *   matched parties on.
 * - $context['party_hat']['filter_operator']: 'AND' or 'OR' to indicate what
 *   operator to use for the hat filter. Defaults to 'OR'.
 * - $context['party_hat']['data_set']: An array of data sets that the matched
 *   party should be allowed based off they hats.
 * - $context['party_hat']['data_set_operator']: 'AND' or 'OR' to indicate what
 *   operator to use for the data set filter. Defaults to 'AND'.
 */
function party_hat_party_acquisition_query_alter(&$query, array &$context) {
  if (empty($context['party_hat'])) {
    return;
  }

  // Use our aliases across both sections.
  $aliases = array();
  $table = 'field_data_party_hat';
  $condition = '%alias.entity_type = :entity_type AND %alias.entity_id = party.pid AND %alias.party_hat_hat_name = :hat_name';

  // Deal with hat filters.
  if (!empty($context['party_hat']['filter'])) {
    // Get our operator and set up our join variables.
    $operator = isset($context['party_hat']['filter_operator']) ? $context['party_hat']['filter_operator']: 'OR';
    $type = $operator == 'AND' ? 'INNER': 'LEFT OUTER';

    // If no
    if ($operator != 'AND') {
      $or = db_or();
    }

    // Add our joins. To avoid duplicates, we need one per hat. If our
    // operator is 'AND' we'll use inner joins. Otherwise we'll use left joins
    // and set up a condition for each in $or.
    foreach ($context['party_hat']['filter'] as $hat_name) {
      // Unfortunately PartyQuery can't do these non-duplicated joins yet...
      $alias = 'party_hat_' . $hat_name;
      $arguments = array(':entity_type' => 'party', ':hat_name' => $hat_name);
      $aliases[$hat_name] = $query->addJoin($type, $table, $alias, $condition, $arguments);

      if ($operator != 'AND') {
        $or->isNotNull("{$aliases[$hat_name]}.entity_id");
      }
    }

    // Add our condition to the query.
    if ($operator != 'AND') {
      $query->condition($or);
    }
  }

  // Deal with data set filters.
  if (!empty($context['party_hat']['data_set'])) {
    // Get our operator and set up our join variables.
    if (isset($context['party_hat']['data_set_operator']) && $context['party_hat']['data_set_operator'] == 'OR') {
      $data_sets = db_or();
    }
    else{
      $data_sets = db_and();
    }

    // Load our hats.
    $hats = entity_load('party_hat');

    // Iterate over the data sets.
    foreach ($context['party_hat']['data_set'] as $data_set_name) {
      // Add an or for each hat that can have this data set.
      $data_set = db_or();

      foreach ($hats as $hat) {
        if (!empty($hat->data['data_sets'][$data_set_name]['has'])) {
          // Create a join if it doesn't already exist.
          if (!isset($aliases[$hat->name])) {
            $alias = 'party_hat_' . $hat->name;
            $arguments = array(':entity_type' => 'party', ':hat_name' => $hat->name);
            $aliases[$hat->name] = $query->leftJoin($table, $alias, $condition, $arguments);
          }

          // Add a condition on this hat.
          $data_set->isNotNull("{$aliases[$hat->name]}.entity_id");
        }
      }

      // Add our data set condition to our parent.
      $data_sets->condition($data_set);
    }

    $query->condition($data_sets);
  }
}

/**
 * Implements hook_party_acquisition_post_acquisition().
 *
 * Allow callers to indicate special hat related behavior:
 * - $context['party_hat']['add']: An array of hats that should be added to the
 *   party at the end of the acquisition process.
 */
function party_hat_party_acquisition_post_acquisition(&$party, &$method, array &$values, array &$context) {
  // Check we have a party.
  if ($party && !empty($context['party_hat']['add'])) {
    // Assign the hats without saving.
    party_hat_hats_assign($party, $context['party_hat']['add'], FALSE);
  }
}
