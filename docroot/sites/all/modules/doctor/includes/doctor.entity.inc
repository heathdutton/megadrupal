<?php
/**
 * @file
 * Doctor entity class handler.
 */
class doctorEntity extends Entity {

  /**
   * The identifier of the the doctor item.
   * @var
   */
  public $id;

  /**
   * The type of the doctor item. By default - 'doctor'.
   * @var
   */
  public $type = 'doctor';

  /**
   * The timestamp the doctor item created.
   * @var
   */
  public $created;

  /**
   * The serialized date of the doctor item.
   * @var
   */
  public $data;

  /**
   * Determine if the item has been watched in the CLI
   * @var
   */
  public $watched;

  function __construct($values) {
    $values += array(
      'created' => time(),
    );

    parent::__construct($values, 'doctor');
  }

  /**
   * Overrides create().
   */
  public function create(array $values = array()) {
    $this->data = serialize($values['data']);
    $this->created = $values['created'];

    return $this;
  }

  /**
   * Un-publish the doctor item.
   */
  function watched() {
    $this->watched = TRUE;
    $this->save();
  }
}
