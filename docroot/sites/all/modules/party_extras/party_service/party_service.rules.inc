<?php
/**
 * @file
 * Rules integrations for Party Service.
 */

/**
 * Implements hook_rules_event_info().
 */
function party_service_rules_event_info() {
  $items = array();
  $items['party_service_post'] = array(
    'label' => t('When a new Party is POSTed to the service'),
    'group' => t('Party Service'),
    'variables' => array(
      'service' => array(
        'type' => 'party_service',
        'label' => t('service'),
      ),
      'party' => array(
        'type' => 'party',
        'label' => t('received party'),
      ),
      'input' => array(
        'type' => 'struct',
        'label' => t('raw input'),
      ),
    ),
  );

  return $items;
}

/**
 * Implements hook_rules_action_info().
 */
function party_service_rules_action_info() {
  $actions = array();
  $actions['party_service_post_party'] = array(
    'label' => t('Send a new Party to a Remote'),
    'parameter' => array(
      'party' => array(
        'type' => 'party',
        'label' => t('Party'),
      ),
      'remote' => array(
        'type' => 'party_service_remote',
        'label' => t('Remote'),
        'options list' => 'party_service_remote_options_list',
      ),
    ),
    'group' => t('Party Services'),
    'base' => 'party_service_rules_action_post_party',
  );

  return $actions;
}

/**
 * Push a party to a remote.
 */
function party_service_rules_action_post_party($party, $remote) {
  $handler = PartyRemoteHandlerCURL::create($remote);
  $handler->post($party);
}