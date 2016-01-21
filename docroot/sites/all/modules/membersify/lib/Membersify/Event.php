<?php

/**
 * Membersify_Event class
 */
class Membersify_Event extends Membersify_ApiResource {
  protected static $object = 'event';

  public $id = "";
  public $event_type = '';
  public $data = array();
  public $livemode = FALSE;
}
