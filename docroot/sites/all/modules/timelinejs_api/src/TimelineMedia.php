<?php

/**
 * @file
 * Contains \Drupal\timelinejs_api\TimelineMedia.
 */

namespace Drupal\timelinejs_api;

/**
 * A representation of a timeline media item.
 */
class TimelineMedia {

  /**
   * @var string
   */
  protected $url;

  /**
   * @var string
   */
  protected $caption;

  /**
   * @var string
   */
  protected $credit;

  /**
   * @var string
   */
  protected $thumbnail;

  /**
   * TimelineMedia constructor.
   */
  public function __construct($url, $caption = '', $credit = '', $thumbnail = '') {
    $this->url = $url;
    $this->caption = $caption;
    $this->credit = $credit;
    $this->thumbnail = $thumbnail;
  }

  /**
   * @return string
   */
  public function getUrl() {
    return $this->url;
  }

  /**
   * @return string
   */
  public function getCaption() {
    return $this->caption;
  }

  /**
   * @return string
   */
  public function getCredit() {
    return $this->credit;
  }

  /**
   * @return string
   */
  public function getThumbnail() {
    return $this->thumbnail;
  }

  /**
   * @return array
   */
  public function toArray() {
    // Only URL is required. The rest is optional.
    $data = [
      'url' => $this->getUrl(),
    ];

    if ($caption = $this->getCaption()) {
      $data['caption'] = $caption;
    }

    if ($credit = $this->getCredit()) {
      $data['credit'] = $credit;
    }

    // @todo Optionally use url as thumbnail instead of generic thumbnail.
    if ($thumbnail = $this->getThumbnail()) {
      $data['thumbnail'] = $thumbnail;
    }

    return $data;
  }

}
