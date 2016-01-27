<?php
/**
 * @file
 * Provides the default class for managing party - Attached entity
 * relationships.
 */

/**
 * Class PartyDefaultDataSet
 *
 * This class manages the attachement of entities to a party, including
 * loading, ordering, attaching and detaching entities. It also provides
 * a helper for creating new attached entities.
 */
class PartyDefaultDataSet {

  /**
   * var string $data_set
   *   The data set name (array key in hook_data_set_info)
   */
  protected $data_set;

  /**
   * var array $entities
   *   An array of attatched entities, keyed by delta. This can contain a mix
   *   of stub entities (with the is_stub property set to TRUE), fully loaded
   *   entities and unsaved entities (with the is_new property set to TRUE).
   */
  protected $entities = array();

  /**
   * var party $party
   *   The party object we're connected to.
   */
  protected $party;

  /**
   * var $saving
   *   A flag to indicate whether we're currently doing a controller save to
   *   prevent concurrency issues.
   *
   * @see PartyDefaultDataSet::save()
   */
  private $saving = FALSE;

  /**
   * Alters entity info for entities requesting CRM integration.
   *
   * @param array $entity_info
   *  The info array for a single entity.
   *  Apparently, you can pass this in from the original array in an
   *  implementation of hook_entity_info_alter, thus:
   *    PartyDefaultDataSet::hook_entity_info_alter($entity_info['profile2']);
   */
  static function hook_entity_info_alter(&$entity_info) {
    $entity_info['view modes']['party'] = array(
      'label' => t('Party Attached Entity'),
      'custom settings' => FALSE,
    );
  }

  /**
   * Acts when a party is deleted.
   */
  public function hook_party_delete() {
    foreach ($this->getEntities() as $entity) {
      $this->detachEntity($entity);
      $entity_type = $this->getDataInfo('entity type');
      entity_delete($entity_type, entity_id($entity_type, $entity));
    }

    $this->clear();
  }

  /**
   * Register subProcesses for a data set merge.
   */
  static function registerMergeProcesses($master_controller, $merger_controller) {
    if (!$master_controller->getEntity() || !$merger_controller->getEntity()) {
      return array();
    }

    $entity_type = $master_controller->getDataInfo('entity type');
    $entity_info = entity_get_info($entity_type);
    $mergeClass = $entity_info['merge handler class'];

    $processes = array();
    $processes['merge_entities'] = array(
      'class' => $mergeClass,
      'entityType' => $entity_type,
      'entity1' => $master_controller->getEntity(),
      'entity2' => $merger_controller->getEntity(),
      'targetEntity' => FALSE,
    );

    return $processes;
  }

  /**
   * Merge two data set controllers together.
   *
   * At the moment this function only works for single cardinality data sets.
   * It merges the first entity on $merger_controller into the first entity on
   * $master_controller.
   *
   * @todo Review this and come up with a better methodology.
   */
  static function runMerge($master_controller, $merger_controller) {
    if (!$master_controller->getEntity() && $merger_controller->getEntity()) {
      $master_controller->attachEntity($merger_controller->getEntity());
      $merger_controller->detachEntityByDelta(0);
      $merger_controller->save();
      $master_controller->save(TRUE);
    }
  }

  /**
   * Constructor
   *
   * @param string $data_set
   *   The data set name.
   * @param party $party
   *   The party object.
   */
  public function __construct($data_set, Party $party) {
    // Check that this is a valid data set
    if (party_get_data_set_info($data_set)) {
      $this->data_set = $data_set;
    }
    else {
      // Throw an exception.
      throw new Exception(t('Failed to construct data set controller for invalid dataset %data_set.', array('%data_set' => $data_set)));
    }

    $this->party = $party;

    if (!empty($this->party->pid)) {
      // Without attached entities, this controller is pretty useless, so let's
      // assume anyone wants them loaded, but we'll do so using stubs to save a
      // full entity load.
      $query = db_select('party_attached_entity', 'ae')
        ->fields('ae', array('delta', 'eid'))
        ->condition('pid', $this->party->pid, '=')
        ->condition('data_set', $this->data_set);
      $result = $query->execute()
        ->fetchAllAssoc('delta');

      // Iterate over our results creating stub entities, setting a flag for us
      // to detect that it is a stub.
      foreach ($result as $delta => $entity) {
        $this->entities[$delta] = entity_create_stub_entity($this->getDataInfo('entity type'), array(
          0 => $entity->eid,
          2 => $this->getDataInfo('entity bundle'),
        ));
        $this->entities[$delta]->is_stub = TRUE;
      }
    }
  }

  /**
   * Method to return the party object
   */
  public function getParty() {
    return $this->party;
  }

  /**
   * Get information from the data set or entity definition.
   *
   * @param string $key
   *   This is the information you want:
   *   - 'type': the data set entity type.
   *   - 'bundle': the data set entity bundle.
   *   - 'id key': the entity id key.
   *   - 'bundle key': the entity bundle key.
   */
  final public function getDataInfo($key) {
    // Get our info
    $party_data_info = party_get_data_set_info($this->data_set);
    $entity_info = entity_get_info($party_data_info['entity type']);

    // Return the relevant information
    switch ($key) {
      // Data Set Info
      case 'name':
        return $this->data_set;
      case 'path element':
        return $party_data_info['path element'];
      case 'label':
        return $party_data_info['label'];
      case 'entity type':
        return $party_data_info['entity type'];
      case 'entity bundle':
        return $party_data_info['entity bundle'];

      // entity Info
      case 'id key':
        return $entity_info['entity keys']['id'];
      case 'bundle key':
        return $entity_info['entity keys']['bundle'];
    }
  }

  /**
   * Get the delta from an entity object.
   *
   * @param $entity
   *   The entity object we want to find the delta for.
   *
   * @return int|FALSE
   *   The delta of $entity, or FALSE if it isn't attached.
   */
  public function getEntityDelta($entity) {
    // If $entity is saved, we can just compare entity ids.
    if (!isset($entity->is_new)) {
      // Flip our entity ids so it's delta keyed by id.
      $deltas = array_flip($this->getEntityIds());
      $entity_id = $entity->{$this->getDataInfo('id key')};
      if (isset($deltas[$entity_id])) {
        return $deltas[$entity_id];
      }
    }
    // Otherwise, we need to actually compare objects.
    else {
      foreach ($this->entities as $delta => $row) {
        if ($entity === $row) {
          return $delta;
        }
      }
    }

    // If we've got here, we can't find it.
    return FALSE;
  }

  /**
   * Load the full entities.
   *
   * @param array|null $deltas
   *   (optional) An array of delta(s) to load or NULL to load all entities.
   *
   * @return $this
   */
  public function loadEntities($deltas = NULL) {
    // Get our array of deltas
    if ($deltas === NULL) {
      // Load all entities
      $deltas = array_keys($this->entities);
    }

    // Iterate over our deltas loading our entities if required.
    foreach ($deltas as $delta) {
      if (isset($this->entities[$delta]) && isset($this->entities[$delta]->is_stub)) {
        // This entity is a stub so needs loading. First get the entity id from
        // stub entity.
        $entity_id = $this->entities[$delta]->{$this->getDataInfo('id key')};
        $entities_loaded = entity_load($this->getDataInfo('entity type'), array($entity_id));
        $this->entities[$delta] = reset($entities_loaded);

        // If we couldn't load the entity, detach it.
        if (!$this->entities[$delta]) {
          $this->detachEntityByDelta($delta);
        }
        // Otherwise add the other properties party needs.
        else {
          $this->entities[$delta]->data_set_name = $this->data_set;
          $this->entities[$delta]->party_attaching_party = $this->party->pid;
        }
      }
    }

    return $this;
  }

  /**
   * Get a particular attached entity
   *
   * @param int $delta
   *   (optional) A delta to get. Defaults to 0.
   * @param bool $create
   *   (optional) Create an entity if it doesn't exist. Defaults to FALSE.
   *
   * @return mixed
   *   An entity object or one doesn't exist and we're not creating, FALSE.
   */
  public function getEntity($delta = 0, $create = FALSE) {
    // If this delta exists, check it's loaded and return it.
    if (isset($this->entities[$delta])) {
      if (isset($this->entities[$delta]->is_stub)) {
        $this->loadEntities(array($delta));
      }
    }

    // If an entity wasn't accessible, it may have been cleared, so check.
    if (!isset($this->entities[$delta])) {
      // Let's create an entity and attach it, but no saving happens unless
      // explicitly called. To maintain unsaved entities, this data controller
      // can be stored against forms etc.
      if ($create) {
        $entity = $this->createEntity();
        $this->attachEntity($entity);
      }
      else {
        // Nothing found and not creating; return false.
        return FALSE;
      }
    }

    return $this->entities[$delta];
  }

  /**
   * Get all attached entities
   *
   * @return array
   *   An array of all this data set's entities keyed by their delta.
   */
  public function getEntities() {
    $this->loadEntities();

    return $this->entities;
  }

  /**
   * Get entity ids/deltas
   *
   * This allows you to get the entity ids and deltas of all the entities
   * without having to load them. There may also be sub entities, in which
   * case the value for that delta will be an empty string, as to not throw
   * a warning if the array is flipped.
   *
   * @return array
   *   Array of entity ids keyed by delta. Unsaved entities return an empty
   *   string.
   */
  public function getEntityIds() {
    $ids = array();

    foreach ($this->entities as $delta => $entity) {
      if (!isset($entity->is_new)) {
        $ids[$delta] = $entity->{$this->getDataInfo('id key')};
      }
      else {
        // This is unsaved, so we use an empty string so warnings don't get
        // emitted on an array_flip().
        $ids[$delta] = '';
      }
    }

    return $ids;
  }

  /**
   * Get actions for the attached entity. Check party access in each case.
   *
   * @return
   *  An array of links suitable for theme_links().
   *
   * @todo: Build these off of delta rather than $party_id, $entity_id. It's
   *  unnecessary to pass the party id here.
   */
  public function getActions($party_id, $entity_id) {
    $party_data_info = party_get_data_set_info($this->data_set);

    // @todo: Rewrite whole function. for now, hack.
    $map = array_flip($this->getEntityIds());
    $delta = $map[$entity_id];

    $actions = array();

    if (party_access('edit', $this->party, $this->data_set)) {
      $actions['edit'] = array(
        'title' => 'Edit',
        'href' => 'party/' . $party_id . '/' . $party_data_info['path element'] . '/edit/' . $delta,
      );
    }
    if (party_access('detach', $this->party, $this->data_set)) {
      $actions['remove'] = array(
        'title' => 'Remove',
        'href' => 'party/' . $party_id . '/' . $party_data_info['path element'] . '/remove/' . $delta,
      );
    }
    return $actions;
  }

  /**
   *  Return the renderable array for one of our attached entities.
   *
   * @param int $delta
   *  The delta of the entity to render.
   * @param $view_mode
   *  (optional) The view mode to use. Defaults to that set in the data set
   *  definition.
   *
   * @return
   *  A renderable array.
   */
  public function display($delta, $view_mode = NULL) {
    $entity = $this->getEntity($delta);

    if (!isset($view_mode)) {
      $party_data_info = party_get_data_set_info($this->data_set);
      $view_mode = $party_data_info['view mode'];
    }

    return entity_view($this->getDataInfo('entity type'), array($entity), $view_mode);
  }

  /**
   * Get the label of one of our attached entities.
   *
   * @param int $delta
   *  The delta of the entity to render.
   *
   * @return
   *  The text of the label.
   */
  public function getLabel($delta) {
    $entity = $this->getEntity($delta);
    $label = entity_label($this->getDataInfo('entity type'), $entity);
    return $label;
  }

  /**
   * Create a new entity
   *
   * This method provides a helper to create a new attached entity for the data
   * set without having to figure out the entity type and bundle.
   *
   * @return object
   *   A newly created unsaved entity of the correct type and bundle.
   */
  public function createEntity() {
   // Create a placeholder entity
    $values = array();
    if ($this->getDataInfo('bundle key')) {
      $values[$this->getDataInfo('bundle key')] = $this->getDataInfo('entity bundle');
    }

    // Create the entity, set our data set and return
    $entity = entity_create($this->getDataInfo('entity type'), $values);

    // Add the other properties party needs.
    $entity->data_set_name = $this->data_set;
    if (isset($this->party->pid)) {
      $entity->party_attaching_party = $this->party->pid;
    }

    return $entity;
  }

  /**
   * Save an entity.
   *
   * This method saves the entity at the position specified by delta. This
   * allows for other modules to use different logic for saving their entities.
   *
   * @param int $delta
   *   (optional) The delta of the entity to save. Defaults to 0.
   *
   * @return object
   *   The entity that has been saved.
   */
  public function saveEntity($delta = 0) {
    $entity = $this->getEntity($delta);

    // If getting the entity failed, return false.
    if (!$entity) {
      return $entity;
    }

    entity_save($this->getDataInfo('entity type'), $entity);
    return $entity;
  }

  /**
   * Attach an entity to the party
   *
   * This method puts the entity in the right place in the $entities array.
   * NB: This does not save the new order, you must call
   * PartyDefaultData::save() to do that. This method cannot be overloaded
   * and any extensions of this class can make use of
   * PartyDefaultDataSet::preAttach() and PartyDefaultDataSet::postAttach()
   * to perform any additional logic required.
   *
   * @param object $entity
   *   The entity we're attaching.
   * @param string $method
   *   The $method we're using can be one of:
   *   - 'append' (default): the entity will be added to the end of the list.
   *   - 'prepend': the entity will be added at the front of the list.
   *   - 'insert': the entity will be inserted at $delta.
   * @param bool $reattach
   *   If this entity is already attached, should we remove it and then
   *   reattach using the requested method. Defaults to FALSE.
   * @param int $delta
   *   If the $method is set to insert. This is the target delta of the
   *   attached entity.
   *
   * @return $this
   *
   * @see PartyDefaultDataSet::preAttach()
   * @see PartyDefaultDataSet::postAttach()
   */
  final public function attachEntity($entity, $method = 'append', $reattach = FALSE, $delta = 0) {
    // Fire any pre attach logic
    $this->preAttach($entity, $method, $delta);

    // Check if this entity is already attached
    $existing_delta = $this->getEntityDelta($entity);
    if ($existing_delta !== FALSE) {
      if ($reattach) {
        // Do this manually as we don't want to trigger the pre/post callbacks.
        unset($this->entities[$existing_delta]);
        $this->entities = array_values($this->entities);
        $existing_delta = FALSE;
      }
      else {
        // Let's make sure our attached entity if fully loaded and update it if necessary
        $this->entities[$existing_delta] = $entity;
      }
    }

    // Only attach if it wasn't already and hasn't been detached.
    if ($existing_delta === FALSE) {
      // Add important information to the entity.
      $entity->data_set_name = $this->data_set;
      $entity->party_attaching_party = $this->party->pid;

      switch ($method) {
        case 'append':
          $this->entities[] = $entity;
          break;
        case 'prepend':
          array_unshift($this->entities, $entity);
          break;
        case 'insert':
          // Put the entity and its availability in the right place
          array_splice($this->entities, $delta, 0, $entity);
          break;
      }

      $this->invoke('attach', $entity, $delta, $method, $reattach);
    }

    // Fire any post attach logic
    $this->postAttach($entity, $method, $delta);

    return $this;
  }

  /**
   * Overload any pre attach logic
   *
   * Both $method and $delta can be taken by reference, allowing you to make
   * alterations to the attach behaviour.
   *
   * @param object $entity
   * @param string $method
   * @param int $delta
   *
   * @see PartyData::attachEntity()
   */
  protected function preAttach($entity, &$method, &$delta) {}

  /**
   * Overload any post attach logic
   *
   * @param object $entity
   * @param string $method
   * @param int $delta
   *
   * @see PartyData::attachEntity()
   */
  protected function postAttach($entity, $method, $delta) {}

  /**
   * Detach an entity
   *
   * This is a helper method to detach an entity when you have it's object.
   * This method cannot be overloaded and any extensions of this class can
   * make use of PartyDefaultDataSet::preAttach() and
   * PartyDefaultDataSet::postAttach() to perform any additional logic
   * required.
   *
   * @param int $delta
   *   The delta of the entity to detach
   *
   * @return $this
   *
   * @see PartyDefaultDataSet::detchEntityByDelta()
   * @see PartyDefaultDataSet::preDetach()
   * @see PartyDefaultDataSet::postDetach()
   */
  final public function detachEntity($entity) {
    // Figure out our delta
    $delta = $this->getEntityDelta($entity);

    // Pass this onto the detachEntityByDelta method to actually detach
    $this->detachEntityByDelta($delta);

    return $this;
  }

  /**
   * Detatch an entity by delta
   *
   * This method cannot be overloaded and any extensions of this class can
   * make use of PartyDefaultDataSet::preAttach() and
   * PartyDefaultDataSet::postAttach() to perform any additional logic
   * required.
   *
   * @param int $delta
   *   The delta of the entity to detach
   * @param bool $return
   *   Whether you want to return the detached entity or $this for chaining.
   *   Defaults to FALSE.
   *
   * @return object|$this
   *   Depending on $return, either the detached entity or this for chaining.
   *
   * @see PartyDefaultDataSet::preDetach()
   * @see PartyDefaultDataSet::postDetach()
   */
  final public function detachEntityByDelta($delta, $return = FALSE) {
    // Fire any pre attach logic
    $this->preDetach($delta);

    if (isset($this->entities[$delta])) {
      // Get our entity for returning if requested
      $entity = $this->entities[$delta];

      // Detach our entity
      unset($this->entities[$delta]);

      // Reset our numeric indexes
      $this->entities = array_values($this->entities);
    }
    else {
      // Can't return the entity if it didn't exist
      $entity = FALSE;
    }

    // Fire any post attach logic
    $this->postDetach($delta);

    $this->invoke('detach', $entity, $delta);

    if ($return) {
      return $entity;
    }
    else {
      return $this;
    }
  }

  /**
   * Overload any pre detach logic
   *
   * $delta can be taken by reference, allowing you to make alterations to the
   * detach behaviour.
   *
   * @param int $delta
   *
   * @see PartyData::detachEntity()
   */
  protected function preDetach($delta) {}

  /**
   * Overload any post detach logic
   *
   * @param int $delta
   *
   * @see PartyData::detachEntity()
   */
  protected function postDetach($delta) {}


  /**
   * Save the attached entities information
   *
   * This method cannot be overloaded and any extensions of this class can
   * make use of PartyDefaultDataSet::preSave() and
   * PartyDefaultDataSet::postSave() to perform any additional logic required.
   *
   * As saving entities could cause further calls to
   * PartyDefaultDataSet::save(), the $has_lock variable stores whether this
   * call to PartyDefaultDataSet::save() is a top level save. Only the top
   * level save will update the party_attached_entity table and store changes
   * to $party->label to prevent PDO issues and conflicting data. Nested calls
   * shouldn't make any unnecessary database writes, both for performance and
   * to prevent data conflicts. This information is handed on to the
   * PartyDefaultDataSet::preSave() and PartyDefaultDataSet::postSave() for
   * overloading classes to continue this behavior. An example of why this is
   * necessary can be found in the issue at http://drupal.org/node/1907744.
   *
   * @param bool $save_entities
   *   Whether or not to save entities as we go through them. If an entity is
   *   unsaved, we will always save it, as otherwise we can't save the deltas.
   *
   * @return $this
   *
   * @todo Remove the need to save entity type and bundle, as it's easy to
   *   sniff our from the data set.
   *
   * @see PartyDefaultDataSet::preSave()
   * @see PartyDefaultDataSet::postSave()
   */
  final public function save($save_entities = FALSE) {
    $has_lock = FALSE;
    if (!$this->saving) {
      // Lock it down to prevent concurrency.
      $this->saving =
        $has_lock = TRUE;
    }

    // Fire any pre attach logic
    $this->preSave($save_entities, $has_lock);

    // Save any entities that need saving.
    foreach ($this->entities as $delta => &$entity) {
      if ($save_entities || isset($entity->is_new)) {
        // Save our entities as we go
        $this->saveEntity($delta);
      }
    }

    if (!empty($has_lock)) {
      $this->clear();

      // Insert our entities
      $query = db_insert('party_attached_entity');
      $query->fields(array('pid', 'eid', 'delta', 'data_set', 'entity_type', 'entity_bundle'));
      foreach ($this->entities as $delta => &$entity) {
        // Add our record
        $query->values(array(
          'pid' => $this->party->pid,
          'eid' => $entity->{$this->getDataInfo('id key')},
          'delta' => $delta,
          'data_set' => $this->data_set,
          'entity_type' => $this->getDataInfo('entity type'),
          'entity_bundle' => $this->getDataInfo('entity bundle'),
        ));
      }
      $query->execute();

      // Release our lock.
      $this->saving = FALSE;
    }

    // Fire any post attach logic
    $this->postSave($save_entities, $has_lock);

    if (!empty($has_lock)) {
      $this->invoke('save');
    }

    // Update the label and primary fields whenever attached entities are
    // changed. This should only trigger a store if we are a locking save.
    $controller = entity_get_controller('party');
    $controller->setPrimaryFields($this->party, $has_lock);

    return $this;
  }

  /**
   * Overload any pre save logic
   *
   * @param bool $save_entities
   *   Whether or not to save entities as we go through them.
   * @param bool $has_lock
   *   Whether this is a top level save call rather than a nested call. Nested
   *   calls shouldn't save anything unnecessary to prevent conflicts. See
   *   PartyDefaultDataSet::save() for more information.
   *
   * @see PartyData::save()
   */
  protected function preSave($save_entities, $has_lock) {}

  /**
   * Overload any post save logic
   *
   * @param bool $save_entities
   *   Whether or not to save entities as we go through them.
   * @param bool $has_lock
   *   Whether this is a top level save call rather than a nested call. Nested
   *   calls shouldn't save anything unnecessary to prevent conflicts. See
   *   PartyDefaultDataSet::save() for more information.
   *
   * @see PartyData::save()
   */
  protected function postSave($save_entities, $has_lock) {}

  /**
   * Clear the attached entities from the database.
   *
   * This will remove all the attachments from the database. This is usually
   * run at the start of the save() method however it is also called in the
   * hook_party_delete() so it gets its own method call here.
   */
  protected function clear() {
    // Clear out our old bits
    $query = db_delete('party_attached_entity');
    $query->condition('pid', $this->party->pid, '=');
    $query->condition('data_set', $this->data_set);
    $query->execute();
  }

  /**
   * Invoke hooks.
   *
   * @param string $hook
   *   The hook to invoke, e.g. 'save'
   */
  public function invoke($hook) {
    // Get the arguments to be passed to the hook.
    $args = func_get_args();
    array_shift($args);
    array_unshift($args, $this);

    // Generate hook names.
    $generic_hook = 'party_data_set_' . $hook;
    $specific_hook = 'party_data_set_' . $hook . '_' . $this->data_set;

    // Call generic hook.
    array_unshift($args, $generic_hook);
    call_user_func_array('module_invoke_all', $args);

    // Call specific hook.
    $args[0] = $specific_hook;
    call_user_func_array('module_invoke_all', $args);
  }

  /**
   * Re-order attached entities
   *
   * This will re-order any entities to match the order specified in $order.
   * Any entities that are left out of $order will be appended to the new
   * order. To remove an entity, use PartyData::detachEntity().
   *
   * @param array $order
   *   A numeric array of old deltas in the new order
   *
   * @return $this
   */
  final public function reorderEntities($order) {
    // First we need to collect missed entities
    $missed_entities = array_diff_key($this->entities, array_fill_keys($order, TRUE));
    $entities = array();

    // Iterate over re-ordering our entities
    foreach ($order as $delta) {
      if (isset($this->entities[$delta])) {
        $entities[] = $this->entities[$delta];
      }
    }

    // Append any missed entities
    foreach ($missed_entities as $entity) {
      $entities[] = $entity;
    }

    // Store array values so we don't have any missed
    $this->entities = $entities;

    return $this;
  }

  /**
   * Shift a particular entity in the order
   *
   * @param int $delta
   *   The delta of the item we're shifting
   * @param string $method
   *   The method of shift we want:
   *    - 'up' Shifts the entity up by $target
   *    - 'down' Shifts the entity down by $target
   *    - 'insert' Moves the entity to that position, shifting everything else
   *    - 'swap' Swaps the entity with the entity in $target
   * @param int $target
   *   If method is 'up' or 'down', shift the element by this amount.
   *   If method is 'swap', swap the entity with the entity at this delta.
   *
   * @return $this
   */
  final public function shiftEntity($delta, $method, $target = 1) {
    if (isset($this->entities[$delta])) {
      $entity = $this->entities[$delta];
      switch ($method) {
        case 'up':
          // Calculate our new position
          $newDelta = min(0, $delta - $target);
          unset($this->entities[$delta]);
          array_splice($this->entities, $newDelta, 0, array($entity));
        break;
        case 'down':
          // Calculate our new position, including removal of the moved entity
          $newDelta = $delta + $target - 1;
          unset($this->entities[$delta]);
          array_splice($this->entities, $newDelta, 0, array($entity));
        break;
        case 'insert':
          array_splice($this->entities, $target, 0, array($entity));
        break;
        case 'swap':
          // Make sure we have the other entity if it exists
          if (isset($this->entities[$target])) {
            $otherEntity = $this->entities[$target];
          }

          // Set our new position for this entity
          $this->entities[$target] = $entity;

          // If we had another entity, put it back in place
          if (isset($otherEntity)) {
            $this->entities[$delta] = $otherEntity;
          }

          // Make sure our deltas are numeric and sequencial
          $this->entities = array_values($this->entities);
        break;
      }
    }

    return $this;
  }

}
