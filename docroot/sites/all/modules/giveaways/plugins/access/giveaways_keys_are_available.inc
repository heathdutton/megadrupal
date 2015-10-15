<?php

$plugin = array(
  'title' => t('Giveaway: keys are available'),
  'description' => t('Controls access by whether this giveaway has available keys'),
  'callback' => 'giveaway_keys_are_available_ctools_access_check',
  'summary' => 'giveaway_keys_are_available_ctools_summary',
  'required context' => new ctools_context_required(t('Node'), 'node'),
);

/**
 * Provide a summary description.
 */
function giveaway_keys_are_available_ctools_summary($conf, $context) {
  return t('This giveaway has available keys');
}

function giveaway_keys_are_available_ctools_access_check($conf, $context) {
  return db_select('giveaway_keys')
    ->fields(NULL, array('gkid'))
    ->condition('gid', $context->data->nid)
    ->condition('uid', 0)
    ->execute()
    ->rowCount();
}
