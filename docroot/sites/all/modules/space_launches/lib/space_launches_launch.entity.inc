<?php

/**
 * @file
 * Defines the Space Launches Launch entity.
 */
class SpaceLaunchesLaunch extends Entity {

  // @codingStandardsIgnoreStart
  public $launch_id = NULL;
  public $source = '';
  public $source_uid = '';
  public $source_updated = 0;
  public $title = '';
  public $description = '';
  public $url = '';
  public $time = 0;
  public $time_is_exact = 0;
  public $created = '';
  public $updated = '';

  // @codingStandardsIgnoreEnd

  /**
   * Override parent constructor.
   */
  public function __construct(array $values = array()) {
    parent::__construct($values, 'space_launches_launch');

    // Check for new entity.
    if (!$this->launch_id) {
      $this->created = time();
    }
  }

  /**
   * Specifies the default entity label.
   */
  protected function defaultLabel() {
    return $this->title;
  }

}
