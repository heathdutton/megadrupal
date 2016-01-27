<?php
/**
 * @file
 * Rules integration for party.
 */

/**
 * Implements hook_rules_data_info()
 */
function party_rules_data_info() {
  return array(
    'party' => array(
      'label' => t('party'),
      'parent' => 'entity',
      'group' => t('Party'),
    ),
  );
}

/**
 * Implements hook_rules_event_info()
 */
function party_rules_event_info() {
  return array(
    'party_insert' => array(
      'group' => t('Party'),
      'module' => 'party',
      'label' => t('After saving a new party'),
      'variables' => array(
        'party' => array('type' => 'party', 'label' => t('created party')),
      ),
    ),
    'party_update' => array(
      'group' => t('Party'),
      'module' => 'party',
      'label' => t('After updating an existing party'),
      'variables' => array(
        'party' => array('type' => 'party', 'label' => t('updated party')),
      ),
    ),
    'party_view' => array(
      'group' => t('Party'),
      'module' => 'party',
      'label' => t('A Party is viewed'),
      'variables' => array(
        'party' => array('type' => 'party', 'label' => t('viewed party')),
      ),
      'help' => t("Note that if drupal's page cache is enabled, this event won't be generated for pages served from cache."),
    ),
    'party_delete' => array(
      'group' => t('Party'),
      'module' => 'party',
      'label' => t('After deleting a party'),
      'variables' => array(
        'party' => array('type' => 'party', 'label' => t('deleted party')),
      ),
    ),
  );
}

/**
 * Implements hook_rules_action_info()
 */
function party_rules_action_info() {
  $items['party_attach_entity'] = array(
    'label' => t('Attach Entity to Party'),
    'group' => t('Party'),
    'parameter' => array(
      'party' => array(
        'type' => 'party',
        'label' => t('Party'),
        'save' => TRUE,
      ),
      'entity' => array(
        'type' => 'entity',
        'label' => t('Entity'),
        'description' => t('The Entity to attach to the Party'),
        'restriction' => 'selector',
        'wrapped' => TRUE,
      ),
      'data_set_name' => array(
        'type' => 'text',
        'label' => t('Data Set Name'),
        'description' => t('The Data Set definition for this entity'),
      ),
    ),
    //'access callback' => 'rules_user_integration_access'
    'base' => 'party_rules_attach_entity',
  );

  $items['party_detach_entity'] = array(
    'label' => t('Detach an Entity from a Party'),
    'group' => t('Party'),
    'parameter' => array(
      'party' => array(
        'type' => 'party',
        'label' => t('Party'),
        'save' => TRUE,
      ),
      'entity' => array(
        'type' => 'entity',
        'label' => t('Entity'),
        'description' => t('The Entity to detach from the Party'),
        'restriction' => 'selector',
        'wrapped' => TRUE,
      ),
      'data_set_name' => array(
        'type' => 'text',
        'label' => t('Data Set Name'),
        'description' => t('The Data Set definition for this entity'),
      ),
    ),
    //'access callback' => 'rules_user_integration_access'
    'base' => 'party_rules_attach_entity',
  );

  return $items;
}

/**
 * Implement the attach entity rule.
 *
 * @todo: Get party loading as a Party object not as stdClass
 */
function party_rules_attach_entity($party, $wrapper, $data_set_name, $settings, $state, $element) {
  $entity = $wrapper->value();

  party_attach_entity($party, $entity, $data_set_name);
}

/**
 * Implement the detach entity rule
 */
function party_rules_detach_entity($party, $wrapper, $data_set_name, $settings, $state, $element) {
  $entity = $wrapper->value();

  party_detach_entity($party, $entity, $data_set_name);
}