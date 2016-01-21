<?php
// $Id$

/**
 * @file
 * Hooks provided by the OAuth module.
 */

// This is dummy constant as they will be never be parsed. Only here for
// auto completion assist and shows the API possibilities to the programmer.
define(OAUTH_HTTP_METHOD_GET, "GET");
define(OAUTH_HTTP_METHOD_POST, "POST");
define(OAUTH_HTTP_METHOD_PUT, "PUT");
define(OAUTH_HTTP_METHOD_HEAD, "HEAD");
define(OAUTH_HTTP_METHOD_DELETE, "DELETE");

define(OAUTH_SIG_METHOD_HMACSHA1, "HMAC-SHA1");
define(OAUTH_SIG_METHOD_HMACSHA256, "HMAC-SHA256");
define(OAUTH_SIG_METHOD_RSASHA1, "RSA-SHA1");
define(OAUTH_SIG_METHOD_PLAINTEXT, "PLAINTEXT");

/**
 * This constant represents putting OAuth parameters in the request URI.
 * @var string
 */
define(OAUTH_AUTH_TYPE_URI, 0x01);

/**
 * This constant represents putting OAuth parameters as part of the HTTP POST
 * body.
 * @var string
 */
define(OAUTH_AUTH_TYPE_FORM, 0x02);

/**
 * This constant represents putting OAuth parameters in the Authorization header.
 * @var string
 */
define(OAUTH_AUTH_TYPE_AUTHORIZATION, 0x03);

/**
 * This constant indicates a NoAuth OAuth request.
 * @var string
 */
define(OAUTH_AUTH_TYPE_NONE, 0x04);

/**
 * Used by Oauth::setReqeustEngine() to set the engine to PHP streams, as
 * opposed to OAUTH_REQENGINE_CURL for Curl.
 * @var integer
 */
define(OAUTH_REQENGINE_STREAMS, 1);

/**
 * Used by Oauth::setReqeustEngine() to set the engine to Curl, as opposed to
 * OAUTH_REQENGINE_STREAMS for PHP streams.
 * @var integer
 */
define(OAUTH_REQENGINE_CURL, 2);

/**
 * Life is good.
 * @var integer
 */
define(OAUTH_OK);

/**
 * @addtogroup hooks
 * @{
 */

/**
 * Define module-provided application types.
 *
 * This hook allows a module to define one or more of its own application types.
 * For example, the twitter module uses it to define an oauth 1.0 application
 * type called "Twitter". The name and attributes of each desired application
 * type are specified in an array returned by the hook.
 *
 * Only module-provided application types should be defined through this hook.
 * User-provided application types should be defined only in the 'oauth_application'
 * database table, and should be maintained by using oauth_type_save() and
 * oauth_type_delete() functions.
 *
 * @return
 *   An array whose keys are oauth application names and whose values identify
 *   properties of those types that the system needs to know about:
 *   - "name": The human-readable name of the oauth application type. Required.
 *   - "base": the base string used to construct callbacks corresponding to
 *      this application type.
 *      (i.e. if base is defined as example_foo, then example_foo_insert will
 *      be called when inserting an application of that type). This string is
 *      usually the name of the module, but not always. Required.
 *   - "description": a brief description of the application type. Required.
 *   - "help": help information shown to the user when creating an application
 *      of this type.. Optional (defaults to '').
 *   - "version": OAuth version, either 1.0 or 2.0 (defaults to '1.0').
 *   - consumer: Application (Client) Key and Secret. Elements:
 *     - key: The value of the consumer key.
 *     - secret: The value of the consumer secret.
 *     - signature_method: This optional parameter defines which signature method
 *       to use, by default it is OAUTH_SIG_METHOD_HMACSHA1 (HMAC-SHA1).
 *     - auth_type: This optional parameter defines how to pass the OAuth
 *       parameters to a consumer, by default it is OAUTH_AUTH_TYPE_AUTHORIZATION
 *       (in the Authorization header).
 *
 *     - class: The name of the class that is used to communicate with Services.
 *       The class has to implement the DrupalOAuthConsumer interface.
 *       Leave blank to use the default OAuthPECLConsumer implementation.
 *   - - file: File information for oauth_delegate function. This function will
 *       delegate authentication or oauth_callback to module specific callback.
 *       This allows specific implementation to take care of extra values from the
 *       provider (such as an email address, age, and so on).
 *       - type: The file's extension. Default to .inc.
 *       - name: The file's name. Default to $key.oauth.
 *   - mapping: An associative array mapping data from webservice to drupal.
 *     - authname: The key to external identity, unicity is guaranteed by the provider.
 *     - username: The user friendly name.
 */
function hook_oauth_info() {
  return array(
    'twitter' => array(
      'label' => t('Twitter'),
      'token table' => 'twitter_token',
      'consumer' => array(
        'key' => variable_get('twitter_consumer_key'),
        'secret' => variable_get('twitter_consumer_secret'),
        'signature_method' => OAUTH_SIG_METHOD_HMACSHA1,
        'auth_type' => OAUTH_AUTH_TYPE_AUTHORIZATION,
        'version' => '1.0',
        'class' => 'OAuthPECLConsumer',
      ),
      'file' => array(
        'type' => 'inc',
        'name' => 'twitter.oauth',
      ),
      'mapping' => array(
        'authname' => 'user_id',
        'username' => 'screen_name',
      ),
    ),
  );
}

/**
* Allow modules to act upon a successful OAuth login.
*
* @param $response
*   Response values from the OAuth Provider.
* @param $account
*   The Drupal user account that logged in
*
*/
function hook_oauth_response($response, $account) {

}

/**
 * Helper to fetch the service and retrieves user informations.
 *
 * @param $uid
 * 	 Uid of the user being updated.
 *
 * @return
 * 	 A response from the service with the following mappings.
 *
 * 	 - 'authname', 'username'.
 *
 *   @see hook_oauth_info().
 */
function hook_oauth_user_update($uid, $authname) {
  $oauth = new OAuthAdapter('mymodule');
  $oauth->setCaller($uid);

  $oauth->fetch('http://api.myprovider.com/user?id=' . $authname);
  $response = $oauth->getLastResponse();

  // Performs some updates.

  // Returns the response to the OAuth module.
  return $response;
}

/**
 * @} End of "addtogroup hooks".
 */