<?php

/**
 * The Controller for Avatar entities
 */
class AvatarTypeController extends EntityAPIControllerExportable {
  public function __construct($entityType) {
    parent::__construct($entityType);
  }

  /**
   * Create a avatar type - we first set up the values that are specific
   * to our avatar type schema but then also go through the EntityAPIController
   * function.
   *
   * @param $type
   *   The machine-readable type of the avatar.
   *
   * @return
   *   A avatar type object with all default fields initialized.
   */
  public function create(array $values = array()) {
    // Add values that are specific to our Avatar
    $values += array(
        'id' => '',
        'is_new' => TRUE,
        'data' => '',
    );
    $avatar_type = parent::create($values);
    return $avatar_type;
  }
}