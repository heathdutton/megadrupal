<?php
/**
 * @file CTools Relationship to get a Party from a User
 */

/**
 * Plugins are described by creating a $plugin array which will be used
 * by the system that includes this file.
 */
$plugin = array(
  'title' => t('Party from User'),
  'keyword' => 'user_party',
  'description' => t('Load a party from a given user'),
  'required context' => new ctools_context_required(t('User'), 'user'),
  'context' => 'party_user_party_from_user_context',
);

/**
 * Return a new context based on an existing context.
 */
function party_user_party_from_user_context($context, $conf) {
  // If unset it wants a generic, unfilled context, which is just NULL.
  if (empty($context->data)) {
    return ctools_context_create_empty('entity:party', NULL);
  }

  $party = party_user_get_party($context->data);

  if ($party) {
      return ctools_context_create('entity:party', $party);
  }
  return ctools_context_create_empty('entity:party', NULL);
}
