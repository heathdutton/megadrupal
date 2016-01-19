<?php
/**
 * @file
 * Implements the ces_message view message.
 *
 * @defgroup ces_message_view_data Ces Message View
 * @ingroup ces_message
 * @{
 * View from message.
 */

/**
 * Implements hook_views_data().
 */
function ces_message_views_data() {
  $data['ces_message']['table']['group'] = t('Message');
  $data['ces_message']['table']['base'] = array(
    'field' => 'mid',
    'title' => t('Messages table'),
    'help' => t('Messages view.'),
    'weight' => -10,
  );

  $data['ces_message']['table']['join'] = array(
    'users' => array(
      'left_field' => 'uid',
      'field' => 'user',
    ),
    'ces_exchange' => array(
      'left_field' => 'id',
      'field' => 'exchange',
    ),
  );
  $data['ces_message']['mid'] = array(
    'title' => t('Message Id'),
    'help' => t('Message identifier.'),
    'field' => array(
      'handler' => 'views_handler_field_numeric',
      'click sortable' => TRUE,
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_numeric',
    ),
  );
  $data['ces_message']['subject'] = array(
    'title' => t('Subject'),
    'help' => t('Subject of the message.'),
    'field' => array(
      'handler' => 'views_handler_field',
      'click sortable' => TRUE,
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_string',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_string',
    ),
  );
  $data['ces_message']['body'] = array(
    'title' => t('Body'),
    'help' => t('Body of the message.'),
    'field' => array(
      'handler' => 'views_handler_field',
      'click sortable' => TRUE,
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_string',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_string',
    ),
  );

  $data['ces_message']['type_message'] = array(
    'title' => t('Type'),
    'help' => t('Type of message.'),
    'field' => array(
      'handler' => 'views_handler_field',
      'click sortable' => TRUE,
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_string',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_string',
    ),
  );
  $data['ces_message']['state'] = array(
    'title' => t('Read'),
    'help' => t('Read on/off.'),
    'field' => array(
      'handler' => 'views_handler_field_boolean',
      'click sortable' => TRUE,
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_boolean_operator',
      'label' => t('State'),
      'type' => 'yes-no',
      'use equal' => TRUE,
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
  );

  $data['ces_message']['created'] = array(
    'title' => t('Created'),
    'help' => t('Creation date.'),
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

  $data['ces_message']['user'] = array(
    'title' => t('User'),
    'help' => t('Id of user.'),
    'field' => array(
      'handler' => 'views_handler_field',
      'click sortable' => TRUE,
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_string',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_string',
    ),
    'relationship' => array(
      'base' => 'users',
      'base field' => 'uid',
      // 'field' => 'nid' -- see hook_views_data_alter(); not needed here.
      'handler' => 'views_handler_relationship',
      'label' => t('Message users relationship'),
      'title' => t('Message users relationship'),
      'help' => t('More information on this relationship'),
    ),
  );
  $data['ces_message']['exchange'] = array(
    'title' => t('Exchange'),
    'help' => t('Exchange.'),
    'field' => array(
      'handler' => 'views_handler_field',
      'click sortable' => TRUE,
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_string',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_string',
    ),
    'relationship' => array(
      'base' => 'ces_exchange',
      'base field' => 'id',
      // 'field' => 'nid' -- see hook_views_data_alter(); not needed here.
      'handler' => 'views_handler_relationship',
      'label' => t('Message Exchange relationship'),
      'title' => t('Message Exchange relationship'),
      'help' => t('More information on this relationship'),
    ),
  );
  return $data;
}
/**
 * @} End of "defgroup ces_message_view".
 */
