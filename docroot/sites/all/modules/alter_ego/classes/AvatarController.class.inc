<?php


/**
 * The Controller for Avatar entities
 */
class AvatarController extends EntityAPIController {
  public function __construct($entityType) {
    parent::__construct($entityType);
  }


  /**
   * Create a avatar - we first set up the values that are specific
   * to our avatar schema but then also go through the EntityAPIController
   * function.
   *
   * @param $type
   *   The machine-readable type of the avatar.
   *
   * @return
   *   A avatar object with all default fields initialized.
   */
  public function create(array $values = array()) {
    // Add values that are specific to our Avatar
    $values += array(
        'aid' => '',
        'is_new' => TRUE,
        'title' => '',
        'created' => '',
        'changed' => '',
        'data' => '',
    );

    //$avatar = parent::create($values);
    // Add is_new property if it is not set.
    $values += array('is_new' => TRUE);

    // See if we've defined a custom class for this entity type.
    if (array_key_exists('custom entity class', $this->entityInfo) && is_array($this->entityInfo['custom entity class'])) {
      if (array_key_exists($values['type'], $this->entityInfo['custom entity class'])) {
        if ($class_name = $this->entityInfo['custom entity class'][$values['type']]) {
          return new $class_name($values, $this->entityType);
        }
      }
    }
    // See if we are using entity_class
    if (isset($this->entityInfo['entity class'])) {
      if ($class_name = $this->entityInfo['entity class']) {
        return new $class_name($values, $this->entityType);
      }
    }
    return (object) $values;
  }

  /**
   * Builds and executes the query for loading.
   *
   * @return The results in a Traversable object.
   */
  public function query($ids, $conditions, $revision_id = FALSE) {

    // Only cast into custom classes if base class is defined.
    if (!empty($this->entityInfo['entity class'])) {
      $results = array();

      // Load created Avatar Types
      $avatar_types = avatar_get_types();
      $load_into = array();
      // If we have defined custom entity classes
      if (array_key_exists('custom entity class', $this->entityInfo) && is_array($this->entityInfo['custom entity class'])) {
        foreach ($avatar_types as $avatar_type => $value) {
          // If this type has a custom class, place it into the load_into variable.
          if (array_key_exists($avatar_type, $this->entityInfo['custom entity class'])) {
            $load_into[$avatar_type] = $this->entityInfo['custom entity class'][$avatar_type];
          }
        }
      }

      // If we have any custom classes, load them first.
      foreach ($load_into as $avatar_type => $class_name) {
        $query = $this->buildQuery($ids, $conditions, $revision_id);
        $query->condition('type', $avatar_type);
        $result = $query->execute();
        $result->setFetchMode(PDO::FETCH_CLASS, $class_name, array(array(), $this->entityType));
        $results += $result->fetchAllAssoc($this->idKey);
      }

      // Load other types.
      $query = $this->buildQuery($ids, $conditions, $revision_id);
      if (!empty($load_into)) {
        $query->condition('type', array_keys($load_into), 'NOT IN');
      }
      $result = $query->execute();
      $result->setFetchMode(PDO::FETCH_CLASS, $this->entityInfo['entity class'], array(array(), $this->entityType));
      $results += $result->fetchAllAssoc($this->idKey);

      return $results;

    }
    else {
      // Build the query.
      $query = $this->buildQuery($ids, $conditions, $revision_id);
      $result = $query->execute();
    }
    return $result;
  }
}
