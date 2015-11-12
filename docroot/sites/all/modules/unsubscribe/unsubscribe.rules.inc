<?php
/**
 * @file
 * Provide rules integration for unsubscribe module.
 */

/**
 * Implements hook_rules_event_info().
 *
 * @ingroup rules
 */
function unsubscribe_rules_event_info() {
  return array(
    'unsubscribe_user_added' => array(
      'label' => t('User is added to the unsubscribe list'),
      'group' => t('Unsubscribe'),
      'variables' => array(
        'account' => array(
          'type' => 'user',
          'label' => t("Unsubscribed user's account"),
        ),
      ),
    ),
    'unsubscribe_user_removed' => array(
      'label' => t('User is removed from the unsubscribe list'),
      'group' => t('Unsubscribe'),
      'variables' => array(
        'account' => array(
          'type' => 'user',
          'label' => t("Unsubscribed user's account"),
        ),
      ),
    ),
  );
}

/**
 * Implementation of hook_rules_action_info().
 *
 * @ingroup rules
 */
function unsubscribe_rules_action_info() {
  return array(
    'unsubscribe_user_add' => array(
      'label' => t('Add to unsubscribe list'),
      'parameter' => array(
        'account' => array(
          'type' => 'user',
          'label' => t('Unsubscribed User'),
        ),
      ),
      'group' => t('User'),
      'access callback' => 'user_access',
      'access arguments' => array('unsubscribe add'),
    ),
    'unsubscribe_user_remove' => array(
      'label' => t('Remove from unsubscribe list'),
      'parameter' => array(
        'account' => array(
          'type' => 'user',
          'label' => t('Unsubscribed User'),
        ),
      ),
      'group' => t('User'),
      'access callback' => 'user_access',
      'access arguments' => array('unsubscribe remove'),
    ),
  );
}
