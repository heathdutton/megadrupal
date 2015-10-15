<?php

/**
 * Membersify_HistoryItem class
 */
class Membersify_HistoryItem extends Membersify_ApiResource {
  protected static $object = 'history';

  public $id = "";
  public $subscription_id = '';
  public $type = '';
  public $txn_id = '';
  public $amount = 0;
  public $currency = '';
  public $created = 0;
  public $livemode = FALSE;
}
