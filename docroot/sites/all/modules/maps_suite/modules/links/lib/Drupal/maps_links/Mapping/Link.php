<?php

namespace Drupal\maps_links\Mapping;

use Drupal\maps_import\Mapping\Mapping;
use Drupal\maps_import\Mapping\Source\MapsSystem as MS;
use Drupal\maps_links\Mapping\Target\Drupal\Relation;
use Drupal\maps_import\Cache\Data\MappingItems as CacheMappingItems;

class Link extends Mapping {

	/**
	 * The entities correspondance table.
	 */
	const DB_ENTITIES_TABLE = 'maps_links_entities';

  /**
   * @inheritdoc
   */
  protected $type = 'link_mapping';

  /**
   * @inheritdoc
   */
  public function getSourceProperties() {
    // @todo use a cache class.
    // @todo handle languages
    static $properties;

    if (!isset($properties)) {
      $properties = parent::buildSourcePropertyHandlers(array(
        'count' => array('title' => 'Count'),
      ));
    }

    return $properties;
  }

  /**
   * @inheritdoc
   * @todo implement log.
   */
  public function process(MS\Entity $entity) {
    self::$currentEntity = $entity;

    try {
      // Set raw values in MaPS Entity.
      foreach ($this->getItems() as $item) {
        $entity->setRawValues($item->getProperty(), $item->getOptions());
      }

      // Create a new Drupal Entity.
      $drupalEntity = new Relation($this->getConverter(), array());

      // Set values in Drupal Entity.
      foreach ($this->getItems() as $item) {
        $drupalEntity->getTranslationHandler()->setValue($item->getField(), $item->getProperty(), $entity, $item->isRequired());
      }

	    try {
          $drupalEntity->save();

          foreach ($drupalEntity->getIdentifiers() as $identifier) {
            $fields = array(
              'link_id' => $entity->id,
              'pid' => $this->getConverter()->getProfile()->getPid(),
              'cid' => $this->getConverter()->getCid(),
              'relation_id' => $identifier,
            );

            // We check if there is already these values in DB_ENTITIES_TABLE;
            $query_select = db_select(self::DB_ENTITIES_TABLE)
              ->fields(self::DB_ENTITIES_TABLE, array('link_id'));
            foreach ($fields as $index => $value) {
              $query_select->condition($index, $value);
            }

            if (!$query_select->execute()->fetchField()) {
              $id = db_insert(self::DB_ENTITIES_TABLE)
                ->fields($fields)
                ->execute();
            }
          }
	    }
	    catch (MappingException $e) {
	      return array();
	    }

	    return array($entity, $drupalEntity);
	  }
    catch (MappingException $e) {
      return array();
    }
  }

  /**
   * @inheritdoc
   */
  public function getTargetFields() {
  	$fields = parent::getTargetFields();
  	unset($fields['endpoints']);
  	return $fields;
  }

  /**
   * @inheritdoc
   */
  public function getItems($type = 'default') {
    return CacheMappingItems::getInstance()->load(array(
      'converter' => $this->getConverter(),
      'type' => 'link',
    ));
  }
}
