<?php

/**
 * Entity class representing the commerce_return_reason entity type.
 */
class CommerceReturnReason extends Entity {

  /**
   * The primary identifier for a return.
   *
   * @var int
   */
  public $return_reason_id = NULL;

  /**
   * The machine name for a return reason.
   *
   * @var string
   * @maxlength 128
   */
  public $name = '';

  /*
  * The human name aka label for a return reason.
  *
  * @var string
  * @maxlength 128
  */
  public $label = '';

  /**
   * A brief description of the return reason.
   *
   * @var string
   * @maxlength 256
   */
  public $description = '';

  /**
   * The bundle name.
   *
   * @var string
   */
  public $type = 'commerce_return_reason';

  /**
   * A serialized data column.
   *
   * @var string
   */
  public $data = '';

  /**
   * Constructor method to instantiate a new CommerceReturn object.
   *
   * @param array $values
   *   Values to be passed to the new CommerceReturnReason object.
   *
   * @throws Exception
   *   A fully loaded commerce_order entity type must be passed into the values
   *   array otherwise it throws a new php Exception.
   *
   * @return CommerceReturnReason
   *   A fully qualified CommerceReturnReason object.
   *
   */
  public function __construct($values = array()) {
    return parent::__construct($values, 'commerce_return_reason');
  }

  public function label() {
    return $this->label;
  }

  protected function defaultUri() {
    $entity_info = entity_get_info('commerce_return');
    $path = $entity_info['admin ui']['path'] . '/manage';

    return array('path' => $path . '/' . $this->name);
  }
}