<?php

/**
 * @file
 * Install cck types for agreservationsmodule.
 */

/**
 * @todo Documentation
 */
function _agres_categories_cck_unittype_category() {

  $content['type'] = array(
    'name' => 'Unit type category',
    'type' => 'agres_category',
    'description' => 'Create categories like: cars, rooms, tables and asign them to unittypes.
Additionaly select Properties like, hourly booking, daily booking...',
    'title_label' => 'Title',
    'body_label' => 'Body',
    'min_word_count' => '0',
    'help' => '',
    'node_options' => array(
      'status' => TRUE,
      'promote' => FALSE,
      'sticky' => FALSE,
      'revision' => FALSE,
    ),
    'language_content_type' => '0',
    'old_type' => 'agres_category',
    'orig_type' => '',
    'module' => 'node',
    'custom' => '1',
    'modified' => '1',
    'locked' => '0',
    'i18n_newnode_current' => 0,
    'i18n_required_node' => 0,
    'i18n_lock_node' => 0,
    'i18n_node' => 1,
  );
  $content['fields'] = array(
    0 => array(
      'label' => 'Booking mode',
      'field_name' => 'field_agres_bookingmode',
      'type' => 'text',
      'widget_type' => 'optionwidgets_select',
      'change' => 'Change basic information',
      'weight' => '2',
      'description' => '',
      'default_value' => array(
        0 => array(
          'value' => '',
        ),
      ),
      'default_value_php' => '',
      'default_value_widget' => NULL,
      'required' => 1,
      'multiple' => '0',
      'text_processing' => '0',
      'max_length' => '',
      'allowed_values' => '1|booking hour
2|booking days',
      'allowed_values_php' => '',
      'op' => 'Save field settings',
      'module' => 'text',
      'widget_module' => 'optionwidgets',
      'columns' => array(
        'value' => array(
          'type' => 'text',
          'size' => 'big',
          'not null' => FALSE,
          'sortable' => TRUE,
          'views' => TRUE,
        ),
      ),
      'display_settings' => array(
        'label' => array(
          'format' => 'above',
          'exclude' => 0,
        ),
        'teaser' => array(
          'format' => 'default',
          'exclude' => 0,
        ),
        'full' => array(
          'format' => 'default',
          'exclude' => 0,
        ),
        4 => array(
          'format' => 'default',
          'exclude' => 0,
        ),
        'token' => array(
          'format' => 'default',
          'exclude' => 0,
        ),
      ),
    ),
    1 => array(
      'label' => 'Unit selection method',
      'field_name' => 'field_agres_unitselectionmethod',
      'type' => 'text',
      'widget_type' => 'optionwidgets_select',
      'change' => 'Change basic information',
      'weight' => '3',
      'description' => '',
      'default_value' => array(
        0 => array(
          'value' => '',
        ),
      ),
      'default_value_php' => '',
      'default_value_widget' => array(
        'field_agres_unitselectionmethod' => array(
          'value' => '',
        ),
      ),
      'required' => 0,
      'multiple' => '0',
      'text_processing' => '0',
      'max_length' => '',
      'allowed_values' => '1|standard
2|ubercart productlist',
      'allowed_values_php' => '',
      'op' => 'Save field settings',
      'module' => 'text',
      'widget_module' => 'optionwidgets',
      'columns' => array(
        'value' => array(
          'type' => 'text',
          'size' => 'big',
          'not null' => FALSE,
          'sortable' => TRUE,
          'views' => TRUE,
        ),
      ),
      'display_settings' => array(
        'label' => array(
          'format' => 'above',
          'exclude' => 0,
        ),
        'teaser' => array(
          'format' => 'default',
          'exclude' => 0,
        ),
        'full' => array(
          'format' => 'default',
          'exclude' => 0,
        ),
        4 => array(
          'format' => 'default',
          'exclude' => 0,
        ),
        'token' => array(
          'format' => 'default',
          'exclude' => 0,
        ),
      ),
    ),
    2 => array(
      'label' => 'Label of date1',
      'field_name' => 'field_agres_datelabel1',
      'type' => 'text',
      'widget_type' => 'text_textfield',
      'change' => 'Change basic information',
      'weight' => '4',
      'rows' => 5,
      'size' => '60',
      'description' => '',
      'default_value' => array(
        0 => array(
          'value' => '',
          'format' => '1',
          '_error_element' => 'default_value_widget][field_agres_datelabel1][0][value',
        ),
      ),
      'default_value_php' => '',
      'default_value_widget' => array(
        'field_agres_datelabel1' => array(
          0 => array(
            'value' => '',
            'format' => '1',
            '_error_element' => 'default_value_widget][field_agres_datelabel1][0][value',
          ),
        ),
      ),
      'required' => 0,
      'multiple' => '0',
      'text_processing' => '1',
      'max_length' => '',
      'allowed_values' => '',
      'allowed_values_php' => '',
      'op' => 'Save field settings',
      'module' => 'text',
      'widget_module' => 'text',
      'columns' => array(
        'value' => array(
          'type' => 'text',
          'size' => 'big',
          'not null' => FALSE,
          'sortable' => TRUE,
          'views' => TRUE,
        ),
        'format' => array(
          'type' => 'int',
          'unsigned' => TRUE,
          'not null' => FALSE,
          'views' => FALSE,
        ),
      ),
      'display_settings' => array(
        'weight' => '32',
        'parent' => '',
        'label' => array(
          'format' => 'above',
        ),
        'teaser' => array(
          'format' => 'default',
          'exclude' => 0,
        ),
        'full' => array(
          'format' => 'default',
          'exclude' => 0,
        ),
        4 => array(
          'format' => 'default',
          'exclude' => 0,
        ),
        'token' => array(
          'format' => 'default',
          'exclude' => 0,
        ),
      ),
    ),
    3 => array(
      'label' => 'Label of date2',
      'field_name' => 'field_agres_datelabel2',
      'type' => 'text',
      'widget_type' => 'text_textfield',
      'change' => 'Change basic information',
      'weight' => '5',
      'rows' => 5,
      'size' => '60',
      'description' => 'if you leave empty this entry field the to date will not be displayed.',
      'default_value' => array(
        0 => array(
          'value' => '',
          'format' => '1',
          '_error_element' => 'default_value_widget][field_agres_datelabel2][0][value',
        ),
      ),
      'default_value_php' => '',
      'default_value_widget' => array(
        'field_agres_datelabel2' => array(
          0 => array(
            'value' => '',
            'format' => '1',
            '_error_element' => 'default_value_widget][field_agres_datelabel2][0][value',
          ),
        ),
      ),
      'required' => 0,
      'multiple' => '0',
      'text_processing' => '1',
      'max_length' => '',
      'allowed_values' => '',
      'allowed_values_php' => '',
      'op' => 'Save field settings',
      'module' => 'text',
      'widget_module' => 'text',
      'columns' => array(
        'value' => array(
          'type' => 'text',
          'size' => 'big',
          'not null' => FALSE,
          'sortable' => TRUE,
          'views' => TRUE,
        ),
        'format' => array(
          'type' => 'int',
          'unsigned' => TRUE,
          'not null' => FALSE,
          'views' => FALSE,
        ),
      ),
      'display_settings' => array(
        'weight' => '33',
        'parent' => '',
        'label' => array(
          'format' => 'above',
        ),
        'teaser' => array(
          'format' => 'default',
          'exclude' => 0,
        ),
        'full' => array(
          'format' => 'default',
          'exclude' => 0,
        ),
        4 => array(
          'format' => 'default',
          'exclude' => 0,
        ),
        'token' => array(
          'format' => 'default',
          'exclude' => 0,
        ),
      ),
    ),
    4 => array(
      'label' => 'Select times too',
      'field_name' => 'field_agres_showtime',
      'type' => 'number_integer',
      'widget_type' => 'optionwidgets_buttons',
      'change' => 'Change basic information',
      'weight' => '6',
      'description' => '',
      'default_value' => array(
        0 => array(
          'value' => '1',
        ),
      ),
      'default_value_php' => '',
      'default_value_widget' => array(
        'field_agres_showtime' => array(
          'value' => '1',
        ),
      ),
      'required' => 0,
      'multiple' => '0',
      'min' => '',
      'max' => '',
      'prefix' => '',
      'suffix' => '',
      'allowed_values' => '0|off
1|on
',
      'allowed_values_php' => '',
      'op' => 'Save field settings',
      'module' => 'number',
      'widget_module' => 'optionwidgets',
      'columns' => array(
        'value' => array(
          'type' => 'int',
          'not null' => FALSE,
          'sortable' => TRUE,
        ),
      ),
      'display_settings' => array(
        'weight' => '34',
        'parent' => '',
        'label' => array(
          'format' => 'inline',
        ),
        'teaser' => array(
          'format' => 'default',
          'exclude' => 0,
        ),
        'full' => array(
          'format' => 'default',
          'exclude' => 0,
        ),
        4 => array(
          'format' => 'default',
          'exclude' => 0,
        ),
        'token' => array(
          'format' => 'default',
          'exclude' => 0,
        ),
      ),
    ),
    5 => array(
      'label' => 'Title of reservation form',
      'field_name' => 'field_agres_titleresform',
      'type' => 'text',
      'widget_type' => 'text_textfield',
      'change' => 'Change basic information',
      'weight' => '7',
      'rows' => 5,
      'size' => '60',
      'description' => '',
      'default_value' => array(
        0 => array(
          'value' => '',
          'format' => '2',
          '_error_element' => 'default_value_widget][field_agres_titleresform][0][value',
        ),
      ),
      'default_value_php' => '',
      'default_value_widget' => array(
        'field_agres_titleresform' => array(
          0 => array(
            'value' => '',
            'format' => '2',
            '_error_element' => 'default_value_widget][field_agres_titleresform][0][value',
          ),
        ),
      ),
      'required' => 0,
      'multiple' => '0',
      'text_processing' => '1',
      'max_length' => '',
      'allowed_values' => '',
      'allowed_values_php' => '',
      'op' => 'Save field settings',
      'module' => 'text',
      'widget_module' => 'text',
      'columns' => array(
        'value' => array(
          'type' => 'text',
          'size' => 'big',
          'not null' => FALSE,
          'sortable' => TRUE,
          'views' => TRUE,
        ),
        'format' => array(
          'type' => 'int',
          'unsigned' => TRUE,
          'not null' => FALSE,
          'views' => FALSE,
        ),
      ),
      'display_settings' => array(
        'label' => array(
          'format' => 'above',
          'exclude' => 0,
        ),
        'teaser' => array(
          'format' => 'default',
          'exclude' => 0,
        ),
        'full' => array(
          'format' => 'default',
          'exclude' => 0,
        ),
        4 => array(
          'format' => 'default',
          'exclude' => 0,
        ),
        'token' => array(
          'format' => 'default',
          'exclude' => 0,
        ),
      ),
    ),
    6 => array(
      'label' => 'Measuringunit',
      'field_name' => 'field_agres_measuringunit',
      'type' => 'text',
      'widget_type' => 'text_textfield',
      'change' => 'Change basic information',
      'weight' => '8',
      'rows' => 5,
      'size' => '60',
      'description' => 'Examples: Hotelrooms -> nights
cars -> days
Pooltable -> hours',
      'default_value' => array(
        0 => array(
          'value' => '',
          '_error_element' => 'default_value_widget][field_agres_measuringunit][0][value',
        ),
      ),
      'default_value_php' => '',
      'default_value_widget' => array(
        'field_agres_measuringunit' => array(
          0 => array(
            'value' => '',
            '_error_element' => 'default_value_widget][field_agres_measuringunit][0][value',
            'format' => '2',
          ),
        ),
      ),
      'required' => 0,
      'multiple' => '0',
      'text_processing' => '1',
      'max_length' => '',
      'allowed_values' => '',
      'allowed_values_php' => '',
      'op' => 'Save field settings',
      'module' => 'text',
      'widget_module' => 'text',
      'columns' => array(
        'value' => array(
          'type' => 'text',
          'size' => 'big',
          'not null' => FALSE,
          'sortable' => TRUE,
          'views' => TRUE,
        ),
        'format' => array(
          'type' => 'int',
          'unsigned' => TRUE,
          'not null' => FALSE,
          'views' => FALSE,
        ),
      ),
      'display_settings' => array(
        'label' => array(
          'format' => 'above',
          'exclude' => 0,
        ),
        'teaser' => array(
          'format' => 'default',
          'exclude' => 0,
        ),
        'full' => array(
          'format' => 'default',
          'exclude' => 0,
        ),
        4 => array(
          'format' => 'default',
          'exclude' => 0,
        ),
        'token' => array(
          'format' => 'default',
          'exclude' => 0,
        ),
      ),
    ),
    7 => array(
      'label' => 'Default checkin time',
      'field_name' => 'field_agres_defaultcheckintime',
      'type' => 'text',
      'widget_type' => 'optionwidgets_select',
      'change' => 'Change basic information',
      'weight' => '9',
      'description' => 'when the guest is not allowed to choose the time of checkin.
this is the time that will be set by the system for checkin.',
      'default_value' => array(
        0 => array(
          'value' => '',
        ),
      ),
      'default_value_php' => '',
      'default_value_widget' => array(
        'field_agres_defaultcheckintime' => array(
          'value' => '',
        ),
      ),
      'required' => 0,
      'multiple' => '0',
      'text_processing' => '0',
      'max_length' => '',
      'allowed_values' => '+09 hour
+10 hour
+11 hour
+12 hour
+13 hour
+14 hour
+15 hour
+16 hour
+17 hour
+18 hour
+19 hour',
      'allowed_values_php' => '',
      'op' => 'Save field settings',
      'module' => 'text',
      'widget_module' => 'optionwidgets',
      'columns' => array(
        'value' => array(
          'type' => 'text',
          'size' => 'big',
          'not null' => FALSE,
          'sortable' => TRUE,
          'views' => TRUE,
        ),
      ),
      'display_settings' => array(
        'label' => array(
          'format' => 'above',
          'exclude' => 0,
        ),
        'teaser' => array(
          'format' => 'default',
          'exclude' => 0,
        ),
        'full' => array(
          'format' => 'default',
          'exclude' => 0,
        ),
        4 => array(
          'format' => 'default',
          'exclude' => 0,
        ),
        'token' => array(
          'format' => 'default',
          'exclude' => 0,
        ),
      ),
    ),
    8 => array(
      'label' => 'Default Chekout time',
      'field_name' => 'field_agres_defaultcheckouttime',
      'type' => 'text',
      'widget_type' => 'optionwidgets_select',
      'change' => 'Change basic information',
      'weight' => '10',
      'description' => 'The default time when a guest has to check out.',
      'default_value' => array(
        0 => array(
          'value' => '',
        ),
      ),
      'default_value_php' => '',
      'default_value_widget' => array(
        'field_agres_defaultcheckouttime' => array(
          'value' => '',
        ),
      ),
      'required' => 0,
      'multiple' => '0',
      'text_processing' => '0',
      'max_length' => '',
      'allowed_values' => '+09 hour
+10 hour
+11 hour
+12 hour
+13 hour
+14 hour
+15 hour
+16 hour
+17 hour
+18 hour
+19 hour',
      'allowed_values_php' => '',
      'op' => 'Save field settings',
      'module' => 'text',
      'widget_module' => 'optionwidgets',
      'columns' => array(
        'value' => array(
          'type' => 'text',
          'size' => 'big',
          'not null' => FALSE,
          'sortable' => TRUE,
          'views' => TRUE,
        ),
      ),
      'display_settings' => array(
        'label' => array(
          'format' => 'above',
          'exclude' => 0,
        ),
        'teaser' => array(
          'format' => 'default',
          'exclude' => 0,
        ),
        'full' => array(
          'format' => 'default',
          'exclude' => 0,
        ),
        4 => array(
          'format' => 'default',
          'exclude' => 0,
        ),
        'token' => array(
          'format' => 'default',
          'exclude' => 0,
        ),
      ),
    ),
    9 => array(
      'label' => 'Allow over night bookings',
      'field_name' => 'field_agres_allowovernight',
      'type' => 'text',
      'widget_type' => 'optionwidgets_select',
      'change' => 'Change basic information',
      'weight' => '11',
      'description' => '',
      'default_value' => array(
        0 => array(
          'value' => '',
        ),
      ),
      'default_value_php' => '',
      'default_value_widget' => array(
        'field_agres_allowovernight' => array(
          'value' => '',
        ),
      ),
      'required' => 1,
      'multiple' => '0',
      'text_processing' => '0',
      'max_length' => '',
      'allowed_values' => '1|yes
2|no',
      'allowed_values_php' => '',
      'op' => 'Save field settings',
      'module' => 'text',
      'widget_module' => 'optionwidgets',
      'columns' => array(
        'value' => array(
          'type' => 'text',
          'size' => 'big',
          'not null' => FALSE,
          'sortable' => TRUE,
          'views' => TRUE,
        ),
      ),
      'display_settings' => array(
        'label' => array(
          'format' => 'above',
          'exclude' => 0,
        ),
        'teaser' => array(
          'format' => 'default',
          'exclude' => 0,
        ),
        'full' => array(
          'format' => 'default',
          'exclude' => 0,
        ),
        4 => array(
          'format' => 'default',
          'exclude' => 0,
        ),
        'token' => array(
          'format' => 'default',
          'exclude' => 0,
        ),
      ),
    ),
    10 => array(
      'label' => 'Hours of service ',
      'field_name' => 'field_agres_servicehours',
      'type' => 'text',
      'widget_type' => 'text_textfield',
      'change' => 'Change basic information',
      'weight' => '12',
      'rows' => 5,
      'size' => '40',
      'description' => 'example: 00:00-23:59 for 24hour service
7 entries 1 for each day, first Monday, last Sunday.
Mark closed days as: x
each slot can have multiple values example: 08:00-12-00,13:00-18:00
->means hours of service are from 08:00 until 12:00 and from 13:00 to 18:00
if overnight booking is allowed, this means only the checkin and checkout times have to be within the
service times. but the unit can be bookend around the clock.',
      'default_value' => array(
        0 => array(
          'value' => '',
          '_error_element' => 'default_value_widget][field_agres_servicehours][0][value',
        ),
      ),
      'default_value_php' => '',
      'default_value_widget' => array(
        'field_agres_servicehours' => array(
          0 => array(
            'value' => '',
            '_error_element' => 'default_value_widget][field_agres_servicehours][0][value',
          ),
        ),
      ),
      'required' => 0,
      'multiple' => '7',
      'text_processing' => '0',
      'max_length' => '',
      'allowed_values' => '',
      'allowed_values_php' => '',
      'op' => 'Save field settings',
      'module' => 'text',
      'widget_module' => 'text',
      'columns' => array(
        'value' => array(
          'type' => 'text',
          'size' => 'big',
          'not null' => FALSE,
          'sortable' => TRUE,
          'views' => TRUE,
        ),
      ),
      'display_settings' => array(
        'label' => array(
          'format' => 'above',
          'exclude' => 0,
        ),
        'teaser' => array(
          'format' => 'default',
          'exclude' => 0,
        ),
        'full' => array(
          'format' => 'default',
          'exclude' => 0,
        ),
        4 => array(
          'format' => 'default',
          'exclude' => 0,
        ),
        'token' => array(
          'format' => 'default',
          'exclude' => 0,
        ),
      ),
    ),
    11 => array(
      'label' => 'Duration',
      'field_name' => 'field_agres_duration',
      'type' => 'duration',
      'widget_type' => 'duration_combo',
      'change' => 'Change basic information',
      'weight' => '13',
      'use_weeks' => 0,
      'largest_metric' => 'days',
      'smallest_metric' => 'minutes',
      'description' => 'leave empty (on 0) if you don´t want to set a default duration of a booking.
if you leave empty the \'Label of date2\' entry, the \'to\' date entry will not be displayed in the search form block, making this default duration a fixed duration!',
      'default_value' => array(
        0 => array(
          'duration' => array(
            'duration' => array(
              'days' => 0,
              'hours' => '0',
              'minutes' => 0,
            ),
            'is_negative' => FALSE,
            'is_valid' => TRUE,
            'conversion_factors' => array(
              'seconds/minutes' => 60,
              'minutes/hours' => 60,
              'hours/days' => 24,
              'days/weeks' => 7,
              'days/months' => 30,
              'weeks/years' => 52.1428571429,
              'months/years' => 12,
            ),
          ),
          'iso8601' => 'PT0S',
          'approx_seconds' => 0,
        ),
      ),
      'default_value_php' => '',
      'default_value_widget' => array(
        'field_agres_duration' => array(
          0 => array(
            'type' => 'months',
            'years' => 0,
            'months' => 0,
            'days' => 0,
            'hours' => 0,
            'minutes' => 0,
            'seconds' => 0,
          ),
        ),
      ),
      'required' => 0,
      'multiple' => '0',
      'op' => 'Save field settings',
      'module' => 'duration',
      'widget_module' => 'duration',
      'columns' => array(
        'iso8601' => array(
          'type' => 'varchar',
          'length' => '64',
          'not null' => TRUE,
          'default' => '',
        ),
        'approx_seconds' => array(
          'type' => 'int',
          'unsigned' => TRUE,
          'not null' => TRUE,
          'default' => 0,
          'sortable' => TRUE,
        ),
      ),
      'display_settings' => array(
        'label' => array(
          'format' => 'above',
          'exclude' => 0,
        ),
        'teaser' => array(
          'format' => 'default',
          'exclude' => 0,
        ),
        'full' => array(
          'format' => 'default',
          'exclude' => 0,
        ),
        4 => array(
          'format' => 'default',
          'exclude' => 0,
        ),
        'token' => array(
          'format' => 'default',
          'exclude' => 0,
        ),
      ),
    ),
    12 => array(
      'label' => 'Show number of units?',
      'field_name' => 'field_agres_shownumberunits',
      'type' => 'text',
      'widget_type' => 'optionwidgets_select',
      'change' => 'Change basic information',
      'weight' => '14',
      'description' => '',
      'default_value' => array(
        0 => array(
          'value' => '',
        ),
      ),
      'default_value_php' => '',
      'default_value_widget' => array(
        'field_agres_shownumberunits' => array(
          'value' => '',
        ),
      ),
      'required' => 0,
      'multiple' => '0',
      'text_processing' => '0',
      'max_length' => '',
      'allowed_values' => '1|yes let the guest choose number of units
2|no, use default unitnumber ',
      'allowed_values_php' => '',
      'op' => 'Save field settings',
      'module' => 'text',
      'widget_module' => 'optionwidgets',
      'columns' => array(
        'value' => array(
          'type' => 'text',
          'size' => 'big',
          'not null' => FALSE,
          'sortable' => TRUE,
          'views' => TRUE,
        ),
      ),
      'display_settings' => array(
        'label' => array(
          'format' => 'above',
          'exclude' => 0,
        ),
        'teaser' => array(
          'format' => 'default',
          'exclude' => 0,
        ),
        'full' => array(
          'format' => 'default',
          'exclude' => 0,
        ),
        4 => array(
          'format' => 'default',
          'exclude' => 0,
        ),
        'token' => array(
          'format' => 'default',
          'exclude' => 0,
        ),
      ),
    ),
    13 => array(
      'label' => 'Ask for Persons',
      'field_name' => 'field_agres_askpersons',
      'type' => 'text',
      'widget_type' => 'optionwidgets_select',
      'change' => 'Change basic information',
      'weight' => '15',
      'description' => '',
      'default_value' => array(
        0 => array(
          'value' => '',
        ),
      ),
      'default_value_php' => '',
      'default_value_widget' => array(
        'field_agres_askpersons' => array(
          'value' => '',
        ),
      ),
      'required' => 0,
      'multiple' => '0',
      'text_processing' => '0',
      'max_length' => '',
      'allowed_values' => '1|ask for Number of Persons during the online booking
2|don´t ask for number of Persons
3|ask for number of Adults and Children seperately',
      'allowed_values_php' => '',
      'op' => 'Save field settings',
      'module' => 'text',
      'widget_module' => 'optionwidgets',
      'columns' => array(
        'value' => array(
          'type' => 'text',
          'size' => 'big',
          'not null' => FALSE,
          'sortable' => TRUE,
          'views' => TRUE,
        ),
      ),
      'display_settings' => array(
        'label' => array(
          'format' => 'above',
          'exclude' => 0,
        ),
        'teaser' => array(
          'format' => 'default',
          'exclude' => 0,
        ),
        'full' => array(
          'format' => 'default',
          'exclude' => 0,
        ),
        4 => array(
          'format' => 'default',
          'exclude' => 0,
        ),
        'token' => array(
          'format' => 'default',
          'exclude' => 0,
        ),
      ),
    ),
    14 => array(
      'label' => 'Default number of units to book',
      'field_name' => 'field_agres_numberunits',
      'type' => 'number_integer',
      'widget_type' => 'number',
      'change' => 'Change basic information',
      'weight' => '16',
      'description' => '',
      'default_value' => array(
        0 => array(
          'value' => '',
          '_error_element' => 'default_value_widget][field_agres_numberunits][0][value',
        ),
      ),
      'default_value_php' => '',
      'default_value_widget' => array(
        'field_agres_numberunits' => array(
          0 => array(
            'value' => '',
            '_error_element' => 'default_value_widget][field_agres_numberunits][0][value',
          ),
        ),
      ),
      'required' => 0,
      'multiple' => '0',
      'min' => '1',
      'max' => '',
      'prefix' => '',
      'suffix' => '',
      'allowed_values' => '',
      'allowed_values_php' => '',
      'op' => 'Save field settings',
      'module' => 'number',
      'widget_module' => 'number',
      'columns' => array(
        'value' => array(
          'type' => 'int',
          'not null' => FALSE,
          'sortable' => TRUE,
        ),
      ),
      'display_settings' => array(
        'label' => array(
          'format' => 'above',
          'exclude' => 0,
        ),
        'teaser' => array(
          'format' => 'default',
          'exclude' => 0,
        ),
        'full' => array(
          'format' => 'default',
          'exclude' => 0,
        ),
        4 => array(
          'format' => 'default',
          'exclude' => 0,
        ),
        'token' => array(
          'format' => 'default',
          'exclude' => 0,
        ),
      ),
    ),
    15 => array(
      'label' => 'Remove Add to Cart button if present',
      'field_name' => 'field_agres_removeaddtocart',
      'type' => 'text',
      'widget_type' => 'optionwidgets_select',
      'change' => 'Change basic information',
      'weight' => '17',
      'description' => '',
      'default_value' => array(
        0 => array(
          'value' => '',
        ),
      ),
      'default_value_php' => '',
      'default_value_widget' => array(
        'field_agres_removeaddtocart' => array(
          'value' => '',
        ),
      ),
      'required' => 0,
      'multiple' => '0',
      'text_processing' => '0',
      'max_length' => '',
      'allowed_values' => '1|yes: remove add to cart
2|add to cart button will be displayed',
      'allowed_values_php' => '',
      'op' => 'Save field settings',
      'module' => 'text',
      'widget_module' => 'optionwidgets',
      'columns' => array(
        'value' => array(
          'type' => 'text',
          'size' => 'big',
          'not null' => FALSE,
          'sortable' => TRUE,
          'views' => TRUE,
        ),
      ),
      'display_settings' => array(
        'label' => array(
          'format' => 'above',
          'exclude' => 0,
        ),
        'teaser' => array(
          'format' => 'default',
          'exclude' => 0,
        ),
        'full' => array(
          'format' => 'default',
          'exclude' => 0,
        ),
        4 => array(
          'format' => 'default',
          'exclude' => 0,
        ),
        'token' => array(
          'format' => 'default',
          'exclude' => 0,
        ),
      ),
    ),
  );
  $content['extra'] = array(
    'title' => '-5',
    'body_field' => '-3',
    'revision_information' => '-2',
    'author' => '-1',
    'options' => '0',
    'menu' => '-4',
    'path' => '1',
  );

  return $content;
}

/**
 * @todo Documentation
 */
function _agres_categories_cck_export_unittype_categories() {
  $content['type'] = array(
    'name' => 'Unit type',
    'type' => 'agreservations_unittype',
    'description' => 'The Type of unit by agreservations module:
Single unit, Double unit, Suite, Presidents Suite, Guestunit...',
    'title_label' => 'Title',
    'body_label' => 'Description',
    'min_word_count' => '0',
    'help' => '',
    'node_options' => array(
      'status' => TRUE,
      'promote' => TRUE,
      'sticky' => FALSE,
      'revision' => FALSE,
    ),
    'language_content_type' => '2',
    'old_type' => 'agreservations_unittype',
    'orig_type' => '',
    'module' => 'uc_product',
    'custom' => '0',
    'modified' => '1',
    'locked' => '0',
    'reset' => 'Reset to defaults',
    'uc_product_shippable' => 0,
    'uc_image' => '',
    'i18n_newnode_current' => 0,
    'i18n_required_node' => 0,
    'i18n_lock_node' => 0,
    'i18n_node' => 1,
  );
  $content['fields'] = array(
    0 => array(
      'label' => 'Capacity',
      'field_name' => 'field_agreservations_capacity',
      'type' => 'number_integer',
      'widget_type' => 'number',
      'change' => 'Change basic information',
      'weight' => '4',
      'description' => 'How many persons are allowed for this unittype?
     children count only 0.5',
      'default_value' => array(
        0 => array(
          'value' => '',
          '_error_element' => 'default_value_widget][field_agreservations_capacity][0][value',
        ),
      ),
      'default_value_php' => '',
      'default_value_widget' => NULL,
      'required' => 1,
      'multiple' => '0',
      'min' => '',
      'max' => '',
      'prefix' => '',
      'suffix' => '',
      'allowed_values' => '',
      'allowed_values_php' => '',
      'op' => 'Save field settings',
      'module' => 'number',
      'widget_module' => 'number',
      'columns' => array(
        'value' => array(
          'type' => 'int',
          'not null' => FALSE,
          'sortable' => TRUE,
        ),
      ),
      'display_settings' => array(
        'label' => array(
          'format' => 'above',
          'exclude' => 0,
        ),
        'teaser' => array(
          'format' => 'default',
          'exclude' => 0,
        ),
        'full' => array(
          'format' => 'default',
          'exclude' => 0,
        ),
        4 => array(
          'format' => 'default',
          'exclude' => 0,
        ),
      ),
    ),
    1 => array(
      'label' => 'Categories',
      'field_name' => 'field_agres_ref_category',
      'type' => 'nodereference',
      'widget_type' => 'nodereference_select',
      'change' => 'Change basic information',
      'weight' => '31',
      'autocomplete_match' => 'contains',
      'size' => 60,
      'description' => '',
      'default_value' => array(
        0 => array(
          'nid' => '',
        ),
      ),
      'default_value_php' => '',
      'default_value_widget' => array(
        'field_agres_ref_category' => array(
          0 => array(
            'nid' => '',
          ),
          'nid' => array(
            'nid' => array(
              0 => '',
            ),
          ),
        ),
      ),
      'required' => 0,
      'multiple' => '1',
      'referenceable_types' => array(
        'agres_category' => 'agres_category',
        'page' => 0,
        'product' => 0,
        'agreservation' => 0,
        'story' => 0,
        'agreservations_unit' => 0,
        'agreservation_unit_info' => 0,
        'agreservations_unittype' => 0,
        'webform' => 0,
        'testproduct' => 0,
      ),
      'advanced_view' => '--',
      'advanced_view_args' => '',
      'op' => 'Save field settings',
      'module' => 'nodereference',
      'widget_module' => 'nodereference',
      'columns' => array(
        'nid' => array(
          'type' => 'int',
          'unsigned' => TRUE,
          'not null' => FALSE,
          'index' => TRUE,
        ),
      ),
      'display_settings' => array(
        'label' => array(
          'format' => 'above',
          'exclude' => 0,
        ),
        'teaser' => array(
          'format' => 'default',
          'exclude' => 0,
        ),
        'full' => array(
          'format' => 'default',
          'exclude' => 0,
        ),
        4 => array(
          'format' => 'default',
          'exclude' => 0,
        ),
        'token' => array(
          'format' => 'default',
          'exclude' => 0,
        ),
      ),
    ),
  );
  $content['extra'] = array(
    'title' => '-5',
    'body_field' => '0',
    'revision_information' => '20',
    'menu' => '-2',
    'taxonomy' => '-3',
    'path' => '30',
    'base' => '-1',
    'body' => '1',
  );

  return $content;
}