<?php

/**
 * @file
 * Views integration for the Total Subcription module
 */

/**
 * Implements hook_views_data().
 */
function total_subscription_views_data() {

  $table = array(
    'total_subscription' => array(
      'table' => array(
        'group' => 'Total Subscription',
        'base' => array(
          'field' => 'tsid',
          'title' => 'Total Subscription',
          'help' => 'Mydons Custom database table',
        ),
      ),
      // Description of tsid.
      'tsid' => array(
        'title' => t('Total Subscription Id'),
        'help' => t('Total Subscription Id field'),
        // Is tsid field sortable TRUE.
        'field' => array(
          'click sortable' => TRUE,
        ),
        // Filter handler for filtering records by tsid.
        'filter' => array(
          'handler' => 'views_handler_filter_numeric',
        ),
        'sort' => array(
          'handler' => 'views_handler_sort',
        ),
      ),
      // Description of mail field.
      'mail' => array(
        'title' => t('Total Subscription Mail'),
        'help' => t('Total Subscription Mail field'),
        'field' => array(
          'click sortable' => TRUE,
        ),
        'filter' => array(
          'handler' => 'views_handler_filter_string',
        ),

        'sort' => array(
          'handler' => 'views_handler_sort',
        ),
      ),
      // Description of bundle field.
      'bundle' => array(
        'title' => t('Total Subscription bundle'),
        'help' => t('Total Subscription bundle field'),
        'field' => array(
          'click sortable' => TRUE,
        ),
        'filter' => array(
          'handler' => 'views_handler_filter_string',
        ),

        'sort' => array(
          'handler' => 'views_handler_sort',
        ),
      ),
      // Description of entity_id field.
      'entity_id' => array(
        'title' => t('Total Subscription entity_id'),
        'help' => t('Total Subscription entity_id field'),
        'field' => array(
          'click sortable' => TRUE,
        ),
        'filter' => array(
          'handler' => 'views_handler_filter_string',
        ),

        'sort' => array(
          'handler' => 'views_handler_sort',
        ),
      ),
      // Description of active field.
      'active' => array(
        'title' => t('Total Subscription active'),
        'help' => t('Total Subscription active field'),
        'field' => array(
          'click sortable' => TRUE,
        ),
        'filter' => array(
          'handler' => 'views_handler_filter_string',
        ),

        'sort' => array(
          'handler' => 'views_handler_sort',
        ),
      ),
      // Description of hash field.
      'hash' => array(
        'title' => t('Total Subscription hash'),
        'help' => t('Total Subscription hash field'),
        'field' => array(
          'click sortable' => TRUE,
        ),
        'filter' => array(
          'handler' => 'views_handler_filter_string',
        ),

        'sort' => array(
          'handler' => 'views_handler_sort',
        ),
      ),
      // Description of entity_type field.
      'entity_type' => array(
        'title' => t('Total Subscription Entity Type'),
        'help' => t('Total Subscription Entity Type field'),
        'field' => array(
          'click sortable' => TRUE,
        ),
        'filter' => array(
          'handler' => 'views_handler_filter_string',
        ),
        'sort' => array(
          'handler' => 'views_handler_sort',
        ),
      ),
    ),
  );
  return $table;
}
