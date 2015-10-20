<?php

/**
 * @file
 * @todo.
 */

/**
 * Class CommerceProduct
 */
class CommerceProduct extends CommerceEntity {
  use CommerceEntityStatus, CommerceEntityUid, CommerceEntityLanguage,
    CommerceEntityRevisionId, CommerceEntityType;

  /**
   * @var
   */
  public $product_id;

  /**
   * @var
   */
  public $sku;

  /**
   * @var
   */
  public $title;

  /**
   * {@inheritdoc}
   */
  public function getEntityType() {
    return 'commerce_product';
  }

  /**
   * @return mixed
   */
  public function getProductId() {
    return $this->product_id;
  }

  /**
   * @return mixed
   */
  public function getSku() {
    return $this->sku;
  }

  /**
   * @param mixed $sku
   */
  public function setSku($sku) {
    $this->sku = $sku;
  }

  /**
   * @return mixed
   */
  public function getTitle() {
    return $this->title;
  }

  /**
   * @param mixed $title
   */
  public function setTitle($title) {
    $this->title = $title;
  }

  /**
   * @return EntityDrupalWrapper
   * @throws \EntityMetadataWrapperException
   */
  public function getUser() {
    return $this->wrapper()->get('user');
  }
}
