<?php

/**
 * Entity class representing the commerce_return entity type.
 */
class CommerceReturn extends Entity {

  /**
   * The primary identifier for a return.
   *
   * @var int
   */
  public $return_id = NULL;

  /**
   * The return number displayed to the customer.
   *
   * @var int
   */
  public $return_number = NULL;

  /**
   * The current revision identifier for a return.
   *
   * @var int
   */
  public $revision_id = NULL;

  /**
   * The bundle name.
   *
   * @var string
   */
  public $type = 'commerce_return';

  /**
   * The user identifier.
   *
   * @var int
   */
  public $uid;

  /**
   * @var string
   * @validate valid_email_address().
   */
  public $mail;

  /**
   * A serialized array of data settings for a return.
   *
   * @var array
   */
  public $data;

  /**
   * The return creation date in timestamp format.
   *
   * @var int
   */
  public $created;

  /**
   * The latest return update in timestamp format.
   *
   * @var int
   */
  public $changed;

  /**
   * The IP Address that created this return.
   *
   * @var string
   */
  public $hostname;

  /**
   * Constructor method to instantiate a new CommerceReturn object.
   *
   * @param array $values
   *   Values to be passed to the new CommerceReturn object.
   *
   * @throws Exception
   *   A fully loaded commerce_order entity type must be passed into the values
   *   array otherwise it throws a new php Exception.
   *
   * @return CommerceReturn
   *   A fully qualified CommerceReturn object.
   *
   */
  public function __construct($values = array()) {
    return parent::__construct($values, 'commerce_return');
  }

  public function isCreatable() {
    //TODO: check the usage of order line items by others returns.
    return TRUE;
  }

  /**
   * Return the order found in the CommerceReturn entity.
   *
   * @return object|bool
   *   A fully loaded commerce_order object. Otherwise return FALSE.
   */
  public function getOrder() {
    $return_wrapper = entity_metadata_wrapper('commerce_return', $this);

    if (array_key_exists('commerce_order_reference', $return_wrapper->getPropertyInfo())) {
      return $return_wrapper->commerce_order_reference;
    }

    return FALSE;
  }

  protected function defaultUri() {
    $entity_info = entity_get_info('commerce_return');
    $path = $entity_info['admin ui']['path'] . '/manage';
    return array('path' => $path . '/' . $this->name);
  }
}
