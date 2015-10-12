<?php
/**
 * @file
 * mm_schedule.features.field_base.inc
 */

/**
 * Implements hook_field_default_field_bases().
 */
function mm_schedule_field_default_field_bases() {
  $field_bases = array();

  // Exported field_base: 'field_allow_mode_change'
  $field_bases['field_allow_mode_change'] = array(
    'active' => 1,
    'cardinality' => 1,
    'deleted' => 0,
    'entity_types' => array(),
    'field_name' => 'field_allow_mode_change',
    'foreign keys' => array(),
    'indexes' => array(
      'value' => array(
        0 => 'value',
      ),
    ),
    'locked' => 0,
    'module' => 'list',
    'settings' => array(
      'allowed_values' => array(
        0 => 'no',
        1 => 'Allow user to change period and display mode',
      ),
      'allowed_values_function' => '',
      'allowed_values_php' => '',
      'max' => '',
      'min' => '',
      'prefix' => '',
      'suffix' => '',
    ),
    'translatable' => 0,
    'type' => 'list_boolean',
    'type_name' => 'mm_calendar',
  );

  // Exported field_base: 'field_default_display'
  $field_bases['field_default_display'] = array(
    'active' => 1,
    'cardinality' => 1,
    'deleted' => 0,
    'entity_types' => array(),
    'field_name' => 'field_default_display',
    'foreign keys' => array(),
    'indexes' => array(
      'value' => array(
        0 => 'value',
      ),
    ),
    'locked' => 0,
    'module' => 'list',
    'settings' => array(
      'allowed_values' => array(
        'day' => 'Day',
        'week' => 'Week',
        'month' => 'Month',
        'year' => 'Year',
      ),
      'allowed_values_function' => '',
      'allowed_values_php' => '',
      'text_processing' => 0,
    ),
    'translatable' => 0,
    'type' => 'list_text',
    'type_name' => 'mm_calendar',
  );

  // Exported field_base: 'field_default_view'
  $field_bases['field_default_view'] = array(
    'active' => 1,
    'cardinality' => 1,
    'deleted' => 0,
    'entity_types' => array(),
    'field_name' => 'field_default_view',
    'foreign keys' => array(),
    'indexes' => array(
      'value' => array(
        0 => 'value',
      ),
    ),
    'locked' => 0,
    'module' => 'list',
    'settings' => array(
      'allowed_values' => array(
        'upcoming' => 'Upcoming',
        'list' => 'List',
        'grid' => 'Grid',
      ),
      'allowed_values_function' => '',
      'allowed_values_php' => '',
      'text_processing' => 0,
    ),
    'translatable' => 0,
    'type' => 'list_text',
    'type_name' => 'mm_calendar',
  );

  // Exported field_base: 'field_first_month'
  $field_bases['field_first_month'] = array(
    'active' => 1,
    'cardinality' => 1,
    'deleted' => 0,
    'entity_types' => array(),
    'field_name' => 'field_first_month',
    'foreign keys' => array(),
    'indexes' => array(
      'value' => array(
        0 => 'value',
      ),
    ),
    'locked' => 0,
    'module' => 'list',
    'settings' => array(
      'allowed_values' => array(
        1 => 'January',
        2 => 'February',
        3 => 'March',
        4 => 'April',
        5 => 'May',
        6 => 'June',
        7 => 'July',
        8 => 'August',
        9 => 'September',
        10 => 'October',
        11 => 'November',
        12 => 'December',
      ),
      'allowed_values_function' => '',
      'allowed_values_php' => '',
      'max' => '',
      'min' => '',
      'prefix' => '',
      'suffix' => '',
    ),
    'translatable' => 0,
    'type' => 'list_integer',
    'type_name' => 'mm_calendar',
  );

  // Exported field_base: 'field_mm_events'
  $field_bases['field_mm_events'] = array(
    'active' => 1,
    'cardinality' => -1,
    'deleted' => 0,
    'entity_types' => array(),
    'field_name' => 'field_mm_events',
    'foreign keys' => array(
      'nid' => array(
        'columns' => array(
          'nid' => 'nid',
        ),
        'table' => 'node',
      ),
    ),
    'indexes' => array(
      'nid' => array(
        0 => 'nid',
      ),
    ),
    'locked' => 0,
    'module' => 'node_reference',
    'settings' => array(
      'referenceable_types' => array(
        'alert' => 0,
        'amherst_book' => 0,
        'amherst_publisher' => 0,
        'content_slider' => 0,
        'content_slider_item' => 0,
        'course' => 0,
        'course_assign' => 0,
        'course_links' => 0,
        'course_noncourse' => 0,
        'course_xlist' => 0,
        'crs_guide' => 0,
        'date' => 0,
        'gallery' => 0,
        'mm_calendar' => 0,
        'mm_event' => 'mm_event',
        'mm_media' => 0,
        'new_media' => 0,
        'portal_page' => 0,
        'redirect' => 0,
        'roster' => 0,
        'rss_page' => 0,
        'story' => 0,
        'subpglist' => 0,
        'test' => 0,
        'viewref' => 0,
        'webform' => 0,
      ),
      'view' => array(
        'args' => array(),
        'display_name' => 'references_1',
        'view_name' => 'mm_calendar',
      ),
    ),
    'translatable' => 0,
    'type' => 'node_reference',
    'type_name' => 'mm_calendar',
  );

  // Exported field_base: 'field_mm_groups'
  $field_bases['field_mm_groups'] = array(
    'active' => 1,
    'cardinality' => -1,
    'deleted' => 0,
    'entity_types' => array(),
    'field_name' => 'field_mm_groups',
    'foreign keys' => array(),
    'indexes' => array(),
    'locked' => 0,
    'module' => 'mm_fields',
    'settings' => array(
      'mm_list_max' => 0,
      'mm_list_min' => 0,
      'mm_list_popup_start' => array(),
      'mm_list_readonly' => 0,
      'mm_list_show_info' => 0,
    ),
    'translatable' => 0,
    'type' => 'mm_fields_mm_grouplist',
    'type_name' => 'mm_calendar',
  );

  // Exported field_base: 'field_mm_pages'
  $field_bases['field_mm_pages'] = array(
    'active' => 1,
    'cardinality' => -1,
    'deleted' => 0,
    'entity_types' => array(),
    'field_name' => 'field_mm_pages',
    'foreign keys' => array(),
    'indexes' => array(),
    'locked' => 0,
    'module' => 'mm_fields',
    'settings' => array(
      'mm_list_enabled' => 'r',
      'mm_list_max' => 0,
      'mm_list_min' => 0,
      'mm_list_popup_start' => array(
        7 => 'Home',
      ),
      'mm_list_readonly' => 0,
      'mm_list_selectable' => 'u',
      'mm_list_show_info' => 1,
    ),
    'translatable' => 0,
    'type' => 'mm_fields_mm_catlist',
    'type_name' => 'mm_calendar',
  );

  // Exported field_base: 'field_show_export'
  $field_bases['field_show_export'] = array(
    'active' => 1,
    'cardinality' => 1,
    'deleted' => 0,
    'entity_types' => array(),
    'field_name' => 'field_show_export',
    'foreign keys' => array(),
    'indexes' => array(
      'value' => array(
        0 => 'value',
      ),
    ),
    'locked' => 0,
    'module' => 'list',
    'settings' => array(
      'allowed_values' => array(
        0 => 'no',
        1 => 'Show the Export link',
      ),
      'allowed_values_function' => '',
      'allowed_values_php' => '',
      'max' => '',
      'min' => '',
      'prefix' => '',
      'suffix' => '',
    ),
    'translatable' => 0,
    'type' => 'list_boolean',
    'type_name' => 'mm_calendar',
  );

  // Exported field_base: 'field_upcoming_items'
  $field_bases['field_upcoming_items'] = array(
    'active' => 1,
    'cardinality' => 1,
    'deleted' => 0,
    'entity_types' => array(),
    'field_name' => 'field_upcoming_items',
    'foreign keys' => array(),
    'indexes' => array(
      'value' => array(
        0 => 'value',
      ),
    ),
    'locked' => 0,
    'module' => 'list',
    'settings' => array(
      'allowed_values' => array(
        1 => 1,
        2 => 2,
        3 => 3,
        4 => 4,
        5 => 5,
        10 => 10,
        15 => 15,
        20 => 20,
        25 => 25,
        30 => 30,
        50 => 50,
        75 => 75,
        100 => 100,
      ),
      'allowed_values_function' => '',
      'allowed_values_php' => '',
    ),
    'translatable' => 0,
    'type' => 'list_integer',
  );

  // Exported field_base: 'field_wd_end'
  $field_bases['field_wd_end'] = array(
    'active' => 1,
    'cardinality' => 1,
    'deleted' => 0,
    'entity_types' => array(),
    'field_name' => 'field_wd_end',
    'foreign keys' => array(),
    'indexes' => array(
      'value' => array(
        0 => 'value',
      ),
    ),
    'locked' => 0,
    'module' => 'list',
    'settings' => array(
      'allowed_values' => array(
        0 => 'midnight',
        30 => '12:30 AM',
        60 => '1:00 AM',
        90 => '1:30 AM',
        120 => '2:00 AM',
        150 => '2:30 AM',
        180 => '3:00 AM',
        210 => '3:30 AM',
        240 => '4:00 AM',
        270 => '4:30 AM',
        300 => '5:00 AM',
        330 => '5:30 AM',
        360 => '6:00 AM',
        390 => '6:30 AM',
        420 => '7:00 AM',
        450 => '7:30 AM',
        480 => '8:00 AM',
        510 => '8:30 AM',
        540 => '9:00 AM',
        570 => '9:30 AM',
        600 => '10:00 AM',
        630 => '10:30 AM',
        660 => '11:00 AM',
        690 => '11:30 AM',
        720 => 'noon',
        750 => '12:30 PM',
        780 => '1:00 PM',
        810 => '1:30 PM',
        840 => '2:00 PM',
        870 => '2:30 PM',
        900 => '3:00 PM',
        930 => '3:30 PM',
        960 => '4:00 PM',
        990 => '4:30 PM',
        1020 => '5:00 PM',
        1050 => '5:30 PM',
        1080 => '6:00 PM',
        1110 => '6:30 PM',
        1140 => '7:00 PM',
        1170 => '7:30 PM',
        1200 => '8:00 PM',
        1230 => '8:30 PM',
        1260 => '9:00 PM',
        1290 => '9:30 PM',
        1320 => '10:00 PM',
        1350 => '10:30 PM',
        1380 => '11:00 PM',
        1410 => '11:30 PM',
      ),
      'allowed_values_function' => '',
      'allowed_values_php' => '',
      'max' => '',
      'min' => '',
      'prefix' => '',
      'suffix' => '',
    ),
    'translatable' => 0,
    'type' => 'list_integer',
    'type_name' => 'mm_calendar',
  );

  // Exported field_base: 'field_wd_inc_maj'
  $field_bases['field_wd_inc_maj'] = array(
    'active' => 1,
    'cardinality' => 1,
    'deleted' => 0,
    'entity_types' => array(),
    'field_name' => 'field_wd_inc_maj',
    'foreign keys' => array(),
    'indexes' => array(
      'value' => array(
        0 => 'value',
      ),
    ),
    'locked' => 0,
    'module' => 'list',
    'settings' => array(
      'allowed_values' => array(
        'half' => '30 minutes',
        'hour' => '1 hour',
      ),
      'allowed_values_function' => '',
      'allowed_values_php' => '',
    ),
    'translatable' => 0,
    'type' => 'list_text',
  );

  // Exported field_base: 'field_wd_increment'
  $field_bases['field_wd_increment'] = array(
    'active' => 1,
    'cardinality' => 1,
    'deleted' => 0,
    'entity_types' => array(),
    'field_name' => 'field_wd_increment',
    'foreign keys' => array(),
    'indexes' => array(
      'value' => array(
        0 => 'value',
      ),
    ),
    'locked' => 0,
    'module' => 'list',
    'settings' => array(
      'allowed_values' => array(
        5 => '5 minutes',
        10 => '10 minutes',
        15 => '15 minutes',
        30 => '30 minutes',
        60 => '1 hour',
      ),
      'allowed_values_function' => '',
      'allowed_values_php' => '',
      'max' => '',
      'min' => '',
      'prefix' => '',
      'suffix' => '',
    ),
    'translatable' => 0,
    'type' => 'list_integer',
    'type_name' => 'mm_calendar',
  );

  // Exported field_base: 'field_wd_start'
  $field_bases['field_wd_start'] = array(
    'active' => 1,
    'cardinality' => 1,
    'deleted' => 0,
    'entity_types' => array(),
    'field_name' => 'field_wd_start',
    'foreign keys' => array(),
    'indexes' => array(
      'value' => array(
        0 => 'value',
      ),
    ),
    'locked' => 0,
    'module' => 'list',
    'settings' => array(
      'allowed_values' => array(
        0 => 'midnight',
        30 => '12:30 AM',
        60 => '1:00 AM',
        90 => '1:30 AM',
        120 => '2:00 AM',
        150 => '2:30 AM',
        180 => '3:00 AM',
        210 => '3:30 AM',
        240 => '4:00 AM',
        270 => '4:30 AM',
        300 => '5:00 AM',
        330 => '5:30 AM',
        360 => '6:00 AM',
        390 => '6:30 AM',
        420 => '7:00 AM',
        450 => '7:30 AM',
        480 => '8:00 AM',
        510 => '8:30 AM',
        540 => '9:00 AM',
        570 => '9:30 AM',
        600 => '10:00 AM',
        630 => '10:30 AM',
        660 => '11:00 AM',
        690 => '11:30 AM',
        720 => 'noon',
        750 => '12:30 PM',
        780 => '1:00 PM',
        810 => '1:30 PM',
        840 => '2:00 PM',
        870 => '2:30 PM',
        900 => '3:00 PM',
        930 => '3:30 PM',
        960 => '4:00 PM',
        990 => '4:30 PM',
        1020 => '5:00 PM',
        1050 => '5:30 PM',
        1080 => '6:00 PM',
        1110 => '6:30 PM',
        1140 => '7:00 PM',
        1170 => '7:30 PM',
        1200 => '8:00 PM',
        1230 => '8:30 PM',
        1260 => '9:00 PM',
        1290 => '9:30 PM',
        1320 => '10:00 PM',
        1350 => '10:30 PM',
        1380 => '11:00 PM',
        1410 => '11:30 PM',
      ),
      'allowed_values_function' => '',
      'allowed_values_php' => '',
      'max' => '',
      'min' => '',
      'prefix' => '',
      'suffix' => '',
    ),
    'translatable' => 0,
    'type' => 'list_integer',
    'type_name' => 'mm_calendar',
  );

  return $field_bases;
}
