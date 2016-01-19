<?php

/**
 * @file
 *
 * Fields to be created and attached to Parish Bulletin module node types.
 */

function _create_parish_bulletin_fields() {
  /**
   * Fields for Bulletin content type.
   *   - field_bulletin_date ('Date', date field)
   *   - field_bulletin ('Bulletin', file field)
   */

  // field_bulletin_date
  field_create_field(array(
    'translatable' => '0',
    'entity_types' => array(),
    'settings' => array(
      'repeat' => 0,
      'granularity' => array(
        'month' => 'month',
        'day' => 'day',
        'year' => 'year',
        'hour' => 0,
        'minute' => 0,
        'second' => 0,
      ),
      'tz_handling' => 'none',
      'timezone_db' => '',
      'todate' => '',
    ),
    'storage' => array(
      'type' => 'field_sql_storage',
      'settings' => array(),
      'module' => 'field_sql_storage',
      'active' => '1',
      'details' => array(
        'sql' => array(
          'FIELD_LOAD_CURRENT' => array(
            'field_data_field_bulletin_date' => array(
              'value' => 'field_bulletin_date_value',
            ),
          ),
          'FIELD_LOAD_REVISION' => array(
            'field_revision_field_bulletin_date' => array(
              'value' => 'field_bulletin_date_value',
            ),
          ),
        ),
      ),
    ),
    'foreign keys' => array(),
    'indexes' => array(),
    'field_name' => 'field_bulletin_date',
    'type' => 'datetime',
    'module' => 'date',
    'active' => '1',
    'locked' => '0',
    'cardinality' => '1',
    'deleted' => '0',
    'columns' => array(
      'value' => array(
        'type' => 'datetime',
        'mysql_type' => 'DATETIME',
        'pgsql_type' => 'timestamp without time zone',
        'sqlite_type' => 'VARCHAR',
        'sqlsrv_type' => 'smalldatetime',
        'not null' => FALSE,
        'sortable' => TRUE,
        'views' => TRUE,
      ),
    ),
    'bundles' => array(
      'node' => array(
        'bulletin',
      ),
    ),
  ));
  field_create_instance(array(
    'label' => 'Date',
    'widget' => array(
      'weight' => '1',
      'type' => 'date_popup',
      'module' => 'date',
      'active' => 1,
      'settings' => array(
        'input_format' => 'm/d/Y - H:i:s',
        'input_format_custom' => '',
        'year_range' => '-3:+3',
        'increment' => '15',
        'label_position' => 'above',
        'text_parts' => array(),
        'repeat_collapsed' => 0,
      ),
    ),
    'settings' => array(
      'default_value' => 'now',
      'default_value_code' => '',
      'default_value2' => 'blank',
      'default_value_code2' => '',
      'user_register_form' => FALSE,
    ),
    'display' => array(
      'default' => array(
        'label' => 'inline',
        'type' => 'date_default',
        'weight' => '1',
        'settings' => array(
          'format_type' => 'short',
          'fromto' => 'both',
          'multiple_number' => '',
          'multiple_from' => '',
          'multiple_to' => '',
          'show_repeat_rule' => 'show',
        ),
        'module' => 'date',
      ),
      'teaser' => array(
        'type' => 'hidden',
        'label' => 'above',
        'settings' => array(),
        'weight' => 0,
      ),
    ),
    'required' => 1,
    'description' => 'Set the date for this bulletin.',
    'field_name' => 'field_bulletin_date',
    'entity_type' => 'node',
    'bundle' => 'bulletin',
    'deleted' => '0',
  ));

  // field_bulletin
  field_create_field(array(
    'translatable' => '0',
    'entity_types' => array(),
    'settings' => array(
      'display_field' => 0,
      'display_default' => 1,
      'uri_scheme' => 'public',
    ),
    'storage' => array(
      'type' => 'field_sql_storage',
      'settings' => array(),
      'module' => 'field_sql_storage',
      'active' => '1',
      'details' => array(
        'sql' => array(
          'FIELD_LOAD_CURRENT' => array(
            'field_data_field_bulletin' => array(
              'fid' => 'field_bulletin_fid',
              'display' => 'field_bulletin_display',
              'description' => 'field_bulletin_description',
            ),
          ),
          'FIELD_LOAD_REVISION' => array(
            'field_revision_field_bulletin' => array(
              'fid' => 'field_bulletin_fid',
              'display' => 'field_bulletin_display',
              'description' => 'field_bulletin_description',
            ),
          ),
        ),
      ),
    ),
    'foreign keys' => array(
      'fid' => array(
        'table' => 'file_managed',
        'columns' => array(
          'fid' => 'fid',
        ),
      ),
    ),
    'indexes' => array(
      'fid' => array(
        'fid',
      ),
    ),
    'field_name' => 'field_bulletin',
    'type' => 'file',
    'module' => 'file',
    'active' => '1',
    'locked' => '0',
    'cardinality' => '1',
    'deleted' => '0',
    'columns' => array(
      'fid' => array(
        'description' => 'The {file_managed}.fid being referenced in this field.',
        'type' => 'int',
        'not null' => FALSE,
        'unsigned' => TRUE,
      ),
      'display' => array(
        'description' => 'Flag to control whether this file should be displayed when viewing content.',
        'type' => 'int',
        'size' => 'tiny',
        'unsigned' => TRUE,
        'not null' => TRUE,
        'default' => 1,
      ),
      'description' => array(
        'description' => 'A description of the file.',
        'type' => 'text',
        'not null' => FALSE,
      ),
    ),
    'bundles' => array(
      'node' => array(
        'bulletin',
      ),
    ),
  ));
  field_create_instance(array(
    'label' => 'Bulletin',
    'widget' => array(
      'weight' => '3',
      'type' => 'file_generic',
      'module' => 'file',
      'active' => 1,
      'settings' => array(
        'progress_indicator' => 'throbber',
        'insert' => 0,
        'insert_styles' => array(
          'auto' => 'auto',
          'link' => 0,
          'image' => 0,
          'colorbox__large' => 0,
          'colorbox__medium' => 0,
          'colorbox__thumbnail' => 0,
          'image_large' => 0,
          'image_medium' => 0,
          'image_thumbnail' => 0,
        ),
        'insert_default' => 'auto',
        'insert_class' => '',
        'insert_width' => '',
      ),
    ),
    'settings' => array(
      'file_directory' => 'bulletins',
      'file_extensions' => 'txt pdf tiff doc docx',
      'max_filesize' => '16 MB',
      'description_field' => 0,
      'user_register_form' => FALSE,
    ),
    'display' => array(
      'default' => array(
        'label' => 'inline',
        'type' => 'file_default',
        'weight' => '2',
        'settings' => array(),
        'module' => 'file',
      ),
      'teaser' => array(
        'type' => 'hidden',
        'label' => 'above',
        'settings' => array(),
        'weight' => 0,
      ),
    ),
    'required' => 1,
    'description' => 'Attach your current bulletin here.',
    'field_name' => 'field_bulletin',
    'entity_type' => 'node',
    'bundle' => 'bulletin',
    'deleted' => '0',
  ));
}
