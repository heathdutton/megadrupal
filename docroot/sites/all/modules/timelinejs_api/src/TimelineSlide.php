<?php

/**
 * @file
 * Contains \Drupal\timelinejs_api\TimelineSlide.
 */

namespace Drupal\timelinejs_api;

/**
 * An object representing a timeline slide.
 *
 * These are used for events and title slides.
 */
class TimelineSlide extends TimelineObjectBase {

  /**
   * @var \Drupal\timelinejs_api\TimelineDate
   */
  protected $date;

  /**
   * @var string
   */
  protected $group;

  /**
   * Attached media object.
   *
   * @var \Drupal\timelinejs_api\TimelineMedia
   */
  protected $media;

  /**
   * TimelineSlide constructor.
   *
   * @param \Drupal\timelinejs_api\TimelineDate $date
   * @param \Drupal\timelinejs_api\TimelineText $text
   * @param string $group
   * @param \Drupal\timelinejs_api\TimelineMedia $media
   */
  public function __construct(TimelineDate $date, TimelineText $text = NULL, $group = '', TimelineMedia $media = NULL) {
    parent::__construct($text);

    $this->date = $date;
    $this->group = $group;
    $this->media = $media;
  }

  /**
   * Gets an array representing a timeline event.
   *
   * @return array
   */
  public function toArray() {
    $data = [
      'start_date' => $this->getDate()->toArray(),
      'text' => $this->getText()->toArray(),
    ];

    // Add group, if there is one.
    if ($group = $this->getGroup()) {
      $data['group'] = $group;
    }

    // Add a media object if there is one.
    if ($media = $this->getMedia()) {
      $data['media'] = $media->toArray();
    }

    return $data;
  }

  /**
   * Gets the date.
   *
   * @return \Drupal\timelinejs_api\TimelineDate
   */
  public function getDate() {
    return $this->date;
  }

  /**
   * Gets the group.
   *
   * @return string
   */
  public function getGroup() {
    return $this->group;
  }

  /**
   * Gets the media object.
   *
   * @return TimelineMedia
   */
  public function getMedia() {
    return $this->media;
  }

  /**
   * Sets the media object.
   *
   * @param TimelineMedia $media
   */
  public function setMedia(TimelineMedia $media) {
    $this->media = $media;
  }

}
