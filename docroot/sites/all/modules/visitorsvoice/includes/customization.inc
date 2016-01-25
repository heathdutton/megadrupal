<?php

/**
 * @file
 * Contains VisitorsvoiceCustomization.
 */


/**
 * Represents a specific customization for a certain item and set of keywords.
 */
class VisitorsvoiceCustomization implements VisitorsvoiceCustomizationInterface {

  /**
   * The type of customization.
   *
   * @var string
   *
   * @see getType()
   */
  protected $type;

  /**
   * The customization ID.
   *
   * @var int
   *
   * @see getCustomizationId()
   */
  protected $customizationId;

  /**
   * The order.
   *
   * @var int
   *
   * @see getOrder()
   */
  protected $order;

  /**
   * Constructs a new customization.
   *
   * @param string $type
   *   The type of customization. See getType().
   * @param int $customizationId
   *   The customization ID. See getCustomizationId().
   * @param int $order
   *   (optional) The order. See getOrder().
   */
  public function __construct($type, $customizationId, $order = 0) {
    $this->type = $type;
    $this->customizationId = $customizationId;
    $this->order = $order;
  }

  /**
   * {@inheritdoc}
   */
  public function getType() {
    return $this->type;
  }

  /**
   * {@inheritdoc}
   */
  public function getCustomizationId() {
    return $this->customizationId;
  }

  /**
   * {@inheritdoc}
   */
  public function getOrder() {
    return $this->order;
  }
}
