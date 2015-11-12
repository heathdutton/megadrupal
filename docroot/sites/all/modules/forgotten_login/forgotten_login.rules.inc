<?php

/**
 * @file
 * Rules integration for forgotten login.
 *
 * @addtogroup rules
 * @{
 */

/**
 * Implements hook_rules_event_info().
 */
function forgotten_login_rules_event_info() {
  $events = array();

  $events['forgotten_login_email_sent'] = array(
    'label' => t('Forgotten login email sent'),
    'group' => t('Forgotten Login'),
    'variables' => array(
      'user' => array(
        'type' => 'user',
        'label' => t('Failed login account'),
      ),
    ),
  );

  return $events;
}

/**
 * @}
 */
