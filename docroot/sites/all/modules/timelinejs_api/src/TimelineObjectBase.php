<?php

/**
 * @file
 * Contains \Drupal\timelinejs_api\TimelineObjectBase.
 */

namespace Drupal\timelinejs_api;

/**
 * Base timeline class for dealing with date objects.
 */
abstract class TimelineObjectBase {

  /**
   * @var \Drupal\timelinejs_api\TimelineText
   */
  protected $text;

  /**
   * TimelineObjectBase constructor.
   *
   * @param \Drupal\timelinejs_api\TimelineText $text
   */
  public function __construct(TimelineText $text = NULL) {
    $this->text = $text;
  }

  /**
   * @return TimelineText
   */
  public function getText() {
    return $this->text;
  }

  /**
   * @param TimelineText $text
   */
  public function setText(TimelineText $text) {
    $this->text = $text;
  }

}
