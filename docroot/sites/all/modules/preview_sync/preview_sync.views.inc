<?php

/**
 * @file
 * Provide views data and handlers for preview_sync.module.
 *
 * @ingroup views_module_handlers
 */

/**
 * Implements hook_views_data().
 */
function preview_sync_views_data() {

  // Define the base group of this table. Fields that don't
  // have a group defined will go into this field by default.
  $data['preview_sync_log']['table']['group']  = t('Preview Sync');

  // Define this as a base table – a table that can be described in itself by
  // views (and not just being brought in as a relationship). In reality this
  // is not very useful for this table, as it isn't really a distinct object of
  // its own, but it makes a good example.
  $data['preview_sync_log']['table']['base'] = array(
    'field' => 'pid', // This is the identifier field for the view.
    'title' => t('Preview Sync'),
    'help' => t('Preview Sync batch item log messages.'),
    'weight' => -10,
  );

  // lid.
  $data['preview_sync_log']['lid'] = array(
    'title' => t('Log ID'),
    'help' => t('The log ID'),
    'field' => array(
      'handler' => 'views_handler_field_numeric',
      'click sortable' => TRUE,
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_numeric',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_numeric',
    ),
  );

  // pid.
  $data['preview_sync_log']['pid'] = array(
    'title' => t('Batch ID'),
    'help' => t('The batch ID'),
    'field' => array(
      'handler' => 'views_handler_field_numeric',
      'click sortable' => TRUE,
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_numeric',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_numeric',
    ),
  );

  // qid.
  $data['preview_sync_log']['qid'] = array(
    'title' => t('Item ID'),
    'help' => t('The queue item ID'),
    'field' => array(
      'handler' => 'views_handler_field_numeric',
      'click sortable' => TRUE,
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_numeric',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_numeric',
    ),
  );

  // uid.
  $data['preview_sync_log']['uid'] = array(
    'title' => t('Author uid'),
    'help' => t('If you need more fields than the uid add the comment: author relationship'),
    'relationship' => array(
      'title' => t('Author'),
      'help' => t("The User ID of the batch item's author."),
      'base' => 'users',
      'base field' => 'uid',
      'handler' => 'views_handler_relationship',
      'label' => t('author'),
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_numeric',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_numeric',
    ),
    'field' => array(
      'handler' => 'views_handler_field_user',
    ),
  );

  // start.
  $data['preview_sync_log']['start'] = array(
    'title' => t('Start date'),
    'help' => t('Date and time of when the queued batch item was started.'),
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

  // duration.
  $data['preview_sync_log']['duration'] = array(
    'title' => t('Item duration'),
    'help' => t('The queue item duration'),
    'field' => array(
      'handler' => 'views_handler_field_numeric',
      'click sortable' => TRUE,
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_numeric',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_numeric',
    ),
  );

  // type.
  $data['preview_sync_log']['type'] = array(
    'title' => t('Type'),
    'help' => t('The type of batch item.'),
    'field' => array(
      'handler' => 'views_handler_field',
      'click sortable' => TRUE,
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_string',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_string',
    ),
  );

  // status.
  $data['preview_sync_log']['status'] = array(
    'title' => t('Status'),
    'help' => t('Whether the item was processed successfully or not.'),
    'field' => array(
      'handler' => 'views_handler_field_boolean',
      'click sortable' => TRUE,
      'output formats' => array(
        'approved-not-approved' => array(t('Success'), t('Failed')),
      ),
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_boolean_operator',
      'label' => t('Successful batch item'),
      'type' => 'yes-no',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
  );

  // message.
  $data['preview_sync_log']['message'] = array(
    'title' => t('Message'),
    'help' => t('The Drush output for the batch item.'),
    'field' => array(
      'handler' => 'views_handler_field',
      'click sortable' => TRUE,
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_string',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_string',
    ),
  );

  return $data;
}
