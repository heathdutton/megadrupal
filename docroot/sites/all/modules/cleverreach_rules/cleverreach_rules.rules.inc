<?php

/**
* Implements hook_rules_action_info() on behalf of the user module.
*/
function cleverreach_rules_rules_action_info() {
  $defaults = array(
   'parameter' => array(
      'key' => array(
        'type' => 'text',
        'label' => t('CleverReach API Key'),
        'save' => TRUE,
      ),
      'list' => array(
        'type' => 'text',
        'label' => t('List ID'),
        'save' => TRUE,
      ),
      'source' => array(
        'type' => 'text',
        'label' => t('Source string'),
        'save' => TRUE,
      ),
      'email' => array(
        'type' => 'text',
        'label' => t('eMail'),
        'save' => TRUE,
      ),
    ),
    'group' => t('CleverReach'),
  );
  $actions['cleverreach_rules_email_add'] = $defaults + array(
    'label' => t('Add eMail to CleverReach newsletter list'),
    'base' => 'cleverreach_rules_email_add',
  );
  $actions['cleverreach_rules_email_remove'] = $defaults + array(
    'label' => t('Remove eMail from CleverReach newsletter list'),
    'base' => 'cleverreach_rules_email_remove',
  );
  return $actions;
}