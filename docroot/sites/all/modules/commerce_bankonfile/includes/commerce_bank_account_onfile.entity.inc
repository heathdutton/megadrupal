<?php

/**
 * Entity class representing the commerce_bank_account_file entity type.
 */
class CommerceBankAccountFile extends Entity {

  /**
   * An ID for bank account.
   *
   * @var integer
   */
  public $bank_account_id;

  /**
   * The uid of the bank account owner.
   *
   * @var integer
   */
  public $uid;

  /**
   * The method_id of the payment method that stored the bank account.
   *
   * @var string
   */
  public $payment_method;

  /**
   * The instance_id of the payment method that stored the bank account.
   *
   * @var string
   */
  public $instance_id;

  /**
   * The id of the bank account at the payment gateway.
   *
   * @var string
   */
  public $remote_id;

  /**
   * The Bank account type.
   *
   * @var string
   */
  public $bank_account_type;

  /**
   * The Bank name.
   *
   * @var string
   */
  public $bank_name;

  /**
   * The name on the bank account.
   *
   * @var string
   */
  public $bank_account_name;

  /**
   * Truncated bank account number (last 4 digits).
   *
   * @var string
   */
  public $bank_account_number;

  /**
   * Truncated aba routing code (last 4 digits).
   *
   * @var string
   */
  public $aba_code;

  /**
   * Expiration month.
   *
   * @var integer
   */
  // public $card_exp_month;

  /**
   * Expiration year.
   *
   * @var integer
   */
  // public $card_exp_year;

  /**
   * Whether this is the default bank account for this payment method instance..
   *
   * @var boolean
   */
  public $instance_default = 1;

  /**
   * Bank account status: inactive (0), active (1), not deletable (2), declined (3).
   *
   * @var integer
   */
  public $status = 1;

  /**
   * The Unix timestamp when the account was first stored.
   *
   * @var integer
   */
  public $created;

  /**
   * The Unix timestamp when the account was last updated.
   *
   * @var integer
   */
  public $changed;

  public function __construct($values = array()) {
    parent::__construct($values, 'commerce_bank_account_file');
  }

  /**
   * Overrides Entity::defaultLabel().
   */
  protected function defaultLabel() {
    $bank_account_type = t('Bank Account');
    if (!empty($this->bank_account_type)) {
      $bank_account_types = commerce_bank_account_onfile_bank_account_types();
      if (isset($bank_account_types[$this->bank_account_type])) {
        $bank_account_type = $bank_account_types[$this->bank_account_type];
      }
    }

    $args = array(
      '@bank_account_name' => $this->bank_account_name,
      '@bank_account_type' => $bank_account_type,
      '@bank_account_number' => $this->bank_account_number,
      '@bank_name' => $this->bank_name,
      '@aba_code' => $this->aba_code,
    );
    // TODO: Include bank name?
    return t('@bank_account_name (Type: @bank_account_type, Routing: @aba_code, Account: @bank_account_number, Bank: @bank_name)', $args);
  }

  /**
   * Overrides Entity::save().
   */
  public function save() {
    $this->changed = REQUEST_TIME;
    // Set the created timestamp during initial save.
    if (!$this->bank_account_id) {
      $this->created = REQUEST_TIME;
    }
    if ($this->bank_account_id) {
      $this->original = $original = entity_load_unchanged('commerce_bank_account_file', $this->bank_account_id);
      // Reactivate a declined bank account after its expiration date has been modified.
      // if ($this->status == 3) {
      //   $exp_month_changed = $this->original->card_exp_month != $this->card_exp_month;
      //   $exp_year_changed = $this->original->card_exp_year != $this->card_exp_year;
      //   if ($exp_month_changed || $exp_year_changed) {
      //     $this->status = 1;
      //   }
      // }
    }

    // Perform the save.
    parent::save();

    // If the bank account is now instance_default, remove the flag from other bank accounts.
    $is_update = isset($original);
    $value_changed = $is_update && $this->instance_default != $original->instance_default;
    if ($this->instance_default && (!$is_update || $value_changed)) {
      $query = new EntityFieldQuery();
      $query->entityCondition('entity_type', 'commerce_bank_account_file');
      $query->entityCondition('entity_id', $this->bank_account_id, '<>');
      $query->propertyCondition('instance_id', $this->instance_id);
      $query->propertyCondition('uid', $this->uid);
      $query->propertyCondition('instance_default', TRUE);
      $result = $query->execute();
      if (isset($result['commerce_echek_file'])) {
        $bank_account_ids = array_keys($result['commerce_bank_account_file']);
        $other_accounts = commerce_bank_account_file_load_multiple($bank_account_ids);
        foreach ($other_accounts as $other_account) {
          $other_account->instance_default = 0;
          commerce_bank_account_file_save($other_account);
        }
      }
    }
  }
}
