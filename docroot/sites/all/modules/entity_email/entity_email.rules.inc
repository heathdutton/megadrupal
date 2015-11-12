<?php

/**
 * @file
 * Rules integration.
 */

/**
 * Implements hook_rules_action_info().
 */
function entity_email_rules_action_info() {
  $actions = array(
    'entity_email_rules_send_message' => array(
      'label' => t('Send Entity Email'),
      'group' => t('Entity Email'),
      'parameter' => array(
        'entity_email_name' => array(
          'type' => 'text',
          'label' => t('The Entity Email Type to use'),
          'options list' => 'entity_email_type_list',
          'description' => t('The Entity Email Type that will be used. You can see the full list or add a new one from <a href="@url">this page</a>.', array('@url' => url('admin/structure/entity_email_type'))),
        ),
        'entity_email_from' => array(
          'type' => 'user',
          'label' => t('User who send the email.'),
          'description' => t('If your entity email type includes user tokens, this user will be used for them.'),
          'optional' => TRUE,
          'default value' => NULL,
        ),
        'entity_email_to' => array(
          'type' => 'user',
          'label' => t('user who received the email.'),
          'description' => t('If your entity email type includes user tokens, this user will be used for them.'),
          'optional' => TRUE,
          'default value' => NULL,
        ),
        'entity-email-node' => array(
          'type' => 'node',
          'label' => t('The associated node with the email.'),
          'description' => t('If your entity email type includes node tokens, this node will be used for them.'),
          'optional' => TRUE,
          'default value' => NULL,
        ),
        'entity-email-mailto' => array(
          'type' => 'text',
          'label' => t('Overide Mail To'),
          'description' => t("Overide who is receiving the email"),
          'optional' => TRUE,
        ),
        'entity-email-mailfrom' => array(
          'type' => 'text',
          'label' => t('Overide Mail From'),
          'description' => t("Leave it empty to use the site-wide configured address."),
          'optional' => TRUE,
        ),
        'entity-email-replyto' => array(
          'type' => 'text',
          'label' => t('Overide Reply To'),
          'description' => t("Leave it empty to use the site-wide configured address."),
          'optional' => TRUE,
        ),
        'entity-email-save' => array(
          'type' => 'boolean',
          'label' => t("Save Email"),
          'restriction' => 'input',
        ),
      ),
    ),
  );

  return $actions;
}

/**
 * Callback for anonymous rules action.
 */
function entity_email_rules_send_message($entity_email_name, $entity_email_from, $entity_email_to, $entity_email_node, $entity_email_mailto, $entity_email_mailfrom, $entity_email_replyto, $entity_email_save) {
  $params = array(
    'fuid' => isset($entity_email_from) ? $entity_email_from->uid : NULL,
    'tuid' => isset($entity_email_to) ? $entity_email_to->uid : NULL,
    'nid' => isset($entity_email_node) ? $entity_email_node->nid : NULL,
    'mail_to' => isset($entity_email_mailto) ? $entity_email_mailto : NULL,
    'mail_from' => isset($entity_email_mailfrom) ? $entity_email_mailfrom : NULL,
    'mail_replyto' => isset($entity_email_replyto) ? $entity_email_replyto : NULL,
  );
  $entity_email_save = isset($entity_email_save) ? TRUE : FALSE;
  entity_email_send($entity_email_name, $params, $entity_email_save);
}

/**
 * Return list of all entity email type for rules configuration.
 */
function entity_email_type_list() {
  $list = array();
  foreach (entity_email_type_load_multiple(FALSE) as $entity_email_type) {
    $list[$entity_email_type->name] = $entity_email_type->label;
  }
  return $list;
}
