<?php

/**
 * @file
 *
 * Fields to be created and attached to Parish Calendar module node types.
 */

function _create_parish_calendar_fields() {

  /**
   * Fields for Event content type.
   *   - field_event_date ('Date', date field)
   *   - field_event_description ('Description', long text field)
   */

  // field_event_date
  field_create_field(array(
    'translatable' => '0',
    'entity_types' => array(),
    'settings' => array(
      'repeat' => 0,
      'granularity' => array(
        'month' => 'month',
        'day' => 'day',
        'hour' => 'hour',
        'minute' => 'minute',
        'year' => 'year',
        'second' => 0,
      ),
      'tz_handling' => 'site',
      'timezone_db' => 'UTC',
      'todate' => 'optional',
    ),
    'storage' => array(
      'type' => 'field_sql_storage',
      'settings' => array(),
      'module' => 'field_sql_storage',
      'active' => '1',
      'details' => array(
        'sql' => array(
          'FIELD_LOAD_CURRENT' => array(
            'field_data_field_event_date' => array(
              'value' => 'field_event_date_value',
              'value2' => 'field_event_date_value2',
            ),
          ),
          'FIELD_LOAD_REVISION' => array(
            'field_revision_field_event_date' => array(
              'value' => 'field_event_date_value',
              'value2' => 'field_event_date_value2',
            ),
          ),
        ),
      ),
    ),
    'foreign keys' => array(),
    'indexes' => array(),
    'field_name' => 'field_event_date',
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
      'value2' => array(
        'type' => 'datetime',
        'mysql_type' => 'DATETIME',
        'pgsql_type' => 'timestamp without time zone',
        'sqlite_type' => 'VARCHAR',
        'sqlsrv_type' => 'smalldatetime',
        'not null' => FALSE,
        'sortable' => TRUE,
        'views' => FALSE,
      ),
    ),
    'bundles' => array(
      'node' => array(
        'event',
      ),
    ),
  ));
  field_create_instance(array(
    'label' => 'Date',
    'widget' => array(
      'weight' => '2',
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
          'format_type' => 'long',
          'show_repeat_rule' => 'show',
          'multiple_number' => '',
          'multiple_from' => '',
          'multiple_to' => '',
          'fromto' => 'both',
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
    'description' => '',
    'field_name' => 'field_event_date',
    'entity_type' => 'node',
    'bundle' => 'event',
    'deleted' => '0',
  ));

  // field_event_description
  field_create_field(array(
    'translatable' => '0',
    'entity_types' => array(),
    'settings' => array(),
    'storage' => array(
      'type' => 'field_sql_storage',
      'settings' => array(),
      'module' => 'field_sql_storage',
      'active' => '1',
      'details' => array(
        'sql' => array(
          'FIELD_LOAD_CURRENT' => array(
            'field_data_field_event_description' => array(
              'value' => 'field_event_description_value',
              'format' => 'field_event_description_format',
            ),
          ),
          'FIELD_LOAD_REVISION' => array(
            'field_revision_field_event_description' => array(
              'value' => 'field_event_description_value',
              'format' => 'field_event_description_format',
            ),
          ),
        ),
      ),
    ),
    'foreign keys' => array(
      'format' => array(
        'table' => 'filter_format',
        'columns' => array(
          'format' => 'format',
        ),
      ),
    ),
    'indexes' => array(
      'format' => array(
        'format',
      ),
    ),
    'field_name' => 'field_event_description',
    'type' => 'text_long',
    'module' => 'text',
    'active' => '1',
    'locked' => '0',
    'cardinality' => '1',
    'deleted' => '0',
    'columns' => array(
      'value' => array(
        'type' => 'text',
        'size' => 'big',
        'not null' => FALSE,
      ),
      'format' => array(
        'type' => 'varchar',
        'length' => 255,
        'not null' => FALSE,
      ),
    ),
    'bundles' => array(
      'node' => array(
        'event',
      ),
    ),
  ));
  field_create_instance(array(
    'label' => 'Description',
    'widget' => array(
      'weight' => '3',
      'type' => 'text_textarea',
      'module' => 'text',
      'active' => 1,
      'settings' => array(
        'rows' => '8',
      ),
    ),
    'settings' => array(
      'text_processing' => '1',
      'user_register_form' => FALSE,
    ),
    'display' => array(
      'default' => array(
        'label' => 'hidden',
        'type' => 'text_default',
        'weight' => '2',
        'settings' => array(),
        'module' => 'text',
      ),
      'teaser' => array(
        'type' => 'hidden',
        'label' => 'above',
        'settings' => array(),
        'weight' => 0,
      ),
    ),
    'required' => 0,
    'description' => '',
    'default_value' => NULL,
    'field_name' => 'field_event_description',
    'entity_type' => 'node',
    'bundle' => 'event',
    'deleted' => '0',
  ));
}
