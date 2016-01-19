<?php

/**
 * @file
 * Contains DailyEventHandler.
 */

namespace Drupal\rules_repeated_events\Plugin\Rules\Event;

/**
 * Event handler for tweets on the personal timeline.
 */
class DailyEventHandler extends RepeatedEventHandlerBase {

  /**
   * Defines the event.
   */
  public static function getInfo() {
    return static::getInfoDefaults() + array(
      'name' => 'daily_events',
      'label' => t('Daily Events.'),
      'variables' => array(),
    );
  }

  /**
   * {@inheritdoc}
   */
  public function getTaskHandler() {
    return 'Drupal\rules_repeated_events\TaskHandler\DailyEventsTaskHandler';
  }

  /**
   * {@inheritdoc}
   */
  public function getDefaults() {
    return array(
      'time' => '12:00am',
    );
  }

  /**
   * {@inheritdoc}
   */
  public function buildForm(array &$form_state) {
    $settings = $this->getSettings();

    $form['time'] = array(
      '#type' => 'jquery_timepicker',
      '#title' => t('Time'),
      '#description' => t('The time to do action daily.'),
      '#default_value' => $settings['time'],
      '#required' => TRUE,
    );

    return $form;
  }

  /**
   * {@inheritdoc}
   */
  public function validate() {
    $settings = $this->getSettings();

    if (!preg_match("/\d{1,2}:\d{2}[a|p]m/", $settings['time'])) {
      $now = time() + rules_repeated_events_get_local_server_timestamp_diff();
      $settings['time'] = date('g', $now) . ':' . date('i', $now) . date('a', $now);
      $this->setSettings($settings);
    }
  }

  /**
   * {@inheritdoc}
   */
  public function summary() {
    $settings = $this->getSettings();
    if ($settings['time']) {
      return t('Actions will be triggered every day on %time.', array('%time' => $settings['time']));
    }
    return $this->eventInfo['label'];
  }

  /**
   * {@inheritdoc}
   */
  public function startWatching() {
    $now = time();

    // Scheduled time in local timezone.
    $settings = $this->getSettings();
    $scheduled_time = strtotime($settings['time'], $now) - rules_repeated_events_get_local_server_timestamp_diff();

    // Find the first scheduled time.
    if ($scheduled_time < $now) {
      // It is late, will trigger on next day.
      $scheduled_time = strtotime('+1 day', $scheduled_time);
    }

    rules_scheduler_schedule_task(array(
      'date' => $scheduled_time,
      'identifier' => "{$this->getEventName()}--{$this->getEventNameSuffix()}",
      'config' => '',
      'data' => $this->getSettings(),
      'handler' => $this->getTaskHandler(),
    ));
  }

}
