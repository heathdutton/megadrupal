<?php
/**
 * @file
 * giveaways.data_default.inc
 */

/**
 * Implements hook_data_default().
 */
function giveaways_data_default() {
  $export = array();

  $data_table = new stdClass();
  $data_table->disabled = FALSE; /* Edit this to true to make a default data_table disabled initially */
  $data_table->api_version = 1;
  $data_table->title = 'Keys to be given away';
  $data_table->name = 'giveaway_keys';
  $data_table->table_schema = array(
    'description' => '',
    'fields' => array(
      'gkid' => array(
        'type' => 'serial',
        'size' => 'normal',
        'unsigned' => TRUE,
        'not null' => TRUE,
        'description' => '',
      ),
      'gid' => array(
        'type' => 'int',
        'size' => 'normal',
        'unsigned' => TRUE,
        'not null' => TRUE,
        'default' => 0,
        'description' => '',
      ),
      'game_key' => array(
        'type' => 'varchar',
        'size' => 'normal',
        'length' => '255',
        'not null' => FALSE,
        'description' => '',
      ),
      'uid' => array(
        'type' => 'int',
        'size' => 'normal',
        'unsigned' => TRUE,
        'not null' => FALSE,
        'description' => '',
      ),
      'ip_address' => array(
        'type' => 'int',
        'size' => 'big',
        'unsigned' => TRUE,
        'not null' => FALSE,
        'description' => '',
      ),
      'claimed_on' => array(
        'type' => 'int',
        'size' => 'normal',
        'unsigned' => TRUE,
        'not null' => FALSE,
        'default' => 0,
        'description' => '',
      ),
    ),
    'name' => 'giveaway_keys',
    'indexes' => array(
      'ip_address' => array(
        0 => 'ip_address',
      ),
    ),
    'primary key' => array(
      0 => 'gkid',
    ),
  );
  $data_table->meta = array(
    'fields' => array(
      'gid' => array(
        'label' => 'Giveaway Id',
        'locked' => FALSE,
        'required' => FALSE,
        'views_field_handler' => 'views_handler_field_numeric',
        'views_filter_handler' => 'views_handler_filter_numeric',
        'views_argument_handler' => 'views_handler_argument_numeric',
        'views_sort_handler' => 'views_handler_sort',
      ),
      'key' => array(
        'label' => 'The actual Key',
        'locked' => FALSE,
        'required' => FALSE,
      ),
      'available' => array(
        'label' => 'whether available or given away',
        'locked' => FALSE,
        'required' => FALSE,
      ),
      'ip_address' => array(
        'label' => 'The IP to which it has been given.',
        'locked' => FALSE,
        'required' => FALSE,
        'views_field_handler' => 'giveaways_handler_field_ip_address',
        'views_filter_handler' => 'views_handler_filter_numeric',
        'views_argument_handler' => 'views_handler_argument_numeric',
        'views_sort_handler' => 'views_handler_sort',
      ),
      'gkid' => array(
        'label' => 'Key Id',
        'views_field_handler' => 'views_handler_field_numeric',
        'views_filter_handler' => 'views_handler_filter_numeric',
        'views_argument_handler' => 'views_handler_argument_numeric',
        'views_sort_handler' => 'views_handler_sort',
      ),
      'game_key' => array(
        'label' => 'Game Key',
        'views_field_handler' => 'views_handler_field',
        'views_filter_handler' => 'views_handler_filter_string',
        'views_argument_handler' => 'views_handler_argument_string',
        'views_sort_handler' => 'views_handler_sort',
      ),
      'uid' => array(
        'label' => 'The user who has claimed it.',
        'views_field_handler' => 'views_handler_field_numeric',
        'views_filter_handler' => 'views_handler_filter_numeric',
        'views_argument_handler' => 'views_handler_argument_numeric',
        'views_sort_handler' => 'views_handler_sort',
      ),
      'claimed_on' => array(
        'label' => 'Timestamp when it was claimed.',
        'views_field_handler' => 'views_handler_field_date',
        'views_filter_handler' => 'views_handler_filter_date',
        'views_argument_handler' => 'views_handler_argument_date',
        'views_sort_handler' => 'views_handler_sort_date',
      ),
    ),
    'label_field' => '',
    'is_entity_type' => 1,
    'entity_id' => 'gkid',
    'join' => array(
      'users' => array(
        'left_field' => 'uid',
        'field' => 'uid',
        'inner_join' => '0',
      ),
      'node' => array(
        'left_field' => 'nid',
        'field' => 'gid',
        'inner_join' => '1',
      ),
    ),
  );
  $export['giveaway_keys'] = $data_table;

  return $export;
}
