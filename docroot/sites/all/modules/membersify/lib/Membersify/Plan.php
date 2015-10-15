<?php

/**
 * MembersifyPlan class
 */
class Membersify_Plan extends Membersify_ApiResource {
  protected static $object = 'plan';

  // Declare properties.
  public $id = '';
  public $name = '';
  public $type = '';
  public $machine_name = '';
  public $description = '';
  public $payment_plan = array();
  public $weight = 0;
  public $data = array();
  public $livemode = FALSE;
}
