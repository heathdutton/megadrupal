<?php

/**
 * @file
 * Documentation of Feeds OAuth hooks.
 */

/**
 * @addtogroup hooks
 * @{
 */

/**
 * Invoked during setup of the OAuth/OAuth2 fetcher to
 * select the source of the OAuth access token. By default,
 * Feeds OAuth provides its own authenticator which performs
 * the standard OAuth/OAuth2 protocol.
 *
 * @return
 *   A array of supported authenticators, where each key is
 *   the callback function of the authenticator, and the value
 *   is the name to be displayed in the OAuth fetcher's settings
 *   form. The callback is used to retrieve the access token pair
 *   at the time of actual feed retrieval.
 *
 * @see feeds_oauth_feeds_oauth_authenticator()
 */
function hook_feeds_oauth_authenticator() {
  return array('twitter_get_tokens' => 'Twitter');
}

/**
 * The authenticator callback that is used to retrieve the user's
 * access token pair.
 *
 * @param $uid
 *   The uid for which to retrieve the access token.
 * @param $site_id
 *   The site identifier as defined in the OAuth fetcher settings. Here,
 *   the "site" refers to the OAuth provider, like Twitter, Facebook, etc.
 * @return
 *   An array with keys 'oauth_token' and 'oauth_token_secret'
 *   corresponding to the user's OAuth token pair for the given
 *   site identifier.
 */
function twitter_get_tokens($uid, $site_id) {
  return db_fetch_array(db_query("SELECT oauth_token, oauth_token_secret FROM {twitter_account} WHERE uid = %d", $uid));
}

/**
 * @} End of "addtogroup hooks".
 */
