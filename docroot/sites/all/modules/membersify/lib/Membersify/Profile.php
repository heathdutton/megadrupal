<?php

/**
 * Membersify_Profile class
 */
class Membersify_Profile extends Membersify_ApiResource {
  protected static $object = 'profile';

  public $id = '';
  public $user_id = 0;
  public $display = '';
  public $status = '';
  public $livemode = FALSE;

  /**
   * Deletes the payment profile via the API.
   *
   * @return mixed
   */
  public function delete() {
    return $this->request('profile/delete', array('id' => $this->id));
  }
}
