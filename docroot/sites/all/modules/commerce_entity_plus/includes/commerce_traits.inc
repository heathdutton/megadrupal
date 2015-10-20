<?php

/**
 * @file
 * Contains Traits for usage by Drupal Commerce entities.
 */

/**
 * Trait CommerceEntityStatus
 */
trait CommerceEntityStatus {
  /**
   * @var mixed
   */
  public $status;

  /**
   * @return mixed
   */
  public function getStatus() {
    return $this->status;
  }

  /**
   * @param mixed $status
   */
  public function setStatus($status) {
    $this->status = $status;
  }
}

/**
 * Trait CommerceEntityUid
 */
trait CommerceEntityUid {
  /**
   * @var int
   */
  public $uid;

  /**
   * @return mixed
   */
  public function getUid() {
    return $this->uid;
  }

  /**
   * @param mixed $uid
   */
  public function setUid($uid) {
    $this->uid = $uid;
  }
}

/**
 * Trait CommerceEntityLanguage
 */
trait CommerceEntityLanguage {
  /**
   * @var
   */
  public $language;

  /**
   * @return mixed
   */
  public function getLanguage() {
    return $this->language;
  }

  /**
   * @param mixed $language
   */
  public function setLanguage($language) {
    $this->language = $language;
  }
}

/**
 * Trait CommerceEntityRevisionId
 */
trait CommerceEntityRevisionId {
  /**
   * @var int
   */
  public $revision_id;

  /**
   * @return mixed
   */
  public function getRevisionId() {
    return $this->revision_id;
  }
}

/**
 * Trait CommerceEntityType
 */
trait CommerceEntityType {
  /**
   * @var string
   */
  public $type;

  /**
   * @return mixed
   */
  public function getType() {
    return $this->type;
  }

  /**
   * @param mixed $type
   */
  public function setType($type) {
    $this->type = $type;
  }
}
