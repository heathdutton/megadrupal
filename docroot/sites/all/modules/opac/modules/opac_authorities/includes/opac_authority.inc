<?php
/**
 * @file
 * Opac authority class and controller.
 */

/**
 * Authority class.
 */
class OpacAuthority extends Entity {
  /**
   * Return opac authoriy label.
   */
  protected function defaultLabel() {
    return $this->title;
  }

  /**
   * Return opac authoriy uri.
   */
  protected function defaultUri() {
    return array('path' => 'opac_authority/' . $this->identifier());
  }
}

class OpacAuthorityController extends EntityAPIController {
  /**
   * Override parent::create.
   *
   * @param array $values
   *   An array of values to set.
   *
   * @return array
   *   A instance of the entity.
   */
  public function create(array $values = array()) {
    global $user;
    $values += array(
      'title' => '',
      'description' => '',
      'created' => REQUEST_TIME,
      'changed' => REQUEST_TIME,
      'uid' => $user->uid,
    );
    return parent::create($values);
  }
}
