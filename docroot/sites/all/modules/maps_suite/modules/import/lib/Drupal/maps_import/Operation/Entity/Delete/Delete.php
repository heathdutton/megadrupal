<?php

namespace Drupal\maps_import\Operation\Entity\Delete;

use Drupal\maps_import\Profile\Profile;
use Drupal\maps_import\Converter\ConverterInterface;
use Drupal\maps_import\Mapping\MappingInterface;
use Drupal\maps_import\Operation\Operation;

class Delete extends Operation {

  /**
   * The related profile.
   * @var Profile
   */
  protected $profile;

  /**
   * The related converter.
   * @var ConverterInterface
   */
  protected $converter;

  /**
   * @inheritdoc
   */
  public function __construct(ConverterInterface $converter) {
    $this->converter = $converter;
    $this->profile = $converter->getProfile();
  }

 /**
   * @inheritdoc
   */
  public function getTitle(){
    return t('Entity Delete');
  }

  /**
   * @inheritdoc
   */
  public function getDescription(){
    return t('Delete all Drupal entities related to the given converter.');
  }

  /**
   * @inheritdoc
   */
  public function getType(){
    return 'entity_delete';
  }

  /**
   * Get all converters (parent + children).
   *
   * @param $converter
   *   The parent converter.
   */
  public function getConverters() {
  	$children = db_select('maps_import_converter', 'c')
      ->fields('c')
      ->condition('parent_id', $this->converter->getCid())
      ->execute()
      ->fetchAllAssoc('cid', \PDO::FETCH_ASSOC);

    $converters = array_keys($children);
    $converters[] = (int) $this->converter->getCid();

    return $converters;
  }

 /**
   * Return the current entities to process.
   *
   *  @param $start
   *    The index to start.
   *  @param $maxEntities
   *    The max limit.
   *
   *  @return array
   *    The libraries to process.
   */
  protected function getCurrentEntities($start = 0, $maxEntities = 0) {
    $query = db_select(MappingInterface::DB_ENTITIES_TABLE, 'e')
      ->fields('e')
      ->condition('cid', $this->getConverters());

    if ($maxEntities > 0) {
      $query->range($start, $maxEntities);
    }

    return $query->execute()
      ->fetchAllAssoc('id', \PDO::FETCH_ASSOC);
  }

  /**
   * @inheritdoc
   */
  public function batchOperations() {
  	$this->getConverters();
    $operations = array();
    $total = db_select(MappingInterface::DB_ENTITIES_TABLE, 'e')
      ->fields('e')
      ->condition('cid', $this->getConverters())
      ->execute()
      ->rowCount();

    $maxEntities = $this->profile->getMaxObjectsPerOp();
    $count = ceil($total / $maxEntities);

    for ($i = 0; $i < $count; $i++) {
      $operations[] = array(
        // Callback function
        array($this, 'process'),
        // Callback arguments
        array(
          0,
          $maxEntities,
          (bool) ($i == ($count - 1)),
        )
      );
    }

    return $operations;
  }

  /**
   * @inheritdoc
   */
  protected function processRun($start = 0, $range = 0) {
  	foreach ($this->getCurrentEntities($start, $range) as $currentEntity) {
  		if ($currentEntity['entity_type'] === 'commerce_product') {
        commerce_product_delete($currentEntity['entity_id']);
  		}
      else {
        entity_delete($this->converter->getEntityType(), $currentEntity['entity_id']);
      }
  	}

    return TRUE;
  }

}
