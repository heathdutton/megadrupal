<?php

/**
 * @file
 * Handle mapping operation.
 */

namespace Drupal\maps_import\Mapping\Mapper;

use Drupal\maps_import\Operation\Operation;
use Drupal\maps_import\Profile\Profile;
use Drupal\maps_import\Mapping\Source\MapsSystem\PropertyWrapper;
use Drupal\maps_suite\Log\Broken as LogBroken;
use Drupal\maps_suite\Log\LogInterface;
use Drupal\maps_suite\Log\Context\Context as LogContext;

/**
 * Base class for all mapping operations.
 */
abstract class Mapper extends Operation {

  /**
   * The list of converters.
   *
   * @var ArrayObject
   */
  protected $converters;

  /**
   * The Log object when it is instanciated, so other classes
   * may write to the Mapper related log.
   *
   * @var LogInterface
   */
  protected static $currentLog;

  /**
   * The current mapping type.
   *
   * @var string
   */
  protected static $currentMappingType;

  /**
   * Class constructor.
   *
   * @param $profile
   *   The profile instance.
   *
   * @return Mapper
   */
  public function __construct(Profile $profile) {
    $this->profile = $profile;
  }

  /**
   * Get the applicable converters.
   */
  protected function getConverters() {
    if (!isset($this->converters)) {
      $this->converters = new \ArrayObject();

      foreach (maps_import_get_converters($this->getProfile()->getPid()) as $converter) {
      	if (is_a($converter, $this->getConverterClass()) && !method_exists($converter, 'getParent')) {
        	$this->converters->append($converter);
        }
      }
    }

    return $this->converters;
  }

  /**
   * Get the expected converter class.
   */
  protected abstract function getConverterClass();

  /**
   * Get the table name that stores the entity fetched from
   * MaPS System®.
   */
  protected abstract function getEntityTable();

  /**
   * Return the current mapping type;
   *
   * @return string
   */
  public static function getCurrentMappingType() {
    return self::$currentMappingType;
  }

  /**
   * Get the total number of objects to map.
   */
  public function getTotal() {
  	return db_query('SELECT COUNT(*) FROM {' . $this->getEntityTable() . '} WHERE pid = :pid AND inserted > 0 ORDER BY inserted ASC', array(':pid' => $this->getProfile()->getPid()))->fetchField();
  }

  /**
   * Map the objects.
   *
   * We used to convert a MaPS object with all its aliases only once, using:
   *
   * @code
   * if (in_array($id, $entityIds)) {
   *   continue;
   * }
   * @endcode
   *
   * But it appeared that this caused inconsistencies with the parent relationships.
   * Neither linear nor hierarchical structure for the exported data from MaPS System®
   * can fix this, because the objects aliases may be present at any level.
   *
   * So we have to convert the original object AND each of its aliases in the order
   * they appear. The parent relationships will then only be the expected ones at the
   * last alias conversion, since all parents should exist at this time.
   *
   * We still use the related entities mechanism in order to avoid the deletion then
   * creation of already existing Drupal entities. This ensure the final status is
   * calcultaed using all aliases. Otherwise, if the first object is not published, the
   * existing Drupal entity is removed then created again through the conversion of the
   * first alias. This make the Drupal entity IDs increase when it can be avoided.
   */
  protected function processRun($index = 1, $range = 0) {
    $range = $this->getProfile()->getMaxObjectsPerOp();
    $total_entities = $this->getTotal();
    $current_context = & $this->context['results'][$this->getType()];

    if (!isset($current_context['total_op'])) {
      $current_context['total_entities'] = $total_entities;
      $current_context['processed_entities'] = 0;
      $current_context['total_op'] = ceil($total_entities / $range);
      $current_context['processed_op'] = 1;
    }

    // Make the log available from outside.
    self::$currentLog = $this->getlog()
      ->setCurrentOperation($index)
      ->setTotalOperations($current_context['total_op']);

    if ($entities = $this->getCurrentEntities($range)) {
      $current_context['processed_entities'] += count($entities);
      $entityIds = array();

    	foreach ($entities as $id => $data) {
    	  $this->getlog()->incrementCurrentOperation();

        $entityIds[] = $id;
        $entity = $this->createEntity($data);

        foreach ($this->getConverters() as $converter) {
          if (!$converter->isMappingAllowed() || $converter->getParentId() > 0) {
            continue;
          }

          $property = PropertyWrapper::load($converter, $converter->getUid());
          $entity->setRawValues($property, array(), $converter, FALSE);

          if (!$values = $property->getValues($entity)) {
            continue;
          }

          // @todo use the entity object in matchConditions() instead of the
          // array. So the $data variable will no more be necessary.
          if ($converter->getFilter()->checkConditions($data)) {
          	// @todo get entity UID value. If not possible, log an error and
            // continue with the next object.
            $uid = $values;
            foreach ($this->getEntitiesFromUid($uid, $property->getId()) as $entity_id => $related_entity) {
              // Caution: the strict operator === should not be used there,
              // because the variable type may differ (string or long). Duplicate
              // entities may cause the import process to fail on some entities,
              // when there are some related entities, which MaPS System® status
              // are not the same (e.g. published and unpublished).
            	if ($related_entity['id'] == $id) {
              	continue;
              }

              // Ensure the related entity matches the converter criteria.
              if ($converter->getFilter()->checkConditions($related_entity)) {
                $entity->addRelatedEntity($related_entity);
              }
            }

            $this->getlog()
              ->moveToContentRoot()
              ->addContext(new LogContext(strtolower($converter->getType()), array('id' => $data['id'] . ': ' . $converter->getBundle())), 'child');

            $converter->getMapping()->process($entity);
            break;
          }
        }

        // Flag the object as processed, even if no converter matched or if
        // an error occurred.
        db_update($this->getEntityTable())
          ->fields(array('inserted' => 0))
          ->condition('pid', $this->getProfile()->getPid())
          ->condition('id', $id)
          ->execute();

        // @todo log the no_match count using log.
        //$this->getlog()->addMessage();
        //$this->context['results']['no_match']++;
      }

      if ($this->isBatch()) {
        $finished = $current_context['processed_entities'] / $current_context['total_entities'];
        $this->context['finished'] = $finished <= 1 ? $finished : 0.9;
      }

      module_invoke_all('maps_import_mapping_finished', $this->profile, $entityIds);
    }
    // Nothing to do, so we terminate the batch if applicable.
    elseif ($this->isBatch()) {
      $this->context['finished'] = 1;
    }

    // The log should no more be available.
    self::$currentLog = NULL;

    return TRUE;
  }

  /**
   * Get the current list of entities to process.
   *
   * @param int $maxEntities
   *   The number of entity to retreive. If not set, we will retreive all
   * entities to process.
   */
  protected function getCurrentEntities($maxEntities) {
    $query = $this->selectQuery()
      ->condition('inserted', 0, '>');

    if ($maxEntities > 0) {
    	$query->range(0, $maxEntities);
    }

    return $query->execute()
      ->fetchAllAssoc('id');
  }

  /**
   * Get all entities that share the same UID.
   *
   * @param string $uid
   *   The uid value.
   * @param string $uid_field
   *   The field used as uid.
   *
   * @return array
   *   The entity array.
   */
  protected function getEntitiesFromUid($uid, $uid_field) {
    return $this->selectQuery()
      ->condition($uid_field, $uid)
      ->execute()
      ->fetchAllAssoc('id');
  }

  /**
   * Prepare a select query with common configuration.
   *
   * The returned query use the table related to current entity.
   *
   * @return SelectQuery
   *   A new SelectQuery instance.
   */
  protected function selectQuery() {
    return db_select($this->getEntityTable(), 'maps_entity', array('fetch' => \PDO::FETCH_ASSOC))
      ->fields('maps_entity')
      ->condition('pid', $this->getProfile()->getPid());
  }

  /**
   * Create a MaPS Entity from the retrieved data.
   *
   * @param $data
   *   The array containing the raw data.
   *
   * @return Drupal\maps_import\Mapping\Source\MapsSystem\EntityInterface
   *   The newly created entity.
   */
  abstract protected function createEntity(array $entity);

  /**
   * Return the LogInterface object used for the current operation.
   *
   * @return LogInterface
   *   The current log object.
   */
  public static function log() {
    if (!isset(self::$currentLog)) {
      self::$currentLog = new LogBroken();
    }

    return self::$currentLog;
  }

}
