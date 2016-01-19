<?php
/**
 * @file
 * mm_schedule.field_group.inc
 */

/**
 * Implements hook_field_group_info().
 */
function mm_schedule_field_group_info() {
  $export = array();

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_appearance|node|mm_calendar|default';
  $field_group->group_name = 'group_appearance';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'mm_calendar';
  $field_group->mode = 'default';
  $field_group->parent_name = '';
  $field_group->data = array(
    'label' => 'Appearance',
    'weight' => '2',
    'children' => array(
      0 => 'field_show_export',
      1 => 'field_default_view',
      2 => 'field_default_display',
      3 => 'field_allow_mode_change',
      4 => 'field_upcoming_items',
    ),
    'format_type' => 'hidden',
    'format_settings' => array(
      'formatter' => 'collapsible',
      'instance_settings' => array(),
    ),
  );
  $export['group_appearance|node|mm_calendar|default'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_appearance|node|mm_calendar|form';
  $field_group->group_name = 'group_appearance';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'mm_calendar';
  $field_group->mode = 'form';
  $field_group->parent_name = '';
  $field_group->data = array(
    'label' => 'Appearance',
    'weight' => '-1',
    'children' => array(
      0 => 'field_show_export',
      1 => 'field_default_view',
      2 => 'field_default_display',
      3 => 'field_allow_mode_change',
      4 => 'group_week_day',
      5 => 'group_year',
      6 => 'group_upcoming',
    ),
    'format_type' => 'fieldset',
    'format_settings' => array(
      'formatter' => 'collapsible',
      'instance_settings' => array(),
    ),
  );
  $export['group_appearance|node|mm_calendar|form'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_appearance|node|mm_calendar|teaser';
  $field_group->group_name = 'group_appearance';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'mm_calendar';
  $field_group->mode = 'teaser';
  $field_group->parent_name = '';
  $field_group->data = array(
    'label' => 'Appearance',
    'weight' => '-1',
    'children' => array(
      0 => 'field_show_export',
      1 => 'field_default_view',
      2 => 'field_default_display',
      3 => 'field_allow_mode_change',
    ),
    'format_type' => 'fieldset',
    'format_settings' => array(
      'formatter' => 'collapsible',
      'instance_settings' => array(),
    ),
  );
  $export['group_appearance|node|mm_calendar|teaser'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_events|node|mm_calendar|default';
  $field_group->group_name = 'group_events';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'mm_calendar';
  $field_group->mode = 'default';
  $field_group->parent_name = '';
  $field_group->data = array(
    'label' => 'Events',
    'weight' => '1',
    'children' => array(
      0 => 'field_mm_events',
      1 => 'field_mm_pages',
    ),
    'format_type' => 'hidden',
    'format_settings' => array(
      'formatter' => 'collapsible',
      'instance_settings' => array(),
    ),
  );
  $export['group_events|node|mm_calendar|default'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_events|node|mm_calendar|form';
  $field_group->group_name = 'group_events';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'mm_calendar';
  $field_group->mode = 'form';
  $field_group->parent_name = '';
  $field_group->data = array(
    'label' => 'Events',
    'weight' => '-2',
    'children' => array(
      0 => 'field_mm_events',
      1 => 'field_mm_pages',
    ),
    'format_type' => 'fieldset',
    'format_settings' => array(
      'formatter' => 'collapsible',
      'instance_settings' => array(),
    ),
  );
  $export['group_events|node|mm_calendar|form'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_events|node|mm_calendar|teaser';
  $field_group->group_name = 'group_events';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'mm_calendar';
  $field_group->mode = 'teaser';
  $field_group->parent_name = '';
  $field_group->data = array(
    'label' => 'Events',
    'weight' => '-2',
    'children' => array(
      0 => 'field_mm_events',
      1 => 'field_mm_pages',
    ),
    'format_type' => 'fieldset',
    'format_settings' => array(
      'formatter' => 'collapsible',
      'instance_settings' => array(),
    ),
  );
  $export['group_events|node|mm_calendar|teaser'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_push|node|mm_calendar|default';
  $field_group->group_name = 'group_push';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'mm_calendar';
  $field_group->mode = 'default';
  $field_group->parent_name = '';
  $field_group->data = array(
    'label' => 'Push to Personal Calendars',
    'weight' => '3',
    'children' => array(
      0 => 'field_mm_groups',
    ),
    'format_type' => 'hidden',
    'format_settings' => array(
      'formatter' => '',
      'instance_settings' => array(),
    ),
  );
  $export['group_push|node|mm_calendar|default'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_push|node|mm_calendar|form';
  $field_group->group_name = 'group_push';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'mm_calendar';
  $field_group->mode = 'form';
  $field_group->parent_name = '';
  $field_group->data = array(
    'label' => 'Push to personal calendars',
    'weight' => '0',
    'children' => array(
      0 => 'field_mm_groups',
    ),
    'format_type' => 'fieldset',
    'format_settings' => array(
      'label' => 'Push to personal calendars',
      'instance_settings' => array(
        'required_fields' => 1,
        'classes' => '',
        'description' => '',
      ),
      'formatter' => 'collapsible',
    ),
  );
  $export['group_push|node|mm_calendar|form'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_push|node|mm_calendar|teaser';
  $field_group->group_name = 'group_push';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'mm_calendar';
  $field_group->mode = 'teaser';
  $field_group->parent_name = '';
  $field_group->data = array(
    'label' => 'Push to Personal Calendars',
    'weight' => '0',
    'children' => array(
      0 => 'field_mm_groups',
    ),
    'format_type' => 'fieldset',
    'format_settings' => array(
      'formatter' => 'collapsible',
      'instance_settings' => array(),
    ),
  );
  $export['group_push|node|mm_calendar|teaser'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_upcoming|node|mm_calendar|form';
  $field_group->group_name = 'group_upcoming';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'mm_calendar';
  $field_group->mode = 'form';
  $field_group->parent_name = 'group_appearance';
  $field_group->data = array(
    'label' => 'Upcoming settings',
    'weight' => '25',
    'children' => array(
      0 => 'field_upcoming_items',
    ),
    'format_type' => 'fieldset',
    'format_settings' => array(
      'label' => 'Upcoming settings',
      'instance_settings' => array(
        'required_fields' => 1,
        'classes' => '',
        'description' => '',
      ),
      'formatter' => 'collapsed',
    ),
  );
  $export['group_upcoming|node|mm_calendar|form'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_week_day|node|mm_calendar|default';
  $field_group->group_name = 'group_week_day';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'mm_calendar';
  $field_group->mode = 'default';
  $field_group->parent_name = '';
  $field_group->data = array(
    'label' => 'Week/Day Grid Settings',
    'weight' => '4',
    'children' => array(
      0 => 'field_wd_end',
      1 => 'field_wd_start',
      2 => 'field_wd_increment',
      3 => 'field_wd_inc_maj',
    ),
    'format_type' => 'hidden',
    'format_settings' => array(
      'formatter' => 'collapsible',
      'instance_settings' => array(),
    ),
  );
  $export['group_week_day|node|mm_calendar|default'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_week_day|node|mm_calendar|form';
  $field_group->group_name = 'group_week_day';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'mm_calendar';
  $field_group->mode = 'form';
  $field_group->parent_name = 'group_appearance';
  $field_group->data = array(
    'label' => 'Week/Day grid settings',
    'weight' => '26',
    'children' => array(
      0 => 'field_wd_end',
      1 => 'field_wd_start',
      2 => 'field_wd_increment',
      3 => 'field_wd_inc_maj',
    ),
    'format_type' => 'fieldset',
    'format_settings' => array(
      'label' => 'Week/Day grid settings',
      'instance_settings' => array(
        'required_fields' => 1,
        'classes' => '',
        'description' => '',
      ),
      'formatter' => 'collapsed',
    ),
  );
  $export['group_week_day|node|mm_calendar|form'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_week_day|node|mm_calendar|teaser';
  $field_group->group_name = 'group_week_day';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'mm_calendar';
  $field_group->mode = 'teaser';
  $field_group->parent_name = '';
  $field_group->data = array(
    'label' => 'Week/Day Grid Settings',
    'weight' => '1',
    'children' => array(
      0 => 'field_wd_end',
      1 => 'field_wd_start',
      2 => 'field_wd_increment',
      3 => 'field_wd_inc_maj',
    ),
    'format_type' => 'fieldset',
    'format_settings' => array(
      'formatter' => 'collapsible',
      'instance_settings' => array(),
    ),
  );
  $export['group_week_day|node|mm_calendar|teaser'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_year|node|mm_calendar|default';
  $field_group->group_name = 'group_year';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'mm_calendar';
  $field_group->mode = 'default';
  $field_group->parent_name = '';
  $field_group->data = array(
    'label' => 'Year Settings',
    'weight' => '5',
    'children' => array(
      0 => 'field_first_month',
    ),
    'format_type' => 'hidden',
    'format_settings' => array(
      'formatter' => 'collapsible',
      'instance_settings' => array(),
    ),
  );
  $export['group_year|node|mm_calendar|default'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_year|node|mm_calendar|form';
  $field_group->group_name = 'group_year';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'mm_calendar';
  $field_group->mode = 'form';
  $field_group->parent_name = 'group_appearance';
  $field_group->data = array(
    'label' => 'Year settings',
    'weight' => '27',
    'children' => array(
      0 => 'field_first_month',
    ),
    'format_type' => 'fieldset',
    'format_settings' => array(
      'label' => 'Year settings',
      'instance_settings' => array(
        'required_fields' => 1,
        'classes' => '',
        'description' => '',
      ),
      'formatter' => 'collapsed',
    ),
  );
  $export['group_year|node|mm_calendar|form'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_year|node|mm_calendar|teaser';
  $field_group->group_name = 'group_year';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'mm_calendar';
  $field_group->mode = 'teaser';
  $field_group->parent_name = '';
  $field_group->data = array(
    'label' => 'Year Settings',
    'weight' => '14',
    'children' => array(
      0 => 'field_first_month',
    ),
    'format_type' => 'fieldset',
    'format_settings' => array(
      'formatter' => 'collapsible',
      'instance_settings' => array(),
    ),
  );
  $export['group_year|node|mm_calendar|teaser'] = $field_group;

  return $export;
}
