<?php

/**
 * Implementation of hook_rules_event_info().
 */
function premium_access_rules_event_info() {
  $defaults = array(
    'group' => t('User'),
    'module' => 'premium_access',
  );

  return array(
    'premium_access_granted' => $defaults + array(
      'label' => t('User is granted premium access'),
      'variables' => array(
        'account' => array('type' => 'user', 'label' => t('Registered user')),
        'profile' => array('type' => 'profile2', 'label' => t('Registered user profile')),
        'node' => array('type' => 'node', 'label' => t('Node to which user gained premium access')),
      ),
    ),
  );
}
