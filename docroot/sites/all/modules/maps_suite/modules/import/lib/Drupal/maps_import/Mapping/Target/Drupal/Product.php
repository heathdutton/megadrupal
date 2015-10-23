<?php

/**
 * @file
 * Class that defines a Drupal node Entity.
 */
namespace Drupal\maps_import\Mapping\Target\Drupal;

use Drupal\maps_import\Mapping\Source\MapsSystem as MS;
use Drupal\maps_import\Converter\ConverterInterface;
use Drupal\maps_import\Mapping\Mapping;
use Drupal\maps_import\Mapping\Target\Drupal\Field\DefaultField;
use Drupal\maps_import\Mapping\Target\Drupal\EntityInterface;

class Product extends Entity {

  /**
   * @inheritdoc
   */
  protected $primaryKey = 'product_id';

  /**
   * @inheritdoc
   */
  public function __construct(ConverterInterface $converter, array $existingEntities = array()) {
    $uid = $converter->getParentId() ? $converter->getParent()->getUid() : $converter->getUid();
    $uidProperty = MS\PropertyWrapper::load($converter, $uid);
    $sku = $uidProperty->getValues(Mapping::getCurrentEntity());
    $existingEntities = $result = array();

    if ($sku) {
      // Retreive existing entities from the commerce_product table.
      $result = db_select('commerce_product', 'cp')
        ->fields('cp')
        ->condition('cp.sku', $sku)
        ->execute()
        ->fetchAllAssoc('sku', \PDO::FETCH_ASSOC);

      // @todo Find a clean way to manage Commerce Product's translation.
      foreach ($result as &$entity) {
        $existingEntities[$converter->getProfile()->getLanguage($entity['language'])] = $entity;
      }
      $existingEntities[EntityInterface::LANGUAGE_NONE] = reset($result);
    }

    parent::__construct($converter, $existingEntities);

    $this->getTranslationHandler()->setValue(DefaultField::load($converter, 'sku'), $uidProperty, Mapping::getCurrentEntity(), TRUE);
  }

  /**
   * @inheritdoc
   */
  protected function getTranslationClass() {
    if (commerce_product_entity_translation_supported_type($this->getBundle())) {
      return 'Drupal\\maps_import\\Mapping\\Target\\Drupal\\Translation\\FieldTranslation';
    }

    return 'Drupal\\maps_import\\Mapping\\Target\\Drupal\\Translation\\NoTranslation';
  }

  /**
   * @inheritdoc
   */
  public function addEntity($languageId, array $entity = array()) {
    $id = isset($entity['product_id']) ? $entity['product_id'] : NULL;
    $this->entities[$languageId] = $this->createEntity($languageId, $id);
  }

  /**
   * @inheritdoc
   */
  public function save() {
  	foreach ($this->getEntities() as $wrapper) {
      $entity = $wrapper->value();

  	  if (empty($entity->created)) {
        $entity->created = time();
      }

      if (empty($entity->uid)) {
        $entity->uid = 0;
      }
  	}

    parent::save();
  }

  /**
   * @inheritdoc
   */
  public function unpublishEntities(ConverterInterface $converter) {
    foreach ($this->getIdentifiers() as $product_id) {
      $product = commerce_product_load($product_id);
      $product->status = 0;

      commerce_product_save($product);
    }
  }

  /**
   * @inheritdoc
   */
  public function hasPublicationFeature() {
    return TRUE;
  }

  public static function getNodeFromProduct($sku) {
    $product = commerce_product_load_by_sku($sku);

  	// Itterate thhrough fields which refer to products.
    foreach (commerce_info_fields('commerce_product_reference') as $field['field_name']) {
	    // Build query.
	    $query = new EntityFieldQuery;
	    $query->entityCondition('entity_type', 'node', '=')
	      ->fieldCondition($field['field_name'], 'product_id', $product->product_id, '=')
	      ->range(0, 1);

	    if ($result = $query->execute()) {
	      // Return node id.
	      return array_shift(array_keys($result['node']));
	    }
	  }

    return false;
  }

  /**
   * @inheritdoc
   */
  public function getIdentifiers() {
    if (empty($this->entities)) {
      return array();
    }

    $ids = array();
    foreach ($this->entities as $languageId => $wrapper) {
      // In case of the entity is not a wrapper
      // @check is this case normal ?
      $values = $wrapper->value();
      $ids[$languageId] = $values->product_id;
    }

    return $ids;
  }
}
