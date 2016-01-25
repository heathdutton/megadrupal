<?php

/**
 * @file
 * Interfaces of synonyms behaviors that are shipped with Synonyms module.
 */

/**
 * General interface of a synonyms behavior.
 *
 * All synonyms behaviors must extend this interface.
 */
interface SynonymsBehavior {
}

/**
 * Interface of "synonyms" behavior.
 *
 * The most basic synonyms behavior.
 */
interface SynonymsSynonymsBehavior extends SynonymsBehavior {

  /**
   * Extract synonyms from a field attached to an entity.
   *
   * We try to pass as many info about context as possible, however, normally
   * you will only need $items to extract the synonyms.
   *
   * @param array $items
   *   Array of items
   * @param array $field
   *   Array of field definition according to Field API
   * @param array $instance
   *   Array of instance definition according to Field API
   * @param object $entity
   *   Fully loaded entity object to which the $field and $instance with $item
   *   values is attached to
   * @param string $entity_type
   *   Type of the entity $entity according to Field API definition of entity
   *   types
   *
   * @return array
   *   Array of synonyms extracted from $items
   */
  public function extractSynonyms($items, $field, $instance, $entity, $entity_type);

  /**
   * Add an entity as a synonym into a field of another entity.
   *
   * Basically this method should be called when you want to add some entity
   * as a synonym to another entity (for example when you merge one entity
   * into another and besides merging want to add synonym of the merged entity
   * into the trunk entity). You should extract synonym value (according to what
   * value is expected in this field) and return it. We try to provide you with
   * as much of context as possible, but normally you would only need
   * $synonym_entity and $synonym_entity_type parameters. Return an empty array
   * if entity of type $synonym_entity_type cannot be converted into a format
   * expected by $field.
   *
   * @param array $items
   *   Array items that already exist in the field into which new synonyms is to
   *   be added
   * @param array $field
   *   Field array definition according to Field API of the field into which new
   *   synonym is to be added
   * @param array $instance
   *   Instance array definition according to Field API of the instance into
   *   which new synonym is to be added
   * @param object $synonym_entity
   *   Fully loaded entity object which has to be added as synonym
   * @param string $synonym_entity_type
   *   Entity type of $synonym_entity
   *
   * @return array
   *   Array of extra items to be merged into the items that already exist in
   *   field values
   */
  public function mergeEntityAsSynonym($items, $field, $instance, $synonym_entity, $synonym_entity_type);

  /**
   * Hash a field item that is enabled as synonym.
   *
   * Your hash function must return such hash that for 2 items that yield the
   * same synonyms their hash must be the same. There is no limit on minimal or
   * maximum hash length, but keep it reasonable, something below 512 symbols.
   * Also, your hash function should strive to minimize hash collisions, i.e.
   * when 2 different items yield the same hash.
   *
   * @param array $item
   *   Field item whose hash is requested
   * @param array $field
   *   Field from which the $item comes from
   * @param array $instance
   *   Instance from which the $item comes from
   *
   * @return string
   *   Hash of the provided $item
   */
  public function synonymItemHash($item, $field, $instance);

  /**
   * Look up entities by their synonyms within a provided field.
   *
   * You are provided with a SQL condition that you should apply to the storage
   * of synonyms within the provided field. And then return result: what
   * entities match by the provided condition through what synonyms.
   *
   * @param QueryConditionInterface $condition
   *   Condition that defines what to search for. It may contain a placeholder
   *   of AbstractSynonymsSynonymsBehavior::COLUMN_PLACEHOLDER which you should
   *   replace by the column name where the synonyms data for your field is
   *   stored in plain text. For it to do, you may extend the
   *   AbstractSynonymsSynonymsBehavior class and then just invoke the
   *   AbstractSynonymsSynonymsBehavior->synonymsFindProcessCondition() method,
   *   so you won't have to worry much about it
   * @param array $field
   *   Field API field definition array of the field within which the search
   *   for synonyms should be performed
   * @param array $instance
   *   Field API instance definition array of the instance within which the
   *   search for synonyms should be performed
   *
   * @return Traversable
   *   Traversable result set of found synonyms and entity IDs to which those
   *   belong. Each element in the result set should be an object and will have
   *   the following structure:
   *   - synonym: (string) Synonym that was found and which satisfies the
   *     provided condition
   *   - entity_id: (int) ID of the entity to which the found synonym belongs
   */
  public function synonymsFind(QueryConditionInterface $condition, $field, $instance);
}

/**
 * Exception thrown by implementations of SynonymsSynonymsBehavior interface.
 */
class SynonymsSynonymsBehaviorException extends Exception {}

/**
 * Starting point for implementing SynonymsSynonymsBehavior interface.
 */
abstract class AbstractSynonymsSynonymsBehavior implements SynonymsSynonymsBehavior {

  /**
   * Constant which denotes placeholder of a synonym column.
   *
   * @var string
   */
  const COLUMN_PLACEHOLDER = '***COLUMN***';

  /**
   * Process condition in 'synonymsFind' method.
   *
   * Process condition in 'synonymsFind' method replacing all references of
   * synonym column with the real name of that column.
   *
   * @param QueryConditionInterface $condition
   *   Condition that should be processed
   * @param string $column
   *   Real name of the synonym column
   */
  protected function synonymsFindProcessCondition(QueryConditionInterface $condition, $column) {
    $condition_array = &$condition->conditions();
    foreach ($condition_array as &$v) {
      if (is_array($v) && isset($v['field'])) {
        if ($v['field'] instanceof QueryConditionInterface) {
          // Recursively process this condition too.
          $this->synonymsFindProcessCondition($v['field'], $column);
        }
        else {
          $v['field'] = str_replace(self::COLUMN_PLACEHOLDER, $column, $v['field']);
        }
      }
    }
  }
}

/**
 * Interface of the autocomplete synonyms behavior.
 */
interface AutocompleteSynonymsBehavior extends SynonymsSynonymsBehavior {
}

/**
 * Interface of the synonyms friendly select behavior.
 */
interface SelectSynonymsBehavior extends SynonymsSynonymsBehavior {
}
