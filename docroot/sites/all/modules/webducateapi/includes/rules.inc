<?php
/**
 * @file
 * Rules module integration.
 */

/**
 * Implements hook_rules_action_info().
 */
function webducateapi_rules_action_info() {
  $actions = array();

  $actions['webducateapi_create_token'] = array(
    'label' => t('Create token'),
    'base' => 'webducateapi_create_token',
    'group' => t('WebducateAPI'),
    'parameter' => array(
      'webducate_id' => array(
        'type' => 'text',
        'label' => t('Webducate ID'),
      ),
      'mail' => array(
        'type' => 'text',
        'label' => t('Email address'),
      ),
    ),
  );

  return $actions;
}
