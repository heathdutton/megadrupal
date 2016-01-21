<?php

/**
 * @file
 * Definition of the AccountData model class.
 */

namespace Drupal\manual_direct_debit;

/**
 * Model class for manual_direct_debit_account_data.
 *
 * Objects of this class store the account data needed for the
 * manual direct debit payment controller.
 */
class AccountData extends \Drupal\little_helpers\DB\Model {
  protected static $table = 'manual_direct_debit_account_data';
  protected static $key = array('pid');
  protected static $values = array('holder', 'account', 'country');
  protected static $serial = FALSE;
  protected static $serialize = array('account' => TRUE);

  /**
   * Builds a basic select query for account data records.
   */
  public static function queryBase() {
    return db_select(static::$table, 'a')->fields('a');
  }

  /**
   * Loads an AccountData object by it's payment id.
   */
  public static function load($pid) {
    $row = static::queryBase()->condition('pid', $pid)->execute()->fetch();
    if ($row) {
      return new static($row, FALSE);
    }
  }

  /**
   * Constructs an AccountData object based on a payment object.
   */
  public static function fromPayment(\Payment $payment, $new = FALSE) {
    $md = &$payment->method_data;
    $data = array();
    $data['pid'] = $payment->pid;
    $data['holder'] = $md['holder'];
    $data['country'] = $md['country'];
    $data['account'] = array();
    foreach (array('iban', 'bic', 'account', 'bank_code', 'payment_date') as $key) {
      $data['account'][$key] = isset($md[$key]) ? $md[$key] : NULL;
    }
    return new static($data, $new);
  }

  public function data() {
    $data = $this->values(array('holder', 'account', 'country'));
    $data['account'] = unserialize($data['account']);
    return $data;
  }
}
