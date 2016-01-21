<?php
/**
 * @file CTools Relationship to get a User from a Party
 */

/**
 * Plugins are described by creating a $plugin array which will be used
 * by the system that includes this file.
 */
$plugin = array(
  'title' => t('User from Party'),
  'keyword' => 'party_user',
  'description' => t('Load a user from a given party'),
  'required context' => new ctools_context_required(t('Party'), 'entity:party'),
  'context' => 'party_user_user_from_party_context',
);

/**
 * Return a new context based on an existing context.
 */
function party_user_user_from_party_context($context, $conf) {
  // If unset it wants a generic, unfilled context, which is just NULL.
  if (empty($context->data)) {
    return ctools_context_create_empty('user', NULL);
  }

  $user = $context->data->getDataSetController('user')->getEntities();
  $user = !empty($user) ? reset($user): NULL;

  if ($user) {
    return ctools_context_create('user', $user);
  }
}
