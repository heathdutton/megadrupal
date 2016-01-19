<?php
/**
 * @file
 * Provide better intergration into the rules module
 */

/**
 * Implements hook_rules_action_info() on behalf of the drupagram module.
 */
function drupagram_actions_rules_action_info() {
  return array(
    'rules_core_drupagram_actions_set_status_action' => array(
      'label' => t('Post a message to drupagram'),
      'group' => t('drupagram'),
      'parameter' => array(
        'message' => array(
          'type' => 'text',
          'label' => t('Message'),
          'description' => t("The content of the instagram."),
        ),
        'sender' => array(
          'type' => 'user',
          'label' => t('Sender'),
          'description' => t("User whose drupagram account will be used."),
        ),
      ),
      'base' => 'drupagram_actions_set_status',
      'access callback' => 'rules_drupagram_actions_access_callback',
    ),
  );
}

/**
 * Fetches drupagram account info and submits with the message to the drupagram API
 *
 * @param $message
 *   The message to post
 * @param $sender
 *   The Drupal user that has a drupagram account
 */
function drupagram_actions_set_status($message, $sender) {
  if ($drupagram_id = db_query("SELECT drupagram_id FROM {drupagram_account} WHERE uid = :uid", array(':uid' => $sender->uid))->fetchField()) {
    module_load_include('inc', 'drupagram');
    $drupagram_account = drupagram_account_load($drupagram_id);
    drupagram_set_status($drupagram_account, $message);
  }
  else {
    watchdog('drupagram', 'drupagram authentication failed. Please check your account name and try again.', array(), WATCHDOG_ERROR);
  }
}

/**
 * Implements hook_rules_condition_info().
 */
function drupagram_actions_rules_condition_info() {
  $defaults = array(
    'group' => t('drupagram'),
    'parameter' => array(
      'user' => array(
        'type' => 'user',
        'label' => t('User'),
        'description' => t('The user to be checked for.'),
      ),
    ),
    'named parameter' => TRUE,
    'access callback' => 'rules_drupagram_actions_access_callback',
  );
  $items['rules_core_drupagram_conditions_user_has_linked'] = $defaults + array(
    'label' => t('User has linked drupagram account'),
    'help' => t('Evaluates to TRUE in case there is a record in the drupagram_account for the provided user.'),
    'base' => 'drupagram_actions_has_linked',
  );

  $items['rules_core_drupagram_conditions_text_is_under_140'] = array(
    'group' => t('drupagram'),
    'named parameter' => TRUE,
    'parameter' => array(
      'text' => array(
        'type' => 'text',
        'label' => t('Text to check'),
      ),
    ),
    'label' => t('Text is under 140 characters'),
    'help' => t('Returns TRUE if the length of the text is 140 or less.'),
    'base' => 'drupagram_actions_less_140',
  );

  return $items;
}

/**
 * The callback function for the Rules condition
 * @param $element
 *   $element['user']: The user to be checked for.
 * @return
 *   TRUE if the user has linked his/her drupagram account.
 */
function drupagram_actions_has_linked($element) {
  return db_query("SELECT drupagram_id FROM {drupagram_account} WHERE uid = :uid", array(':uid' => $element['user']->uid))->fetchField() ? TRUE : FALSE;
}

/**
 * The callback function for the Rules condition
 * @param $element
 *   $element['user']: The user to be checked for.
 * @return
 *   TRUE if the user has linked his/her drupagram account.
 */
function drupagram_actions_less_140($element) {
  return drupal_strlen($element['text']) < 141;
}

/**
 * The callback function to access the condition
 */
function rules_drupagram_actions_access_callback($type, $name) {
  return user_access('add drupagram accounts');
}
