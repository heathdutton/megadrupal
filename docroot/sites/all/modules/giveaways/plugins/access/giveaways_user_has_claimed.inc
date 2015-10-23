<?php

$plugin = array(
  'title' => t('Giveaway: user has claimed this giveaway'),
  'description' => t('Controls access by whether the user has already claimed this giveaway'),
  'callback' => 'giveaway_user_has_claimed_ctools_access_check',
  'summary' => 'giveaway_user_has_claimed_ctools_summary',
  'required context' => array(
    new ctools_context_required(t('User'), 'user'),
    new ctools_context_required(t('Node'), 'node'),
  ),
);

/**
 * Provide a summary description.
 */
function giveaway_user_has_claimed_ctools_summary($conf, $context) {
  return t('User has already claimed this giveaway');
}

function giveaway_user_has_claimed_ctools_access_check($conf, $context) {
  //sanity
  if ($context[0]->type[2] != 'user' or $context[1]->type[2] != 'node') {
    return FALSE;
  }

  //if the user is anonymous, we assume he hasn't claimed.
  if ($context[0]->data->uid == 0) {
    return FALSE;
  }

  return db_select('giveaway_keys')
    ->fields(NULL, array('gkid'))
    ->condition('gid', $context[1]->data->nid)
    ->condition('uid', $context[0]->data->uid)
    ->execute()
    ->rowCount();
}
