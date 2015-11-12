<?php

class NodeAnnounceTestCase extends DrupalWebTestCase {
  /**
   * This tests to ensure that I've created the links that I expect using the
   * label attribute on them. It's a bit nicer than the text checking.
   */

  public $format = "Y-m-d H:i";

  public function createDateField() {
    # I got here by doing a features export, then grabbing
    # the relevant bits, if anyone needs to figure this out for 8
    $this->field = array(
      'active' => '1',
      'cardinality' => '1',
      'deleted' => '0',
      'entity_types' => array(),
      'field_name' => 'field_date',
      'foreign keys' => array(),
      'indexes' => array(),
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
      'type' => 'datetime',
    );

    $this->field = field_create_field($this->field);
    $this->field_id = $this->field['id'];
    $this->instance = array(
      'bundle' => 'event',
      'deleted' => '0',
      'description' => '',
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
          'weight' => '0',
        ),
        'teaser' => array(
          'label' => 'above',
          'settings' => array(),
          'type' => 'hidden',
          'weight' => 0,
        ),
      ),  'entity_type' => 'node',
      'field_name' => 'field_date',
      'label' => 'Date',
      'required' => 0,
      'settings' => array(
        'default_value' => 'now',
        'default_value2' => 'strtotime',
        'default_value_code' => '',
        'default_value_code2' => '+2 hours',
        'user_register_form' => FALSE,
      ),
      'widget' => array(
        'active' => 1,
        'module' => 'date',
        'settings' => array(
          'display_all_day' => 0,
          'increment' => 15,
          'input_format' => 'Y-m-d H:i:s',
          'input_format_custom' => '',
          'label_position' => 'above',
          'text_parts' => array(),
          'year_range' => '-3:+3',
        ),
        'type' => 'date_text',
        'weight' => '1',
      ),
    );
    field_create_instance($this->instance);
  }

  /**
   *  These are utility functions for the subclasses to use
   */

  public function CreateEvent($options) {
    $this->drupalGet('node/add/event');
    if (array_key_exists('date', $options)) {
      $options['field_date[und][0][value][date]'] = $options['date'];
      unset($options['date']);
    }
    if (array_key_exists('date_to', $options)) {
      $options['field_date[und][0][value2][date]'] = $options['date_to'];
      unset($options['date_to']);
    }

    $this->drupalPost(NULL, $options, t('Save'));
  }

  public function CreateAnnounce($options) {
    $this->drupalGet('admin/config/system/node_announce/add');
    $this->drupalPost(NULL, $options, t('Create'));
  }

  public function Get($url)
  {
    return $this->drupalGet($url);
  }

  public function Post($one, $two, $three)
  {
    return $this->drupalPost($one, $two, $three);
  }

}