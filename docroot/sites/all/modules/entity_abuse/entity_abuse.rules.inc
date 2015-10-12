<?php

/**
 * @file
 * Contains integration with Rules module.
 */

/**
 * Implements hook_rules_action_info().
 */
function entity_abuse_rules_action_info() {
  return array(
    'entity_abuse_add_abuse_action' => array(
      'label' => t('Add abuse'),
      'group' => t('Entity Abuse'),
      'parameter' => array(
        'entity' => array(
          'type' => 'entity',
          'label' => t('Abused entity'),
          'description' => t('Entity for which complain should be added.')
        ),
        'user' => array(
          'type' => 'user',
          'label' => t('Abuse author'),
          'description' => t('User who adds the complain.'),
        ),
        'reason' => array(
          'type' => 'text',
          'label' => t('Reason'),
          'description' => t('Reason of adding complain.'),
          'optional' => TRUE,
        ),
      ),
    ),
    'entity_abuse_cancel_abuse_action' => array(
      'label' => t('Cancel abuse'),
      'group' => t('Entity Abuse'),
      'parameter' => array(
        'entity' => array(
          'type' => 'entity',
          'label' => t('Entity from which abuse should be canceled')
        ),
        'user' => array(
          'type' => 'user',
          'label' => t('User that added an abuse'),
          'description' => t('IMPORTANT NOTE: if you leave this field empty, all abuses from the selected entity will be deleted.'),
          'optional' => TRUE,
        ),
      ),
    ),
  );
}

/**
 * Implements hook_rules_condition_info().
 */
function entity_abuse_rules_condition_info() {
  return array(
    'entity_abuse_check_entity_abuses' => array(
      'label' => t('Entity has abuses'),
      'group' => t('Entity Abuse'),
      'parameter' => array(
        'entity' => array(
          'type' => 'entity',
          'label' => t('Entity')
        ),
        'operator' => array(
          'type' => 'text',
          'label' => t('Operator'),
          'options list' => '_entity_abuse_condition_operator_options',
          'restriction' => 'input',
        ),
        'abuses' => array(
          'type' => 'integer',
          'label' => t('Amount of abuses'),
          'restriction' => 'input',
          'default' => 1,
        ),
      ),
    ),
  );
}

/**
 * Implements hook_rules_event_info().
 */
function entity_abuse_rules_event_info() {
  /*$items = array(
    'node_insert' => array(
      'label' => t('After saving new content'),
      'group' => t('Node'),
      'variables' => rules_events_node_variables(t('created content')),
    ),
    'node_update' => array(
      'label' => t('After updating existing content'),
      'group' => t('Node'),
      'variables' => rules_events_node_variables(t('updated content'), TRUE),
    ),
    'node_presave' => array(
      'label' => t('Content is going to be saved'),
      'group' => t('Node'),
      'variables' => rules_events_node_variables(t('saved content'), TRUE),
    ),
    'node_view' => array(
      'label' => t('Content is going to be viewed'),
      'group' => t('Node'),
      'variables' => rules_events_node_variables(t('viewed content')) + array(
        'view_mode' => array('type' => 'text', 'label' => t('view mode')),
      ),
    ),
    'node_delete' => array(
      'label' => t('After deleting content'),
      'group' => t('Node'),
      'variables' => rules_events_node_variables(t('deleted content')),
    ),
  );
  // Specify that on presave the node is saved anyway.
  $items['node_presave']['variables']['node']['skip save'] = TRUE;
  return $items;*/
}

/**
 * Options list callback for the operator parameter of the condition.
 */
function _entity_abuse_condition_operator_options() {
  return array(
    '>'  => t('Greater than'),
    '>=' => t('Greater than or equal'),
    '='  => t('Equal to'),
    '<=' => t('Less than or equal'),
    '<'  => t('Less than'),
  );
}

/**
 * Adds an abuse to the entity.
 */
function entity_abuse_add_abuse_action($entity, $account, $reason) {

  // Get entity info.
  $entity_id = $entity->getIdentifier();
  $entity_type = $entity->type();

  // Add complain only if current user didn't complain before.
  entity_abuse_include('api');
  $user_complained = entity_abuse_user_has_complain($entity_id, $entity_type, $account->uid);
  if (!$user_complained) {
    entity_abuse_add($entity_id, $entity_type, $reason, $account->uid);
  }
}

/**
 * Cancel abuses from entity.
 */
function entity_abuse_cancel_abuse_action($entity, $account) {

  // Get entity info.
  $entity_id = $entity->getIdentifier();
  $entity_type = $entity->type();

  entity_abuse_include('api');

  if (empty($account)) {
    // Cancel all abuses from entity.
    entity_abuse_cancel($entity_id, $entity_type, TRUE);
  }
  else {

    // Cancel abuse only if user has abuse for this entity.
    $user_complained = entity_abuse_user_has_complain($entity_id, $entity_type, $account->uid);
    if ($user_complained) {

      // Cancel abuses only from selected user.
      entity_abuse_cancel($entity_id, $entity_type, FALSE, $account->uid);
    }
  }
}

/**
 * Checks if entity has some amount of abuses.
 */
function entity_abuse_check_entity_abuses($entity, $operator, $abuses) {

  // Get entity info.
  $entity_id = $entity->getIdentifier();
  $entity_type = $entity->type();

  // Load current amount of abuses.
  entity_abuse_include('api');
  $count_abuses = entity_abuse_get_abuse_amount($entity_type, $entity_id);

  switch ($operator) {
    case '>' : return $count_abuses >  $abuses;
    case '>=': return $count_abuses >= $abuses;
    case '=' : return $count_abuses == $abuses;
    case '<' : return $count_abuses <  $abuses;
    case '<=': return $count_abuses <= $abuses;
  }

  return FALSE;
}
