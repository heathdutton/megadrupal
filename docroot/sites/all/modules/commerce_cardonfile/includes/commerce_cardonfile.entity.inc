<?php

/**
 * Entity class representing the commerce_cardonfile entity type.
 */
class CommerceCardOnFile extends Entity {

  /**
   * The card id.
   *
   * @var integer
   */
  public $card_id;

  /**
   * The uid of the card owner.
   *
   * @var integer
   */
  public $uid;

  /**
   * The method_id of the payment method that stored the card.
   *
   * @var string
   */
  public $payment_method;

  /**
   * The instance_id of the payment method that stored the card.
   *
   * @var string
   */
  public $instance_id;

  /**
   * The id of the card at the payment gateway.
   *
   * @var string
   */
  public $remote_id;

  /**
   * The card type.
   *
   * @var string
   */
  public $card_type;

  /**
   * The name on the card.
   *
   * @var string
   */
  public $card_name;

  /**
   * Truncated card number (last 4 digits).
   *
   * @var string
   */
  public $card_number;

  /**
   * Expiration month.
   *
   * @var integer
   */
  public $card_exp_month;

  /**
   * Expiration year.
   *
   * @var integer
   */
  public $card_exp_year;

  /**
   * Whether this is the default card for this payment method instance..
   *
   * @var boolean
   */
  public $instance_default = 1;

  /**
   * Card status: inactive (0), active (1), not deletable (2), declined (3).
   *
   * @var integer
   */
  public $status = 1;

  /**
   * The Unix timestamp when the card was first stored.
   *
   * @var integer
   */
  public $created;

  /**
   * The Unix timestamp when the card was last updated.
   *
   * @var integer
   */
  public $changed;

  public function __construct($values = array()) {
    parent::__construct($values, 'commerce_cardonfile');
  }

  /**
   * Overrides Entity::defaultLabel().
   */
  protected function defaultLabel() {
    $card_type = t('Card');
    if (!empty($this->card_type)) {
      $card_types = commerce_cardonfile_credit_card_types();
      if (isset($card_types[$this->card_type])) {
        $card_type = $card_types[$this->card_type];
      }
    }

    $args = array(
      '@card_type' => $card_type,
      '@card_number' => $this->card_number
    );
    return t('@card_type ending in @card_number', $args);
  }

  /**
   * Overrides Entity::save().
   */
  public function save() {
    $this->changed = REQUEST_TIME;
    // Set the created timestamp during initial save.
    if (!$this->card_id) {
      $this->created = REQUEST_TIME;
    }
    if ($this->card_id) {
      $this->original = $original = entity_load_unchanged('commerce_cardonfile', $this->card_id);
      // Reactivate a declined card after its expiration date has been modified.
      if ($this->status == 3) {
        $exp_month_changed = $this->original->card_exp_month != $this->card_exp_month;
        $exp_year_changed = $this->original->card_exp_year != $this->card_exp_year;
        if ($exp_month_changed || $exp_year_changed) {
          $this->status = 1;
        }
      }
    }

    // Perform the save.
    parent::save();

    // If the card is now instance_default, remove the flag from other cards.
    $is_update = isset($original);
    $value_changed = $is_update && $this->instance_default != $original->instance_default;
    if ($this->instance_default && (!$is_update || $value_changed)) {
      $query = new EntityFieldQuery();
      $query->entityCondition('entity_type', 'commerce_cardonfile');
      $query->entityCondition('entity_id', $this->card_id, '<>');
      $query->propertyCondition('instance_id', $this->instance_id);
      $query->propertyCondition('uid', $this->uid);
      $query->propertyCondition('instance_default', TRUE);
      $result = $query->execute();
      if (isset($result['commerce_cardonfile'])) {
        $card_ids = array_keys($result['commerce_cardonfile']);
        $other_cards = commerce_cardonfile_load_multiple($card_ids);
        foreach ($other_cards as $other_card) {
          $other_card->instance_default = 0;
          commerce_cardonfile_save($other_card);
        }
      }
    }
  }
}
