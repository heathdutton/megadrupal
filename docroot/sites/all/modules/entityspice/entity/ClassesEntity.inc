<?php

/**
 * The controller class for entities, contains methods for the entity CRUD
 * operations. The load method is inherited from the default controller.
 */
abstract class EntitySpiceEntityController extends EntityAPIController {
  /**
   * Saves an entity.
   *
   * @param $entity
   *   The full entity object to save.
   *
   * @return entity
   *   The saved entity object.
   */
  public function save($entity, DatabaseTransaction $transaction = NULL) {
    $transaction = isset($transaction) ? $transaction : db_transaction();
    try {
      $entity->updated = REQUEST_TIME;

      // New entity, set created property.
      if (isset($entity->is_new) && $entity->is_new) {
        $entity->created = REQUEST_TIME;
      }
      // Set revision flags. By default, save a new revision and set it as
      // default.
      else {
        if (!isset($entity->is_new_revision)) {
          $entity->is_new_revision = TRUE;
        }
        if (!isset($entity->default_revision)) {
          $entity->default_revision = TRUE;
        }
      }

      parent::save($entity, $transaction);

      module_invoke_all($this::_entity_type . '_save', $entity);
      return $entity;
    }
    catch (Exception $e) {
      $transaction->rollback();
      watchdog_exception($this->entityType, $e);
      throw $e;
    }
  }

  /**
   * Deletes multiple entities by ID.
   *
   * Checks to see if each entity can be removed or not and if not, ignore it.
   * In case none of them are allowed to be deleted returns FALSE.
   *
   * @param array $entity_ids
   *   An array of entity IDs to delete.
   *
   * @return bool
   *   TRUE on success, FALSE otherwise.
   */
  public function delete($entity_ids, DatabaseTransaction $transaction = NULL) {
    if (!empty($entity_ids)) {
      $entities = $this->load($entity_ids, array());

      // Ensure the entities can actually be deleted.
      $hook0 = $this::_entity_type . '_can_delete';
      $hook1 = $this::_entity_type . '_predelete';
      foreach ((array) $entities as $entity_id => $entity) {
        if (in_array(FALSE, module_invoke_all($hook0, $entity, $this::_entity_type))) {
          unset($entities[$entity_id]);
        }
        else {
          module_invoke_all($hook1, $entity, $this::_entity_type);
        }
      }

      // If none of the specified entities can be deleted, return FALSE.
      if (empty($entities)) {
        return FALSE;
      }
      // Otherwise, update the list of entity IDs to those which are deletable.
      else {
        $entity_ids = array_keys($entities);
      }

      $transaction = db_transaction();
      try {
        parent::delete($entity_ids, $transaction);
      }
      catch (Exception $e) {
        if (isset($transaction)) {
          $transaction->rollback();
        }
        watchdog_exception($this->entityType, $e);
        throw $e;
      }
    }

    return TRUE;
  }

  /**
   * Returns ID of entity.
   */
  public function getID() {
    return $this->{_entityspice_getinfo($this::_entity_type, 'idkey')}->value();
  }

  /**
   * Sets ID of entity.
   */
  public function setID($id) {
    $this->{_entityspice_getinfo($this::_entity_type, 'idkey')}->set($id);
    return $this;
  }
}

/**
 * The class used for entities.
 */
abstract class EntitySpiceEntity extends Entity {
  // Holds a singleton reference to a EntityAPIController.
  protected $wrapper = NULL;

  protected function defaultUri() {
    $uri = _entityspice_getinfo($this::_entity_type, 'default uri base');
    return array('path' => $uri . '/' . $this->identifier());
  }

  /**
   * Override parent constructor with entity type.
   */
  public function __construct(array $values = []) {
    parent::__construct($values, $this::_entity_type);
  }

  /**
   * Used by $this->buildContent to add title field. Should not be called if
   * entity does not have a title field.
   */
  protected function buildContentTitleField(array $content = array()) {
    $wrapper = $this->wrapped();

    $title = $wrapper->title->value();
    $content['title'] = array(
      '#theme' => 'entityspice_entity_property_field',
      '#label' => t('Title'),
      '#items' => array('#markup' =>  check_plain($title)),
      '#classes' => 'field field-label-inline clearfix',
    );

    return $content;
  }

  /**
   * Override buildContent() to add entity properties.
   */
  public function buildContent($view_mode = 'full', $langcode = NULL, array $content = array()) {
    if(_entityspice_getinfo($this::_entity_type, 'has title')) {
      $content += $this->buildContentTitleField($content);
    }

    return entity_get_controller($this->entityType)
      ->buildContent($this, $view_mode, $langcode, $content);
  }

  protected function defaultLabel() {
    return $this->wrapped()->title->value();
  }

  /**
   * Returns EntityMetadataWrapper object for this entity
   */
  public function wrapped() {
    if (!$this->wrapper) {
      $this->wrapper = entity_metadata_wrapper($this::_entity_type, $this);
    }

    return $this->wrapper;
  }

  /**
   * Returns ID of entity.
   */
  public function getID() {
    return $this->{$this->IDKey()};
  }

  /**
   * Sets ID of entity.
   */
  public function setID($id) {
    $this->{$this->IDKey()} = $id;
    return $this;
  }

  protected function IDKey() {
    return _entityspice_getinfo($this::_entity_type, 'idkey');
  }
}

/**
 *  Controls metadata for entity.
 *
 *  entityspice utilizes custom property attributes to determine if a property
 *  should be available as a filter on the listing page. These attributes are
 *  filter: TRUE to add as a filter.
 *  filter_operator: EFQ supported operators. Defaults to = or IN depending on
 *  value submitted
 *  field_type: textfield, select, etc.
 *  options list: callback that returns the options for this field.
 */
abstract class EntitySpiceMetadataController extends EntityDefaultMetadataController {
  /*
   * Overrides entityPropertyInfo().
   */
  public function entityPropertyInfo() {
    $info = parent::entityPropertyInfo();
    $properties = &$info[$this->type]['properties'];

    $options_list = $this::_entity_type . '_bundle_options_list';
    $admin_permission = 'administer ' . $this::_entity_type;

    $spicy_info = _entityspice_getinfo($this::_entity_type);

    if($spicy_info['has bundle']) {
      $key = $spicy_info['bundle key name'];
      $properties[$key] = array(
        'label' => t('Bundle'),
        'description' => t('The bundle of the entity.'),
        'type' => 'token',
        'setter callback' => 'entity_property_verbatim_set',
        'setter permission' => $admin_permission,
        'options list' => $options_list,
        'required' => TRUE,
        'schema field' => $spicy_info['bundle key name'],
      );
    }

    // @FIXME make title field name customizable.
    if($spicy_info['has title']) {
      $properties['title'] = array(
        'label' => t('Title'),
        'type' => 'text',
        'filter' => TRUE,
        'description' => t('Title of the entity.'),
        'setter callback' => 'entity_property_verbatim_set',
        'entity views field' => TRUE,
      );
    }

    if($spicy_info['has created']) {
      $properties['created'] = array(
        'entity views field' => TRUE,
        'label' => t('Created'),
        'description' => t('The date the entity was created.'),
        'type' => 'date',
        'schema field' => 'created',
        'setter callback' => 'entity_property_verbatim_set',
        'setter permission' => $admin_permission,
      );
    }

    if($spicy_info['has updated']) {
      $properties['updated'] = array(
        'entity views field' => TRUE,
        'label' => t('Updated'),
        'description' => t('The date the entity was updated.'),
        'type' => 'date',
        'schema field' => 'updated',
        'setter callback' => 'entity_property_verbatim_set',
        'setter permission' => $admin_permission,
      );
    }

    if($spicy_info['has uid']) {
      // @FIXME make having uid customizable.
      $properties['uid'] = array(
        'label' => t('Owner'),
        'type' => 'user',
        'description' => t('The owner of the entity.'),
        'schema field' => 'uid',
        'setter permission' => $admin_permission,
        'entity views field' => TRUE,
      );
    }

    return $info;
  }
}
