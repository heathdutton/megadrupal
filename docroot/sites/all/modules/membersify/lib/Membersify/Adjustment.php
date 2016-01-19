<?php

/**
 * Membersify_Adjustment class
 */
class Membersify_Adjustment extends Membersify_ApiResource {
  protected static $object = 'adjustment';

  public $id = '';
  public $type = '';
  public $object_type = '';
  public $value = 0;
  public $scope = '';
  public $display = '';
  public $status = '';
  public $optional = FALSE;
  public $livemode = FALSE;
  public $data = array();
}
