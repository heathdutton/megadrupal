<?php

/**
 * @file
 * @todo.
 */

/**
 * Class CommerceCustomerProfile
 */
class CommerceCustomerProfile extends CommerceEntity {
  use CommerceEntityStatus, CommerceEntityUid, CommerceEntityRevisionId,
    CommerceEntityType;

  /**
   * @var int
   */
  public $profile_id;

  /**
   * {@inheritdoc}
   */
  public function getEntityType() {
    return 'commerce_customer_profile';
  }

  /**
   * @return mixed
   */
  public function getProfileId() {
    return $this->profile_id;
  }

}
