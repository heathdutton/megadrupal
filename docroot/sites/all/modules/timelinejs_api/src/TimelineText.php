<?php

/**
 * @file
 * Contains \Drupal\timelinejs_api\TimelineText.
 */

namespace Drupal\timelinejs_api;

/**
 * A representation of timeline text.
 */
class TimelineText {

  /**
   * @var string
   */
  protected $headline;

  /**
   * @var string
   */
  protected $text;

  /**
   * TimelineText constructor.
   *
   * @param string $headline
   * @param string $text
   */
  public function __construct($headline = '', $text = '') {
    $this->headline = $headline;
    $this->text = $text;
  }

  /**
   * @return array
   */
  public function toArray() {
    return [
      'headline' => filter_xss_admin($this->getHeadline()),
      'text' => filter_xss_admin($this->getText()),
    ];
  }

  /**
   * @return string
   */
  public function getHeadline() {
    return $this->headline;
  }

  /**
   * @return string
   */
  public function getText() {
    return $this->text;
  }


}
