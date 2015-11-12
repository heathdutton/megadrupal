<?php

/**
 * @file
 * Conect our ecard entity with views by hook_views_data().
 */

/**
 * Implements hook_views_data().
 *
 * @todo Add joins to other entity tables. There are no joins to other tables
 * because we would have to make sure the entity type is respected.
 */
function ecard_views_data() {
  $data['ecard']['table']['group'] = t('Ecard');

  $data['ecard']['table']['base'] = array(
    'field' => 'hash',
    'title' => t('Ecard'),
    'help' => t('Ecards created on this site.'),
  );

  $string_field = array(
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'field' => array(
      'handler' => 'views_handler_field',
      'click sortable' => TRUE,
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_string',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_string',
    ),
  );

  $numeric_field = array('sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'field' => array(
      'handler' => 'views_handler_field_numeric',
      'click sortable' => TRUE,
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_numeric',
    ),
    'filter' => array(
      'handler' => 'views_handler_field_numeric',
    ),
  );

  $data['ecard']['hash'] = array(
    'title' => t('Hash'),
    'help' => t('The hash identifier of an ecard.'),
  );
  $data['ecard']['hash'] += $string_field;

  $data['ecard']['entity_type'] = array(
    'title' => t('Entity Type'),
    'help' => t('The entity type the ecard is saved in.'),
  );
  $data['ecard']['entity_type'] += $string_field;

  $data['ecard']['bundle'] = array(
    'title' => t('Entity Bundle'),
    'help' => t('The bundle the entity is saved in.'),
  );
  $data['ecard']['bundle'] += $string_field;

  $data['ecard']['entity_id'] = array(
    'title' => t('Entity ID'),
    'help' => t('The entity:bundle:id the ecard is saved in.'),
  );
  $data['ecard']['entity_id'] += $numeric_field;

  $data['ecard']['mail_from'] = array(
    'title' => t('Mail From'),
    'help' => t('The mail of the sender.'),
  );
  $data['ecard']['mail_from'] += $string_field;

  $data['ecard']['name_from'] = array(
    'title' => t('Name From'),
    'help' => t('The name of the sender.'),
  );
  $data['ecard']['name_from'] += $string_field;

  $data['ecard']['mail_to'] = array(
    'title' => t('Mail to'),
    'help' => t('The mail of the recipient.'),
  );
  $data['ecard']['mail_to'] += $string_field;

  $data['ecard']['name_to'] = array(
    'title' => t('Name to'),
    'help' => t('The name of the recipient.'),
  );
  $data['ecard']['name_to'] += $string_field;

  $data['ecard']['text'] = array(
    'title' => t('Text'),
    'help' => t('The content of the ecard.'),
  );
  $data['ecard']['text'] += $string_field;

  $data['ecard']['pick_up'] = array(
    'title' => t('Pickup'),
    'help' => t('Flag if the ecard has been picked up.'),
    'field' => array(
      'handler' => 'views_handler_field_boolean',
      'click sortable' => TRUE,
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_boolean_operator',
      'label' => t('Ecard pickup'),
      'type' => 'yes-no',
      // use boolean_field = 1 instead of boolean_field <> 0 in WHERE statment
      'use equal' => TRUE,
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
  );

  $data['ecard']['sent'] = array(
    'title' => t('Sent'),
    'help' => t('Flag if the ecard has been sent.'),
    'field' => array(
      'handler' => 'views_handler_field_boolean',
      'click sortable' => TRUE,
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_boolean_operator',
      'label' => t('Ecard sent'),
      'type' => 'yes-no',
      // use boolean_field = 1 instead of boolean_field <> 0 in WHERE statment
      'use equal' => TRUE,
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
  );

  $data['ecard']['notify'] = array(
    'title' => t('Notfiy'),
    'help' => t('Flag if the sender should be notified when the ecard is picked up.'),
    'field' => array(
      'handler' => 'views_handler_field_boolean',
      'click sortable' => TRUE,
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_boolean_operator',
      'label' => t('Ecard notify'),
      'type' => 'yes-no',
      // use boolean_field = 1 instead of boolean_field <> 0 in WHERE statment
      'use equal' => TRUE,
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
  );

  $data['ecard']['created'] = array(
    'title' => t('Created'),
    'help' => t('When the ecard was created.'),
    'field' => array(
      'handler' => 'views_handler_field_date',
      'click sortable' => TRUE,
    ),
    'sort' => array(
      'handler' => 'views_handler_sort_date',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_date',
    ),
  );

  $data['ecard']['sent_time'] = array(
    'title' => t('Sent time'),
    'help' => t('When the ecard was sent.'),
    'field' => array(
      'handler' => 'views_handler_field_date',
      'click sortable' => TRUE,
    ),
    'sort' => array(
      'handler' => 'views_handler_sort_date',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_date',
    ),
  );

  $data['ecard']['scheduled_time'] = array(
    'title' => t('Scheduled time'),
    'help' => t('When the ecard is/was scheduled to be sent.'),
    'field' => array(
      'handler' => 'views_handler_field_date',
      'click sortable' => TRUE,
    ),
    'sort' => array(
      'handler' => 'views_handler_sort_date',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_date',
    ),
  );

  $data['ecard']['nid'] = array(
    'title' => t('Ecard nid'),
    'help' => t('The node id of the node where the ecard was created.'),
    'field' => array(
      'handler' => 'views_handler_field_node',
      'click sortable' => TRUE,
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_numeric',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_node_nid',
      'parent' => 'views_handler_argument_numeric',
    ),
    'relationship' => array(
      'handler' => 'views_handler_relationship',
      'base' => 'node',
      'base field' => 'nid',
      'label' => t('Node'),
    ),
  );

  return $data;
}
