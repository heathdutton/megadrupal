<?php
/**
 * @file
 * Hooks provided by the Social Counters module.
 */

/**
 * @defgroup social_counters_api_hooks Social Counters API Hooks
 * @{
 * Functions to define and modify information about social counters.
 *
 * These hooks and all the related callbacks may be defined in a separate file
 * named module.social_counters.inc
 * @}
 */

/**
 * Obtains all available social counters.
 *
 * @return
 *   An array of information defining the module's social counters. The array
 *   contains a sub-array with the social counter unique name as the key.
 *   Possible attributes:
 *   - "callback": The name of callback function that is used to get number of social counter(required).
 *   - "title": The human readable name of the social counter, will be used in auto generated forms and output(required).
 *   - "dependencies": Dependencies of social counter(optional). Array whith "key" => "value", where(optional):
 *      - "key": Name of the module that is required for this social counter(required).
 *      - "value": Error that will be appeared in the administrative interface if required module isn't enabled(required).
 *   - "variables": Array with "name" => "data", where:
 *      - "name": Name of the variable(only "id" key is required).
 *      - "data": Array with following keys:
 *         - "title": The title which will be displayed in administrative interface(required).
 *         - "description": The title which will be displayed in administrative interface(optional).
 *
 *  @see hook_social_counters_alter()
 */
function hook_social_counters() {
  // My Social Counter.
  $items['my_social_counter'] = array(
    'callback' => 'my_social_counter_get_followers',
    'title' => t('My Social Counter'),
    'dependencies' => array(
      'oauth_common' => t("My Social Counter functionality depends on the <a href='https://drupal.org/project/oauth'>OAuth module</a>."),
    ),
    'variables' => array(
      'id' => array(
        'title' => t('My Social Counter account'),
        'description' => t('The My Social Counter account to pull the number of followers.'),
      ),
      'token' => array(
        'title' => t('OAuth Token (My Social Counter).'),
        'description' => t('OAuth Token (My Social Counter).'),
      ),
    ),
  );

  return $items;
}

/**
 * Alter the social counters definitions.
 *
 * @param $counters
 *   The social counters array, keyed by social counters name.
 *
 * @see hook_social_counters()
 */
function hook_social_counters_alter(&$counters) {
  $counters['facebook']['title'] = t('My title');
  $counters['facebook']['callback'] = 'my_facebook_callback';
}

/**
 * Example of callback function.
 *
 * @param $variables
 *   Array where 'key' is name of the variable defined in hook_social_counters and 'value' is value for
 *   this variable that is set in administrative interface.
 *
 *  @return
 *    Number of followers or FALSE if something goes wrong.
 */
function my_social_counter_get_followers($variables) {
  $number = FALSE;

  try {
    $url = url('https://my-social-network-com/api/' . $variables['id'], array('query' => array(
      'access_token' => $variables['token'],
      'fields' => 'likes',
    )));

    $response = drupal_http_request($url);
    $result = json_decode($response->data);
    if ($response->code == 200) {
      $number = $result->likes;
    }
    else {
      if (!empty($result->error)) {
        $message = $variables['social_network'] . ': ' . $result->error->message;
        watchdog('social_counters', check_plain($message), array(), WATCHDOG_WARNING);
      }
    }
  }
  catch (Exception $e) {
    watchdog_exception('social_counters', $e);
  }

  return $number;
}
