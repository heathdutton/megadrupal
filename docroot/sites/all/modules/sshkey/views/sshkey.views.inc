<?php

/**
 * @file
 * Views integration and data for the SSH key module.
 */

/**
 * Implements hook_views_data().
 */
function sshkey_views_data() {
  // Basic table information.
  $data['sshkey']['table']['group'] = t('SSH keys');

  // Advertise this table as a possible base table
  $data['sshkey']['table']['base'] = array(
    'field' => 'key_id',
    'title' => t('SSH Public keys'),
    'help' => t('Listings of SSH public keys.'),
    'weight' => 10,
  );

  // Add joins for all other entity types.
  $entity_tables = array();
  foreach (entity_get_info() as $entity_type => $entity_info) {
    if ($entity_type == 'sshkey' || empty($entity_info['base table']) || empty($entity_info['entity keys']['id'])) {
      continue;
    }

    $data['sshkey']['table']['join'][$entity_info['base table']] = array(
      'type' => 'INNER',
      'left_field' => $entity_info['entity keys']['id'],
      'field' => 'entity_id',
    );

    $entity_tables[$entity_info['base table']] = $entity_type;
  }

  // {sshkey}.key_id
  $data['sshkey']['key_id'] = array(
    'title' => t('SSH key ID'),
    'help' => t('The internal ID of the SSH key.'),
    'field' => array(
      'handler' => 'views_handler_field',
      'click sortable' => TRUE,
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_numeric',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_numeric',
      'allow empty' => TRUE,
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
  );

  // {sshkey}.fingerprint
  $data['sshkey']['fingerprint'] = array(
    'title' => t('Fingerprint'),
    'help' => t('The MD5 fingerprint of the SSH key.'),
    'field' => array(
      'handler' => 'views_handler_field',
      'click sortable' => TRUE,
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_string',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_string',
      'allow empty' => TRUE,
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
  );

  // {sshkey}.title
  $data['sshkey']['title'] = array(
    'title' => t('Title'),
    'help' => t('The title of the SSH key.'),
    'field' => array(
      'handler' => 'views_handler_field',
      'click sortable' => TRUE,
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_string',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_string',
      'allow empty' => TRUE,
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
  );

  // {sshkey}.value
  $data['sshkey']['value'] = array(
    'title' => t('Key value'),
    'help' => t('The raw value of the SSH key.'),
    'field' => array(
      'handler' => 'views_handler_field',
      'click sortable' => TRUE,
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_string',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_string',
      'allow empty' => TRUE,
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
  );

  // {sshkey}.changed
  $data['sshkey']['changed'] = array(
    'title' => t('Last modified'),
    'help' => t('The last date/time the SSH key was modified.'),
    'field' => array(
      'handler' => 'views_handler_field_date',
      'click sortable' => TRUE,
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_date',
    ),
  );

  $data['sshkey']['operations'] = array(
    'field' => array(
      'title' => t('Operations'),
      'help' => t('Provide links to operations the user can perform for the SSH key.'),
      'handler' => 'sshkey_handler_field_sshkey_operations',
      'entity_tables' => $entity_tables,
    ),
  );

  $data['sshkey']['view_sshkey'] = array(
    'field' => array(
      'title' => t('View link'),
      'help' => t('Provide a simple link to view the SSH key.'),
      'handler' => 'sshkey_handler_field_sshkey_link_view',
      'entity_tables' => $entity_tables,
    ),
  );

  $data['sshkey']['edit_sshkey'] = array(
    'field' => array(
      'title' => t('Edit link'),
      'help' => t('Provide a simple link to edit the SSH key.'),
      'handler' => 'sshkey_handler_field_sshkey_link_edit',
      'entity_tables' => $entity_tables,
    ),
  );

  $data['sshkey']['delete_sshkey'] = array(
    'field' => array(
      'title' => t('Delete link'),
      'help' => t('Provide a simple link to delete the SSH key.'),
      'handler' => 'sshkey_handler_field_sshkey_link_delete',
      'entity_tables' => $entity_tables,
    ),
  );

  return $data;
}
