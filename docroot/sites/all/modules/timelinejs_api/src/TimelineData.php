<?php

/**
 * @file
 * Contains \Drupal\timelinejs_api\TimelineData.
 */

namespace Drupal\timelinejs_api;

/**
 * A class representing timeline JS data.
 */
class TimelineData {

  /**
   * @var \Drupal\timelinejs_api\TimelineSlide
   */
  protected $title;

  /**
   * @var \Drupal\timelinejs_api\TimelineSlide[]
   */
  protected $events = [];

  /**
   * @var \Drupal\timelinejs_api\TimelineEra[]
   */
  protected $eras = [];

  /**
   * @return \Drupal\timelinejs_api\TimelineSlide
   */
  public function getTitle() {
    return $this->title;
  }

  /**
   * @param TimelineSlide $title
   */
  public function setTitle(TimelineSlide $title) {
    $this->title = $title;
  }

  /**
   * @return TimelineSlide[]
   */
  public function getEvents() {
    return $this->events;
  }

  /**
   * Adds an event object.
   *
   * @param \Drupal\timelinejs_api\TimelineSlide $event
   *
   * @return $this
   */
  public function addEvent(TimelineSlide $event) {
    $this->events[] = $event;
    return $this;
  }

  /**
   * Adds multiple event objects.
   *
   * @param \Drupal\timelinejs_api\TimelineSlide[] $events
   *
   * @return $this
   */
  public function addEvents(array $events) {
    $this->events = array_merge($this->events, array_values($events));
    return $this;
  }

  /**
   * Set entire events arrays.
   *
   * This will replace any existing events.
   *
   * @param TimelineSlide[] $events
   */
  public function setEvents($events) {
    $this->events = $events;
  }

  /**
   * @return TimelineEra[]
   */
  public function getEras() {
    return $this->eras;
  }

  /**
   * Adds an era object.
   *
   * @param \Drupal\timelinejs_api\TimelineEra $era
   *
   * @return $this
   */
  public function addEra(TimelineEra $era) {
    $this->eras[] = $era;
    return $this;
  }

  /**
   * Adds multiple era objects.
   *
   * @param \Drupal\timelinejs_api\TimelineEra[] $eras
   *
   * @return $this
   */
  public function addEras(array $eras) {
    $this->eras = array_merge($this->eras, array_values($eras));
    return $this;
  }

  /**
   * Set entire eras array.
   *
   * This will replace any existing eras.
   *
   * @param TimelineEra[] $eras
   */
  public function setEras(array $eras) {
    $this->eras = $eras;
  }

  /**
   * Returns an array representation of this timeline data.
   *
   * @return array
   */
  public function toArray() {
    $data =  [
      'events' => $this->processObjectsToArray($this->events),
      'eras' => $this->processObjectsToArray($this->eras),
    ];

    // Add the title if one has been set.
    if ($title = $this->getTitle()) {
      $data['title'] = $title->toArray();
    }

    return $data;
  }

  /**
   * Collects an array of data from each object.
   *
   * @param array $objects
   *   An array of objects to call toArray() on and return data for.
   *
   * @return array
   *   An array of aggregated data from all objects.
   */
  protected function processObjectsToArray(array $objects) {
    return array_map(function($object) {
      return $object->toArray();
    }, $objects);
  }

}
