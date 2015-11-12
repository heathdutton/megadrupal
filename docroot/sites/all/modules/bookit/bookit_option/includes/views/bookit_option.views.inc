<?php

/**
 * Implements hook_views_data()
 */
function bookit_option_views_data() {
  $data = array();

  $data['bookit_option']['table']['group']  = t('Booking Option');

  $data['bookit_option']['table']['base'] = array(
    'field' => 'option_id',
    'title' => t('Booking Option'),
    'help' => t('Booking Options of the booking platform.'),
    'access query tag' => 'bookit_option_access',
  );

  $data['bookit_option']['table']['entity type'] = 'bookit_option';

  $data['bookit_option']['table']['default_relationship'] = array(
    'bookit_option_revision' => array(
      'table' => 'bookit_option_revision',
      'field' => 'revision_id',
    ),
  );

  // Expose the option ID.
  $data['bookit_option']['option_id'] = array(
    'title' => t('Booking Option ID'),
    'help' => t('The unique internal identifier of the booking option.'),
    'field' => array(
      'handler' => 'bookit_option_handler_field_option',
      'click sortable' => TRUE,
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_numeric',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'argument' => array(
      'handler' => 'bookit_option_handler_argument_option_id',
    ),
  );

  // Expose the option type.
  $data['bookit_option']['type'] = array(
    'title' => t('Type'),
    'help' => t('The human-readable name of the type of the booking option.'),
    'field' => array(
      'handler' => 'bookit_option_handler_field_option_type',
      'click sortable' => TRUE,
    ),
    'filter' => array(
      'handler' => 'bookit_option_handler_filter_option_type',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_string',
    ),
  );

  // Expose the option title.
  $data['bookit_option']['title'] = array(
    'title' => t('Title'),
    'help' => t('The title of the option used for administrative display.'),
    'field' => array(
      'handler' => 'bookit_option_handler_field_option',
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
    $data['bookit_option']['language'] = array(
      'title' => t('Language'),
      'help' => t('The language the booking option is in.'),
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
  $data['bookit_option']['uid'] = array(
    'title' => t('Creator'),
    'help' => t('Relate a booking option to the user who created it.'),
    'relationship' => array(
      'handler' => 'views_handler_relationship',
      'base' => 'users',
      'field' => 'uid',
      'label' => t('Booking option creator'),
    ),
  );

  // Expose the created and changed timestamps.
  $data['bookit_option']['created'] = array(
    'title' => t('Created date'),
    'help' => t('The date the booking option was created.'),
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

  $data['bookit_option']['created_fulldate'] = array(
    'title' => t('Created date'),
    'help' => t('In the form of CCYYMMDD.'),
    'argument' => array(
      'field' => 'created',
      'handler' => 'views_handler_argument_node_created_fulldate',
    ),
  );

  $data['bookit_option']['created_year_month'] = array(
    'title' => t('Created year + month'),
    'help' => t('In the form of YYYYMM.'),
    'argument' => array(
      'field' => 'created',
      'handler' => 'views_handler_argument_node_created_year_month',
    ),
  );

  $data['bookit_option']['created_timestamp_year'] = array(
    'title' => t('Created year'),
    'help' => t('In the form of YYYY.'),
    'argument' => array(
      'field' => 'created',
      'handler' => 'views_handler_argument_node_created_year',
    ),
  );

  $data['bookit_option']['created_month'] = array(
    'title' => t('Created month'),
    'help' => t('In the form of MM (01 - 12).'),
    'argument' => array(
      'field' => 'created',
      'handler' => 'views_handler_argument_node_created_month',
    ),
  );

  $data['bookit_option']['created_day'] = array(
    'title' => t('Created day'),
    'help' => t('In the form of DD (01 - 31).'),
    'argument' => array(
      'field' => 'created',
      'handler' => 'views_handler_argument_node_created_day',
    ),
  );

  $data['bookit_option']['created_week'] = array(
    'title' => t('Created week'),
    'help' => t('In the form of WW (01 - 53).'),
    'argument' => array(
      'field' => 'created',
      'handler' => 'views_handler_argument_node_created_week',
    ),
  );

  $data['bookit_option']['changed'] = array(
    'title' => t('Updated date'),
    'help' => t('The date the booking option was last updated.'),
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

  $data['bookit_option']['changed_fulldate'] = array(
    'title' => t('Updated date'),
    'help' => t('In the form of CCYYMMDD.'),
    'argument' => array(
      'field' => 'changed',
      'handler' => 'views_handler_argument_node_created_fulldate',
    ),
  );

  $data['bookit_option']['changed_year_month'] = array(
    'title' => t('Updated year + month'),
    'help' => t('In the form of YYYYMM.'),
    'argument' => array(
      'field' => 'changed',
      'handler' => 'views_handler_argument_node_created_year_month',
    ),
  );

  $data['bookit_option']['changed_timestamp_year'] = array(
    'title' => t('Updated year'),
    'help' => t('In the form of YYYY.'),
    'argument' => array(
      'field' => 'changed',
      'handler' => 'views_handler_argument_node_created_year',
    ),
  );

  $data['bookit_option']['changed_month'] = array(
    'title' => t('Updated month'),
    'help' => t('In the form of MM (01 - 12).'),
    'argument' => array(
      'field' => 'changed',
      'handler' => 'views_handler_argument_node_created_month',
    ),
  );

  $data['bookit_option']['changed_day'] = array(
    'title' => t('Updated day'),
    'help' => t('In the form of DD (01 - 31).'),
    'argument' => array(
      'field' => 'changed',
      'handler' => 'views_handler_argument_node_created_day',
    ),
  );

  $data['bookit_option']['changed_week'] = array(
    'title' => t('Updated week'),
    'help' => t('In the form of WW (01 - 53).'),
    'argument' => array(
      'field' => 'changed',
      'handler' => 'views_handler_argument_node_created_week',
    ),
  );

  $data['bookit_option']['operations'] = array(
    'field' => array(
      'title' => t('Operations links'),
      'help' => t('Display all the available operations links for the booking option.'),
      'handler' => 'bookit_option_handler_field_option_operations',
    ),
  );

  $data['bookit_option']['empty_text'] = array(
    'title' => t('Empty text'),
    'help' => t('Displays an appropriate empty text message for booking option lists.'),
    'area' => array(
      'handler' => 'bookit_option_handler_area_empty_text',
    ),
  );

  return $data;
}

/**
 * Implements hook_views_data_alter()
 */
function bookit_option_views_data_alter(&$data) {
  foreach (commerce_info_fields('entityreference') as $field_name => $field) {
    if ($field['settings']['target_type'] == 'bookit_option' and isset($field['bundles']['bookit_item'])) {
      list($label, $all_labels) = field_views_field_label($field_name);

      // Bookable item's calculated price for specific date range and booking option.
      $data["field_data_{$field_name}"]["price"] = array(
        'title' => t('@label Price', array('@label' => $label)),
        'group' => t('Bookable Item'),
        'help' => t('The calculated price of a bookable item for a specific date range and booking option.'),
        'field' => array(
          'table' => "field_data_{$field_name}",
          'real field' => "{$field_name}_target_id",
          'additional fields' => array("{$field_name}_target_id"),
          'handler' => 'bookit_option_views_handler_field_price',
          'click sortable' => TRUE,
        ),
        'filter' => array(
          'table' => "field_data_{$field_name}",
          'real field' => "{$field_name}_target_id",
          'additional fields' => array("{$field_name}_target_id"),
          'handler' => 'bookit_option_views_handler_filter_price',
        ),
        'sort' => array(
          'table' => "field_data_{$field_name}",
          'real field' => "{$field_name}_target_id",
          'additional fields' => array("{$field_name}_target_id"),
          'handler' => 'bookit_option_views_handler_sort_price',
        ),
      );

      // Booking button.
      $data["field_data_{$field_name}"]['booking_button'] = array(
        'title' => t('@label Booking Button', array('@label' => $label)),
        'group' => t('Bookable Item'),
        'help' => t('Displays a booking button form element.'),
        'field' => array(
          'table' => "field_data_{$field_name}",
          'real field' => "{$field_name}_target_id",
          'additional fields' => array("entity_id"),
          'handler' => 'bookit_option_handler_field_booking_button',
        ),
      );
    }
  }
}
