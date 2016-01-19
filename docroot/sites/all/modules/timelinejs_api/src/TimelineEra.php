<?php

/**
 * @file
 * Contains \Drupal\timelinejs_api\TimelineEra.
 */

namespace Drupal\timelinejs_api;

/**
 * An object representing a timeline era.
 */
class TimelineEra extends TimelineObjectBase {

  /**
   * @var \Drupal\timelinejs_api\TimelineDate
   */
  protected $startDate;

  /**
   * @var \Drupal\timelinejs_api\TimelineDate
   */
  protected $endDate;

  /**
   * TimelineSlide constructor.
   *
   * @param \Drupal\timelinejs_api\TimelineDate $start_date
   * @param \Drupal\timelinejs_api\TimelineDate $end_date
   * @param \Drupal\timelinejs_api\TimelineText $text
   */
  public function __construct(TimelineDate $start_date, TimelineDate $end_date, TimelineText $text = NULL) {
    if ($start_date->getDate() == $end_date->getDate()) {
      throw new \InvalidArgumentException('Start and end dates cannot be the same for an era');
    }

    parent::__construct($text);

    $this->startDate = $start_date;
    $this->endDate = $end_date;
  }

  /**
   * Gets an array representing a timeline event.
   *
   * @return array
   */
  public function toArray() {
    $data = [
      'start_date' => $this->getStartDate()->toArray(),
      'end_date' => $this->getEndDate()->toArray(),
    ];

    if ($text = $this->getText()) {
      $data['text'] = $this->getText()->toArray();
    }

    return $data;
  }

  /**
   * Gets the start date.
   *
   * @return \Drupal\timelinejs_api\TimelineDate
   */
  public function getStartDate() {
    return $this->startDate;
  }

  /**
   * Gets the end date.
   *
   * @return \Drupal\timelinejs_api\TimelineDate
   */
  public function getEndDate() {
    return $this->endDate;
  }

}
