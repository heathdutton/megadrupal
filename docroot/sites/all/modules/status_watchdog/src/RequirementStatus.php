<?php
/**
 * @file
 * A requirement as returned from hook_requirements().
 */

namespace Drupal\status_watchdog;

class RequirementStatus {

  protected $title;
  protected $value;
  protected $description;
  protected $severity;

  /**
   * Create a new RequirementStatus object from a requirement array.
   *
   * @param array $requirement
   *   A requirement as returned from hook_requirements().
   */
  public function __construct($requirement) {
    foreach ($requirement as $key => $value) {
      $this->{$key} = $value;
    }
  }

  /**
   * Load all runtime requirements from installed modules.
   *
   * @return RequirementStatus[]
   *   An array of requirements.
   */
  public static function runtime() {
    // Load .install files.
    include_once DRUPAL_ROOT . '/includes/install.inc';
    drupal_load_updates();

    // Check run-time requirements and status information.
    $requirements = module_invoke_all('requirements', 'runtime');
    usort($requirements, '_system_sort_requirements');

    $return = array();
    foreach ($requirements as $r) {
      $return[] = new static($r);
    }

    return $return;
  }

  /**
   * Log this requirement to the watchdog.
   */
  public function log() {
    watchdog('system_status', '@title status: @value@description', $this->placeholders(), $this->watchdogStatus());
  }

  /**
   * Return placeholders for this requirement suitable for format_string().
   *
   * @return array
   *   An array of placeholders.
   */
  protected function placeholders() {
    $placeholders = array(
      '@title' => $this->title,
      '@value' => $this->value,
      '@description' => '.',
    );

    if ($this->description) {
      $placeholders['@description'] = ': ' . $this->description;
    }

    return $placeholders;
  }

  /**
   * Map the status of this requirement to a WATCHDOG_* constant.
   *
   * @return int
   *   The Watchdog status of this requirement.
   */
  protected function watchdogStatus() {
    include_once DRUPAL_ROOT . '/includes/install.inc';

    switch ($this->severity) {
      case REQUIREMENT_INFO:
        return WATCHDOG_INFO;

      case REQUIREMENT_OK:
        return WATCHDOG_NOTICE;

      case REQUIREMENT_WARNING:
        return WATCHDOG_WARNING;

      case REQUIREMENT_ERROR:
        return WATCHDOG_ERROR;
    }

    return NULL;
  }

  /**
   * Return the string associated with this requirement.
   *
   * @return string
   *   The requirement text.
   */
  public function __toString() {
    return t('@title status: @value@description', $this->placeholders());
  }
}
