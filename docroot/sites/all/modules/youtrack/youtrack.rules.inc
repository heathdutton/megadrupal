<?php

/**
 * Implement hook_rules_action_info().
 */
function youtrack_rules_action_info() {
  return array();
  return array(
    'youtrack_create_issue' => array(
      'label' => t('Create a new issue with YouTrack'),
      'parameter' => array(
        'user' => array('type' => 'user', 'label' => t('Recipient')),
      ),
      'group' => t('YouTrack'),
      'base' => 'rules_action_mail_user',
      'callbacks' => array(
        'validate' => 'rules_action_custom_validation',
        'help' => 'rules_mail_help',
      ),
    ),
  );
}
