<?php

/**
 * @file
 * Install cck types for agreservationsmodule.
 */

/**
 * @todo Documentation
 */
function _agreservations_cck_export_unit() {

  $content['type'] = array(
    'name' => 'Unit',
    'type' => 'agreservations_unit',
    'description' => t('reservable units implemented by agreservations module'),
    'title_label' => 'Unitname',
    'body_label' => '',
    'min_word_count' => '0',
    'help' => '',
    'node_options' => array(
      'status' => TRUE,
      'promote' => TRUE,
      'sticky' => FALSE,
      'revision' => FALSE,
    ),
    'language_content_type' => '0',
    'old_type' => 'agreservations_unit',
    'orig_type' => '',
    'module' => 'node',
    'custom' => '1',
    'modified' => '1',
    'locked' => '0',
    'content_profile_use' => 0,
    'comment' => '2',
    'comment_default_mode' => '4',
    'comment_default_order' => '1',
    'comment_default_per_page' => '50',
    'comment_controls' => '3',
    'comment_anonymous' => 0,
    'comment_subject_field' => '1',
    'comment_preview' => '1',
    'comment_form_location' => '0',
    'availability_calendars' => '0',
  );
  $content['fields'] = array(
    0 => array(
      'label' => 'Unitnumber',
      'field_name' => 'field_agreservations_unitnumber',
      'type' => 'number_integer',
      'widget_type' => 'number',
      'change' => 'Change basic information',
      'weight' => '1',
      'description' => '',
      'default_value' => array(
        0 => array(
          'value' => '',
          '_error_element' => 'default_value_widget][field_agreservations_unitnumber][0][value',
        ),
      ),
      'default_value_php' => '',
      'default_value_widget' => NULL,
      'group' => FALSE,
      'required' => 0,
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
        'token' => array(
          'format' => 'default',
          'exclude' => 0,
        ),
      ),
    ),
    1 => array(
      'label' => 'unittype',
      'field_name' => 'field_agreservations_unittype',
      'type' => 'nodereference',
      'widget_type' => 'nodereference_select',
      'change' => 'Change basic information',
      'weight' => '2',
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
        'field_agreservations_unittype' => array(
          0 => array(
            'nid' => '',
          ),
          'nid' => array(
            'nid' => '',
          ),
        ),
      ),
      'group' => FALSE,
      'required' => 1,
      'multiple' => '0',
      'referenceable_types' => array(
        'agreservations_unittype' => 'agreservations_unittype',
        'unit' => 1,
        'page' => FALSE,
        'product' => FALSE,
        'profile' => FALSE,
        'agreservation' => FALSE,
        'story' => FALSE,
        'agreservations_unit' => FALSE,
        'agreservation_unit_info' => FALSE,
      ),
      'advanced_view' => '--',
      'advanced_view_args' => '',
      'cck_referential_integrity_behavior' => '',
      'show_add_link' => TRUE,
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
      ),
    ),
  );
  $content['extra'] = array(
    'title' => '-5',
    'revision_information' => '20',
    'comment_settings' => '30',
    'menu' => '-2',
    'path' => '30',
  );


  // paste code before this line.
  return $content;
}

function _agreservations_cck_export_unittype() {
  $content['type'] = array(
    'name' => 'Unit type',
    'type' => 'agreservations_unittype',
    'description' => 'The Type of unit by agreservations module:
Single unit, Double unit, Suite, Presidents Suite, Guestunit...',
    'title_label' => 'Title',
    'body_label' => '',
    'min_word_count' => '0',
    'help' => '',
    'node_options' => array(
      'status' => TRUE,
      'promote' => TRUE,
      'sticky' => FALSE,
      'revision' => FALSE,
    ),
    'language_content_type' => '0',
    'old_type' => 'agreservations_unittype',
    'orig_type' => '',
    'module' => 'node',
    'custom' => '1',
    'modified' => '1',
    'locked' => '0',
    'comment' => '2',
    'comment_default_mode' => '4',
    'comment_default_order' => '1',
    'comment_default_per_page' => '50',
    'comment_controls' => '3',
    'comment_anonymous' => 0,
    'comment_subject_field' => '1',
    'comment_preview' => '1',
    'comment_form_location' => '0',
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
      'default_value_widget' => array(
        'field_agreservations_capacity' => array(
          0 => array(
            'value' => '',
            '_error_element' => 'default_value_widget][field_agreservations_capacity][0][value',
          ),
        ),
      ),
      'group' => FALSE,
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
  );
  $content['extra'] = array(
    'title' => '-5',
    'body_field' => '0',
    'menu' => '-2',
  );

  return $content;
}

/**
 * @todo Documentation
 */
function _agreservations_cck_export_agreservation() {

  $content['type'] = array(
    'name' => 'Reservation',
    'type' => 'agreservation',
    'description' => '',
    'title_label' => 'Title',
    'body_label' => '',
    'min_word_count' => '0',
    'help' => '',
    'node_options' => array(
      'status' => TRUE,
      'promote' => TRUE,
      'sticky' => FALSE,
      'revision' => FALSE,
    ),
    'language_content_type' => '0',
    'old_type' => 'agreservation',
    'orig_type' => '',
    'module' => 'node',
    'custom' => '1',
    'modified' => '1',
    'locked' => '0',
    'widgets_enabled' => 0,
    'widgets_default' => array(
    ),
    'content_profile_use' => 0,
    'comment' => '2',
    'comment_default_mode' => '4',
    'comment_default_order' => '1',
    'comment_default_per_page' => '50',
    'comment_controls' => '3',
    'comment_anonymous' => 0,
    'comment_subject_field' => '1',
    'comment_preview' => '1',
    'comment_form_location' => '0',
    'availability_calendars' => '0',
  );
  $content['fields'] = array(
    0 => array(
      'label' => 'Reservationdate',
      'field_name' => 'field_agres_checkinout',
      'type' => 'datetime',
      'widget_type' => 'date_popup',
      'change' => 'Change basic information',
      'weight' => '-1',
      'default_value' => 'blank',
      'default_value2' => 'same',
      'default_value_code' => '',
      'default_value_code2' => '',
      'input_format' => 'd.m.Y - H:i',
      'input_format_custom' => '',
      'year_range' => '-3:+3',
      'increment' => '1',
      'advanced' => array(
        'label_position' => 'above',
        'text_parts' => array(
          'year' => 0,
          'month' => 0,
          'day' => 0,
          'hour' => 0,
          'minute' => 0,
          'second' => 0,
        ),
      ),
      'label_position' => 'above',
      'text_parts' => array(
      ),
      'description' => '',
      'group' => FALSE,
      'required' => 1,
      'multiple' => '0',
      'repeat' => 0,
      'todate' => 'required',
      'granularity' => array(
        'year' => 'year',
        'month' => 'month',
        'day' => 'day',
        'hour' => 'hour',
        'minute' => 'minute',
      ),
      'default_format' => 'medium',
      'tz_handling' => 'none',
      'timezone_db' => '',
      'op' => 'Save field settings',
      'module' => 'date',
      'widget_module' => 'date',
      'columns' => array(
        'value' => array(
          'type' => 'datetime',
          'not null' => FALSE,
          'sortable' => TRUE,
          'views' => TRUE,
        ),
        'value2' => array(
          'type' => 'datetime',
          'not null' => FALSE,
          'sortable' => TRUE,
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
        2 => array(
          'format' => 'default',
          'exclude' => 0,
        ),
        3 => array(
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
      'label' => 'unitreference',
      'field_name' => 'field_agres_ref_unit',
      'type' => 'nodereference',
      'widget_type' => 'nodereference_buttons',
      'change' => 'Change basic information',
      'weight' => 0,
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
        'field_agres_ref_unit' => array(
          0 => array(
            'nid' => '',
          ),
          'nid' => array(
            'nid' => array(
              '' => 1,
              517 => FALSE,
              518 => FALSE,
              527 => FALSE,
              528 => FALSE,
              566 => FALSE,
            ),
          ),
        ),
      ),
      'group' => FALSE,
      'required' => 0,
      'multiple' => '1',
      'referenceable_types' => array(
        'agreservations_unit' => 'agreservations_unit',
        'page' => 0,
        'product' => 0,
        'profile' => 0,
        'story' => 0,
        'agreservation' => 0,
        'agreservation2' => 0,
        'agreservations_unittype' => 0,
        'agreservation_unit_info' => FALSE,
      ),
      'advanced_view' => '--',
      'advanced_view_args' => '',
      'cck_referential_integrity_behavior' => '',
      'show_add_link' => 1,
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
        2 => array(
          'format' => 'default',
          'exclude' => 0,
        ),
        3 => array(
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
      'label' => 'Order_id',
      'field_name' => 'field_agres_orderid',
      'type' => 'number_integer',
      'widget_type' => 'number',
      'change' => 'Change basic information',
      'weight' => '1',
      'description' => '',
      'default_value' => array(
        0 => array(
          'value' => '',
          '_error_element' => 'default_value_widget][field_agres_orderid][0][value',
        ),
      ),
      'default_value_php' => '',
      'default_value_widget' => array(
        'field_agres_orderid' => array(
          0 => array(
            'value' => '',
            '_error_element' => 'default_value_widget][field_agres_orderid][0][value',
          ),
        ),
      ),
      'group' => FALSE,
      'required' => 0,
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
        'token' => array(
          'format' => 'default',
          'exclude' => 0,
        ),
      ),
    ),
    3 => array(
      'label' => 'agreservations_Status',
      'field_name' => 'field_agres_status',
      'type' => 'number_integer',
      'widget_type' => 'number',
      'change' => 'Change basic information',
      'weight' => '2',
      'description' => '',
      'default_value' => array(
        0 => array(
          'value' => '',
          '_error_element' => 'default_value_widget][field_agres_status][0][value',
        ),
      ),
      'default_value_php' => '',
      'default_value_widget' => array(
        'field_agres_status' => array(
          0 => array(
            'value' => '',
            '_error_element' => 'default_value_widget][field_agres_status][0][value',
          ),
        ),
      ),
      'group' => FALSE,
      'required' => 0,
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
        'token' => array(
          'format' => 'default',
          'exclude' => 0,
        ),
      ),
    ),
    4 => array(
      'label' => 'Userreference',
      'field_name' => 'field_agres_userref',
      'type' => 'userreference',
      'widget_type' => 'userreference_autocomplete',
      'change' => 'Change basic information',
      'weight' => '3',
      'autocomplete_match' => 'contains',
      'size' => '60',
      'reverse_link' => 0,
      'description' => '',
      'default_value' => array(
        0 => array(
          'uid' => NULL,
          '_error_element' => 'default_value_widget][field_agres_userref][0][uid][uid',
        ),
      ),
      'default_value_php' => '',
      'default_value_widget' => array(
        'field_agres_userref' => array(
          0 => array(
            'uid' => array(
              'uid' => '',
              '_error_element' => 'default_value_widget][field_agres_userref][0][uid][uid',
            ),
            '_error_element' => 'default_value_widget][field_agres_userref][0][uid][uid',
          ),
        ),
      ),
      'group' => FALSE,
      'required' => 0,
      'multiple' => '0',
      'referenceable_roles' => array(
        2 => 0,
      ),
      'referenceable_status' => '',
      'advanced_view' => 'hotelcustomers',
      'advanced_view_args' => '',
      'cck_referential_integrity_behavior' => '',
      'show_add_link' => 1,
      'op' => 'Save field settings',
      'module' => 'userreference',
      'widget_module' => 'userreference',
      'columns' => array(
        'uid' => array(
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
    5 => array(
      'label' => 'number of adults',
      'field_name' => 'field_agres_adults',
      'type' => 'number_integer',
      'widget_type' => 'number',
      'change' => 'Change basic information',
      'weight' => '4',
      'description' => '',
      'default_value' => array(
        0 => array(
          'value' => '',
          '_error_element' => 'default_value_widget][field_agres_adults][0][value',
        ),
      ),
      'default_value_php' => '',
      'default_value_widget' => array(
        'field_agres_adults' => array(
          0 => array(
            'value' => '',
            '_error_element' => 'default_value_widget][field_agres_adults][0][value',
          ),
        ),
      ),
      'group' => FALSE,
      'required' => 0,
      'multiple' => '0',
      'min' => '1',
      'max' => '10',
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
        2 => array(
          'format' => 'default',
          'exclude' => 0,
        ),
        3 => array(
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
      'label' => 'number of Children',
      'field_name' => 'field_agres_children',
      'type' => 'number_integer',
      'widget_type' => 'number',
      'change' => 'Change basic information',
      'weight' => '5',
      'description' => '',
      'default_value' => array(
        0 => array(
          'value' => '',
          '_error_element' => 'default_value_widget][field_agres_children][0][value',
        ),
      ),
      'default_value_php' => '',
      'default_value_widget' => array(
        'field_agres_children' => array(
          0 => array(
            'value' => '',
            '_error_element' => 'default_value_widget][field_agres_children][0][value',
          ),
        ),
      ),
      'group' => FALSE,
      'required' => 0,
      'multiple' => '0',
      'min' => '0',
      'max' => '10',
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
        2 => array(
          'format' => 'default',
          'exclude' => 0,
        ),
        3 => array(
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
      'label' => 'Persons',
      'field_name' => 'field_agres_persons',
      'type' => 'number_integer',
      'widget_type' => 'number',
      'change' => 'Change basic information',
      'weight' => '6',
      'description' => '',
      'default_value' => array(
        0 => array(
          'value' => '',
          '_error_element' => 'default_value_widget][field_agres_persons][0][value',
        ),
      ),
      'default_value_php' => '',
      'default_value_widget' => array(
        'field_agres_persons' => array(
          0 => array(
            'value' => '',
            '_error_element' => 'default_value_widget][field_agres_persons][0][value',
          ),
        ),
      ),
      'group' => FALSE,
      'required' => 0,
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
        2 => array(
          'format' => 'default',
          'exclude' => 0,
        ),
        3 => array(
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
    'revision_information' => '-3',
    'comment_settings' => '-2',
    'menu' => '-4',
    'path' => '30',
  );


  return $content;
}

function _agreservations_cck_export_unitinfo() {
  $content['type'] = array(
    'name' => 'Unit info per reservation',
    'type' => 'agreservation_unit_info',
    'description' => 'resourceregarded Info of an agreservation',
    'title_label' => 'Title',
    'body_label' => '',
    'min_word_count' => '0',
    'help' => '',
    'node_options' => array(
      'status' => TRUE,
      'promote' => TRUE,
      'sticky' => FALSE,
      'revision' => FALSE,
    ),
    'language_content_type' => '0',
    'old_type' => 'agreservation_unit_info',
    'orig_type' => '',
    'module' => 'node',
    'custom' => '1',
    'modified' => '1',
    'locked' => '0',
    'widgets_enabled' => 0,
    'widgets_default' => array(
    ),
    'content_profile_use' => 0,
    'comment' => '2',
    'comment_default_mode' => '4',
    'comment_default_order' => '1',
    'comment_default_per_page' => '50',
    'comment_controls' => '3',
    'comment_anonymous' => 0,
    'comment_subject_field' => '1',
    'comment_preview' => '1',
    'comment_form_location' => '0',
    'availability_calendars' => '0',
  );
  $content['fields'] = array(
    0 => array(
      'label' => 'Reference Agreservation',
      'field_name' => 'field_agres_ref_agreservation',
      'type' => 'nodereference',
      'widget_type' => 'nodereference_select',
      'change' => 'Change basic information',
      'weight' => '31',
      'autocomplete_match' => 'contains',
      'size' => '60',
      'description' => '',
      'default_value' => array(
        0 => array(
          'nid' => '',
        ),
      ),
      'default_value_php' => '',
      'default_value_widget' => NULL,
      'group' => FALSE,
      'required' => 1,
      'multiple' => '1',
      'referenceable_types' => array(
        'agreservation' => 'agreservation',
        'page' => 0,
        'product' => 0,
        'profile' => 0,
        'story' => 0,
        'agreservations_unittype' => 0,
        'agreservation_unit_info' => 0,
        'agreservations_unit' => 0,
      ),
      'advanced_view' => '--',
      'advanced_view_args' => '',
      'cck_referential_integrity_behavior' => '',
      'show_add_link' => 0,
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
        'weight' => '31',
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
        2 => array(
          'format' => 'default',
          'exclude' => 0,
        ),
        3 => array(
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
      'label' => 'Reserved Room',
      'field_name' => 'field_agres_refsingle_unit',
      'type' => 'nodereference',
      'widget_type' => 'nodereference_select',
      'change' => 'Change basic information',
      'weight' => '32',
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
        'field_agres_refsingle_unit' => array(
          0 => array(
            'nid' => '',
          ),
          'nid' => array(
            'nid' => '',
          ),
        ),
      ),
      'group' => FALSE,
      'required' => 1,
      'multiple' => '0',
      'referenceable_types' => array(
        'agreservations_unit' => 'agreservations_unit',
        'page' => 0,
        'product' => 0,
        'profile' => 0,
        'story' => 0,
        'agreservation' => 0,
        'agreservations_unittype' => 0,
        'agreservation_unit_info' => 0,
      ),
      'advanced_view' => '--',
      'advanced_view_args' => '',
      'cck_referential_integrity_behavior' => '',
      'show_add_link' => 1,
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
        2 => array(
          'format' => 'default',
          'exclude' => 0,
        ),
        3 => array(
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
      'label' => 'Persons',
      'field_name' => 'field_agres_unit_persons',
      'type' => 'number_integer',
      'widget_type' => 'number',
      'change' => 'Change basic information',
      'weight' => '33',
      'description' => '',
      'default_value' => array(
        0 => array(
          'value' => '',
          '_error_element' => 'default_value_widget][field_agres_unit_persons][0][value',
        ),
      ),
      'default_value_php' => '',
      'default_value_widget' => array(
        'field_agres_unit_persons' => array(
          0 => array(
            'value' => '',
            '_error_element' => 'default_value_widget][field_agres_unit_persons][0][value',
          ),
        ),
      ),
      'group' => FALSE,
      'required' => 0,
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
        'weight' => '33',
        'parent' => '',
        'label' => array(
          'format' => 'above',
        ),
        'teaser' => array(
          'format' => 'editable',
          'exclude' => 0,
        ),
        'full' => array(
          'format' => 'editable',
          'exclude' => 0,
        ),
        4 => array(
          'format' => 'default',
          'exclude' => 0,
        ),
        2 => array(
          'format' => 'default',
          'exclude' => 0,
        ),
        3 => array(
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
    'revision_information' => '20',
    'comment_settings' => '30',
    'menu' => '-2',
    'path' => '30',
  );

  return $content;
}