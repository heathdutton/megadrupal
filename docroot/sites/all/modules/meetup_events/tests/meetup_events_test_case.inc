<?php

class MeetupEventsTestCase extends DrupalWebTestCase {
  /**
   * This tests to ensure that I've created the links that I expect using the
   * label attribute on them. It's a bit nicer than the text checking.
   */

  public $format = "Y-m-d H:i";

  public function createDateField($type) {
    $datefield = array(
      'field_name' => 'field_date',
      'type_name' => $type,
      'display_settings' =>
      array(
        'weight' => '-4',
        'parent' => '',
        'label' =>
        array(
          'format' => 'inline',
        ),
        'teaser' =>
        array(
          'format' => 'default',
          'exclude' => 0,
        ),
        'full' =>
        array(
          'format' => 'default',
          'exclude' => 0,
        ),
        4 =>
        array(
          'format' => 'default',
          'exclude' => 0,
        ),
        5 =>
        array(
          'format' => 'default',
          'exclude' => 0,
        ),
        'token' =>
        array(
          'format' => 'default',
          'exclude' => 0,
        ),
      ),
      'widget_active' => '1',
      'type' => 'date',
      'required' => '1',
      'multiple' => '1',
      'db_storage' => '0',
      'module' => 'date',
      'active' => '1',
      'locked' => '0',
      'columns' =>
      array(
        'value' =>
        array(
          'type' => 'varchar',
          'length' => 20,
          'not null' => FALSE,
          'sortable' => TRUE,
          'views' => TRUE,
        ),
        'value2' =>
        array(
          'type' => 'varchar',
          'length' => 20,
          'not null' => FALSE,
          'sortable' => TRUE,
          'views' => FALSE,
        ),
        'rrule' =>
        array(
          'type' => 'text',
          'not null' => FALSE,
          'sortable' => FALSE,
          'views' => FALSE,
        ),
      ),
      'granularity' =>
      array(
        'year' => 'year',
        'month' => 'month',
        'day' => 'day',
        'hour' => 'hour',
        'minute' => 'minute',
      ),
      'timezone_db' => 'UTC',
      'tz_handling' => 'site',
      'todate' => 'optional',
      'repeat' => 1,
      'repeat_collapsed' => '1',
      'default_format' => 'default',
      'widget' =>
      array(
        'default_value' => 'now',
        'default_value_code' => '',
        'default_value2' => 'same',
        'default_value_code2' => '',
        'input_format' => 'Y-m-d H:i:s',
        'input_format_custom' => '',
        'increment' => '15',
        'text_parts' =>
        array(
        ),
        'year_range' => '-7:+3',
        'label_position' => 'above',
        'label' => 'Date',
        'weight' => '-4',
        'description' => '',
        'type' => 'date_text_repeat',
        'module' => 'date',
      ),
    );

    content_field_instance_create($datefield);
  }

  public function assertLinkLabel($label, $index = 0, $message = '', $group = 'Other') {
    $links = $this->xpath('//a[@label="' . $label . '"]');
    $message = ($message ?  $message : t('Link with label "!label" found.', array('!label' => $label)));
    return $this->assert(isset($links[$index]), $message, $group);
  }

  /**
   *  These are utility functions for the subclasses to use
   */
  public function CreateType($type, $options) {
    $this->drupalGet('node/add/' . $type);
    if (array_key_exists('date', $options)) {
      $options['field_date[value][date]'] = $options['date'];
      unset($options['date']);
    }
    if (array_key_exists('date_to', $options)) {
      $options['field_date[value2][date]'] = $options['date_to'];
      unset($options['date_to']);
    }

    $this->drupalPost(NULL, $options, t('Save'));
  }

  public function CreateMeeting($options) {
    $this->drupalGet('node/add/meeting');
    if (array_key_exists('meeting_date', $options)) {
      $options['field_meeting_date[value][date]'] = $options['meeting_date'];
      unset($options['meeting_date']);
    }
    if (array_key_exists('meeting_date_to', $options)) {
      $options['field_meeting_date[value2][date]'] = $options['meeting_date_to'];
      unset($options['meeting_date_to']);
    }

    $this->drupalPost(NULL, $options, t('Save'));
  }

  public function CreateAnnounce($options) {
    $this->drupalGet('admin/build/node_announce/add');
    $this->drupalPost(NULL, $options, t('Create'));
  }

  public function Get($url) {
    return $this->drupalGet($url);
  }

  public function Post($one, $two, $three) {
    return $this->drupalPost($one, $two, $three);
  }

   /**
   * Runs cron in the Drupal installed by Simpletest.
   */
  protected function cronRun() {
    $this->drupalGet($GLOBALS['base_url'] . '/cron.php', array('external' => TRUE, 'query' => array('cron_key' => variable_get('cron_key', 'drupal'))));
  }

}