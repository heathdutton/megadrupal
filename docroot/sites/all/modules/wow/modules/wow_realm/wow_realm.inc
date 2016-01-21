<?php

/**
 * @file
 * Include functions for the wow_realm module.
 */

/**
 * Menu callback; Retrieve a JSON object containing autocomplete suggestion for
 * existing realms.
 */
function wow_realm_autocomplete($string = '') {
  $matches = array();

  if ($string) {
    $length = drupal_strlen($string);
    $result = db_select('wow_realm')
      ->fields('wow_realm', array('slug', 'region', 'name'))
      ->condition('name', db_like($string) . '%', 'LIKE')
      ->range(0, 10)
      ->execute();
    foreach ($result as $realm) {
      $name = wow_realm_format($realm);
      // Make bold the user input: <strong>Ar</strong>chimonde.
      $matches[$name] = '<strong>' . drupal_substr($name, 0, $length) . '</strong>' . drupal_substr($name, $length);
    }
  }

  drupal_json_output($matches);
}

/**
 * Updates the full list of realms.
 *
 * Fetch all existing realms for a given region, update them in the database.
 *
 * @param string $region
 *  The region API.
 */
function wow_realm_update_process($region, &$context) {
  // Use the $context['sandbox'] to track progress between successive calls.
  if (!isset($context['sandbox']['progress'])) {
    $context['sandbox'] = array();
    $context['sandbox']['progress'] = 0;

    // Save realm data.
    $realms = wow_realm_fetch($region);
    $context['sandbox']['total'] = count($realms);
    $context['sandbox']['realms'] = $realms;
    $context['sandbox']['indexes'] = array_keys($realms);
  }

  $start = $context['sandbox']['progress'];
  $end = $context['sandbox']['progress'] + 10;

  if ($end > $context['sandbox']['total']) {
    $end = $context['sandbox']['total'];
  }

  // Process the realm list from the service.
  for ($i = $start; $i < $end; $i++) {
    $slug = $context['sandbox']['indexes'][$i];
    $realm = $context['sandbox']['realms'][$slug];
    $realm->save();

    // Update the progress information.
    $context['sandbox']['progress']++;
  }

  $args = array(
    '!total' => $context['sandbox']['total'],
    '!progress' => $context['sandbox']['progress'],
  );
  $context['message'] = t('Saved a total of !progress realms out of !total.', $args);

  // Inform the batch engine of the progress and provide an estimation of the
  // completion level reached.
  if ($context['sandbox']['progress'] != $context['sandbox']['total']) {
    $context['finished'] = ($context['sandbox']['progress'] / $context['sandbox']['total']);
  }
}

/**
 * Batch 'finished' callback. Clean-up realms that are no longer updated.
 */
function wow_realm_delete_process($success, $results, $operations) {
  if ($success) {
    $rids = db_select('wow_realm', 'realm')
      ->fields('realm', array('rid'))
      ->condition('lastFetched', 0)
      ->execute()
      ->fetchAll(PDO::FETCH_COLUMN);
    entity_get_controller('wow_realm')->delete($rids);
  }
}
