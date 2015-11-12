<?php

/**
 * Implements hook_views_data()
 */
function bookit_item_views_data() {
  $data = array();

  $data['bookit_item']['table']['group']  = t('Bookable Item');

  $data['bookit_item']['table']['base'] = array(
    'field' => 'item_id',
    'title' => t('Bookable Item'),
    'help' => t('Bookable Items of the booking platform.'),
    'access query tag' => 'bookit_item_access',
  );

  $data['bookit_item']['table']['entity type'] = 'bookit_item';

  $data['bookit_item']['table']['default_relationship'] = array(
    'bookit_item_revision' => array(
      'table' => 'bookit_item_revision',
      'field' => 'revision_id',
    ),
  );

  // Expose the item ID.
  $data['bookit_item']['item_id'] = array(
    'title' => t('Bookable Item ID'),
    'help' => t('The unique internal identifier of the bookable item.'),
    'field' => array(
      'handler' => 'bookit_item_handler_field_item',
      'click sortable' => TRUE,
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_numeric',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'argument' => array(
      'handler' => 'bookit_item_handler_argument_item_id',
    ),
  );

  // Expose the item type.
  $data['bookit_item']['type'] = array(
    'title' => t('Booking Type'),
    'help' => t('The human-readable name of the booking type of the bookable item.'),
    'field' => array(
      'handler' => 'bookit_item_handler_field_item_type',
      'click sortable' => TRUE,
    ),
    'filter' => array(
      'handler' => 'bookit_item_handler_filter_item_type',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_string',
    ),
  );

  // Expose the item title.
  $data['bookit_item']['title'] = array(
    'title' => t('Title'),
    'help' => t('The title of the bookable item used for administrative display.'),
    'field' => array(
      'handler' => 'bookit_item_handler_field_item',
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

  if (module_exists('locale')) {
    // Expose the language
    $data['bookit_item']['language'] = array(
      'title' => t('Language'),
      'help' => t('The language the bookable item is in.'),
      'field' => array(
        'handler' => 'views_handler_field_locale_language',
        'click sortable' => TRUE,
      ),
      'filter' => array(
        'handler' => 'views_handler_filter_locale_language',
      ),
      'argument' => array(
        'handler' => 'views_handler_argument_locale_language',
      ),
      'sort' => array(
        'handler' => 'views_handler_sort',
      ),
    );
  }

  // Expose the creator uid.
  $data['bookit_item']['uid'] = array(
    'title' => t('Creator'),
    'help' => t('Relate a bookable item to the user who created it.'),
    'relationship' => array(
      'handler' => 'views_handler_relationship',
      'base' => 'users',
      'field' => 'uid',
      'label' => t('Booking item creator'),
    ),
  );

  // Expose the item status.
  $data['bookit_item']['status'] = array(
    'title' => t('Status'),
    'help' => t('Whether or not the bookable item is active.'),
    'field' => array(
      'handler' => 'views_handler_field_boolean',
      'click sortable' => TRUE,
      'output formats' => array(
        'active-disabled' => array(t('Active'), t('Disabled')),
      ),
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_boolean_operator',
      'label' => t('Active'),
      'type' => 'yes-no',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
  );

  // Expose the created and changed timestamps.
  $data['bookit_item']['created'] = array(
    'title' => t('Created date'),
    'help' => t('The date the bookable item was created.'),
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

  $data['bookit_item']['created_fulldate'] = array(
    'title' => t('Created date'),
    'help' => t('In the form of CCYYMMDD.'),
    'argument' => array(
      'field' => 'created',
      'handler' => 'views_handler_argument_node_created_fulldate',
    ),
  );

  $data['bookit_item']['created_year_month'] = array(
    'title' => t('Created year + month'),
    'help' => t('In the form of YYYYMM.'),
    'argument' => array(
      'field' => 'created',
      'handler' => 'views_handler_argument_node_created_year_month',
    ),
  );

  $data['bookit_item']['created_timestamp_year'] = array(
    'title' => t('Created year'),
    'help' => t('In the form of YYYY.'),
    'argument' => array(
      'field' => 'created',
      'handler' => 'views_handler_argument_node_created_year',
    ),
  );

  $data['bookit_item']['created_month'] = array(
    'title' => t('Created month'),
    'help' => t('In the form of MM (01 - 12).'),
    'argument' => array(
      'field' => 'created',
      'handler' => 'views_handler_argument_node_created_month',
    ),
  );

  $data['bookit_item']['created_day'] = array(
    'title' => t('Created day'),
    'help' => t('In the form of DD (01 - 31).'),
    'argument' => array(
      'field' => 'created',
      'handler' => 'views_handler_argument_node_created_day',
    ),
  );

  $data['bookit_item']['created_week'] = array(
    'title' => t('Created week'),
    'help' => t('In the form of WW (01 - 53).'),
    'argument' => array(
      'field' => 'created',
      'handler' => 'views_handler_argument_node_created_week',
    ),
  );

  $data['bookit_item']['changed'] = array(
    'title' => t('Updated date'),
    'help' => t('The date the item was last updated.'),
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

  $data['bookit_item']['changed_fulldate'] = array(
    'title' => t('Updated date'),
    'help' => t('In the form of CCYYMMDD.'),
    'argument' => array(
      'field' => 'changed',
      'handler' => 'views_handler_argument_node_created_fulldate',
    ),
  );

  $data['bookit_item']['changed_year_month'] = array(
    'title' => t('Updated year + month'),
    'help' => t('In the form of YYYYMM.'),
    'argument' => array(
      'field' => 'changed',
      'handler' => 'views_handler_argument_node_created_year_month',
    ),
  );

  $data['bookit_item']['changed_timestamp_year'] = array(
    'title' => t('Updated year'),
    'help' => t('In the form of YYYY.'),
    'argument' => array(
      'field' => 'changed',
      'handler' => 'views_handler_argument_node_created_year',
    ),
  );

  $data['bookit_item']['changed_month'] = array(
    'title' => t('Updated month'),
    'help' => t('In the form of MM (01 - 12).'),
    'argument' => array(
      'field' => 'changed',
      'handler' => 'views_handler_argument_node_created_month',
    ),
  );

  $data['bookit_item']['changed_day'] = array(
    'title' => t('Updated day'),
    'help' => t('In the form of DD (01 - 31).'),
    'argument' => array(
      'field' => 'changed',
      'handler' => 'views_handler_argument_node_created_day',
    ),
  );

  $data['bookit_item']['changed_week'] = array(
    'title' => t('Updated week'),
    'help' => t('In the form of WW (01 - 53).'),
    'argument' => array(
      'field' => 'changed',
      'handler' => 'views_handler_argument_node_created_week',
    ),
  );

  $data['bookit_item']['operations'] = array(
    'field' => array(
      'title' => t('Operations links'),
      'help' => t('Display all the available operations links for the item.'),
      'handler' => 'bookit_item_handler_field_item_operations',
    ),
  );

  $data['bookit_item']['empty_text'] = array(
    'title' => t('Empty text'),
    'help' => t('Displays an appropriate empty text message for bookable item lists.'),
    'area' => array(
      'handler' => 'bookit_item_handler_area_empty_text',
    ),
  );

  /**
   * Integrate the item revision table.
   */
  $data['bookit_item_revision']['table']['entity type'] = 'bookit_item';
  $data['bookit_item_revision']['table']['group']  = t('Bookable Item revision');

  // Advertise this table as a possible base table.
  $data['bookit_item_revision']['table']['base'] = array(
    'field' => 'revision_id',
    'title' => t('Bookable Item revision'),
    'help' => t('Bookable Item revision is a history of changes to a bookable item.'),
    'defaults' => array(
      'field' => 'title',
    ),
  );

  $data['bookit_item_revision']['table']['join'] = array(
    'bookit_item' => array(
      'left_field' => 'revision_id',
      'field' => 'revision_id',
    )
  );

  $data['bookit_item_revision']['table']['default_relationship'] = array(
    'bookit_item' => array(
      'table' => 'bookit_item',
      'field' => 'revision_id',
    ),
  );

  // Expose the revision item ID
  $data['bookit_item_revision']['item_id'] = array(
    'title' => t('Bookable Item ID'),
    'help' => t('The unique internal identifier of the bookable item.'),
    'field' => array(
      'handler' => 'bookit_item_handler_field_item',
      'click sortable' => TRUE,
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_numeric',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'argument' => array(
      'handler' => 'bookit_item_handler_argument_item_id',
    ),
  );

  // Expose the revision ID.
  $data['bookit_item_revision']['revision_id'] = array(
    'title' => t('Revision ID'),
    'help' => t('The revision ID of the bookable item revision.'),
    'field' => array(
      'click sortable' => TRUE,
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_numeric',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'relationship' => array(
      'handler' => 'views_handler_relationship',
      'base' => 'bookit_item',
      'base field' => 'revision_id',
      'title' => t('Bookable Item'),
      'label' => t('Latest item revision'),
    ),
  );

  // Expose the item revision title.
  $data['bookit_item_revision']['title'] = array(
    'title' => t('Title'),
    'help' => t('The title of the bookable item revision used for administrative display.'),
    'field' => array(
      'handler' => 'bookit_item_handler_field_item',
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

  // Expose the item revision user ID.
  $data['bookit_item_revision']['revision_uid'] = array(
    'title' => t('User'),
    'help' => t('Relate a bookable item revision to the user who created the revision.'),
    'relationship' => array(
      'handler' => 'views_handler_relationship',
      'base' => 'users',
      'base field' => 'uid',
      'field' => 'revision_uid',
      'field_name' => 'revision_uid',
      'label' => t('Revision user'),
    ),
  );

  // Expose the item revision status.
  $data['bookit_item_revision']['status'] = array(
    'title' => t('Status'),
    'help' => t('Whether or not the bookable item was active at the time of the revision.'),
    'field' => array(
      'handler' => 'views_handler_field_boolean',
      'click sortable' => TRUE,
      'output formats' => array(
        'active-disabled' => array(t('Active'), t('Disabled')),
      ),
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_boolean_operator',
      'label' => t('Active'),
      'type' => 'yes-no',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
  );

  // Expose the order revision log.
  $data['bookit_item_revision']['log'] = array(
    'title' => t('Log message'),
    'help' => t('The log message entered when the revision was created.'),
    'field' => array(
      'handler' => 'views_handler_field_xss',
     ),
    'filter' => array(
      'handler' => 'views_handler_filter_string',
    ),
  );

  // Expose the revision timestamp.
  $data['bookit_item_revision']['revision_timestamp'] = array(
    'title' => t('Revision date'),
    'help' => t('The date the bookable item revision was created.'),
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

  return $data;
}
