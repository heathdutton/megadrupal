<?php
/**
 * @file
 * xcri_cap.features.field.inc
 */

/**
 * Implements hook_field_default_fields().
 */
function xcri_cap_field_default_fields() {
  $fields = array();

  // Exported field: 'field_collection_item-field_credit-field_credit_level'.
  $fields['field_collection_item-field_credit-field_credit_level'] = array(
    'field_config' => array(
      'active' => '1',
      'cardinality' => '1',
      'deleted' => '0',
      'entity_types' => array(),
      'field_name' => 'field_credit_level',
      'foreign keys' => array(
        'format' => array(
          'columns' => array(
            'format' => 'format',
          ),
          'table' => 'filter_format',
        ),
      ),
      'indexes' => array(
        'format' => array(
          0 => 'format',
        ),
      ),
      'locked' => '0',
      'module' => 'text',
      'settings' => array(
        'max_length' => '50',
      ),
      'translatable' => '0',
      'type' => 'text',
    ),
    'field_instance' => array(
      'bundle' => 'field_credit',
      'default_value' => NULL,
      'deleted' => '0',
      'description' => 'The point in a credit scheme or framework at which credits are awarded.',
      'display' => array(
        'default' => array(
          'label' => 'above',
          'module' => 'text',
          'settings' => array(),
          'type' => 'text_default',
          'weight' => 0,
        ),
      ),
      'entity_type' => 'field_collection_item',
      'field_name' => 'field_credit_level',
      'label' => 'Level',
      'required' => 0,
      'settings' => array(
        'text_processing' => '0',
        'user_register_form' => FALSE,
      ),
      'widget' => array(
        'active' => 1,
        'module' => 'text',
        'settings' => array(
          'size' => '60',
        ),
        'type' => 'text_textfield',
        'weight' => '32',
      ),
    ),
  );

  // Exported field: 'field_collection_item-field_credit-field_credit_scheme'.
  $fields['field_collection_item-field_credit-field_credit_scheme'] = array(
    'field_config' => array(
      'active' => '1',
      'cardinality' => '1',
      'deleted' => '0',
      'entity_types' => array(),
      'field_name' => 'field_credit_scheme',
      'foreign keys' => array(
        'format' => array(
          'columns' => array(
            'format' => 'format',
          ),
          'table' => 'filter_format',
        ),
      ),
      'indexes' => array(
        'format' => array(
          0 => 'format',
        ),
      ),
      'locked' => '0',
      'module' => 'text',
      'settings' => array(
        'max_length' => '255',
      ),
      'translatable' => '0',
      'type' => 'text',
    ),
    'field_instance' => array(
      'bundle' => 'field_credit',
      'default_value' => NULL,
      'deleted' => '0',
      'description' => 'The name of the credit scheme under which credits are awarded.',
      'display' => array(
        'default' => array(
          'label' => 'above',
          'module' => 'text',
          'settings' => array(),
          'type' => 'text_default',
          'weight' => 1,
        ),
      ),
      'entity_type' => 'field_collection_item',
      'field_name' => 'field_credit_scheme',
      'label' => 'Scheme',
      'required' => 0,
      'settings' => array(
        'text_processing' => '0',
        'user_register_form' => FALSE,
      ),
      'widget' => array(
        'active' => 1,
        'module' => 'text',
        'settings' => array(
          'size' => '60',
        ),
        'type' => 'text_textfield',
        'weight' => '34',
      ),
    ),
  );

  // Exported field: 'field_collection_item-field_credit-field_value'.
  $fields['field_collection_item-field_credit-field_value'] = array(
    'field_config' => array(
      'active' => '1',
      'cardinality' => '1',
      'deleted' => '0',
      'entity_types' => array(),
      'field_name' => 'field_value',
      'foreign keys' => array(),
      'indexes' => array(),
      'locked' => '0',
      'module' => 'number',
      'settings' => array(),
      'translatable' => '0',
      'type' => 'number_integer',
    ),
    'field_instance' => array(
      'bundle' => 'field_credit',
      'default_value' => NULL,
      'deleted' => '0',
      'description' => 'The number of credits awarded.',
      'display' => array(
        'default' => array(
          'label' => 'above',
          'module' => 'number',
          'settings' => array(
            'decimal_separator' => '.',
            'prefix_suffix' => TRUE,
            'scale' => 0,
            'thousand_separator' => ' ',
          ),
          'type' => 'number_integer',
          'weight' => 2,
        ),
      ),
      'entity_type' => 'field_collection_item',
      'field_name' => 'field_value',
      'label' => 'Value',
      'required' => 0,
      'settings' => array(
        'max' => '',
        'min' => '0',
        'prefix' => '',
        'suffix' => '',
        'user_register_form' => FALSE,
      ),
      'widget' => array(
        'active' => 0,
        'module' => 'number',
        'settings' => array(),
        'type' => 'number',
        'weight' => '35',
      ),
    ),
  );

  // Exported field: 'node-course_presentation-body'.
  $fields['node-course_presentation-body'] = array(
    'field_config' => array(
      'active' => '1',
      'cardinality' => '1',
      'deleted' => '0',
      'entity_types' => array(
        0 => 'node',
      ),
      'field_name' => 'body',
      'foreign keys' => array(
        'format' => array(
          'columns' => array(
            'format' => 'format',
          ),
          'table' => 'filter_format',
        ),
      ),
      'indexes' => array(
        'format' => array(
          0 => 'format',
        ),
      ),
      'locked' => '0',
      'module' => 'text',
      'settings' => array(),
      'translatable' => '0',
      'type' => 'text_with_summary',
    ),
    'field_instance' => array(
      'bundle' => 'course_presentation',
      'default_value' => NULL,
      'deleted' => '0',
      'description' => 'General summary of the course. <b>If empty, the value will be inherited from the course taxonomy.</b>',
      'display' => array(
        'default' => array(
          'label' => 'hidden',
          'module' => 'text',
          'settings' => array(),
          'type' => 'text_default',
          'weight' => '0',
        ),
        'teaser' => array(
          'label' => 'hidden',
          'module' => 'text',
          'settings' => array(
            'trim_length' => 600,
          ),
          'type' => 'text_summary_or_trimmed',
          'weight' => 0,
        ),
      ),
      'entity_type' => 'node',
      'field_name' => 'body',
      'label' => 'Body',
      'required' => 0,
      'settings' => array(
        'display_summary' => 1,
        'text_processing' => '1',
        'user_register_form' => FALSE,
      ),
      'widget' => array(
        'active' => 1,
        'module' => 'text',
        'settings' => array(
          'rows' => '20',
          'summary_rows' => 5,
        ),
        'type' => 'text_textarea_with_summary',
        'weight' => '35',
      ),
    ),
  );

  // Exported field: 'node-course_presentation-field_abstract'.
  $fields['node-course_presentation-field_abstract'] = array(
    'field_config' => array(
      'active' => '1',
      'cardinality' => '1',
      'deleted' => '0',
      'entity_types' => array(),
      'field_name' => 'field_abstract',
      'foreign keys' => array(
        'format' => array(
          'columns' => array(
            'format' => 'format',
          ),
          'table' => 'filter_format',
        ),
      ),
      'indexes' => array(
        'format' => array(
          0 => 'format',
        ),
      ),
      'locked' => '0',
      'module' => 'text',
      'settings' => array(
        'max_length' => '140',
      ),
      'translatable' => '0',
      'type' => 'text',
    ),
    'field_instance' => array(
      'bundle' => 'course_presentation',
      'default_value' => NULL,
      'deleted' => '0',
      'description' => 'A short one-sentence description for use in a course list. <b>If empty, the value will be inherited from the course taxonomy.</b>',
      'display' => array(
        'default' => array(
          'label' => 'above',
          'module' => 'text',
          'settings' => array(),
          'type' => 'text_default',
          'weight' => 5,
        ),
        'teaser' => array(
          'label' => 'above',
          'settings' => array(),
          'type' => 'hidden',
          'weight' => 0,
        ),
      ),
      'entity_type' => 'node',
      'field_name' => 'field_abstract',
      'label' => 'Abstract',
      'required' => 0,
      'settings' => array(
        'text_processing' => '0',
        'user_register_form' => FALSE,
      ),
      'widget' => array(
        'active' => 1,
        'module' => 'text',
        'settings' => array(
          'size' => '60',
        ),
        'type' => 'text_textfield',
        'weight' => '32',
      ),
    ),
  );

  // Exported field: 'node-course_presentation-field_age'.
  $fields['node-course_presentation-field_age'] = array(
    'field_config' => array(
      'active' => '1',
      'cardinality' => '1',
      'deleted' => '0',
      'entity_types' => array(),
      'field_name' => 'field_age',
      'foreign keys' => array(
        'format' => array(
          'columns' => array(
            'format' => 'format',
          ),
          'table' => 'filter_format',
        ),
      ),
      'indexes' => array(
        'format' => array(
          0 => 'format',
        ),
      ),
      'locked' => '0',
      'module' => 'text',
      'settings' => array(
        'max_length' => '255',
      ),
      'translatable' => '0',
      'type' => 'text',
    ),
    'field_instance' => array(
      'bundle' => 'course_presentation',
      'default_value' => NULL,
      'deleted' => '0',
      'description' => 'The intended age range for which the presentation is suitable. The value of this element <b>MUST</b> be one of: <i>any</i>, <i>not known</i>, <i>x-y</i>, <i>x+</i> where x and y MUST be non-negative integers.',
      'display' => array(
        'default' => array(
          'label' => 'above',
          'module' => 'text',
          'settings' => array(),
          'type' => 'text_default',
          'weight' => 7,
        ),
        'teaser' => array(
          'label' => 'above',
          'settings' => array(),
          'type' => 'hidden',
          'weight' => 0,
        ),
      ),
      'entity_type' => 'node',
      'field_name' => 'field_age',
      'label' => 'Age',
      'required' => 0,
      'settings' => array(
        'text_processing' => '0',
        'user_register_form' => FALSE,
      ),
      'widget' => array(
        'active' => 1,
        'module' => 'text',
        'settings' => array(
          'size' => '60',
        ),
        'type' => 'text_textfield',
        'weight' => '6',
      ),
    ),
  );

  // Exported field: 'node-course_presentation-field_application_procedure'.
  $fields['node-course_presentation-field_application_procedure'] = array(
    'field_config' => array(
      'active' => '1',
      'cardinality' => '1',
      'deleted' => '0',
      'entity_types' => array(),
      'field_name' => 'field_application_procedure',
      'foreign keys' => array(
        'format' => array(
          'columns' => array(
            'format' => 'format',
          ),
          'table' => 'filter_format',
        ),
      ),
      'indexes' => array(
        'format' => array(
          0 => 'format',
        ),
      ),
      'locked' => '0',
      'module' => 'text',
      'settings' => array(),
      'translatable' => '0',
      'type' => 'text_long',
    ),
    'field_instance' => array(
      'bundle' => 'course_presentation',
      'default_value' => NULL,
      'deleted' => '0',
      'description' => 'Text describing how to apply for the learning opportunity. <b>If empty, the value will be inherited from the course taxonomy.</b>',
      'display' => array(
        'default' => array(
          'label' => 'above',
          'module' => 'text',
          'settings' => array(),
          'type' => 'text_default',
          'weight' => 8,
        ),
        'teaser' => array(
          'label' => 'above',
          'settings' => array(),
          'type' => 'hidden',
          'weight' => 0,
        ),
      ),
      'entity_type' => 'node',
      'field_name' => 'field_application_procedure',
      'label' => 'Application procedure',
      'required' => 0,
      'settings' => array(
        'text_processing' => '1',
        'user_register_form' => FALSE,
      ),
      'widget' => array(
        'active' => 1,
        'module' => 'text',
        'settings' => array(
          'rows' => '15',
        ),
        'type' => 'text_textarea',
        'weight' => '33',
      ),
    ),
  );

  // Exported field: 'node-course_presentation-field_apply'.
  $fields['node-course_presentation-field_apply'] = array(
    'field_config' => array(
      'active' => '1',
      'cardinality' => '1',
      'deleted' => '0',
      'entity_types' => array(),
      'field_name' => 'field_apply',
      'foreign keys' => array(),
      'indexes' => array(),
      'locked' => '0',
      'module' => 'date',
      'settings' => array(
        'cache_count' => '4',
        'cache_enabled' => 0,
        'granularity' => array(
          'day' => 'day',
          'hour' => 0,
          'minute' => 0,
          'month' => 'month',
          'second' => 0,
          'year' => 'year',
        ),
        'timezone_db' => '',
        'todate' => 'optional',
        'tz_handling' => 'none',
      ),
      'translatable' => '0',
      'type' => 'date',
    ),
    'field_instance' => array(
      'bundle' => 'course_presentation',
      'deleted' => '0',
      'description' => 'The date range from which applications can be accepted for the presentation.',
      'display' => array(
        'default' => array(
          'label' => 'above',
          'module' => 'date',
          'settings' => array(
            'format_type' => 'long',
            'fromto' => 'both',
            'multiple_from' => '',
            'multiple_number' => '',
            'multiple_to' => '',
          ),
          'type' => 'date_default',
          'weight' => 9,
        ),
        'teaser' => array(
          'label' => 'above',
          'settings' => array(),
          'type' => 'hidden',
          'weight' => 0,
        ),
      ),
      'entity_type' => 'node',
      'field_name' => 'field_apply',
      'label' => 'Apply',
      'required' => 0,
      'settings' => array(
        'default_value' => 'now',
        'default_value2' => 'same',
        'default_value_code' => '',
        'default_value_code2' => '',
        'user_register_form' => FALSE,
      ),
      'widget' => array(
        'active' => 1,
        'module' => 'date',
        'settings' => array(
          'increment' => '15',
          'input_format' => 'Y-m-d H:i:s',
          'input_format_custom' => '',
          'label_position' => 'above',
          'text_parts' => array(),
          'year_range' => '-3:+3',
        ),
        'type' => 'date_popup',
        'weight' => '9',
      ),
    ),
  );

  // Exported field: 'node-course_presentation-field_apply_to'.
  $fields['node-course_presentation-field_apply_to'] = array(
    'field_config' => array(
      'active' => '1',
      'cardinality' => '1',
      'deleted' => '0',
      'entity_types' => array(),
      'field_name' => 'field_apply_to',
      'foreign keys' => array(),
      'indexes' => array(),
      'locked' => '0',
      'module' => 'link',
      'settings' => array(
        'attributes' => array(
          'class' => '',
          'rel' => '',
          'target' => 'default',
        ),
        'display' => array(
          'url_cutoff' => 80,
        ),
        'enable_tokens' => 1,
        'title' => 'optional',
        'title_maxlength' => 128,
        'title_value' => '',
        'url' => 0,
      ),
      'translatable' => '0',
      'type' => 'link_field',
    ),
    'field_instance' => array(
      'bundle' => 'course_presentation',
      'default_value' => NULL,
      'deleted' => '0',
      'description' => 'A URL to further information about, or an online application system for, the presentation.',
      'display' => array(
        'default' => array(
          'label' => 'above',
          'module' => 'link',
          'settings' => array(),
          'type' => 'link_default',
          'weight' => 10,
        ),
        'teaser' => array(
          'label' => 'above',
          'settings' => array(),
          'type' => 'hidden',
          'weight' => 0,
        ),
      ),
      'entity_type' => 'node',
      'field_name' => 'field_apply_to',
      'label' => 'Apply to',
      'required' => 0,
      'settings' => array(
        'attributes' => array(
          'class' => '',
          'configurable_title' => 0,
          'rel' => '',
          'target' => 'default',
          'title' => '',
        ),
        'display' => array(
          'url_cutoff' => '80',
        ),
        'enable_tokens' => 1,
        'title' => 'none',
        'title_maxlength' => '128',
        'title_value' => '',
        'url' => 0,
        'user_register_form' => FALSE,
        'validate_url' => 1,
      ),
      'widget' => array(
        'active' => 0,
        'module' => 'link',
        'settings' => array(),
        'type' => 'link_field',
        'weight' => '10',
      ),
    ),
  );

  // Exported field: 'node-course_presentation-field_assessment'.
  $fields['node-course_presentation-field_assessment'] = array(
    'field_config' => array(
      'active' => '1',
      'cardinality' => '1',
      'deleted' => '0',
      'entity_types' => array(),
      'field_name' => 'field_assessment',
      'foreign keys' => array(
        'format' => array(
          'columns' => array(
            'format' => 'format',
          ),
          'table' => 'filter_format',
        ),
      ),
      'indexes' => array(
        'format' => array(
          0 => 'format',
        ),
      ),
      'locked' => '0',
      'module' => 'text',
      'settings' => array(),
      'translatable' => '0',
      'type' => 'text_long',
    ),
    'field_instance' => array(
      'bundle' => 'course_presentation',
      'default_value' => NULL,
      'deleted' => '0',
      'description' => 'Text describing the broad approach to assessment used in the learning opportunity. <b>If empty, the value will be inherited from the course taxonomy.</b>',
      'display' => array(
        'default' => array(
          'label' => 'above',
          'module' => 'text',
          'settings' => array(),
          'type' => 'text_default',
          'weight' => 11,
        ),
        'teaser' => array(
          'label' => 'above',
          'settings' => array(),
          'type' => 'hidden',
          'weight' => 0,
        ),
      ),
      'entity_type' => 'node',
      'field_name' => 'field_assessment',
      'label' => 'Assessment',
      'required' => 0,
      'settings' => array(
        'text_processing' => '1',
        'user_register_form' => FALSE,
      ),
      'widget' => array(
        'active' => 1,
        'module' => 'text',
        'settings' => array(
          'rows' => '15',
        ),
        'type' => 'text_textarea',
        'weight' => '34',
      ),
    ),
  );

  // Exported field: 'node-course_presentation-field_attendance_mode'.
  $fields['node-course_presentation-field_attendance_mode'] = array(
    'field_config' => array(
      'active' => '1',
      'cardinality' => '1',
      'deleted' => '0',
      'entity_types' => array(),
      'field_name' => 'field_attendance_mode',
      'foreign keys' => array(),
      'indexes' => array(
        'value' => array(
          0 => 'value',
        ),
      ),
      'locked' => '0',
      'module' => 'list',
      'settings' => array(
        'allowed_values' => array(
          'CM' => 'Campus',
          'DA' => 'Distance with attendance',
          'DS' => 'Distance without attendance',
          'NC' => 'Face-to-face non-campus',
          'MM' => 'Mixed mode',
          'ON' => 'Online (no attendance)',
          'WB' => 'Work-based',
        ),
        'allowed_values_function' => '',
      ),
      'translatable' => '0',
      'type' => 'list_text',
    ),
    'field_instance' => array(
      'bundle' => 'course_presentation',
      'default_value' => NULL,
      'deleted' => '0',
      'description' => 'The type of location at which the student will undertake the presentation.',
      'display' => array(
        'default' => array(
          'label' => 'above',
          'module' => 'list',
          'settings' => array(),
          'type' => 'list_default',
          'weight' => 12,
        ),
        'teaser' => array(
          'label' => 'above',
          'settings' => array(),
          'type' => 'hidden',
          'weight' => 0,
        ),
      ),
      'entity_type' => 'node',
      'field_name' => 'field_attendance_mode',
      'label' => 'Attendance mode',
      'required' => 0,
      'settings' => array(
        'user_register_form' => FALSE,
      ),
      'widget' => array(
        'active' => 1,
        'module' => 'options',
        'settings' => array(),
        'type' => 'options_select',
        'weight' => '11',
      ),
    ),
  );

  // Exported field: 'node-course_presentation-field_attendance_pattern'.
  $fields['node-course_presentation-field_attendance_pattern'] = array(
    'field_config' => array(
      'active' => '1',
      'cardinality' => '1',
      'deleted' => '0',
      'entity_types' => array(),
      'field_name' => 'field_attendance_pattern',
      'foreign keys' => array(),
      'indexes' => array(
        'value' => array(
          0 => 'value',
        ),
      ),
      'locked' => '0',
      'module' => 'list',
      'settings' => array(
        'allowed_values' => array(
          'DT' => 'Daytime',
          'EV' => 'Evening',
          'TW' => 'Twilight',
          'DR' => 'Day/Block release',
          'WE' => 'Weekend',
          'CS' => 'Customised',
        ),
        'allowed_values_function' => '',
      ),
      'translatable' => '0',
      'type' => 'list_text',
    ),
    'field_instance' => array(
      'bundle' => 'course_presentation',
      'default_value' => NULL,
      'deleted' => '0',
      'description' => 'The period in the day and/or frequency during which attendance at a venue is required (if any).',
      'display' => array(
        'default' => array(
          'label' => 'above',
          'module' => 'list',
          'settings' => array(),
          'type' => 'list_default',
          'weight' => 13,
        ),
        'teaser' => array(
          'label' => 'above',
          'settings' => array(),
          'type' => 'hidden',
          'weight' => 0,
        ),
      ),
      'entity_type' => 'node',
      'field_name' => 'field_attendance_pattern',
      'label' => 'Attendance pattern',
      'required' => 0,
      'settings' => array(
        'user_register_form' => FALSE,
      ),
      'widget' => array(
        'active' => 1,
        'module' => 'options',
        'settings' => array(),
        'type' => 'options_select',
        'weight' => '12',
      ),
    ),
  );

  // Exported field: 'node-course_presentation-field_cost'.
  $fields['node-course_presentation-field_cost'] = array(
    'field_config' => array(
      'active' => '1',
      'cardinality' => '1',
      'deleted' => '0',
      'entity_types' => array(),
      'field_name' => 'field_cost',
      'foreign keys' => array(
        'format' => array(
          'columns' => array(
            'format' => 'format',
          ),
          'table' => 'filter_format',
        ),
      ),
      'indexes' => array(
        'format' => array(
          0 => 'format',
        ),
      ),
      'locked' => '0',
      'module' => 'text',
      'settings' => array(),
      'translatable' => '0',
      'type' => 'text_long',
    ),
    'field_instance' => array(
      'bundle' => 'course_presentation',
      'default_value' => NULL,
      'deleted' => '0',
      'description' => 'A textual description of the cost associated with obtaining access to the presentation, including reference to bursaries or other special arrangements for specific groups of learners. <b>"GBP" instead of "£".</b>',
      'display' => array(
        'default' => array(
          'label' => 'above',
          'module' => 'text',
          'settings' => array(),
          'type' => 'text_default',
          'weight' => 14,
        ),
        'teaser' => array(
          'label' => 'above',
          'settings' => array(),
          'type' => 'hidden',
          'weight' => 0,
        ),
      ),
      'entity_type' => 'node',
      'field_name' => 'field_cost',
      'label' => 'Cost',
      'required' => 0,
      'settings' => array(
        'text_processing' => '0',
        'user_register_form' => FALSE,
      ),
      'widget' => array(
        'active' => 1,
        'module' => 'text',
        'settings' => array(
          'rows' => '15',
        ),
        'type' => 'text_textarea',
        'weight' => '13',
      ),
    ),
  );

  // Exported field: 'node-course_presentation-field_course'.
  $fields['node-course_presentation-field_course'] = array(
    'field_config' => array(
      'active' => '1',
      'cardinality' => '1',
      'deleted' => '0',
      'entity_types' => array(),
      'field_name' => 'field_course',
      'foreign keys' => array(
        'tid' => array(
          'columns' => array(
            'tid' => 'tid',
          ),
          'table' => 'taxonomy_term_data',
        ),
      ),
      'indexes' => array(
        'tid' => array(
          0 => 'tid',
        ),
      ),
      'locked' => '0',
      'module' => 'taxonomy',
      'settings' => array(
        'allowed_values' => array(
          0 => array(
            'vocabulary' => 'course',
            'parent' => '0',
          ),
        ),
      ),
      'translatable' => '0',
      'type' => 'taxonomy_term_reference',
    ),
    'field_instance' => array(
      'bundle' => 'course_presentation',
      'default_value' => NULL,
      'deleted' => '0',
      'description' => 'The learning opportunity.',
      'display' => array(
        'default' => array(
          'label' => 'above',
          'module' => 'taxonomy',
          'settings' => array(),
          'type' => 'taxonomy_term_reference_link',
          'weight' => 6,
        ),
        'teaser' => array(
          'label' => 'above',
          'settings' => array(),
          'type' => 'hidden',
          'weight' => 0,
        ),
      ),
      'entity_type' => 'node',
      'field_name' => 'field_course',
      'label' => 'Course',
      'required' => 1,
      'settings' => array(
        'user_register_form' => FALSE,
      ),
      'widget' => array(
        'active' => 1,
        'module' => 'options',
        'settings' => array(),
        'type' => 'options_select',
        'weight' => '2',
      ),
    ),
  );

  // Exported field: 'node-course_presentation-field_duration'.
  $fields['node-course_presentation-field_duration'] = array(
    'field_config' => array(
      'active' => '1',
      'cardinality' => '1',
      'deleted' => '0',
      'entity_types' => array(),
      'field_name' => 'field_duration',
      'foreign keys' => array(
        'format' => array(
          'columns' => array(
            'format' => 'format',
          ),
          'table' => 'filter_format',
        ),
      ),
      'indexes' => array(
        'format' => array(
          0 => 'format',
        ),
      ),
      'locked' => '0',
      'module' => 'text',
      'settings' => array(
        'max_length' => '255',
      ),
      'translatable' => '0',
      'type' => 'text',
    ),
    'field_instance' => array(
      'bundle' => 'course_presentation',
      'default_value' => NULL,
      'deleted' => '0',
      'description' => 'Information about how long the presentation will last.',
      'display' => array(
        'default' => array(
          'label' => 'above',
          'module' => 'text',
          'settings' => array(),
          'type' => 'text_default',
          'weight' => 15,
        ),
        'teaser' => array(
          'label' => 'above',
          'settings' => array(),
          'type' => 'hidden',
          'weight' => 0,
        ),
      ),
      'entity_type' => 'node',
      'field_name' => 'field_duration',
      'label' => 'Duration',
      'required' => 0,
      'settings' => array(
        'text_processing' => '0',
        'user_register_form' => FALSE,
      ),
      'widget' => array(
        'active' => 1,
        'module' => 'text',
        'settings' => array(
          'size' => '60',
        ),
        'type' => 'text_textfield',
        'weight' => '8',
      ),
    ),
  );

  // Exported field: 'node-course_presentation-field_language_of_assessment'.
  $fields['node-course_presentation-field_language_of_assessment'] = array(
    'field_config' => array(
      'active' => '1',
      'cardinality' => '1',
      'deleted' => '0',
      'entity_types' => array(),
      'field_name' => 'field_language_of_assessment',
      'foreign keys' => array(),
      'indexes' => array(
        'value' => array(
          0 => 'value',
        ),
      ),
      'locked' => '0',
      'module' => 'list',
      'settings' => array(
        'allowed_values' => array(
          'ab' => 'Abkhazian',
          'aa' => 'Afar',
          'af' => 'Afrikaans',
          'ak' => 'Akan',
          'sq' => 'Albanian',
          'am' => 'Amharic',
          'ar' => 'Arabic',
          'an' => 'Aragonese',
          'hy' => 'Armenian',
          'as' => 'Assamese',
          'av' => 'Avaric',
          'ae' => 'Avestan',
          'ay' => 'Aymara',
          'az' => 'Azerbaijani',
          'bm' => 'Bambara',
          'ba' => 'Bashkir',
          'eu' => 'Basque',
          'be' => 'Belarusian',
          'bn' => 'Bengali',
          'bh' => 'Bihari languages',
          'bi' => 'Bislama',
          'nb' => 'Norwegian Bokmål',
          'bs' => 'Bosnian',
          'br' => 'Breton',
          'bg' => 'Bulgarian',
          'my' => 'Burmese',
          'es' => 'Spanish',
          'ca' => 'Valencian',
          'km' => 'Central Khmer',
          'ch' => 'Chamorro',
          'ce' => 'Chechen',
          'ny' => 'Nyanja',
          'zh' => 'Chinese',
          'za' => 'Zhuang',
          'cu' => 'Old Slavonic',
          'cv' => 'Chuvash',
          'kw' => 'Cornish',
          'co' => 'Corsican',
          'cr' => 'Cree',
          'hr' => 'Croatian',
          'cs' => 'Czech',
          'da' => 'Danish',
          'dv' => 'Maldivian',
          'nl' => 'Flemish',
          'dz' => 'Dzongkha',
          'en' => 'English',
          'eo' => 'Esperanto',
          'et' => 'Estonian',
          'ee' => 'Ewe',
          'fo' => 'Faroese',
          'fj' => 'Fijian',
          'fi' => 'Finnish',
          'fr' => 'French',
          'ff' => 'Fulah',
          'gd' => 'Scottish Gaelic',
          'gl' => 'Galician',
          'lg' => 'Ganda',
          'ka' => 'Georgian',
          'de' => 'German',
          'ki' => 'Kikuyu',
          'el' => 'Greek, Modern (1453-)',
          'kl' => 'Kalaallisut',
          'gn' => 'Guarani',
          'gu' => 'Gujarati',
          'ht' => 'Haitian Creole',
          'ha' => 'Hausa',
          'he' => 'Hebrew',
          'hz' => 'Herero',
          'hi' => 'Hindi',
          'ho' => 'Hiri Motu',
          'hu' => 'Hungarian',
          'is' => 'Icelandic',
          'io' => 'Ido',
          'ig' => 'Igbo',
          'id' => 'Indonesian',
          'ia' => 'Interlingua (International Auxiliary Language Association)',
          'ie' => 'Occidental',
          'iu' => 'Inuktitut',
          'ik' => 'Inupiaq',
          'ga' => 'Irish',
          'it' => 'Italian',
          'ja' => 'Japanese',
          'jv' => 'Javanese',
          'kn' => 'Kannada',
          'kr' => 'Kanuri',
          'ks' => 'Kashmiri',
          'kk' => 'Kazakh',
          'rw' => 'Kinyarwanda',
          'ky' => 'Kyrgyz',
          'kv' => 'Komi',
          'kg' => 'Kongo',
          'ko' => 'Korean',
          'kj' => 'Kwanyama',
          'ku' => 'Kurdish',
          'lo' => 'Lao',
          'la' => 'Latin',
          'lv' => 'Latvian',
          'lb' => 'Luxembourgish',
          'li' => 'Limburgish',
          'ln' => 'Lingala',
          'lt' => 'Lithuanian',
          'lu' => 'Luba-Katanga',
          'mk' => 'Macedonian',
          'mg' => 'Malagasy',
          'ms' => 'Malay',
          'ml' => 'Malayalam',
          'mt' => 'Maltese',
          'gv' => 'Manx',
          'mi' => 'Maori',
          'mr' => 'Marathi',
          'mh' => 'Marshallese',
          'ro' => 'Romanian',
          'mn' => 'Mongolian',
          'na' => 'Nauru',
          'nv' => 'Navajo',
          'nd' => 'North Ndebele',
          'nr' => 'South Ndebele',
          'ng' => 'Ndonga',
          'ne' => 'Nepali',
          'se' => 'Northern Sami',
          'no' => 'Norwegian',
          'nn' => 'Nynorsk, Norwegian',
          'ii' => 'Sichuan Yi',
          'oc' => 'Occitan (post 1500)',
          'oj' => 'Ojibwa',
          'or' => 'Oriya',
          'om' => 'Oromo',
          'os' => 'Ossetic',
          'pi' => 'Pali',
          'pa' => 'Punjabi',
          'ps' => 'Pushto',
          'fa' => 'Persian',
          'pl' => 'Polish',
          'pt' => 'Portuguese',
          'qu' => 'Quechua',
          'rm' => 'Romansh',
          'rn' => 'Rundi',
          'ru' => 'Russian',
          'sm' => 'Samoan',
          'sg' => 'Sango',
          'sa' => 'Sanskrit',
          'sc' => 'Sardinian',
          'sr' => 'Serbian',
          'sn' => 'Shona',
          'sd' => 'Sindhi',
          'si' => 'Sinhalese',
          'sk' => 'Slovak',
          'sl' => 'Slovenian',
          'so' => 'Somali',
          'st' => 'Sotho, Southern',
          'su' => 'Sundanese',
          'sw' => 'Swahili',
          'ss' => 'Swati',
          'sv' => 'Swedish',
          'tl' => 'Tagalog',
          'ty' => 'Tahitian',
          'tg' => 'Tajik',
          'ta' => 'Tamil',
          'tt' => 'Tatar',
          'te' => 'Telugu',
          'th' => 'Thai',
          'bo' => 'Tibetan',
          'ti' => 'Tigrinya',
          'to' => 'Tonga (Tonga Islands)',
          'ts' => 'Tsonga',
          'tn' => 'Tswana',
          'tr' => 'Turkish',
          'tk' => 'Turkmen',
          'tw' => 'Twi',
          'ug' => 'Uyghur',
          'uk' => 'Ukrainian',
          'ur' => 'Urdu',
          'uz' => 'Uzbek',
          've' => 'Venda',
          'vi' => 'Vietnamese',
          'vo' => 'Volapük',
          'wa' => 'Walloon',
          'cy' => 'Welsh',
          'fy' => 'Western Frisian',
          'wo' => 'Wolof',
          'xh' => 'Xhosa',
          'yi' => 'Yiddish',
          'yo' => 'Yoruba',
          'zu' => 'Zulu',
        ),
        'allowed_values_function' => '',
      ),
      'translatable' => '0',
      'type' => 'list_text',
    ),
    'field_instance' => array(
      'bundle' => 'course_presentation',
      'default_value' => NULL,
      'deleted' => '0',
      'description' => 'A language in which the presentation is assessed.',
      'display' => array(
        'default' => array(
          'label' => 'above',
          'module' => 'list',
          'settings' => array(),
          'type' => 'list_default',
          'weight' => 18,
        ),
        'teaser' => array(
          'label' => 'above',
          'settings' => array(),
          'type' => 'hidden',
          'weight' => 0,
        ),
      ),
      'entity_type' => 'node',
      'field_name' => 'field_language_of_assessment',
      'label' => 'Language of assessment',
      'required' => 0,
      'settings' => array(
        'user_register_form' => FALSE,
      ),
      'widget' => array(
        'active' => 1,
        'module' => 'options',
        'settings' => array(),
        'type' => 'options_select',
        'weight' => '14',
      ),
    ),
  );

  // Exported field: 'node-course_presentation-field_language_of_instruction'.
  $fields['node-course_presentation-field_language_of_instruction'] = array(
    'field_config' => array(
      'active' => '1',
      'cardinality' => '1',
      'deleted' => '0',
      'entity_types' => array(),
      'field_name' => 'field_language_of_instruction',
      'foreign keys' => array(),
      'indexes' => array(
        'value' => array(
          0 => 'value',
        ),
      ),
      'locked' => '0',
      'module' => 'list',
      'settings' => array(
        'allowed_values' => array(
          'ab' => 'Abkhazian',
          'aa' => 'Afar',
          'af' => 'Afrikaans',
          'ak' => 'Akan',
          'sq' => 'Albanian',
          'am' => 'Amharic',
          'ar' => 'Arabic',
          'an' => 'Aragonese',
          'hy' => 'Armenian',
          'as' => 'Assamese',
          'av' => 'Avaric',
          'ae' => 'Avestan',
          'ay' => 'Aymara',
          'az' => 'Azerbaijani',
          'bm' => 'Bambara',
          'ba' => 'Bashkir',
          'eu' => 'Basque',
          'be' => 'Belarusian',
          'bn' => 'Bengali',
          'bh' => 'Bihari languages',
          'bi' => 'Bislama',
          'nb' => 'Norwegian Bokmål',
          'bs' => 'Bosnian',
          'br' => 'Breton',
          'bg' => 'Bulgarian',
          'my' => 'Burmese',
          'es' => 'Spanish',
          'ca' => 'Valencian',
          'km' => 'Central Khmer',
          'ch' => 'Chamorro',
          'ce' => 'Chechen',
          'ny' => 'Nyanja',
          'zh' => 'Chinese',
          'za' => 'Zhuang',
          'cu' => 'Old Slavonic',
          'cv' => 'Chuvash',
          'kw' => 'Cornish',
          'co' => 'Corsican',
          'cr' => 'Cree',
          'hr' => 'Croatian',
          'cs' => 'Czech',
          'da' => 'Danish',
          'dv' => 'Maldivian',
          'nl' => 'Flemish',
          'dz' => 'Dzongkha',
          'en' => 'English',
          'eo' => 'Esperanto',
          'et' => 'Estonian',
          'ee' => 'Ewe',
          'fo' => 'Faroese',
          'fj' => 'Fijian',
          'fi' => 'Finnish',
          'fr' => 'French',
          'ff' => 'Fulah',
          'gd' => 'Scottish Gaelic',
          'gl' => 'Galician',
          'lg' => 'Ganda',
          'ka' => 'Georgian',
          'de' => 'German',
          'ki' => 'Kikuyu',
          'el' => 'Greek, Modern (1453-)',
          'kl' => 'Kalaallisut',
          'gn' => 'Guarani',
          'gu' => 'Gujarati',
          'ht' => 'Haitian Creole',
          'ha' => 'Hausa',
          'he' => 'Hebrew',
          'hz' => 'Herero',
          'hi' => 'Hindi',
          'ho' => 'Hiri Motu',
          'hu' => 'Hungarian',
          'is' => 'Icelandic',
          'io' => 'Ido',
          'ig' => 'Igbo',
          'id' => 'Indonesian',
          'ia' => 'Interlingua (International Auxiliary Language Association)',
          'ie' => 'Occidental',
          'iu' => 'Inuktitut',
          'ik' => 'Inupiaq',
          'ga' => 'Irish',
          'it' => 'Italian',
          'ja' => 'Japanese',
          'jv' => 'Javanese',
          'kn' => 'Kannada',
          'kr' => 'Kanuri',
          'ks' => 'Kashmiri',
          'kk' => 'Kazakh',
          'rw' => 'Kinyarwanda',
          'ky' => 'Kyrgyz',
          'kv' => 'Komi',
          'kg' => 'Kongo',
          'ko' => 'Korean',
          'kj' => 'Kwanyama',
          'ku' => 'Kurdish',
          'lo' => 'Lao',
          'la' => 'Latin',
          'lv' => 'Latvian',
          'lb' => 'Luxembourgish',
          'li' => 'Limburgish',
          'ln' => 'Lingala',
          'lt' => 'Lithuanian',
          'lu' => 'Luba-Katanga',
          'mk' => 'Macedonian',
          'mg' => 'Malagasy',
          'ms' => 'Malay',
          'ml' => 'Malayalam',
          'mt' => 'Maltese',
          'gv' => 'Manx',
          'mi' => 'Maori',
          'mr' => 'Marathi',
          'mh' => 'Marshallese',
          'ro' => 'Romanian',
          'mn' => 'Mongolian',
          'na' => 'Nauru',
          'nv' => 'Navajo',
          'nd' => 'North Ndebele',
          'nr' => 'South Ndebele',
          'ng' => 'Ndonga',
          'ne' => 'Nepali',
          'se' => 'Northern Sami',
          'no' => 'Norwegian',
          'nn' => 'Nynorsk, Norwegian',
          'ii' => 'Sichuan Yi',
          'oc' => 'Occitan (post 1500)',
          'oj' => 'Ojibwa',
          'or' => 'Oriya',
          'om' => 'Oromo',
          'os' => 'Ossetic',
          'pi' => 'Pali',
          'pa' => 'Punjabi',
          'ps' => 'Pushto',
          'fa' => 'Persian',
          'pl' => 'Polish',
          'pt' => 'Portuguese',
          'qu' => 'Quechua',
          'rm' => 'Romansh',
          'rn' => 'Rundi',
          'ru' => 'Russian',
          'sm' => 'Samoan',
          'sg' => 'Sango',
          'sa' => 'Sanskrit',
          'sc' => 'Sardinian',
          'sr' => 'Serbian',
          'sn' => 'Shona',
          'sd' => 'Sindhi',
          'si' => 'Sinhalese',
          'sk' => 'Slovak',
          'sl' => 'Slovenian',
          'so' => 'Somali',
          'st' => 'Sotho, Southern',
          'su' => 'Sundanese',
          'sw' => 'Swahili',
          'ss' => 'Swati',
          'sv' => 'Swedish',
          'tl' => 'Tagalog',
          'ty' => 'Tahitian',
          'tg' => 'Tajik',
          'ta' => 'Tamil',
          'tt' => 'Tatar',
          'te' => 'Telugu',
          'th' => 'Thai',
          'bo' => 'Tibetan',
          'ti' => 'Tigrinya',
          'to' => 'Tonga (Tonga Islands)',
          'ts' => 'Tsonga',
          'tn' => 'Tswana',
          'tr' => 'Turkish',
          'tk' => 'Turkmen',
          'tw' => 'Twi',
          'ug' => 'Uyghur',
          'uk' => 'Ukrainian',
          'ur' => 'Urdu',
          'uz' => 'Uzbek',
          've' => 'Venda',
          'vi' => 'Vietnamese',
          'vo' => 'Volapük',
          'wa' => 'Walloon',
          'cy' => 'Welsh',
          'fy' => 'Western Frisian',
          'wo' => 'Wolof',
          'xh' => 'Xhosa',
          'yi' => 'Yiddish',
          'yo' => 'Yoruba',
          'zu' => 'Zulu',
        ),
        'allowed_values_function' => '',
      ),
      'translatable' => '0',
      'type' => 'list_text',
    ),
    'field_instance' => array(
      'bundle' => 'course_presentation',
      'default_value' => NULL,
      'deleted' => '0',
      'description' => '',
      'display' => array(
        'default' => array(
          'label' => 'above',
          'module' => 'list',
          'settings' => array(),
          'type' => 'list_default',
          'weight' => 19,
        ),
        'teaser' => array(
          'label' => 'above',
          'settings' => array(),
          'type' => 'hidden',
          'weight' => 0,
        ),
      ),
      'entity_type' => 'node',
      'field_name' => 'field_language_of_instruction',
      'label' => 'Language of instruction',
      'required' => 0,
      'settings' => array(
        'user_register_form' => FALSE,
      ),
      'widget' => array(
        'active' => 1,
        'module' => 'options',
        'settings' => array(),
        'type' => 'options_select',
        'weight' => '15',
      ),
    ),
  );

  // Exported field: 'node-course_presentation-field_learning_outcome'.
  $fields['node-course_presentation-field_learning_outcome'] = array(
    'field_config' => array(
      'active' => '1',
      'cardinality' => '1',
      'deleted' => '0',
      'entity_types' => array(),
      'field_name' => 'field_learning_outcome',
      'foreign keys' => array(
        'format' => array(
          'columns' => array(
            'format' => 'format',
          ),
          'table' => 'filter_format',
        ),
      ),
      'indexes' => array(
        'format' => array(
          0 => 'format',
        ),
      ),
      'locked' => '0',
      'module' => 'text',
      'settings' => array(),
      'translatable' => '0',
      'type' => 'text_long',
    ),
    'field_instance' => array(
      'bundle' => 'course_presentation',
      'default_value' => NULL,
      'deleted' => '0',
      'description' => 'Text describing the learning outcomes for a course of study. <b>If empty, the value will be inherited from the course taxonomy.</b>',
      'display' => array(
        'default' => array(
          'label' => 'above',
          'module' => 'text',
          'settings' => array(),
          'type' => 'text_default',
          'weight' => 20,
        ),
        'teaser' => array(
          'label' => 'above',
          'settings' => array(),
          'type' => 'hidden',
          'weight' => 0,
        ),
      ),
      'entity_type' => 'node',
      'field_name' => 'field_learning_outcome',
      'label' => 'Learning outcome',
      'required' => 0,
      'settings' => array(
        'text_processing' => '1',
        'user_register_form' => FALSE,
      ),
      'widget' => array(
        'active' => 1,
        'module' => 'text',
        'settings' => array(
          'rows' => '15',
        ),
        'type' => 'text_textarea',
        'weight' => '36',
      ),
    ),
  );

  // Exported field: 'node-course_presentation-field_objective'.
  $fields['node-course_presentation-field_objective'] = array(
    'field_config' => array(
      'active' => '1',
      'cardinality' => '1',
      'deleted' => '0',
      'entity_types' => array(),
      'field_name' => 'field_objective',
      'foreign keys' => array(
        'format' => array(
          'columns' => array(
            'format' => 'format',
          ),
          'table' => 'filter_format',
        ),
      ),
      'indexes' => array(
        'format' => array(
          0 => 'format',
        ),
      ),
      'locked' => '0',
      'module' => 'text',
      'settings' => array(),
      'translatable' => '0',
      'type' => 'text_long',
    ),
    'field_instance' => array(
      'bundle' => 'course_presentation',
      'default_value' => NULL,
      'deleted' => '0',
      'description' => 'Text describing aims or learning objectives of the learning opportunity. <b>If empty, the value will be inherited from the course taxonomy.</b>',
      'display' => array(
        'default' => array(
          'label' => 'above',
          'module' => 'text',
          'settings' => array(),
          'type' => 'text_default',
          'weight' => 21,
        ),
        'teaser' => array(
          'label' => 'above',
          'settings' => array(),
          'type' => 'hidden',
          'weight' => 0,
        ),
      ),
      'entity_type' => 'node',
      'field_name' => 'field_objective',
      'label' => 'Objective',
      'required' => 0,
      'settings' => array(
        'text_processing' => '1',
        'user_register_form' => FALSE,
      ),
      'widget' => array(
        'active' => 1,
        'module' => 'text',
        'settings' => array(
          'rows' => '15',
        ),
        'type' => 'text_textarea',
        'weight' => '37',
      ),
    ),
  );

  // Exported field: 'node-course_presentation-field_places'.
  $fields['node-course_presentation-field_places'] = array(
    'field_config' => array(
      'active' => '1',
      'cardinality' => '1',
      'deleted' => '0',
      'entity_types' => array(),
      'field_name' => 'field_places',
      'foreign keys' => array(
        'format' => array(
          'columns' => array(
            'format' => 'format',
          ),
          'table' => 'filter_format',
        ),
      ),
      'indexes' => array(
        'format' => array(
          0 => 'format',
        ),
      ),
      'locked' => '0',
      'module' => 'text',
      'settings' => array(
        'max_length' => '255',
      ),
      'translatable' => '0',
      'type' => 'text',
    ),
    'field_instance' => array(
      'bundle' => 'course_presentation',
      'default_value' => NULL,
      'deleted' => '0',
      'description' => 'A textual description of the number of places available for participants in the presentation. MAY be a simple integer.',
      'display' => array(
        'default' => array(
          'label' => 'above',
          'module' => 'text',
          'settings' => array(),
          'type' => 'text_default',
          'weight' => 22,
        ),
        'teaser' => array(
          'label' => 'above',
          'settings' => array(),
          'type' => 'hidden',
          'weight' => 0,
        ),
      ),
      'entity_type' => 'node',
      'field_name' => 'field_places',
      'label' => 'Places',
      'required' => 0,
      'settings' => array(
        'text_processing' => '0',
        'user_register_form' => FALSE,
      ),
      'widget' => array(
        'active' => 1,
        'module' => 'text',
        'settings' => array(
          'size' => '60',
        ),
        'type' => 'text_textfield',
        'weight' => '16',
      ),
    ),
  );

  // Exported field: 'node-course_presentation-field_prerequisite'.
  $fields['node-course_presentation-field_prerequisite'] = array(
    'field_config' => array(
      'active' => '1',
      'cardinality' => '1',
      'deleted' => '0',
      'entity_types' => array(),
      'field_name' => 'field_prerequisite',
      'foreign keys' => array(
        'format' => array(
          'columns' => array(
            'format' => 'format',
          ),
          'table' => 'filter_format',
        ),
      ),
      'indexes' => array(
        'format' => array(
          0 => 'format',
        ),
      ),
      'locked' => '0',
      'module' => 'text',
      'settings' => array(),
      'translatable' => '0',
      'type' => 'text_long',
    ),
    'field_instance' => array(
      'bundle' => 'course_presentation',
      'default_value' => NULL,
      'deleted' => '0',
      'description' => 'Textual description of the entry requirements (academic and / or non-academic, formal or informal) for entry to a learning opportunity. <b>If empty, the value will be inherited from the course taxonomy.</b>',
      'display' => array(
        'default' => array(
          'label' => 'above',
          'module' => 'text',
          'settings' => array(),
          'type' => 'text_default',
          'weight' => 23,
        ),
        'teaser' => array(
          'label' => 'above',
          'settings' => array(),
          'type' => 'hidden',
          'weight' => 0,
        ),
      ),
      'entity_type' => 'node',
      'field_name' => 'field_prerequisite',
      'label' => 'Prerequisite',
      'required' => 0,
      'settings' => array(
        'text_processing' => '1',
        'user_register_form' => FALSE,
      ),
      'widget' => array(
        'active' => 1,
        'module' => 'text',
        'settings' => array(
          'rows' => '15',
        ),
        'type' => 'text_textarea',
        'weight' => '38',
      ),
    ),
  );

  // Exported field: 'node-course_presentation-field_provider'.
  $fields['node-course_presentation-field_provider'] = array(
    'field_config' => array(
      'active' => '1',
      'cardinality' => '-1',
      'deleted' => '0',
      'entity_types' => array(),
      'field_name' => 'field_provider',
      'foreign keys' => array(
        'tid' => array(
          'columns' => array(
            'tid' => 'tid',
          ),
          'table' => 'taxonomy_term_data',
        ),
      ),
      'indexes' => array(
        'tid' => array(
          0 => 'tid',
        ),
      ),
      'locked' => '0',
      'module' => 'taxonomy',
      'settings' => array(
        'allowed_values' => array(
          0 => array(
            'vocabulary' => 'provider',
            'parent' => '0',
          ),
        ),
      ),
      'translatable' => '0',
      'type' => 'taxonomy_term_reference',
    ),
    'field_instance' => array(
      'bundle' => 'course_presentation',
      'default_value' => NULL,
      'deleted' => '0',
      'description' => 'The learning opportunity provider. A provider is any organisation, component of an organisation or partnership that offers one or more learning opportunities.',
      'display' => array(
        'default' => array(
          'label' => 'above',
          'module' => 'taxonomy',
          'settings' => array(),
          'type' => 'taxonomy_term_reference_link',
          'weight' => '2',
        ),
        'teaser' => array(
          'label' => 'above',
          'settings' => array(),
          'type' => 'hidden',
          'weight' => 0,
        ),
      ),
      'entity_type' => 'node',
      'field_name' => 'field_provider',
      'label' => 'Provider',
      'required' => 1,
      'settings' => array(
        'user_register_form' => FALSE,
      ),
      'widget' => array(
        'active' => 1,
        'module' => 'options',
        'settings' => array(),
        'type' => 'options_select',
        'weight' => '1',
      ),
    ),
  );

  // Exported field: 'node-course_presentation-field_qualification'.
  $fields['node-course_presentation-field_qualification'] = array(
    'field_config' => array(
      'active' => '1',
      'cardinality' => '-1',
      'deleted' => '0',
      'entity_types' => array(),
      'field_name' => 'field_qualification',
      'foreign keys' => array(
        'tid' => array(
          'columns' => array(
            'tid' => 'tid',
          ),
          'table' => 'taxonomy_term_data',
        ),
      ),
      'indexes' => array(
        'tid' => array(
          0 => 'tid',
        ),
      ),
      'locked' => '0',
      'module' => 'taxonomy',
      'settings' => array(
        'allowed_values' => array(
          0 => array(
            'vocabulary' => 'qualification',
            'parent' => '0',
          ),
        ),
      ),
      'translatable' => '0',
      'type' => 'taxonomy_term_reference',
    ),
    'field_instance' => array(
      'bundle' => 'course_presentation',
      'default_value' => NULL,
      'deleted' => '0',
      'description' => 'Mandatory only if course leads to qualification. Holds the data relating to a named formal recognition of achievement or competence (award) certified by an awarding body that can result from studying a course.',
      'display' => array(
        'default' => array(
          'label' => 'above',
          'module' => 'taxonomy',
          'settings' => array(),
          'type' => 'taxonomy_term_reference_link',
          'weight' => 3,
        ),
        'teaser' => array(
          'label' => 'above',
          'settings' => array(),
          'type' => 'hidden',
          'weight' => 0,
        ),
      ),
      'entity_type' => 'node',
      'field_name' => 'field_qualification',
      'label' => 'Qualification',
      'required' => 0,
      'settings' => array(
        'user_register_form' => FALSE,
      ),
      'widget' => array(
        'active' => 1,
        'module' => 'options',
        'settings' => array(),
        'type' => 'options_select',
        'weight' => '3',
      ),
    ),
  );

  // Exported field: 'node-course_presentation-field_regulations'.
  $fields['node-course_presentation-field_regulations'] = array(
    'field_config' => array(
      'active' => '1',
      'cardinality' => '1',
      'deleted' => '0',
      'entity_types' => array(),
      'field_name' => 'field_regulations',
      'foreign keys' => array(
        'format' => array(
          'columns' => array(
            'format' => 'format',
          ),
          'table' => 'filter_format',
        ),
      ),
      'indexes' => array(
        'format' => array(
          0 => 'format',
        ),
      ),
      'locked' => '0',
      'module' => 'text',
      'settings' => array(),
      'translatable' => '0',
      'type' => 'text_long',
    ),
    'field_instance' => array(
      'bundle' => 'course_presentation',
      'default_value' => NULL,
      'deleted' => '0',
      'description' => 'Textual description of the organisation\'s academic and administrative regulations relevant to a learning opportunity. <b>Textual description of the organisation\'s academic and administrative regulations relevant to a learning opportunity.</b>',
      'display' => array(
        'default' => array(
          'label' => 'above',
          'module' => 'text',
          'settings' => array(),
          'type' => 'text_default',
          'weight' => 24,
        ),
        'teaser' => array(
          'label' => 'above',
          'settings' => array(),
          'type' => 'hidden',
          'weight' => 0,
        ),
      ),
      'entity_type' => 'node',
      'field_name' => 'field_regulations',
      'label' => 'Regulations',
      'required' => 0,
      'settings' => array(
        'text_processing' => '1',
        'user_register_form' => FALSE,
      ),
      'widget' => array(
        'active' => 1,
        'module' => 'text',
        'settings' => array(
          'rows' => '15',
        ),
        'type' => 'text_textarea',
        'weight' => '39',
      ),
    ),
  );

  // Exported field: 'node-course_presentation-field_start_end'.
  $fields['node-course_presentation-field_start_end'] = array(
    'field_config' => array(
      'active' => '1',
      'cardinality' => '1',
      'deleted' => '0',
      'entity_types' => array(),
      'field_name' => 'field_start_end',
      'foreign keys' => array(),
      'indexes' => array(),
      'locked' => '0',
      'module' => 'date',
      'settings' => array(
        'cache_count' => '4',
        'cache_enabled' => 0,
        'granularity' => array(
          'day' => 'day',
          'hour' => 'hour',
          'minute' => 'minute',
          'month' => 'month',
          'second' => 0,
          'year' => 'year',
        ),
        'timezone_db' => 'UTC',
        'todate' => 'optional',
        'tz_handling' => 'site',
      ),
      'translatable' => '0',
      'type' => 'date',
    ),
    'field_instance' => array(
      'bundle' => 'course_presentation',
      'deleted' => '0',
      'description' => 'Information about the start and end date and/or time of the presentation.',
      'display' => array(
        'default' => array(
          'label' => 'above',
          'module' => 'date',
          'settings' => array(
            'format_type' => 'long',
            'fromto' => 'both',
            'multiple_from' => '',
            'multiple_number' => '',
            'multiple_to' => '',
          ),
          'type' => 'date_default',
          'weight' => 16,
        ),
        'teaser' => array(
          'label' => 'above',
          'settings' => array(),
          'type' => 'hidden',
          'weight' => 0,
        ),
      ),
      'entity_type' => 'node',
      'field_name' => 'field_start_end',
      'label' => 'Start/End',
      'required' => 0,
      'settings' => array(
        'default_value' => 'now',
        'default_value2' => 'same',
        'default_value_code' => '',
        'default_value_code2' => '',
        'user_register_form' => FALSE,
      ),
      'widget' => array(
        'active' => 1,
        'module' => 'date',
        'settings' => array(
          'increment' => '15',
          'input_format' => 'Y-m-d H:i:s',
          'input_format_custom' => '',
          'label_position' => 'above',
          'text_parts' => array(),
          'year_range' => '-3:+3',
        ),
        'type' => 'date_popup',
        'weight' => '7',
      ),
    ),
  );

  // Exported field: 'node-course_presentation-field_study_mode'.
  $fields['node-course_presentation-field_study_mode'] = array(
    'field_config' => array(
      'active' => '1',
      'cardinality' => '1',
      'deleted' => '0',
      'entity_types' => array(),
      'field_name' => 'field_study_mode',
      'foreign keys' => array(),
      'indexes' => array(
        'value' => array(
          0 => 'value',
        ),
      ),
      'locked' => '0',
      'module' => 'list',
      'settings' => array(
        'allowed_values' => array(
          'NK' => 'Not known',
          'FL' => 'Flexible',
          'FT' => 'Full time',
          'PF' => 'Part of a full time programme',
          'PT' => 'Part time',
        ),
        'allowed_values_function' => '',
      ),
      'translatable' => '0',
      'type' => 'list_text',
    ),
    'field_instance' => array(
      'bundle' => 'course_presentation',
      'default_value' => array(
        0 => array(
          'value' => 'NK',
        ),
      ),
      'deleted' => '0',
      'description' => '',
      'display' => array(
        'default' => array(
          'label' => 'above',
          'module' => 'list',
          'settings' => array(),
          'type' => 'list_default',
          'weight' => 25,
        ),
        'teaser' => array(
          'label' => 'above',
          'settings' => array(),
          'type' => 'hidden',
          'weight' => 0,
        ),
      ),
      'entity_type' => 'node',
      'field_name' => 'field_study_mode',
      'label' => 'Study mode',
      'required' => 0,
      'settings' => array(
        'user_register_form' => FALSE,
      ),
      'widget' => array(
        'active' => 1,
        'module' => 'options',
        'settings' => array(),
        'type' => 'options_select',
        'weight' => '17',
      ),
    ),
  );

  // Exported field: 'node-course_presentation-field_subject'.
  $fields['node-course_presentation-field_subject'] = array(
    'field_config' => array(
      'active' => '1',
      'cardinality' => '-1',
      'deleted' => '0',
      'entity_types' => array(),
      'field_name' => 'field_subject',
      'foreign keys' => array(
        'format' => array(
          'columns' => array(
            'format' => 'format',
          ),
          'table' => 'filter_format',
        ),
      ),
      'indexes' => array(
        'format' => array(
          0 => 'format',
        ),
      ),
      'locked' => '0',
      'module' => 'text',
      'settings' => array(
        'max_length' => '100',
      ),
      'translatable' => '0',
      'type' => 'text',
    ),
    'field_instance' => array(
      'bundle' => 'course_presentation',
      'default_value' => NULL,
      'deleted' => '0',
      'description' => 'The topic of the course. <b>If empty, the value will be inherited from the course taxonomy.</b>',
      'display' => array(
        'default' => array(
          'label' => 'above',
          'module' => 'text',
          'settings' => array(),
          'type' => 'text_default',
          'weight' => 26,
        ),
        'teaser' => array(
          'label' => 'above',
          'settings' => array(),
          'type' => 'hidden',
          'weight' => 0,
        ),
      ),
      'entity_type' => 'node',
      'field_name' => 'field_subject',
      'label' => 'Subject',
      'required' => 0,
      'settings' => array(
        'text_processing' => '0',
        'user_register_form' => FALSE,
      ),
      'widget' => array(
        'active' => 1,
        'module' => 'text',
        'settings' => array(
          'size' => '60',
        ),
        'type' => 'text_textfield',
        'weight' => '40',
      ),
    ),
  );

  // Exported field: 'node-course_presentation-field_venue'.
  $fields['node-course_presentation-field_venue'] = array(
    'field_config' => array(
      'active' => '1',
      'cardinality' => '-1',
      'deleted' => '0',
      'entity_types' => array(),
      'field_name' => 'field_venue',
      'foreign keys' => array(
        'tid' => array(
          'columns' => array(
            'tid' => 'tid',
          ),
          'table' => 'taxonomy_term_data',
        ),
      ),
      'indexes' => array(
        'tid' => array(
          0 => 'tid',
        ),
      ),
      'locked' => '0',
      'module' => 'taxonomy',
      'settings' => array(
        'allowed_values' => array(
          0 => array(
            'vocabulary' => 'venue',
            'parent' => '0',
          ),
        ),
      ),
      'translatable' => '0',
      'type' => 'taxonomy_term_reference',
    ),
    'field_instance' => array(
      'bundle' => 'course_presentation',
      'default_value' => NULL,
      'deleted' => '0',
      'description' => ' Holds the information relating to the main locations where a learning opportunity is presented.',
      'display' => array(
        'default' => array(
          'label' => 'above',
          'module' => 'taxonomy',
          'settings' => array(),
          'type' => 'taxonomy_term_reference_link',
          'weight' => 28,
        ),
        'teaser' => array(
          'label' => 'above',
          'settings' => array(),
          'type' => 'hidden',
          'weight' => 0,
        ),
      ),
      'entity_type' => 'node',
      'field_name' => 'field_venue',
      'label' => 'Venue',
      'required' => 0,
      'settings' => array(
        'user_register_form' => FALSE,
      ),
      'widget' => array(
        'active' => 1,
        'module' => 'options',
        'settings' => array(),
        'type' => 'options_select',
        'weight' => '4',
      ),
    ),
  );

  // Exported field: 'taxonomy_term-course-field_abstract'.
  $fields['taxonomy_term-course-field_abstract'] = array(
    'field_config' => array(
      'active' => '1',
      'cardinality' => '1',
      'deleted' => '0',
      'entity_types' => array(),
      'field_name' => 'field_abstract',
      'foreign keys' => array(
        'format' => array(
          'columns' => array(
            'format' => 'format',
          ),
          'table' => 'filter_format',
        ),
      ),
      'indexes' => array(
        'format' => array(
          0 => 'format',
        ),
      ),
      'locked' => '0',
      'module' => 'text',
      'settings' => array(
        'max_length' => '140',
      ),
      'translatable' => '0',
      'type' => 'text',
    ),
    'field_instance' => array(
      'bundle' => 'course',
      'default_value' => NULL,
      'deleted' => '0',
      'description' => 'Preferred. A short one-sentence description for use in a course list.',
      'display' => array(
        'default' => array(
          'label' => 'above',
          'module' => 'text',
          'settings' => array(),
          'type' => 'text_default',
          'weight' => 0,
        ),
      ),
      'entity_type' => 'taxonomy_term',
      'field_name' => 'field_abstract',
      'label' => 'Abstract',
      'required' => 0,
      'settings' => array(
        'text_processing' => '0',
        'user_register_form' => FALSE,
      ),
      'widget' => array(
        'active' => 1,
        'module' => 'text',
        'settings' => array(
          'size' => '60',
        ),
        'type' => 'text_textfield',
        'weight' => '1.01',
      ),
    ),
  );

  // Exported field: 'taxonomy_term-course-field_application_procedure'.
  $fields['taxonomy_term-course-field_application_procedure'] = array(
    'field_config' => array(
      'active' => '1',
      'cardinality' => '1',
      'deleted' => '0',
      'entity_types' => array(),
      'field_name' => 'field_application_procedure',
      'foreign keys' => array(
        'format' => array(
          'columns' => array(
            'format' => 'format',
          ),
          'table' => 'filter_format',
        ),
      ),
      'indexes' => array(
        'format' => array(
          0 => 'format',
        ),
      ),
      'locked' => '0',
      'module' => 'text',
      'settings' => array(),
      'translatable' => '0',
      'type' => 'text_long',
    ),
    'field_instance' => array(
      'bundle' => 'course',
      'default_value' => NULL,
      'deleted' => '0',
      'description' => 'Text describing how to apply for the learning opportunity.',
      'display' => array(
        'default' => array(
          'label' => 'above',
          'module' => 'text',
          'settings' => array(),
          'type' => 'text_default',
          'weight' => 1,
        ),
      ),
      'entity_type' => 'taxonomy_term',
      'field_name' => 'field_application_procedure',
      'label' => 'Application procedure',
      'required' => 0,
      'settings' => array(
        'text_processing' => '1',
        'user_register_form' => FALSE,
      ),
      'widget' => array(
        'active' => 1,
        'module' => 'text',
        'settings' => array(
          'rows' => '15',
        ),
        'type' => 'text_textarea',
        'weight' => '1.02',
      ),
    ),
  );

  // Exported field: 'taxonomy_term-course-field_assessment'.
  $fields['taxonomy_term-course-field_assessment'] = array(
    'field_config' => array(
      'active' => '1',
      'cardinality' => '1',
      'deleted' => '0',
      'entity_types' => array(),
      'field_name' => 'field_assessment',
      'foreign keys' => array(
        'format' => array(
          'columns' => array(
            'format' => 'format',
          ),
          'table' => 'filter_format',
        ),
      ),
      'indexes' => array(
        'format' => array(
          0 => 'format',
        ),
      ),
      'locked' => '0',
      'module' => 'text',
      'settings' => array(),
      'translatable' => '0',
      'type' => 'text_long',
    ),
    'field_instance' => array(
      'bundle' => 'course',
      'default_value' => NULL,
      'deleted' => '0',
      'description' => 'Text describing the broad approach to assessment used in the learning opportunity.',
      'display' => array(
        'default' => array(
          'label' => 'above',
          'module' => 'text',
          'settings' => array(),
          'type' => 'text_default',
          'weight' => 2,
        ),
      ),
      'entity_type' => 'taxonomy_term',
      'field_name' => 'field_assessment',
      'label' => 'Assessment',
      'required' => 0,
      'settings' => array(
        'text_processing' => '1',
        'user_register_form' => FALSE,
      ),
      'widget' => array(
        'active' => 1,
        'module' => 'text',
        'settings' => array(
          'rows' => '15',
        ),
        'type' => 'text_textarea',
        'weight' => '1.03',
      ),
    ),
  );

  // Exported field: 'taxonomy_term-course-field_credit'.
  $fields['taxonomy_term-course-field_credit'] = array(
    'field_config' => array(
      'active' => '1',
      'cardinality' => '-1',
      'deleted' => '0',
      'entity_types' => array(),
      'field_name' => 'field_credit',
      'foreign keys' => array(),
      'indexes' => array(),
      'locked' => '0',
      'module' => 'field_collection',
      'settings' => array(
        'path' => '',
      ),
      'translatable' => '0',
      'type' => 'field_collection',
    ),
    'field_instance' => array(
      'bundle' => 'course',
      'default_value' => NULL,
      'deleted' => '0',
      'description' => 'An account of the credits that can be obtained from completion of a course.',
      'display' => array(
        'default' => array(
          'label' => 'above',
          'module' => 'field_collection',
          'settings' => array(
            'add' => 'Add',
            'delete' => 'Delete',
            'description' => TRUE,
            'edit' => 'Edit',
            'view_mode' => 'full',
          ),
          'type' => 'field_collection_view',
          'weight' => 3,
        ),
      ),
      'entity_type' => 'taxonomy_term',
      'field_name' => 'field_credit',
      'label' => 'Credit',
      'required' => 0,
      'settings' => array(
        'user_register_form' => FALSE,
      ),
      'widget' => array(
        'active' => 0,
        'module' => 'field_collection',
        'settings' => array(),
        'type' => 'field_collection_embed',
        'weight' => '1.04',
      ),
    ),
  );

  // Exported field: 'taxonomy_term-course-field_learning_outcome'.
  $fields['taxonomy_term-course-field_learning_outcome'] = array(
    'field_config' => array(
      'active' => '1',
      'cardinality' => '1',
      'deleted' => '0',
      'entity_types' => array(),
      'field_name' => 'field_learning_outcome',
      'foreign keys' => array(
        'format' => array(
          'columns' => array(
            'format' => 'format',
          ),
          'table' => 'filter_format',
        ),
      ),
      'indexes' => array(
        'format' => array(
          0 => 'format',
        ),
      ),
      'locked' => '0',
      'module' => 'text',
      'settings' => array(),
      'translatable' => '0',
      'type' => 'text_long',
    ),
    'field_instance' => array(
      'bundle' => 'course',
      'default_value' => NULL,
      'deleted' => '0',
      'description' => 'Text describing the learning outcomes for a course of study.',
      'display' => array(
        'default' => array(
          'label' => 'above',
          'module' => 'text',
          'settings' => array(),
          'type' => 'text_default',
          'weight' => 5,
        ),
      ),
      'entity_type' => 'taxonomy_term',
      'field_name' => 'field_learning_outcome',
      'label' => 'Learning outcome',
      'required' => 0,
      'settings' => array(
        'text_processing' => '1',
        'user_register_form' => FALSE,
      ),
      'widget' => array(
        'active' => 1,
        'module' => 'text',
        'settings' => array(
          'rows' => '15',
        ),
        'type' => 'text_textarea',
        'weight' => '1.05',
      ),
    ),
  );

  // Exported field: 'taxonomy_term-course-field_objective'.
  $fields['taxonomy_term-course-field_objective'] = array(
    'field_config' => array(
      'active' => '1',
      'cardinality' => '1',
      'deleted' => '0',
      'entity_types' => array(),
      'field_name' => 'field_objective',
      'foreign keys' => array(
        'format' => array(
          'columns' => array(
            'format' => 'format',
          ),
          'table' => 'filter_format',
        ),
      ),
      'indexes' => array(
        'format' => array(
          0 => 'format',
        ),
      ),
      'locked' => '0',
      'module' => 'text',
      'settings' => array(),
      'translatable' => '0',
      'type' => 'text_long',
    ),
    'field_instance' => array(
      'bundle' => 'course',
      'default_value' => NULL,
      'deleted' => '0',
      'description' => 'Text describing aims or learning objectives of the learning opportunity.',
      'display' => array(
        'default' => array(
          'label' => 'above',
          'module' => 'text',
          'settings' => array(),
          'type' => 'text_default',
          'weight' => 6,
        ),
      ),
      'entity_type' => 'taxonomy_term',
      'field_name' => 'field_objective',
      'label' => 'Objective',
      'required' => 0,
      'settings' => array(
        'text_processing' => '1',
        'user_register_form' => FALSE,
      ),
      'widget' => array(
        'active' => 1,
        'module' => 'text',
        'settings' => array(
          'rows' => '15',
        ),
        'type' => 'text_textarea',
        'weight' => '1.06',
      ),
    ),
  );

  // Exported field: 'taxonomy_term-course-field_prerequisite'.
  $fields['taxonomy_term-course-field_prerequisite'] = array(
    'field_config' => array(
      'active' => '1',
      'cardinality' => '1',
      'deleted' => '0',
      'entity_types' => array(),
      'field_name' => 'field_prerequisite',
      'foreign keys' => array(
        'format' => array(
          'columns' => array(
            'format' => 'format',
          ),
          'table' => 'filter_format',
        ),
      ),
      'indexes' => array(
        'format' => array(
          0 => 'format',
        ),
      ),
      'locked' => '0',
      'module' => 'text',
      'settings' => array(),
      'translatable' => '0',
      'type' => 'text_long',
    ),
    'field_instance' => array(
      'bundle' => 'course',
      'default_value' => NULL,
      'deleted' => '0',
      'description' => 'Textual description of the entry requirements (academic and / or non-academic, formal or informal) for entry to a learning opportunity.',
      'display' => array(
        'default' => array(
          'label' => 'above',
          'module' => 'text',
          'settings' => array(),
          'type' => 'text_default',
          'weight' => 7,
        ),
      ),
      'entity_type' => 'taxonomy_term',
      'field_name' => 'field_prerequisite',
      'label' => 'Prerequisite',
      'required' => 0,
      'settings' => array(
        'text_processing' => '1',
        'user_register_form' => FALSE,
      ),
      'widget' => array(
        'active' => 1,
        'module' => 'text',
        'settings' => array(
          'rows' => '15',
        ),
        'type' => 'text_textarea',
        'weight' => '1.07',
      ),
    ),
  );

  // Exported field: 'taxonomy_term-course-field_regulations'.
  $fields['taxonomy_term-course-field_regulations'] = array(
    'field_config' => array(
      'active' => '1',
      'cardinality' => '1',
      'deleted' => '0',
      'entity_types' => array(),
      'field_name' => 'field_regulations',
      'foreign keys' => array(
        'format' => array(
          'columns' => array(
            'format' => 'format',
          ),
          'table' => 'filter_format',
        ),
      ),
      'indexes' => array(
        'format' => array(
          0 => 'format',
        ),
      ),
      'locked' => '0',
      'module' => 'text',
      'settings' => array(),
      'translatable' => '0',
      'type' => 'text_long',
    ),
    'field_instance' => array(
      'bundle' => 'course',
      'default_value' => NULL,
      'deleted' => '0',
      'description' => 'Textual description of the organisation\'s academic and administrative regulations relevant to a learning opportunity.',
      'display' => array(
        'default' => array(
          'label' => 'above',
          'module' => 'text',
          'settings' => array(),
          'type' => 'text_default',
          'weight' => 8,
        ),
      ),
      'entity_type' => 'taxonomy_term',
      'field_name' => 'field_regulations',
      'label' => 'Regulations',
      'required' => 0,
      'settings' => array(
        'text_processing' => '1',
        'user_register_form' => FALSE,
      ),
      'widget' => array(
        'active' => 1,
        'module' => 'text',
        'settings' => array(
          'rows' => '15',
        ),
        'type' => 'text_textarea',
        'weight' => '1.08',
      ),
    ),
  );

  // Exported field: 'taxonomy_term-course-field_subject'.
  $fields['taxonomy_term-course-field_subject'] = array(
    'field_config' => array(
      'active' => '1',
      'cardinality' => '-1',
      'deleted' => '0',
      'entity_types' => array(),
      'field_name' => 'field_subject',
      'foreign keys' => array(
        'format' => array(
          'columns' => array(
            'format' => 'format',
          ),
          'table' => 'filter_format',
        ),
      ),
      'indexes' => array(
        'format' => array(
          0 => 'format',
        ),
      ),
      'locked' => '0',
      'module' => 'text',
      'settings' => array(
        'max_length' => '100',
      ),
      'translatable' => '0',
      'type' => 'text',
    ),
    'field_instance' => array(
      'bundle' => 'course',
      'default_value' => NULL,
      'deleted' => '0',
      'description' => 'The topic of the course.',
      'display' => array(
        'default' => array(
          'label' => 'above',
          'module' => 'text',
          'settings' => array(),
          'type' => 'text_default',
          'weight' => 9,
        ),
      ),
      'entity_type' => 'taxonomy_term',
      'field_name' => 'field_subject',
      'label' => 'Subject',
      'required' => 1,
      'settings' => array(
        'text_processing' => '0',
        'user_register_form' => FALSE,
      ),
      'widget' => array(
        'active' => 1,
        'module' => 'text',
        'settings' => array(
          'size' => '60',
        ),
        'type' => 'text_textfield',
        'weight' => '1.09',
      ),
    ),
  );

  // Exported field: 'taxonomy_term-course-field_type'.
  $fields['taxonomy_term-course-field_type'] = array(
    'field_config' => array(
      'active' => '1',
      'cardinality' => '1',
      'deleted' => '0',
      'entity_types' => array(),
      'field_name' => 'field_type',
      'foreign keys' => array(
        'format' => array(
          'columns' => array(
            'format' => 'format',
          ),
          'table' => 'filter_format',
        ),
      ),
      'indexes' => array(
        'format' => array(
          0 => 'format',
        ),
      ),
      'locked' => '0',
      'module' => 'text',
      'settings' => array(
        'max_length' => '50',
      ),
      'translatable' => '0',
      'type' => 'text',
    ),
    'field_instance' => array(
      'bundle' => 'course',
      'default_value' => NULL,
      'deleted' => '0',
      'description' => 'A grouping of similar courses in terms of target audience.',
      'display' => array(
        'default' => array(
          'label' => 'above',
          'module' => 'text',
          'settings' => array(),
          'type' => 'text_default',
          'weight' => 10,
        ),
      ),
      'entity_type' => 'taxonomy_term',
      'field_name' => 'field_type',
      'label' => 'Type',
      'required' => 0,
      'settings' => array(
        'text_processing' => '0',
        'user_register_form' => FALSE,
      ),
      'widget' => array(
        'active' => 1,
        'module' => 'text',
        'settings' => array(
          'size' => '60',
        ),
        'type' => 'text_textfield',
        'weight' => '1.10',
      ),
    ),
  );

  // Exported field: 'taxonomy_term-course-field_url'.
  $fields['taxonomy_term-course-field_url'] = array(
    'field_config' => array(
      'active' => '1',
      'cardinality' => '1',
      'deleted' => '0',
      'entity_types' => array(),
      'field_name' => 'field_url',
      'foreign keys' => array(),
      'indexes' => array(),
      'locked' => '0',
      'module' => 'link',
      'settings' => array(
        'attributes' => array(
          'class' => '',
          'rel' => '',
          'target' => 'default',
        ),
        'display' => array(
          'url_cutoff' => 80,
        ),
        'enable_tokens' => 1,
        'title' => 'optional',
        'title_maxlength' => 128,
        'title_value' => '',
        'url' => 0,
      ),
      'translatable' => '0',
      'type' => 'link_field',
    ),
    'field_instance' => array(
      'bundle' => 'course',
      'default_value' => NULL,
      'deleted' => '0',
      'description' => 'You can specify a course URL if it\'s different from the drupal page for this entity, otherwise you can leave this field blank.',
      'display' => array(
        'default' => array(
          'label' => 'above',
          'module' => 'link',
          'settings' => array(),
          'type' => 'link_default',
          'weight' => 11,
        ),
      ),
      'entity_type' => 'taxonomy_term',
      'field_name' => 'field_url',
      'label' => 'URL',
      'required' => 0,
      'settings' => array(
        'attributes' => array(
          'class' => '',
          'configurable_title' => 0,
          'rel' => '',
          'target' => 'default',
          'title' => '',
        ),
        'display' => array(
          'url_cutoff' => '80',
        ),
        'enable_tokens' => 1,
        'title' => 'none',
        'title_maxlength' => '128',
        'title_value' => '',
        'url' => 0,
        'user_register_form' => FALSE,
        'validate_url' => 1,
      ),
      'widget' => array(
        'active' => 0,
        'module' => 'link',
        'settings' => array(),
        'type' => 'link_field',
        'weight' => '1.11',
      ),
    ),
  );

  // Exported field: 'taxonomy_term-provider-field_address'.
  $fields['taxonomy_term-provider-field_address'] = array(
    'field_config' => array(
      'active' => '1',
      'cardinality' => '1',
      'deleted' => '0',
      'entity_types' => array(),
      'field_name' => 'field_address',
      'foreign keys' => array(
        'format' => array(
          'columns' => array(
            'format' => 'format',
          ),
          'table' => 'filter_format',
        ),
      ),
      'indexes' => array(
        'format' => array(
          0 => 'format',
        ),
      ),
      'locked' => '0',
      'module' => 'text',
      'settings' => array(),
      'translatable' => '0',
      'type' => 'text_long',
    ),
    'field_instance' => array(
      'bundle' => 'provider',
      'default_value' => NULL,
      'deleted' => '0',
      'description' => 'Specifies the geographical or spatial location of the provider\'s main administrative site.',
      'display' => array(
        'default' => array(
          'label' => 'above',
          'module' => 'text',
          'settings' => array(),
          'type' => 'text_default',
          'weight' => 0,
        ),
      ),
      'entity_type' => 'taxonomy_term',
      'field_name' => 'field_address',
      'label' => 'Address',
      'required' => 0,
      'settings' => array(
        'text_processing' => '0',
        'user_register_form' => FALSE,
      ),
      'widget' => array(
        'active' => 1,
        'module' => 'text',
        'settings' => array(
          'rows' => '5',
        ),
        'type' => 'text_textarea',
        'weight' => '31',
      ),
    ),
  );

  // Exported field: 'taxonomy_term-provider-field_email'.
  $fields['taxonomy_term-provider-field_email'] = array(
    'field_config' => array(
      'active' => '1',
      'cardinality' => '1',
      'deleted' => '0',
      'entity_types' => array(),
      'field_name' => 'field_email',
      'foreign keys' => array(),
      'indexes' => array(),
      'locked' => '0',
      'module' => 'email',
      'settings' => array(),
      'translatable' => '0',
      'type' => 'email',
    ),
    'field_instance' => array(
      'bundle' => 'provider',
      'default_value' => NULL,
      'deleted' => '0',
      'description' => 'Email address of the provider\'s main administrative site.',
      'display' => array(
        'default' => array(
          'label' => 'above',
          'module' => 'email',
          'settings' => array(),
          'type' => 'email_default',
          'weight' => 1,
        ),
      ),
      'entity_type' => 'taxonomy_term',
      'field_name' => 'field_email',
      'label' => 'Email',
      'required' => 0,
      'settings' => array(
        'user_register_form' => FALSE,
      ),
      'widget' => array(
        'active' => 1,
        'module' => 'email',
        'settings' => array(
          'size' => '60',
        ),
        'type' => 'email_textfield',
        'weight' => '32',
      ),
    ),
  );

  // Exported field: 'taxonomy_term-provider-field_fax'.
  $fields['taxonomy_term-provider-field_fax'] = array(
    'field_config' => array(
      'active' => '1',
      'cardinality' => '1',
      'deleted' => '0',
      'entity_types' => array(),
      'field_name' => 'field_fax',
      'foreign keys' => array(
        'format' => array(
          'columns' => array(
            'format' => 'format',
          ),
          'table' => 'filter_format',
        ),
      ),
      'indexes' => array(
        'format' => array(
          0 => 'format',
        ),
      ),
      'locked' => '0',
      'module' => 'text',
      'settings' => array(
        'max_length' => '255',
      ),
      'translatable' => '0',
      'type' => 'text',
    ),
    'field_instance' => array(
      'bundle' => 'provider',
      'default_value' => NULL,
      'deleted' => '0',
      'description' => 'Fax number of the provider\'s main administrative site. UK National Format Layout (Area Code and Number).',
      'display' => array(
        'default' => array(
          'label' => 'above',
          'module' => 'text',
          'settings' => array(),
          'type' => 'text_default',
          'weight' => 2,
        ),
      ),
      'entity_type' => 'taxonomy_term',
      'field_name' => 'field_fax',
      'label' => 'Fax',
      'required' => 0,
      'settings' => array(
        'text_processing' => '0',
        'user_register_form' => FALSE,
      ),
      'widget' => array(
        'active' => 1,
        'module' => 'text',
        'settings' => array(
          'size' => '60',
        ),
        'type' => 'text_textfield',
        'weight' => '34',
      ),
    ),
  );

  // Exported field: 'taxonomy_term-provider-field_location_url'.
  $fields['taxonomy_term-provider-field_location_url'] = array(
    'field_config' => array(
      'active' => '1',
      'cardinality' => '1',
      'deleted' => '0',
      'entity_types' => array(),
      'field_name' => 'field_location_url',
      'foreign keys' => array(),
      'indexes' => array(),
      'locked' => '0',
      'module' => 'link',
      'settings' => array(
        'attributes' => array(
          'class' => '',
          'rel' => '',
          'target' => 'default',
        ),
        'display' => array(
          'url_cutoff' => 80,
        ),
        'enable_tokens' => 1,
        'title' => 'optional',
        'title_maxlength' => 128,
        'title_value' => '',
        'url' => 0,
      ),
      'translatable' => '0',
      'type' => 'link_field',
    ),
    'field_instance' => array(
      'bundle' => 'provider',
      'default_value' => NULL,
      'deleted' => '0',
      'description' => 'A URL with information relating to the location of the provider, for example a map or directions.',
      'display' => array(
        'default' => array(
          'label' => 'above',
          'module' => 'link',
          'settings' => array(),
          'type' => 'link_default',
          'weight' => 7,
        ),
      ),
      'entity_type' => 'taxonomy_term',
      'field_name' => 'field_location_url',
      'label' => 'URL',
      'required' => 0,
      'settings' => array(
        'attributes' => array(
          'class' => '',
          'configurable_title' => 0,
          'rel' => '',
          'target' => 'default',
          'title' => '',
        ),
        'display' => array(
          'url_cutoff' => '80',
        ),
        'enable_tokens' => 1,
        'title' => 'none',
        'title_maxlength' => '255',
        'title_value' => '',
        'url' => 0,
        'user_register_form' => FALSE,
        'validate_url' => 1,
      ),
      'widget' => array(
        'active' => 0,
        'module' => 'link',
        'settings' => array(),
        'type' => 'link_field',
        'weight' => '37',
      ),
    ),
  );

  // Exported field: 'taxonomy_term-provider-field_phone'.
  $fields['taxonomy_term-provider-field_phone'] = array(
    'field_config' => array(
      'active' => '1',
      'cardinality' => '1',
      'deleted' => '0',
      'entity_types' => array(),
      'field_name' => 'field_phone',
      'foreign keys' => array(
        'format' => array(
          'columns' => array(
            'format' => 'format',
          ),
          'table' => 'filter_format',
        ),
      ),
      'indexes' => array(
        'format' => array(
          0 => 'format',
        ),
      ),
      'locked' => '0',
      'module' => 'text',
      'settings' => array(
        'max_length' => '255',
      ),
      'translatable' => '0',
      'type' => 'text',
    ),
    'field_instance' => array(
      'bundle' => 'provider',
      'default_value' => NULL,
      'deleted' => '0',
      'description' => 'Main telephone number of the provider\'s main administrative site. UK National Format Layout (Area Code and Number).',
      'display' => array(
        'default' => array(
          'label' => 'above',
          'module' => 'text',
          'settings' => array(),
          'type' => 'text_default',
          'weight' => 3,
        ),
      ),
      'entity_type' => 'taxonomy_term',
      'field_name' => 'field_phone',
      'label' => 'Phone',
      'required' => 0,
      'settings' => array(
        'text_processing' => '0',
        'user_register_form' => FALSE,
      ),
      'widget' => array(
        'active' => 1,
        'module' => 'text',
        'settings' => array(
          'size' => '60',
        ),
        'type' => 'text_textfield',
        'weight' => '33',
      ),
    ),
  );

  // Exported field: 'taxonomy_term-provider-field_postcode'.
  $fields['taxonomy_term-provider-field_postcode'] = array(
    'field_config' => array(
      'active' => '1',
      'cardinality' => '1',
      'deleted' => '0',
      'entity_types' => array(),
      'field_name' => 'field_postcode',
      'foreign keys' => array(
        'format' => array(
          'columns' => array(
            'format' => 'format',
          ),
          'table' => 'filter_format',
        ),
      ),
      'indexes' => array(
        'format' => array(
          0 => 'format',
        ),
      ),
      'locked' => '0',
      'module' => 'text',
      'settings' => array(
        'max_length' => '255',
      ),
      'translatable' => '0',
      'type' => 'text',
    ),
    'field_instance' => array(
      'bundle' => 'provider',
      'default_value' => NULL,
      'deleted' => '0',
      'description' => 'Full official postcode of the address of the provider\'s main site.',
      'display' => array(
        'default' => array(
          'label' => 'above',
          'module' => 'text',
          'settings' => array(),
          'type' => 'text_default',
          'weight' => 4,
        ),
      ),
      'entity_type' => 'taxonomy_term',
      'field_name' => 'field_postcode',
      'label' => 'Postcode',
      'required' => 0,
      'settings' => array(
        'text_processing' => '0',
        'user_register_form' => FALSE,
      ),
      'widget' => array(
        'active' => 1,
        'module' => 'text',
        'settings' => array(
          'size' => '60',
        ),
        'type' => 'text_textfield',
        'weight' => '35',
      ),
    ),
  );

  // Exported field: 'taxonomy_term-provider-field_town'.
  $fields['taxonomy_term-provider-field_town'] = array(
    'field_config' => array(
      'active' => '1',
      'cardinality' => '1',
      'deleted' => '0',
      'entity_types' => array(),
      'field_name' => 'field_town',
      'foreign keys' => array(
        'format' => array(
          'columns' => array(
            'format' => 'format',
          ),
          'table' => 'filter_format',
        ),
      ),
      'indexes' => array(
        'format' => array(
          0 => 'format',
        ),
      ),
      'locked' => '0',
      'module' => 'text',
      'settings' => array(
        'max_length' => '50',
      ),
      'translatable' => '0',
      'type' => 'text',
    ),
    'field_instance' => array(
      'bundle' => 'provider',
      'default_value' => NULL,
      'deleted' => '0',
      'description' => 'The name of the town or city in which the provider\'s main site is situated. Its purpose is for use with software functionality that requires a town or city for searching.',
      'display' => array(
        'default' => array(
          'label' => 'above',
          'module' => 'text',
          'settings' => array(),
          'type' => 'text_default',
          'weight' => 5,
        ),
      ),
      'entity_type' => 'taxonomy_term',
      'field_name' => 'field_town',
      'label' => 'Town',
      'required' => 0,
      'settings' => array(
        'text_processing' => '0',
        'user_register_form' => FALSE,
      ),
      'widget' => array(
        'active' => 1,
        'module' => 'text',
        'settings' => array(
          'size' => '60',
        ),
        'type' => 'text_textfield',
        'weight' => '36',
      ),
    ),
  );

  // Exported field: 'taxonomy_term-provider-field_url'.
  $fields['taxonomy_term-provider-field_url'] = array(
    'field_config' => array(
      'active' => '1',
      'cardinality' => '1',
      'deleted' => '0',
      'entity_types' => array(),
      'field_name' => 'field_url',
      'foreign keys' => array(),
      'indexes' => array(),
      'locked' => '0',
      'module' => 'link',
      'settings' => array(
        'attributes' => array(
          'class' => '',
          'rel' => '',
          'target' => 'default',
        ),
        'display' => array(
          'url_cutoff' => 80,
        ),
        'enable_tokens' => 1,
        'title' => 'optional',
        'title_maxlength' => 128,
        'title_value' => '',
        'url' => 0,
      ),
      'translatable' => '0',
      'type' => 'link_field',
    ),
    'field_instance' => array(
      'bundle' => 'provider',
      'default_value' => NULL,
      'deleted' => '0',
      'description' => 'You can specify an URL such as your provider\'s homepage. If empty, provider\'s internal drupal path will be used.',
      'display' => array(
        'default' => array(
          'label' => 'above',
          'module' => 'link',
          'settings' => array(),
          'type' => 'link_default',
          'weight' => 8,
        ),
      ),
      'entity_type' => 'taxonomy_term',
      'field_name' => 'field_url',
      'label' => 'URL',
      'required' => 0,
      'settings' => array(
        'attributes' => array(
          'class' => '',
          'configurable_title' => 0,
          'rel' => '',
          'target' => 'default',
          'title' => '',
        ),
        'display' => array(
          'url_cutoff' => '80',
        ),
        'enable_tokens' => 1,
        'title' => 'none',
        'title_maxlength' => '128',
        'title_value' => '',
        'url' => 0,
        'user_register_form' => FALSE,
        'validate_url' => 1,
      ),
      'widget' => array(
        'active' => 0,
        'module' => 'link',
        'settings' => array(),
        'type' => 'link_field',
        'weight' => '2',
      ),
    ),
  );

  // Exported field: 'taxonomy_term-qualification-field_abbr'.
  $fields['taxonomy_term-qualification-field_abbr'] = array(
    'field_config' => array(
      'active' => '1',
      'cardinality' => '1',
      'deleted' => '0',
      'entity_types' => array(),
      'field_name' => 'field_abbr',
      'foreign keys' => array(
        'format' => array(
          'columns' => array(
            'format' => 'format',
          ),
          'table' => 'filter_format',
        ),
      ),
      'indexes' => array(
        'format' => array(
          0 => 'format',
        ),
      ),
      'locked' => '0',
      'module' => 'text',
      'settings' => array(
        'max_length' => '25',
      ),
      'translatable' => '0',
      'type' => 'text',
    ),
    'field_instance' => array(
      'bundle' => 'qualification',
      'default_value' => NULL,
      'deleted' => '0',
      'description' => 'Abbreviation used as a shortened form of the qualification awarded.',
      'display' => array(
        'default' => array(
          'label' => 'above',
          'module' => 'text',
          'settings' => array(),
          'type' => 'text_default',
          'weight' => 0,
        ),
      ),
      'entity_type' => 'taxonomy_term',
      'field_name' => 'field_abbr',
      'label' => 'Abbreviation',
      'required' => 0,
      'settings' => array(
        'text_processing' => '0',
        'user_register_form' => FALSE,
      ),
      'widget' => array(
        'active' => 1,
        'module' => 'text',
        'settings' => array(
          'size' => '60',
        ),
        'type' => 'text_textfield',
        'weight' => '1',
      ),
    ),
  );

  // Exported field: 'taxonomy_term-qualification-field_accredited_by'.
  $fields['taxonomy_term-qualification-field_accredited_by'] = array(
    'field_config' => array(
      'active' => '1',
      'cardinality' => '1',
      'deleted' => '0',
      'entity_types' => array(),
      'field_name' => 'field_accredited_by',
      'foreign keys' => array(
        'format' => array(
          'columns' => array(
            'format' => 'format',
          ),
          'table' => 'filter_format',
        ),
      ),
      'indexes' => array(
        'format' => array(
          0 => 'format',
        ),
      ),
      'locked' => '0',
      'module' => 'text',
      'settings' => array(
        'max_length' => '100',
      ),
      'translatable' => '0',
      'type' => 'text',
    ),
    'field_instance' => array(
      'bundle' => 'qualification',
      'default_value' => NULL,
      'deleted' => '0',
      'description' => 'The common name of an organisation that accredits the qualification.',
      'display' => array(
        'default' => array(
          'label' => 'above',
          'module' => 'text',
          'settings' => array(),
          'type' => 'text_default',
          'weight' => 1,
        ),
      ),
      'entity_type' => 'taxonomy_term',
      'field_name' => 'field_accredited_by',
      'label' => 'Accredited by',
      'required' => 0,
      'settings' => array(
        'text_processing' => '0',
        'user_register_form' => FALSE,
      ),
      'widget' => array(
        'active' => 1,
        'module' => 'text',
        'settings' => array(
          'size' => '60',
        ),
        'type' => 'text_textfield',
        'weight' => '2',
      ),
    ),
  );

  // Exported field: 'taxonomy_term-qualification-field_awarded_by'.
  $fields['taxonomy_term-qualification-field_awarded_by'] = array(
    'field_config' => array(
      'active' => '1',
      'cardinality' => '1',
      'deleted' => '0',
      'entity_types' => array(),
      'field_name' => 'field_awarded_by',
      'foreign keys' => array(
        'format' => array(
          'columns' => array(
            'format' => 'format',
          ),
          'table' => 'filter_format',
        ),
      ),
      'indexes' => array(
        'format' => array(
          0 => 'format',
        ),
      ),
      'locked' => '0',
      'module' => 'text',
      'settings' => array(
        'max_length' => '100',
      ),
      'translatable' => '0',
      'type' => 'text',
    ),
    'field_instance' => array(
      'bundle' => 'qualification',
      'default_value' => NULL,
      'deleted' => '0',
      'description' => 'The common name of the awarding body.',
      'display' => array(
        'default' => array(
          'label' => 'above',
          'module' => 'text',
          'settings' => array(),
          'type' => 'text_default',
          'weight' => 2,
        ),
      ),
      'entity_type' => 'taxonomy_term',
      'field_name' => 'field_awarded_by',
      'label' => 'Awarded by',
      'required' => 1,
      'settings' => array(
        'text_processing' => '0',
        'user_register_form' => FALSE,
      ),
      'widget' => array(
        'active' => 1,
        'module' => 'text',
        'settings' => array(
          'size' => '60',
        ),
        'type' => 'text_textfield',
        'weight' => '3',
      ),
    ),
  );

  // Exported field: 'taxonomy_term-qualification-field_education_level'.
  $fields['taxonomy_term-qualification-field_education_level'] = array(
    'field_config' => array(
      'active' => '1',
      'cardinality' => '1',
      'deleted' => '0',
      'entity_types' => array(),
      'field_name' => 'field_education_level',
      'foreign keys' => array(
        'format' => array(
          'columns' => array(
            'format' => 'format',
          ),
          'table' => 'filter_format',
        ),
      ),
      'indexes' => array(
        'format' => array(
          0 => 'format',
        ),
      ),
      'locked' => '0',
      'module' => 'text',
      'settings' => array(
        'max_length' => '50',
      ),
      'translatable' => '0',
      'type' => 'text',
    ),
    'field_instance' => array(
      'bundle' => 'qualification',
      'default_value' => NULL,
      'deleted' => '0',
      'description' => 'Information about the progression through an educational or training context represented by the qualification.',
      'display' => array(
        'default' => array(
          'label' => 'above',
          'module' => 'text',
          'settings' => array(),
          'type' => 'text_default',
          'weight' => 4,
        ),
      ),
      'entity_type' => 'taxonomy_term',
      'field_name' => 'field_education_level',
      'label' => 'Education level',
      'required' => 0,
      'settings' => array(
        'text_processing' => '0',
        'user_register_form' => FALSE,
      ),
      'widget' => array(
        'active' => 1,
        'module' => 'text',
        'settings' => array(
          'size' => '60',
        ),
        'type' => 'text_textfield',
        'weight' => '6',
      ),
    ),
  );

  // Exported field: 'taxonomy_term-qualification-field_professional_status'.
  $fields['taxonomy_term-qualification-field_professional_status'] = array(
    'field_config' => array(
      'active' => '1',
      'cardinality' => '1',
      'deleted' => '0',
      'entity_types' => array(),
      'field_name' => 'field_professional_status',
      'foreign keys' => array(
        'format' => array(
          'columns' => array(
            'format' => 'format',
          ),
          'table' => 'filter_format',
        ),
      ),
      'indexes' => array(
        'format' => array(
          0 => 'format',
        ),
      ),
      'locked' => '0',
      'module' => 'text',
      'settings' => array(),
      'translatable' => '0',
      'type' => 'text_long',
    ),
    'field_instance' => array(
      'bundle' => 'qualification',
      'default_value' => NULL,
      'deleted' => '0',
      'description' => 'Description of any professional standing of the qualification, including reference to specific professional bodies.',
      'display' => array(
        'default' => array(
          'label' => 'above',
          'module' => 'text',
          'settings' => array(),
          'type' => 'text_default',
          'weight' => 3,
        ),
      ),
      'entity_type' => 'taxonomy_term',
      'field_name' => 'field_professional_status',
      'label' => 'Professional status',
      'required' => 0,
      'settings' => array(
        'text_processing' => '1',
        'user_register_form' => FALSE,
      ),
      'widget' => array(
        'active' => 1,
        'module' => 'text',
        'settings' => array(
          'rows' => '15',
        ),
        'type' => 'text_textarea',
        'weight' => '5',
      ),
    ),
  );

  // Exported field: 'taxonomy_term-qualification-field_type'.
  $fields['taxonomy_term-qualification-field_type'] = array(
    'field_config' => array(
      'active' => '1',
      'cardinality' => '1',
      'deleted' => '0',
      'entity_types' => array(),
      'field_name' => 'field_type',
      'foreign keys' => array(
        'format' => array(
          'columns' => array(
            'format' => 'format',
          ),
          'table' => 'filter_format',
        ),
      ),
      'indexes' => array(
        'format' => array(
          0 => 'format',
        ),
      ),
      'locked' => '0',
      'module' => 'text',
      'settings' => array(
        'max_length' => '50',
      ),
      'translatable' => '0',
      'type' => 'text',
    ),
    'field_instance' => array(
      'bundle' => 'qualification',
      'default_value' => NULL,
      'deleted' => '0',
      'description' => '',
      'display' => array(
        'default' => array(
          'label' => 'above',
          'module' => 'text',
          'settings' => array(),
          'type' => 'text_default',
          'weight' => 5,
        ),
      ),
      'entity_type' => 'taxonomy_term',
      'field_name' => 'field_type',
      'label' => 'Type',
      'required' => 0,
      'settings' => array(
        'text_processing' => '0',
        'user_register_form' => FALSE,
      ),
      'widget' => array(
        'active' => 1,
        'module' => 'text',
        'settings' => array(
          'size' => '60',
        ),
        'type' => 'text_textfield',
        'weight' => '7',
      ),
    ),
  );

  // Exported field: 'taxonomy_term-venue-field_address'.
  $fields['taxonomy_term-venue-field_address'] = array(
    'field_config' => array(
      'active' => '1',
      'cardinality' => '1',
      'deleted' => '0',
      'entity_types' => array(),
      'field_name' => 'field_address',
      'foreign keys' => array(
        'format' => array(
          'columns' => array(
            'format' => 'format',
          ),
          'table' => 'filter_format',
        ),
      ),
      'indexes' => array(
        'format' => array(
          0 => 'format',
        ),
      ),
      'locked' => '0',
      'module' => 'text',
      'settings' => array(),
      'translatable' => '0',
      'type' => 'text_long',
    ),
    'field_instance' => array(
      'bundle' => 'venue',
      'default_value' => NULL,
      'deleted' => '0',
      'description' => 'Specifies the geographical or spatial location of the venue.',
      'display' => array(
        'default' => array(
          'label' => 'above',
          'module' => 'text',
          'settings' => array(),
          'type' => 'text_default',
          'weight' => 1,
        ),
      ),
      'entity_type' => 'taxonomy_term',
      'field_name' => 'field_address',
      'label' => 'Address',
      'required' => 1,
      'settings' => array(
        'text_processing' => '0',
        'user_register_form' => FALSE,
      ),
      'widget' => array(
        'active' => 1,
        'module' => 'text',
        'settings' => array(
          'rows' => '5',
        ),
        'type' => 'text_textarea',
        'weight' => '34',
      ),
    ),
  );

  // Exported field: 'taxonomy_term-venue-field_email'.
  $fields['taxonomy_term-venue-field_email'] = array(
    'field_config' => array(
      'active' => '1',
      'cardinality' => '1',
      'deleted' => '0',
      'entity_types' => array(),
      'field_name' => 'field_email',
      'foreign keys' => array(),
      'indexes' => array(),
      'locked' => '0',
      'module' => 'email',
      'settings' => array(),
      'translatable' => '0',
      'type' => 'email',
    ),
    'field_instance' => array(
      'bundle' => 'venue',
      'default_value' => NULL,
      'deleted' => '0',
      'description' => 'Preferred. Email address of the venue.',
      'display' => array(
        'default' => array(
          'label' => 'above',
          'module' => 'email',
          'settings' => array(),
          'type' => 'email_default',
          'weight' => 2,
        ),
      ),
      'entity_type' => 'taxonomy_term',
      'field_name' => 'field_email',
      'label' => 'Email',
      'required' => 0,
      'settings' => array(
        'user_register_form' => FALSE,
      ),
      'widget' => array(
        'active' => 1,
        'module' => 'email',
        'settings' => array(
          'size' => '60',
        ),
        'type' => 'email_textfield',
        'weight' => '35',
      ),
    ),
  );

  // Exported field: 'taxonomy_term-venue-field_fax'.
  $fields['taxonomy_term-venue-field_fax'] = array(
    'field_config' => array(
      'active' => '1',
      'cardinality' => '1',
      'deleted' => '0',
      'entity_types' => array(),
      'field_name' => 'field_fax',
      'foreign keys' => array(
        'format' => array(
          'columns' => array(
            'format' => 'format',
          ),
          'table' => 'filter_format',
        ),
      ),
      'indexes' => array(
        'format' => array(
          0 => 'format',
        ),
      ),
      'locked' => '0',
      'module' => 'text',
      'settings' => array(
        'max_length' => '255',
      ),
      'translatable' => '0',
      'type' => 'text',
    ),
    'field_instance' => array(
      'bundle' => 'venue',
      'default_value' => NULL,
      'deleted' => '0',
      'description' => 'Fax number of the venue. UK National Format Layout (Area Code and Number).',
      'display' => array(
        'default' => array(
          'label' => 'above',
          'module' => 'text',
          'settings' => array(),
          'type' => 'text_default',
          'weight' => 3,
        ),
      ),
      'entity_type' => 'taxonomy_term',
      'field_name' => 'field_fax',
      'label' => 'Fax',
      'required' => 0,
      'settings' => array(
        'text_processing' => '0',
        'user_register_form' => FALSE,
      ),
      'widget' => array(
        'active' => 1,
        'module' => 'text',
        'settings' => array(
          'size' => '60',
        ),
        'type' => 'text_textfield',
        'weight' => '37',
      ),
    ),
  );

  // Exported field: 'taxonomy_term-venue-field_location_url'.
  $fields['taxonomy_term-venue-field_location_url'] = array(
    'field_config' => array(
      'active' => '1',
      'cardinality' => '1',
      'deleted' => '0',
      'entity_types' => array(),
      'field_name' => 'field_location_url',
      'foreign keys' => array(),
      'indexes' => array(),
      'locked' => '0',
      'module' => 'link',
      'settings' => array(
        'attributes' => array(
          'class' => '',
          'rel' => '',
          'target' => 'default',
        ),
        'display' => array(
          'url_cutoff' => 80,
        ),
        'enable_tokens' => 1,
        'title' => 'optional',
        'title_maxlength' => 128,
        'title_value' => '',
        'url' => 0,
      ),
      'translatable' => '0',
      'type' => 'link_field',
    ),
    'field_instance' => array(
      'bundle' => 'venue',
      'default_value' => NULL,
      'deleted' => '0',
      'description' => 'A URL with information relating to the location of the venue, for example a map or directions.',
      'display' => array(
        'default' => array(
          'label' => 'above',
          'module' => 'link',
          'settings' => array(),
          'type' => 'link_default',
          'weight' => 7,
        ),
      ),
      'entity_type' => 'taxonomy_term',
      'field_name' => 'field_location_url',
      'label' => 'URL',
      'required' => 0,
      'settings' => array(
        'attributes' => array(
          'class' => '',
          'configurable_title' => 0,
          'rel' => '',
          'target' => 'default',
          'title' => '',
        ),
        'display' => array(
          'url_cutoff' => '80',
        ),
        'enable_tokens' => 1,
        'title' => 'none',
        'title_maxlength' => '255',
        'title_value' => '',
        'url' => 0,
        'user_register_form' => FALSE,
        'validate_url' => 1,
      ),
      'widget' => array(
        'active' => 0,
        'module' => 'link',
        'settings' => array(),
        'type' => 'link_field',
        'weight' => '40',
      ),
    ),
  );

  // Exported field: 'taxonomy_term-venue-field_phone'.
  $fields['taxonomy_term-venue-field_phone'] = array(
    'field_config' => array(
      'active' => '1',
      'cardinality' => '1',
      'deleted' => '0',
      'entity_types' => array(),
      'field_name' => 'field_phone',
      'foreign keys' => array(
        'format' => array(
          'columns' => array(
            'format' => 'format',
          ),
          'table' => 'filter_format',
        ),
      ),
      'indexes' => array(
        'format' => array(
          0 => 'format',
        ),
      ),
      'locked' => '0',
      'module' => 'text',
      'settings' => array(
        'max_length' => '255',
      ),
      'translatable' => '0',
      'type' => 'text',
    ),
    'field_instance' => array(
      'bundle' => 'venue',
      'default_value' => NULL,
      'deleted' => '0',
      'description' => 'Preferred. Telephone number of the venue. Telephone number of the venue.',
      'display' => array(
        'default' => array(
          'label' => 'above',
          'module' => 'text',
          'settings' => array(),
          'type' => 'text_default',
          'weight' => 4,
        ),
      ),
      'entity_type' => 'taxonomy_term',
      'field_name' => 'field_phone',
      'label' => 'Phone',
      'required' => 0,
      'settings' => array(
        'text_processing' => '0',
        'user_register_form' => FALSE,
      ),
      'widget' => array(
        'active' => 1,
        'module' => 'text',
        'settings' => array(
          'size' => '60',
        ),
        'type' => 'text_textfield',
        'weight' => '36',
      ),
    ),
  );

  // Exported field: 'taxonomy_term-venue-field_postcode'.
  $fields['taxonomy_term-venue-field_postcode'] = array(
    'field_config' => array(
      'active' => '1',
      'cardinality' => '1',
      'deleted' => '0',
      'entity_types' => array(),
      'field_name' => 'field_postcode',
      'foreign keys' => array(
        'format' => array(
          'columns' => array(
            'format' => 'format',
          ),
          'table' => 'filter_format',
        ),
      ),
      'indexes' => array(
        'format' => array(
          0 => 'format',
        ),
      ),
      'locked' => '0',
      'module' => 'text',
      'settings' => array(
        'max_length' => '255',
      ),
      'translatable' => '0',
      'type' => 'text',
    ),
    'field_instance' => array(
      'bundle' => 'venue',
      'default_value' => NULL,
      'deleted' => '0',
      'description' => 'Full official postcode of the address of the venue.',
      'display' => array(
        'default' => array(
          'label' => 'above',
          'module' => 'text',
          'settings' => array(),
          'type' => 'text_default',
          'weight' => 5,
        ),
      ),
      'entity_type' => 'taxonomy_term',
      'field_name' => 'field_postcode',
      'label' => 'Postcode',
      'required' => 0,
      'settings' => array(
        'text_processing' => '0',
        'user_register_form' => FALSE,
      ),
      'widget' => array(
        'active' => 1,
        'module' => 'text',
        'settings' => array(
          'size' => '60',
        ),
        'type' => 'text_textfield',
        'weight' => '38',
      ),
    ),
  );

  // Exported field: 'taxonomy_term-venue-field_town'.
  $fields['taxonomy_term-venue-field_town'] = array(
    'field_config' => array(
      'active' => '1',
      'cardinality' => '1',
      'deleted' => '0',
      'entity_types' => array(),
      'field_name' => 'field_town',
      'foreign keys' => array(
        'format' => array(
          'columns' => array(
            'format' => 'format',
          ),
          'table' => 'filter_format',
        ),
      ),
      'indexes' => array(
        'format' => array(
          0 => 'format',
        ),
      ),
      'locked' => '0',
      'module' => 'text',
      'settings' => array(
        'max_length' => '50',
      ),
      'translatable' => '0',
      'type' => 'text',
    ),
    'field_instance' => array(
      'bundle' => 'venue',
      'default_value' => NULL,
      'deleted' => '0',
      'description' => 'The name of the town or city in which the venue is situated.',
      'display' => array(
        'default' => array(
          'label' => 'above',
          'module' => 'text',
          'settings' => array(),
          'type' => 'text_default',
          'weight' => 6,
        ),
      ),
      'entity_type' => 'taxonomy_term',
      'field_name' => 'field_town',
      'label' => 'Town',
      'required' => 0,
      'settings' => array(
        'text_processing' => '0',
        'user_register_form' => FALSE,
      ),
      'widget' => array(
        'active' => 1,
        'module' => 'text',
        'settings' => array(
          'size' => '60',
        ),
        'type' => 'text_textfield',
        'weight' => '39',
      ),
    ),
  );

  // Translatables
  // Included for use with string extractors like potx.
  t(' Holds the information relating to the main locations where a learning opportunity is presented.');
  t('A URL to further information about, or an online application system for, the presentation.');
  t('A URL with information relating to the location of the provider, for example a map or directions.');
  t('A URL with information relating to the location of the venue, for example a map or directions.');
  t('A grouping of similar courses in terms of target audience.');
  t('A language in which the presentation is assessed.');
  t('A short one-sentence description for use in a course list. <b>If empty, the value will be inherited from the course taxonomy.</b>');
  t('A textual description of the cost associated with obtaining access to the presentation, including reference to bursaries or other special arrangements for specific groups of learners. <b>"GBP" instead of "£".</b>');
  t('A textual description of the number of places available for participants in the presentation. MAY be a simple integer.');
  t('Abbreviation');
  t('Abbreviation used as a shortened form of the qualification awarded.');
  t('Abstract');
  t('Accredited by');
  t('Address');
  t('Age');
  t('An account of the credits that can be obtained from completion of a course.');
  t('Application procedure');
  t('Apply');
  t('Apply to');
  t('Assessment');
  t('Attendance mode');
  t('Attendance pattern');
  t('Awarded by');
  t('Body');
  t('Cost');
  t('Course');
  t('Credit');
  t('Description of any professional standing of the qualification, including reference to specific professional bodies.');
  t('Duration');
  t('Education level');
  t('Email');
  t('Email address of the provider\'s main administrative site.');
  t('Fax');
  t('Fax number of the provider\'s main administrative site. UK National Format Layout (Area Code and Number).');
  t('Fax number of the venue. UK National Format Layout (Area Code and Number).');
  t('Full official postcode of the address of the provider\'s main site.');
  t('Full official postcode of the address of the venue.');
  t('General summary of the course. <b>If empty, the value will be inherited from the course taxonomy.</b>');
  t('Information about how long the presentation will last.');
  t('Information about the progression through an educational or training context represented by the qualification.');
  t('Information about the start and end date and/or time of the presentation.');
  t('Language of assessment');
  t('Language of instruction');
  t('Learning outcome');
  t('Level');
  t('Main telephone number of the provider\'s main administrative site. UK National Format Layout (Area Code and Number).');
  t('Mandatory only if course leads to qualification. Holds the data relating to a named formal recognition of achievement or competence (award) certified by an awarding body that can result from studying a course.');
  t('Objective');
  t('Phone');
  t('Places');
  t('Postcode');
  t('Preferred. A short one-sentence description for use in a course list.');
  t('Preferred. Email address of the venue.');
  t('Preferred. Telephone number of the venue. Telephone number of the venue.');
  t('Prerequisite');
  t('Professional status');
  t('Provider');
  t('Qualification');
  t('Regulations');
  t('Scheme');
  t('Specifies the geographical or spatial location of the provider\'s main administrative site.');
  t('Specifies the geographical or spatial location of the venue.');
  t('Start/End');
  t('Study mode');
  t('Subject');
  t('Text describing aims or learning objectives of the learning opportunity.');
  t('Text describing aims or learning objectives of the learning opportunity. <b>If empty, the value will be inherited from the course taxonomy.</b>');
  t('Text describing how to apply for the learning opportunity.');
  t('Text describing how to apply for the learning opportunity. <b>If empty, the value will be inherited from the course taxonomy.</b>');
  t('Text describing the broad approach to assessment used in the learning opportunity.');
  t('Text describing the broad approach to assessment used in the learning opportunity. <b>If empty, the value will be inherited from the course taxonomy.</b>');
  t('Text describing the learning outcomes for a course of study.');
  t('Text describing the learning outcomes for a course of study. <b>If empty, the value will be inherited from the course taxonomy.</b>');
  t('Textual description of the entry requirements (academic and / or non-academic, formal or informal) for entry to a learning opportunity.');
  t('Textual description of the entry requirements (academic and / or non-academic, formal or informal) for entry to a learning opportunity. <b>If empty, the value will be inherited from the course taxonomy.</b>');
  t('Textual description of the organisation\'s academic and administrative regulations relevant to a learning opportunity.');
  t('Textual description of the organisation\'s academic and administrative regulations relevant to a learning opportunity. <b>Textual description of the organisation\'s academic and administrative regulations relevant to a learning opportunity.</b>');
  t('The common name of an organisation that accredits the qualification.');
  t('The common name of the awarding body.');
  t('The date range from which applications can be accepted for the presentation.');
  t('The intended age range for which the presentation is suitable. The value of this element <b>MUST</b> be one of: <i>any</i>, <i>not known</i>, <i>x-y</i>, <i>x+</i> where x and y MUST be non-negative integers.');
  t('The learning opportunity provider. A provider is any organisation, component of an organisation or partnership that offers one or more learning opportunities.');
  t('The learning opportunity.');
  t('The name of the credit scheme under which credits are awarded.');
  t('The name of the town or city in which the provider\'s main site is situated. Its purpose is for use with software functionality that requires a town or city for searching.');
  t('The name of the town or city in which the venue is situated.');
  t('The number of credits awarded.');
  t('The period in the day and/or frequency during which attendance at a venue is required (if any).');
  t('The point in a credit scheme or framework at which credits are awarded.');
  t('The topic of the course.');
  t('The topic of the course. <b>If empty, the value will be inherited from the course taxonomy.</b>');
  t('The type of location at which the student will undertake the presentation.');
  t('Town');
  t('Type');
  t('URL');
  t('Value');
  t('Venue');
  t('You can specify a course URL if it\'s different from the drupal page for this entity, otherwise you can leave this field blank.');
  t('You can specify an URL such as your provider\'s homepage. If empty, provider\'s internal drupal path will be used.');

  return $fields;
}
