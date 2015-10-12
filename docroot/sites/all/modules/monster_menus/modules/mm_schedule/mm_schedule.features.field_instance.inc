<?php
/**
 * @file
 * mm_schedule.features.field_instance.inc
 */

/**
 * Implements hook_field_default_field_instances().
 */
function mm_schedule_field_default_field_instances() {
  $field_instances = array();

  // Exported field_instance: 'node-mm_calendar-body'
  $field_instances['node-mm_calendar-body'] = array(
    'bundle' => 'mm_calendar',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => '',
    'display' => array(
      'default' => array(
        'label' => 'hidden',
        'module' => 'text',
        'settings' => array(),
        'type' => 'text_default',
        'weight' => 0,
      ),
      'ical' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
      'teaser' => array(
        'label' => 'hidden',
        'module' => 'text',
        'settings' => array(
          'trim_length' => 600,
        ),
        'type' => 'text_summary_or_trimmed',
        'weight' => 16,
      ),
    ),
    'entity_type' => 'node',
    'field_name' => 'body',
    'label' => 'Body',
    'required' => FALSE,
    'settings' => array(
      'display_summary' => TRUE,
      'text_processing' => 1,
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'module' => 'text',
      'settings' => array(
        'mm_cck_default_modes' => array(),
        'mm_cck_groups' => array(),
        'mm_cck_use_defaults' => TRUE,
        'mm_cck_users' => array(),
        'rows' => 20,
        'summary_rows' => 5,
      ),
      'type' => 'text_textarea_with_summary',
      'weight' => -4,
    ),
  );

  // Exported field_instance: 'node-mm_calendar-field_allow_mode_change'
  $field_instances['node-mm_calendar-field_allow_mode_change'] = array(
    'bundle' => 'mm_calendar',
    'default_value' => array(
      0 => array(
        'value' => 1,
      ),
    ),
    'default_value_function' => '',
    'default_value_php' => '',
    'deleted' => 0,
    'description' => '',
    'display' => array(
      'default' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 3,
      ),
      'full' => array(
        'label' => 'above',
        'module' => 'number',
        'settings' => array(
          'decimal_separator' => '.',
          'prefix_suffix' => TRUE,
          'scale' => 0,
          'thousand_separator' => '',
        ),
        'type' => 'number_integer',
        'weight' => 3,
      ),
      'ical' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
      'rss' => array(
        'label' => 'above',
        'module' => 'number',
        'settings' => array(
          'decimal_separator' => '.',
          'prefix_suffix' => TRUE,
          'scale' => 0,
          'thousand_separator' => '',
        ),
        'type' => 'number_integer',
        'weight' => 3,
      ),
      'teaser' => array(
        'label' => 'above',
        'module' => 'list',
        'settings' => array(),
        'type' => 'list_default',
        'weight' => 3,
      ),
    ),
    'entity_type' => 'node',
    'field_name' => 'field_allow_mode_change',
    'label' => 'Allow user to change period and display mode',
    'required' => 0,
    'settings' => array(
      'max' => '',
      'min' => '',
      'prefix' => '',
      'suffix' => '',
      'user_register_form' => FALSE,
    ),
    'weight' => 3,
    'widget' => array(
      'active' => 1,
      'module' => 'options',
      'settings' => array(
        'display_label' => 0,
        'mm_cck_default_modes' => array(),
        'mm_cck_groups' => array(),
        'mm_cck_use_defaults' => 1,
        'mm_cck_users' => array(),
      ),
      'type' => 'options_onoff',
      'weight' => 22,
    ),
    'widget_type' => 'optionwidgets_onoff',
  );

  // Exported field_instance: 'node-mm_calendar-field_default_display'
  $field_instances['node-mm_calendar-field_default_display'] = array(
    'bundle' => 'mm_calendar',
    'default_value' => array(
      0 => array(
        'value' => 'month',
      ),
    ),
    'default_value_function' => '',
    'default_value_php' => '',
    'deleted' => 0,
    'description' => '',
    'display' => array(
      'default' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 7,
      ),
      'full' => array(
        'label' => 'above',
        'module' => 'text',
        'settings' => array(),
        'type' => 'text_default',
        'weight' => 5,
      ),
      'ical' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
      'rss' => array(
        'label' => 'above',
        'module' => 'text',
        'settings' => array(),
        'type' => 'text_default',
        'weight' => 5,
      ),
      'teaser' => array(
        'label' => 'above',
        'module' => 'list',
        'settings' => array(),
        'type' => 'list_default',
        'weight' => 5,
      ),
    ),
    'entity_type' => 'node',
    'field_name' => 'field_default_display',
    'label' => 'Default period',
    'required' => 1,
    'settings' => array(
      'text_processing' => 0,
      'user_register_form' => FALSE,
    ),
    'weight' => 5,
    'widget' => array(
      'active' => 1,
      'module' => 'options',
      'settings' => array(
        'mm_cck_default_modes' => array(),
        'mm_cck_groups' => array(),
        'mm_cck_use_defaults' => 1,
        'mm_cck_users' => array(),
      ),
      'type' => 'options_select',
      'weight' => 20,
    ),
    'widget_type' => 'optionwidgets_select',
  );

  // Exported field_instance: 'node-mm_calendar-field_default_view'
  $field_instances['node-mm_calendar-field_default_view'] = array(
    'bundle' => 'mm_calendar',
    'default_value' => array(
      0 => array(
        'value' => 'grid',
      ),
    ),
    'default_value_function' => '',
    'default_value_php' => '',
    'deleted' => 0,
    'description' => '',
    'display' => array(
      'default' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 8,
      ),
      'full' => array(
        'label' => 'above',
        'module' => 'text',
        'settings' => array(),
        'type' => 'text_default',
        'weight' => 6,
      ),
      'ical' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
      'rss' => array(
        'label' => 'above',
        'module' => 'text',
        'settings' => array(),
        'type' => 'text_default',
        'weight' => 6,
      ),
      'teaser' => array(
        'label' => 'above',
        'module' => 'list',
        'settings' => array(),
        'type' => 'list_default',
        'weight' => 6,
      ),
    ),
    'entity_type' => 'node',
    'field_name' => 'field_default_view',
    'label' => 'Default Upcoming/List/Grid mode',
    'required' => 1,
    'settings' => array(
      'text_processing' => 0,
      'user_register_form' => FALSE,
    ),
    'weight' => 6,
    'widget' => array(
      'active' => 1,
      'module' => 'options',
      'settings' => array(
        'mm_cck_default_modes' => array(),
        'mm_cck_groups' => array(),
        'mm_cck_use_defaults' => 1,
        'mm_cck_users' => array(),
      ),
      'type' => 'options_select',
      'weight' => 21,
    ),
    'widget_type' => 'optionwidgets_select',
  );

  // Exported field_instance: 'node-mm_calendar-field_first_month'
  $field_instances['node-mm_calendar-field_first_month'] = array(
    'bundle' => 'mm_calendar',
    'default_value' => array(
      0 => array(
        'value' => 1,
      ),
    ),
    'default_value_function' => '',
    'default_value_php' => '',
    'deleted' => 0,
    'description' => 'Choose the starting month for year views. Use September if your calendar should be displayed based on the school year.',
    'display' => array(
      'default' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 15,
      ),
      'full' => array(
        'label' => 'above',
        'module' => 'number',
        'settings' => array(
          'decimal_separator' => '.',
          'prefix_suffix' => TRUE,
          'scale' => 0,
          'thousand_separator' => '',
        ),
        'type' => 'number_integer',
        'weight' => 15,
      ),
      'ical' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
      'rss' => array(
        'label' => 'above',
        'module' => 'number',
        'settings' => array(
          'decimal_separator' => '.',
          'prefix_suffix' => TRUE,
          'scale' => 0,
          'thousand_separator' => '',
        ),
        'type' => 'number_integer',
        'weight' => 15,
      ),
      'teaser' => array(
        'label' => 'above',
        'module' => 'list',
        'settings' => array(),
        'type' => 'list_default',
        'weight' => 15,
      ),
    ),
    'entity_type' => 'node',
    'field_name' => 'field_first_month',
    'label' => 'First month of the year',
    'required' => 1,
    'settings' => array(
      'file_extensions' => '',
      'user_register_form' => FALSE,
    ),
    'weight' => 15,
    'widget' => array(
      'active' => 1,
      'module' => 'options',
      'settings' => array(
        'mm_cck_default_modes' => array(),
        'mm_cck_groups' => array(),
        'mm_cck_use_defaults' => 1,
        'mm_cck_users' => array(),
      ),
      'type' => 'options_select',
      'weight' => 15,
    ),
    'widget_type' => 'optionwidgets_select',
  );

  // Exported field_instance: 'node-mm_calendar-field_mm_events'
  $field_instances['node-mm_calendar-field_mm_events'] = array(
    'bundle' => 'mm_calendar',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => 'To add an event, type part of its title, then choose it from the resulting list.',
    'display' => array(
      'default' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 18,
      ),
      'full' => array(
        'label' => 'above',
        'module' => 'node_reference',
        'settings' => array(),
        'type' => 'node_reference_default',
        'weight' => 7,
      ),
      'ical' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
      'rss' => array(
        'label' => 'above',
        'module' => 'node_reference',
        'settings' => array(),
        'type' => 'node_reference_default',
        'weight' => 7,
      ),
      'teaser' => array(
        'label' => 'above',
        'module' => 'node_reference',
        'settings' => array(),
        'type' => 'node_reference_default',
        'weight' => 7,
      ),
    ),
    'entity_type' => 'node',
    'field_name' => 'field_mm_events',
    'label' => 'Individual events',
    'required' => 0,
    'settings' => array(
      'file_extensions' => '',
      'user_register_form' => FALSE,
    ),
    'weight' => 7,
    'widget' => array(
      'active' => 1,
      'module' => 'node_reference',
      'settings' => array(
        'autocomplete_match' => 'contains',
        'autocomplete_path' => 'node_reference/autocomplete',
        'mm_cck_default_modes' => array(),
        'mm_cck_groups' => array(),
        'mm_cck_use_defaults' => 1,
        'mm_cck_users' => array(),
        'size' => 60,
      ),
      'type' => 'node_reference_autocomplete',
      'weight' => 7,
    ),
    'widget_type' => 'nodereference_autocomplete',
  );

  // Exported field_instance: 'node-mm_calendar-field_mm_groups'
  $field_instances['node-mm_calendar-field_mm_groups'] = array(
    'bundle' => 'mm_calendar',
    'default_value' => '',
    'deleted' => 0,
    'description' => 'Members of the selected group(s) will see events from this calendar on their personal "My Calendar" page.',
    'display' => array(
      'default' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 11,
      ),
      'full' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 11,
      ),
      'ical' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
      'rss' => array(
        'label' => 'above',
        'module' => 'mm_fields',
        'settings' => array(),
        'type' => 'mm_fields_link_page',
        'weight' => 11,
      ),
      'teaser' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 11,
      ),
    ),
    'entity_type' => 'node',
    'field_name' => 'field_mm_groups',
    'label' => 'Group(s):',
    'required' => FALSE,
    'settings' => array(
      'user_register_form' => FALSE,
    ),
    'weight' => 11,
    'widget' => array(
      'active' => 1,
      'module' => 'mm_fields',
      'settings' => array(
        'default_value' => NULL,
        'default_value_php' => NULL,
        'mm_cck_default_modes' => array(),
        'mm_cck_groups' => array(),
        'mm_cck_use_defaults' => TRUE,
        'mm_cck_users' => array(),
        'mm_list_max' => 0,
        'mm_list_min' => 0,
        'mm_list_popup_start' => array(),
        'mm_list_readonly' => FALSE,
        'mm_list_show_info' => TRUE,
      ),
      'type' => 'mm_grouplist',
      'weight' => 11,
    ),
    'widget_type' => 'mm_grouplist',
  );

  // Exported field_instance: 'node-mm_calendar-field_mm_pages'
  $field_instances['node-mm_calendar-field_mm_pages'] = array(
    'bundle' => 'mm_calendar',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => '',
    'display' => array(
      'default' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 17,
      ),
      'full' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 6,
      ),
      'ical' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
      'rss' => array(
        'label' => 'above',
        'module' => 'mm_fields',
        'settings' => array(),
        'type' => 'mm_fields_link_page',
        'weight' => 6,
      ),
      'teaser' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 6,
      ),
    ),
    'entity_type' => 'node',
    'field_name' => 'field_mm_pages',
    'label' => 'All events on these page(s):',
    'required' => 0,
    'settings' => array(
      'file_extensions' => '',
      'user_register_form' => FALSE,
    ),
    'weight' => 6,
    'widget' => array(
      'active' => 1,
      'module' => 'mm_fields',
      'settings' => array(
        'advanced' => array(
          'mm_list_enabled' => 'r',
          'mm_list_max' => 0,
          'mm_list_min' => 0,
          'mm_list_popup_start' => array(),
          'mm_list_selectable' => 'u',
          'mm_list_show_info' => 1,
        ),
        'mm_cck_default_modes' => array(),
        'mm_cck_groups' => array(),
        'mm_cck_use_defaults' => 1,
        'mm_cck_users' => array(),
        'mm_list_enabled' => 'r',
        'mm_list_max' => 0,
        'mm_list_min' => 0,
        'mm_list_popup_start' => array(),
        'mm_list_readonly' => 0,
        'mm_list_selectable' => 'u',
        'mm_list_show_info' => TRUE,
      ),
      'type' => 'mm_catlist',
      'weight' => 6,
    ),
    'widget_type' => 'mm_catlist',
  );

  // Exported field_instance: 'node-mm_calendar-field_show_export'
  $field_instances['node-mm_calendar-field_show_export'] = array(
    'bundle' => 'mm_calendar',
    'default_value' => array(
      0 => array(
        'value' => 1,
      ),
    ),
    'deleted' => 0,
    'description' => '',
    'display' => array(
      'default' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 5,
      ),
      'full' => array(
        'label' => 'above',
        'module' => 'number',
        'settings' => array(
          'decimal_separator' => '.',
          'prefix_suffix' => TRUE,
          'scale' => 0,
          'thousand_separator' => '',
        ),
        'type' => 'number_integer',
        'weight' => 4,
      ),
      'ical' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
      'rss' => array(
        'label' => 'above',
        'module' => 'number',
        'settings' => array(
          'decimal_separator' => '.',
          'prefix_suffix' => TRUE,
          'scale' => 0,
          'thousand_separator' => '',
        ),
        'type' => 'number_integer',
        'weight' => 4,
      ),
      'teaser' => array(
        'label' => 'above',
        'module' => 'list',
        'settings' => array(),
        'type' => 'list_default',
        'weight' => 4,
      ),
    ),
    'entity_type' => 'node',
    'field_name' => 'field_show_export',
    'label' => 'Show the Export link',
    'required' => FALSE,
    'settings' => array(
      'max' => '',
      'min' => '',
      'prefix' => '',
      'suffix' => '',
      'user_register_form' => FALSE,
    ),
    'weight' => 4,
    'widget' => array(
      'active' => 1,
      'module' => 'options',
      'settings' => array(
        'default_value_php' => NULL,
        'display_label' => 0,
        'mm_cck_default_modes' => array(),
        'mm_cck_groups' => array(),
        'mm_cck_use_defaults' => TRUE,
        'mm_cck_users' => array(),
      ),
      'type' => 'options_onoff',
      'weight' => 24,
    ),
    'widget_type' => 'optionwidgets_onoff',
  );

  // Exported field_instance: 'node-mm_calendar-field_upcoming_items'
  $field_instances['node-mm_calendar-field_upcoming_items'] = array(
    'bundle' => 'mm_calendar',
    'default_value' => array(
      0 => array(
        'value' => 10,
      ),
    ),
    'default_value_function' => '',
    'default_value_php' => '',
    'deleted' => 0,
    'description' => 'The number of events that will be shown when the page first loads. The user can click on a "more" link to show additional events.',
    'display' => array(
      'default' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 6,
      ),
    ),
    'entity_type' => 'node',
    'field_name' => 'field_upcoming_items',
    'label' => 'Events in list',
    'required' => 1,
    'settings' => array(
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'active' => 1,
      'module' => 'options',
      'settings' => array(
        'mm_cck_default_modes' => array(),
        'mm_cck_groups' => array(),
        'mm_cck_use_defaults' => 1,
        'mm_cck_users' => array(),
      ),
      'type' => 'options_select',
      'weight' => 16,
    ),
  );

  // Exported field_instance: 'node-mm_calendar-field_wd_end'
  $field_instances['node-mm_calendar-field_wd_end'] = array(
    'bundle' => 'mm_calendar',
    'default_value' => array(
      0 => array(
        'value' => 1020,
      ),
    ),
    'default_value_function' => '',
    'default_value_php' => '',
    'deleted' => 0,
    'description' => '',
    'display' => array(
      'default' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 1,
      ),
      'full' => array(
        'label' => 'above',
        'module' => 'number',
        'settings' => array(
          'decimal_separator' => '.',
          'prefix_suffix' => TRUE,
          'scale' => 0,
          'thousand_separator' => '',
        ),
        'type' => 'number_integer',
        'weight' => 1,
      ),
      'ical' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
      'rss' => array(
        'label' => 'above',
        'module' => 'number',
        'settings' => array(
          'decimal_separator' => '.',
          'prefix_suffix' => TRUE,
          'scale' => 0,
          'thousand_separator' => '',
        ),
        'type' => 'number_integer',
        'weight' => 1,
      ),
      'teaser' => array(
        'label' => 'above',
        'module' => 'list',
        'settings' => array(),
        'type' => 'list_default',
        'weight' => 1,
      ),
    ),
    'entity_type' => 'node',
    'field_name' => 'field_wd_end',
    'label' => 'End time',
    'required' => 1,
    'settings' => array(
      'file_extensions' => '',
      'user_register_form' => FALSE,
    ),
    'weight' => 1,
    'widget' => array(
      'active' => 1,
      'module' => 'options',
      'settings' => array(
        'mm_cck_default_modes' => array(),
        'mm_cck_groups' => array(),
        'mm_cck_use_defaults' => 1,
        'mm_cck_users' => array(),
      ),
      'type' => 'options_select',
      'weight' => 1,
    ),
    'widget_type' => 'optionwidgets_select',
  );

  // Exported field_instance: 'node-mm_calendar-field_wd_inc_maj'
  $field_instances['node-mm_calendar-field_wd_inc_maj'] = array(
    'bundle' => 'mm_calendar',
    'default_value' => array(
      0 => array(
        'value' => 'hour',
      ),
    ),
    'default_value_function' => '',
    'default_value_php' => '',
    'deleted' => 0,
    'description' => '',
    'display' => array(
      'default' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 2,
      ),
      'ical' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
      'teaser' => array(
        'label' => 'above',
        'module' => 'list',
        'settings' => array(),
        'type' => 'list_default',
        'weight' => 2,
      ),
    ),
    'entity_type' => 'node',
    'field_name' => 'field_wd_inc_maj',
    'label' => 'Major time increment',
    'required' => 1,
    'settings' => array(
      'file_extensions' => '',
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'active' => 1,
      'module' => 'options',
      'settings' => array(
        'mm_cck_default_modes' => array(),
        'mm_cck_groups' => array(),
        'mm_cck_use_defaults' => 1,
        'mm_cck_users' => array(),
      ),
      'type' => 'options_select',
      'weight' => 2,
    ),
  );

  // Exported field_instance: 'node-mm_calendar-field_wd_increment'
  $field_instances['node-mm_calendar-field_wd_increment'] = array(
    'bundle' => 'mm_calendar',
    'default_value' => array(
      0 => array(
        'value' => 30,
      ),
    ),
    'default_value_function' => '',
    'default_value_php' => '',
    'deleted' => 0,
    'description' => '',
    'display' => array(
      'default' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 3,
      ),
      'full' => array(
        'label' => 'above',
        'module' => 'number',
        'settings' => array(
          'decimal_separator' => '.',
          'prefix_suffix' => TRUE,
          'scale' => 0,
          'thousand_separator' => '',
        ),
        'type' => 'number_integer',
        'weight' => 2,
      ),
      'ical' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
      'rss' => array(
        'label' => 'above',
        'module' => 'number',
        'settings' => array(
          'decimal_separator' => '.',
          'prefix_suffix' => TRUE,
          'scale' => 0,
          'thousand_separator' => '',
        ),
        'type' => 'number_integer',
        'weight' => 2,
      ),
      'teaser' => array(
        'label' => 'above',
        'module' => 'list',
        'settings' => array(),
        'type' => 'list_default',
        'weight' => 3,
      ),
    ),
    'entity_type' => 'node',
    'field_name' => 'field_wd_increment',
    'label' => 'Minor time increment',
    'required' => 1,
    'settings' => array(
      'file_extensions' => '',
      'user_register_form' => FALSE,
    ),
    'weight' => 2,
    'widget' => array(
      'active' => 1,
      'module' => 'options',
      'settings' => array(
        'mm_cck_default_modes' => array(),
        'mm_cck_groups' => array(),
        'mm_cck_use_defaults' => 1,
        'mm_cck_users' => array(),
      ),
      'type' => 'options_select',
      'weight' => 3,
    ),
    'widget_type' => 'optionwidgets_select',
  );

  // Exported field_instance: 'node-mm_calendar-field_wd_start'
  $field_instances['node-mm_calendar-field_wd_start'] = array(
    'bundle' => 'mm_calendar',
    'default_value' => array(
      0 => array(
        'value' => 540,
      ),
    ),
    'default_value_function' => '',
    'default_value_php' => '',
    'deleted' => 0,
    'description' => '',
    'display' => array(
      'default' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
      'full' => array(
        'label' => 'above',
        'module' => 'number',
        'settings' => array(
          'decimal_separator' => '.',
          'prefix_suffix' => TRUE,
          'scale' => 0,
          'thousand_separator' => '',
        ),
        'type' => 'number_integer',
        'weight' => 0,
      ),
      'ical' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
      'rss' => array(
        'label' => 'above',
        'module' => 'number',
        'settings' => array(
          'decimal_separator' => '.',
          'prefix_suffix' => TRUE,
          'scale' => 0,
          'thousand_separator' => '',
        ),
        'type' => 'number_integer',
        'weight' => 0,
      ),
      'teaser' => array(
        'label' => 'above',
        'module' => 'list',
        'settings' => array(),
        'type' => 'list_default',
        'weight' => 0,
      ),
    ),
    'entity_type' => 'node',
    'field_name' => 'field_wd_start',
    'label' => 'Start time',
    'required' => 1,
    'settings' => array(
      'file_extensions' => '',
      'user_register_form' => FALSE,
    ),
    'weight' => 0,
    'widget' => array(
      'active' => 1,
      'module' => 'options',
      'settings' => array(
        'mm_cck_default_modes' => array(),
        'mm_cck_groups' => array(),
        'mm_cck_use_defaults' => 1,
        'mm_cck_users' => array(),
      ),
      'type' => 'options_select',
      'weight' => 0,
    ),
    'widget_type' => 'optionwidgets_select',
  );

  // Exported field_instance: 'node-mm_event-body'
  $field_instances['node-mm_event-body'] = array(
    'bundle' => 'mm_event',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => '',
    'display' => array(
      'default' => array(
        'label' => 'hidden',
        'module' => 'text',
        'settings' => array(),
        'type' => 'text_default',
        'weight' => 0,
      ),
      'ical' => array(
        'label' => 'hidden',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
      'search_index' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
      'teaser' => array(
        'label' => 'hidden',
        'module' => 'text',
        'settings' => array(
          'trim_length' => 600,
        ),
        'trim_length' => 600,
        'type' => 'text_summary_or_trimmed',
        'weight' => 0,
      ),
      'upcoming_events' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
    ),
    'entity_type' => 'node',
    'field_name' => 'body',
    'label' => 'Description',
    'required' => FALSE,
    'settings' => array(
      'display_summary' => TRUE,
      'text_processing' => 1,
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'module' => 'text',
      'settings' => array(
        'mm_cck_default_modes' => array(),
        'mm_cck_groups' => array(),
        'mm_cck_use_defaults' => TRUE,
        'mm_cck_users' => array(),
        'rows' => 20,
        'summary_rows' => 5,
      ),
      'type' => 'text_textarea_with_summary',
      'weight' => 0,
    ),
  );

  // Exported field_instance: 'node-mm_event-field_start_datetime'
  $field_instances['node-mm_event-field_start_datetime'] = array(
    'bundle' => 'mm_event',
    'deleted' => 0,
    'description' => '',
    'display' => array(
      'default' => array(
        'label' => 'hidden',
        'module' => 'date',
        'settings' => array(
          'format_type' => 'field_start_datetime_default',
          'fromto' => 'both',
          'multiple_from' => '',
          'multiple_number' => '',
          'multiple_to' => '',
          'show_repeat_rule' => 'show',
        ),
        'type' => 'date_default',
        'weight' => 1,
      ),
      'full' => array(
        'label' => 'hidden',
        'module' => 'date',
        'settings' => array(
          'format_type' => 'field_start_datetime_default',
          'fromto' => 'both',
          'multiple_from' => '',
          'multiple_number' => '',
          'multiple_to' => '',
          'show_repeat_rule' => 'show',
        ),
        'type' => 'date_default',
        'weight' => 1,
      ),
      'ical' => array(
        'label' => 'hidden',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 1,
      ),
      'rss' => array(
        'label' => 'hidden',
        'module' => 'date',
        'settings' => array(
          'format_type' => 'field_start_datetime_default',
          'fromto' => 'both',
          'multiple_from' => '',
          'multiple_number' => '',
          'multiple_to' => '',
          'show_repeat_rule' => 'show',
        ),
        'type' => 'date_default',
        'weight' => 1,
      ),
      'search_index' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
      'teaser' => array(
        'label' => 'hidden',
        'module' => 'date',
        'settings' => array(
          'format_type' => 'field_start_datetime_default',
          'fromto' => 'both',
          'multiple_from' => '',
          'multiple_number' => '',
          'multiple_to' => '',
          'show_repeat_rule' => 'show',
        ),
        'type' => 'date_default',
        'weight' => 1,
      ),
      'upcoming_events' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
    ),
    'entity_type' => 'node',
    'field_name' => 'field_start_datetime',
    'label' => 'Date/time',
    'required' => 0,
    'settings' => array(
      'default_value' => 'now',
      'default_value2' => 'same',
      'default_value_code' => '',
      'default_value_code2' => '',
      'sequence' => array(
        'active' => 1,
        'edit_default' => 'all',
        'modal' => 1,
        'show_edit' => 1,
      ),
      'user_register_form' => FALSE,
    ),
    'weight' => 1,
    'widget' => array(
      'active' => 1,
      'module' => 'date',
      'settings' => array(
        'display_all_day' => 1,
        'increment' => 5,
        'input_format' => 'm/d/Y - g:i:sa',
        'input_format_custom' => '',
        'label_position' => 'above',
        'mm_cck_default_modes' => array(),
        'mm_cck_groups' => array(),
        'mm_cck_use_defaults' => 1,
        'mm_cck_users' => array(),
        'repeat_collapsed' => 1,
        'text_parts' => array(),
        'year_range' => '-0:+5',
      ),
      'type' => 'date_popup',
      'weight' => 1,
    ),
    'widget_type' => 'date_popup_repeat',
  );

  // Exported field_instance: 'node-mm_event-upload'
  $field_instances['node-mm_event-upload'] = array(
    'bundle' => 'mm_event',
    'deleted' => 0,
    'description' => '',
    'display' => array(
      'default' => array(
        'label' => 'hidden',
        'module' => 'file',
        'settings' => array(),
        'type' => 'file_table',
        'weight' => 0,
      ),
      'full' => array(
        'label' => 'hidden',
        'module' => 'file',
        'settings' => array(),
        'type' => 'file_table',
        'weight' => 0,
      ),
      'ical' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 2,
      ),
      'rss' => array(
        'label' => 'hidden',
        'module' => 'file',
        'settings' => array(),
        'type' => 'file_table',
        'weight' => 0,
      ),
      'search_index' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
      'teaser' => array(
        'label' => 'hidden',
        'module' => NULL,
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
      'upcoming_events' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
    ),
    'entity_type' => 'node',
    'field_name' => 'upload',
    'label' => 'File attachments',
    'required' => 0,
    'settings' => array(
      'description_field' => 1,
      'file_directory' => '',
      'file_extensions' => 'aif aiff avi csv dcr doc docx flv gif gz html jp2 jpeg jpg kml kmz mov mp3 mp4 mpeg mpg mxd nb pdb pdf pmf png ppt pptx rtf sit swf tar txt wav webarchive wma wmv wpd wpf xls xlsx zip',
      'max_filesize' => '1 MB',
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'module' => 'file',
      'settings' => array(
        'mm_cck_default_modes' => array(),
        'mm_cck_groups' => array(),
        'mm_cck_use_defaults' => TRUE,
        'mm_cck_users' => array(),
        'progress_indicator' => 'throbber',
      ),
      'type' => 'file_generic',
      'weight' => 1,
    ),
  );

  // Translatables
  // Included for use with string extractors like potx.
  t('All events on these page(s):');
  t('Allow user to change period and display mode');
  t('Body');
  t('Choose the starting month for year views. Use September if your calendar should be displayed based on the school year.');
  t('Date/time');
  t('Default Upcoming/List/Grid mode');
  t('Default period');
  t('Description');
  t('End time');
  t('Events in list');
  t('File attachments');
  t('First month of the year');
  t('Group(s):');
  t('Individual events');
  t('Major time increment');
  t('Members of the selected group(s) will see events from this calendar on their personal "My Calendar" page.');
  t('Minor time increment');
  t('Show the Export link');
  t('Start time');
  t('The number of events that will be shown when the page first loads. The user can click on a "more" link to show additional events.');
  t('To add an event, type part of its title, then choose it from the resulting list.');

  return $field_instances;
}
