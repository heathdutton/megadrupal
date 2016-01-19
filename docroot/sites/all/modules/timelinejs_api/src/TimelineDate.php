<?php

/**
 * @file
 * Contains \Drupal\timelinejs_api\TimelineDate.
 */

namespace Drupal\timelinejs_api;

/**
 * A representation of timeline date data.
 */
class TimelineDate {

  /**
   * @var \DateTime
   */
  protected $date;

  /**
   * TimelineDate constructor.
   *
   * @param \DateTime $date
   */
  public function __construct(\DateTime $date) {
    $this->date = $date;
  }

  /**
   * Gets the DateTime object.
   *
   * @return \DateTime
   */
  public function getDate() {
    return $this->date;
  }

  /**
   * Helper factory method to create date objects from strings.
   *
   * @param string $date
   * @param string|\DateTimeZone $timezone
   *
   * @return self
   */
  public static function createFromDateString($date, $timezone = NULL) {
    // Allow string timezones.
    if (!empty($timezone) && !is_object($timezone)) {
      $timezone = new \DateTimeZone($timezone);
    }

    return new static(new \DateTime($date, $timezone));
  }

  /**
   * Formats an array of date data.
   *
   * @return array
   */
  public function toArray() {
    return [
      'day' => $this->date->format('d'),
      'month' => $this->date->format('m'),
      'year' => $this->date->format('Y'),
      'display_date' => $this->date->format('F j, Y'),
    ];
  }

}
