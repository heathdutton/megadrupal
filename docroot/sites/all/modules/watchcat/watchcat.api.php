<?php
/**
 * @file
 * Hooks and alters provided by Watchcat.
 */


/**
 * Defines one or more verbs to use with Watchcat.
 *
 * @return
 *   An associative array describing the verb structure.
 *   The value for each entry is an associateive array that may contain the
 *   following entries:
 *     - title: Short description.
 *     - summary: Long description.
 *     - actor: The actor entity type (In most cases 'user').
 *     - object (optional): The object entity type.
 *     - target (optional): The target entity type.
 *
 * In title and summary tokens can be used.
 * This is 'fake tokens' meaning that they are not real tokens in the definiton
 * but they are converted to real based on the different entity types given.
 *
 * [actor:name] will become [user:name] if the verbs actor is 'user'.
 */
function hook_watchcat_verbs() {
  $verbs = array();

  $verbs['login'] = array(
    'title' => 'User [actor:name] logged in.',
    'summary' => 'User [actor:name] logged in.',
    'actor' => 'user',
  );

  return $verbs;
}

/**
 * Alter verbs.
 *
 * @param $attributes
 *   An associative array with verbs.
 *
 * @see hook_watchcat_verbs()
 */
function hook_watchcat_verbs_alter(&$verbs) {
}
