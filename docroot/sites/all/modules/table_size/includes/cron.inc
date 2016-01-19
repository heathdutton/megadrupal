<?php
/**
 * @file
 * Provides the cron functionality for the Table Size module.
 */

/**
 * Provies hook implementations for the Table Size module.
 */
class TableSizeCron {

  const RUN_NEVER = 0;
  const RUN_EVERY_TIME = 1;
  const RUN_BETWEEN_INTERVALS = 2;

  /**
   * @var int
   * A timestamp of when the last Table Size cron run occured.
   */
  private $last_run;

  /**
   * @var array
   * An array of various cron run settings.
   */
  private $settings;

  public function __construct() {
    $this->last_run = LazyVars::get('table_size_last_cron', 0, FALSE);
    $this->settings = LazyVars::get('table_size_cron_settings', 0, FALSE);
  }

  /**
   * Provides an implementation for hook_cron().
   */
  public function run() {
    if ($this->checkConditions()) {
      $langs = language_list();
      if (isset($this->settings['language']) && isset($langs[$this->settings['language']])) {
        $language = $langs[$this->settings['language']];
      }
      else {
        global $language;
      }
      $params = TableSizeCheck::getTablesExceededQuota();
      LazyVars::set('table_size_last_cron', REQUEST_TIME, FALSE);
      drupal_mail('table_size', 'table_size_notification', $this->settings['email'], $language, $params);
    }
  }

  /**
   * Determines if the cron should run or not.
   *
   * @return bool
   */
  private function checkConditions() {
    if (!$this->settings) {
      return FALSE;
    }

    if ($this->settings['run'] == self::RUN_EVERY_TIME) {
      return TRUE;
    }

    $minutes_now = date('G') * 60 + date('i');
    $minutes_min = TableSizeUtil::timeToMinutes($this->settings['min']);
    $minutes_max = TableSizeUtil::timeToMinutes($this->settings['max']);
    if (
      $this->settings['run'] == self::RUN_BETWEEN_INTERVALS &&
      date('j', $this->last_run) != date('j') &&
      $minutes_now > $minutes_min && $minutes_now < $minutes_max
    ) {
      return TRUE;
    }

    return FALSE;
  }

}