<?php
/**
 * @file
 * Entity classes to manage Lyris objects.
 */
/**
 * Interface for abstract LyrisEntity objects.
 */
interface LyrisEntityInterface {
  /**
   * Constructor
   *
   * @abstract
   *
   * Needs to pass struct and object type data to the LyrisEntity parent.
   * Ex. parent::__construct('ListStruct', 'lyris_list', $values);
   */
  public function __construct();

  /**
   * Return a displayable title for the entity.
   *
   * @abstract
   */
  public function getDisplayName();
}

/**
 * Base class for Lyris objects.
 */
class LyrisEntity extends Entity {
  // The unique entity id per type.
  public $eid;
  public $entity_type;
  public $creator;
  public $created;
  public $changer;
  public $changed;
  public $synced;
  public $old_values;

  // The Lyris struct type.
  public $struct_type;

  // The struct array from Lyris.
  public $struct = array();

  // The API version loading this object.
  public $api_version;

  // The default server.
  public $server = LYRIS_MODE_DEFAULT;

  // An associative array to link a default struct field to its unique row in
  // the defaults database.  If a default value has not yet been set, this will
  // be empty.  This allows us to use drupal_write_record() without calling the
  // db before each insert or update to determine the appropriate action.
  public $field_defaults = array();

  // An associative array of fields to role IDs to permission status:
  // $fieldperm[FieldName][rid] = status;
  public $field_perms = array();

  // Key to indicate whether the perms have been loaded in case there are no
  // perm overrides set in the database.
  public $perms_loaded = array();

  /**
   * Constructor
   */
  public function __construct($struct_type, $entity_type, $values = array()) {
    $this->is_new = !(isset($this->eid));

    $this->struct_type = $struct_type;
    $this->entity_type = $entity_type;
    parent::__construct($values, $entity_type);
  }

  /**
   * Entity API.
   */
  /**
   * Save the standard entity data.
   */
  public function save() {
    global $user;

    $this->changer = $user->uid;
    $this->changed = REQUEST_TIME;

    if (!$this->eid) {
      $this->is_new;
      $this->creator = $user->uid;
      $this->created = REQUEST_TIME;
    }

    $status = parent::save();

    if ($status == SAVED_NEW) {
      drupal_write_record(LYRIS_ENTITY_TABLE, $this);
    }
    else {
      drupal_write_record(LYRIS_ENTITY_TABLE, $this, array('eid', 'entity_type'));
    }

    return $status;
  }

  /**
   * Set the Lyris synced value to the current time for this entity.
   */
  public function setSynced($op, $time = REQUEST_TIME) {
    // Associate the Lyris object to the Local object.
    if ($op == SAVED_NEW) {
      $info = entity_get_info($this->entity_type);
      $lyris_id_field = $info['entity keys']['lyris_id'];

      if ($this->localVal($lyris_id_field)) {
        db_update($info['base table'])->fields(array(llf($lyris_id_field) => $this->localVal($lyris_id_field)))->condition('eid', $this->eid)->execute();
      }
    }

    // Update the metadata.
    db_update(LYRIS_ENTITY_TABLE)->fields(array('synced' => $time))->condition('eid', $this->eid)->condition('entity_type', $this->entity_type)->execute();
  }

  /**
   * Determine whether the current entity has changes that need to be pushed to
   * Lyris.
   */
  public function hasChanges() {
    return (bool) ($this->changed >= $this->synced);
  }

  /**
   * Store a field value into the 'old_values' field so we can update only the
   * fields that need to be updated if we're not pushing entire entites, such
   * as with members.
   */
  protected function changingField($field) {
    $this->old_values = unserialize($this->old_values);

    // If the field is already set as changed, don't reset it.  We need to store
    // the value that Lyris currently has as the old_value.  If the user has
    // been changing their email numerous times before syncing to lyris, for
    // example, we need to know the email address that lyris is expecting.
    if (!isset($this->old_values[lrf($field)])) {
      $this->old_values[lrf($field)] = $this->localVal($field);
    }

    $this->old_values = serialize($this->old_values);
  }

  /**
   * Clear field names from the 'changes' field after they are successfully
   * pushed to Lyris.
   *
   * @param $clear
   *   If NULL, clears all field names.
   *   If STRING, removes the specified field name from the list.
   *   If ARRAY, removes all specified field names from the list.
   */
  public function changedFieldClear($clear = NULL) {
    $this->old_values = unserialize($this->old_values);

    if (!$clear) {
      $this->old_values = array();
    }
    else {
      foreach ((array) $clear as $field) {
        if (isset($this->old_values[lrf($field)])) {
          unset($this->old_values[lrf($field)]);
        }
      }
    }

    // Update the field in the DB.
    db_update(LYRIS_ENTITY_TABLE)
      ->fields(array('old_values' => serialize($this->old_values)))
      ->condition('eid', $this->eid)
      ->condition('entity_type', $this->entity_type)
      ->execute();

    // If there are no fields left to clear, set the record as synced.
    if (empty($this->old_values)) {
      $this->setSynced(SAVED_UPDATED);
    }

    $this->old_values = serialize($this->old_values);
  }

  /**
   * Get the value of a local Lyris property given a Lyris field name or a
   * local field name.
   */
  public function localVal($field) {
    if ($this->lyrisFieldIsLocal($field) && property_exists($this, $field)) {
      return $this->$field;
    }
    elseif (!$this->lyrisFieldIsLocal($field)) {
      return $this->localVal($this->fieldConvertLocal($field));
    }
  }

  /**
   * Get the value from a Lyris struct given a Lyris field name or a
   * local field name.
   */
  public function structVal($field) {
    if (!$this->lyrisFieldIsLocal($field) && array_key_exists($field, $this->struct)) {
      // Handle arrays
      if (stristr($field, '][')) {
        list($parent, $child) = explode('][', $field);
        if (isset($this->struct[$parent]) && is_array($this->struct[$parent]) && array_key_exists($child, $this->struct[$parent])) {
          return $this->struct[$parent][$child];
        }
      }
      return $this->struct[$field];
    }
    elseif ($this->lyrisFieldIsLocal($field)) {
      return $this->structVal($this->fieldConvertLocal($field));
    }
  }

  /**
   * Safely set a local value given a Lyris fieldname.
   */
  public function setLocalVal($field, $val) {
    $field = $this->fieldConvertLocal($field);
    if (property_exists($this, $field)) {
      // Run the data through a translator to ensure the proper value is set for
      // this particular system.
      if (array_key_exists(lrf($field), $this->struct)) {
        $this->translateField($field, $val, 'local');
      }

      // Log the change to the old_values field if this is an existing entity.
      if (!isset($this->is_new) || !$this->is_new) {
        $this->changingField($field);
      }

      // Set the new value.
      $this->$field = $val;
    }
  }

  /**
   * Safely set an allowed struct value, trimming off DB prefixes and creating
   * arrays if needed.
   */
  public function setStructVal($field, $val) {
    // Strip off field prefixes
    if (stripos($field, LYRIS_DB_FIELD_PREFIX) === 0) {
      $field = substr($field, strlen(LYRIS_DB_FIELD_PREFIX));
    }

    // Run the data through a translator to ensure the proper value is set for
    // this particular system.
    $this->translateField($field, $val, 'lyris');

    // Set array value.
    if (stristr($field, '__')) {
      list($parent, $child) = explode('__', $field);
      if (isset($this->struct[$parent]) && is_array($this->struct[$parent]) && array_key_exists($child, $this->struct[$parent])) {
        $this->struct[$parent][$child] = $val;
      }
    }
    // Set first-level value.
    elseif (array_key_exists($field, $this->struct)) {
      $this->struct[$field] = $val;
    }
  }

  /**
   * Set a struct value from a Drupal field.
   */
  public function fieldSetStruct($field_name, $struct_name, $concat = FALSE, $instance = 0) {
    $instances = field_get_items('lyris_content', $this, $field_name);

    // Concatenate all instances of this field if the field cardinality was set
    // higher than one.
    if ($concat) {
      $data = '';
      foreach ($instances as $i) {
        $data .= $i['value'];
      }
    }
    else {
      $data = (isset($instances[$instance]) ? $instances[$instance]['value'] : '');
    }

    // Set the struct value
    $this->setStructVal($field_name, $data);
  }

  /**
   * Attempt to determine the API version for this object.
   */
  protected function getApiVersion() {
    if (!empty($this->api_version)) {
      return;
    }

    // First, let's see if we have a server variable set.
    if (isset($this->server)) {
      $this->api_version = lyris_api_version($this->server);
    }
    // Go with the default
    else {
      $this->api_version = lyris_api_version();
    }
  }

  /**
   * Safely merge data into existing object fields.
   */
  public function mergeObjectData($data = array()) {
    foreach ($data as $field => $value) {
      $this->setLocalVal($field, $value);
    }
  }

  /**
   * Safely merge data into an object struct array.
   */
  public function mergeStructData($data = array()) {
    foreach ($data as $field => $value) {
      $this->setStructVal($field, $value);
    }
  }

  /**
   * Translate certain fields to be usable by each system.
   *
   * @param $field
   *   The name of the field to translate.
   * @param $val
   *   A reference to the value of the field to translate.
   * @system
   *   The destination system to translate for.
   */
  static function translateField($field, &$val, $system = 'local') {
    switch ($system) {
      case 'local':
        $field = llf($field);

        // Serialize arrays.
        if (is_array($val)) {
          $val = serialize($val);
          break;
        }

        // Check for dates
        if (preg_match('/^\d{4}\-\d{2}\-\d{2}T\d{2}\:\d{2}\:\d{2}Z$/', $val)) {
          $val = max(strtotime($val), 0);
          break;
        }
        break;

      case 'lyris':
        $field = lrf($field);

        // Unserialize Arrays
        if (LyrisEntity::is_serialized($val)) {
          $val = unserialize($val);
          break;
        }

        // Check for dates
        if (stristr($field, 'date')) {
          $val = format_date((int) $val, 'custom', 'Y-m-d\TH:i:s\Z', 'GMT');
          break;
        }
        break;
    }
  }

  /**
   * Directly set a translated field.
   */
  public function translateFieldSet($field, $system = 'local') {
    $val = $this->localVal($field);
    $this->setLocalVal($field, $val);
  }

  /**
   * Prepare a new LyrisEntity object by populating the object level with the
   * appropriate struct fields and initial default override values.
   *
   * This should not be called on entity_load() as that process will load all
   * the fields from the db record.
   *
   * @param $parent_eid
   *   The entity id of the entity containing values that this entity will
   *   inherit from defaults then for the parent entity directly.
   */
  public function prepareObject($parent_eid = 0) {
    // Get the API version to load.
    $this->getApiVersion();

    // Load the default struct
    $struct = lyris_struct($this->struct_type, 'local', $this->api_version);
    $this->struct = lyris_struct($this->struct_type, 'lyris', $this->api_version);

    // Load the global defaults
    $sel = db_select(LYRIS_DEFAULTS_TABLE, 'l');
    $sel->fields('l')
        ->condition('l.struct', $this->struct_type);

    // First build the global defaults
    $sql = clone($sel);
    $sql->condition('l.parent_eid', 0);
    $this->_prepareObject($sql, $struct);

    // Next build the entity-specific default overrides.
    if ($parent_eid) {
      $sql = clone($sel);
      $sql->condition('l.parent_eid', $parent_eid);
      $this->_prepareObject($sql, $struct);
    }
  }

  /**
   * Helper function to populate object defaults.
   */
  private function _prepareObject($sql, &$struct) {
    $res = $sql->execute();

    // Overwrite loaded struct values with valid DB values.
    while ($row = $res->fetchAssoc()) {
      if (array_key_exists($row['name'], $struct)) {
        $struct[$row['name']] = unserialize($row['default_value']);
        $this->field_defaults[$row['name']] = array('sfid' => $row['sfid'], 'parent_eid' => $row['parent_eid']);
      }
    }

    // Populate the entity with default struct values.
    foreach ($struct as $field => $value) {
      if (!empty($this->field_defaults[$field]) && isset($this->$field)) {
        $this->field_defaults[$field]['previous'] = $this->$field;
      }
      $this->$field = $value;
    }
  }

  /**
   * Populate field permissions for a struct.  All data processed here should
   * already be in local DB format (prefixed and flat).
   */
  public function populatePerms($rids = array(), $parent_eid = 0) {
    if (empty($rids)) {
      $rids = array_keys(user_roles());
    }

    // To prevent duplicate processes, don't load perms for roles that have
    // already been loaded.
    $unloaded = array();
    foreach ($rids as $rid) {
      if (!array_key_exists($rid, $this->perms_loaded)) {
        $unloaded[] = $rid;
        $this->perms_loaded[$rid] = $rid;
      }
    }

    // Load any unloaded role-field permissions.
    if (!empty($unloaded)) {
      // First get the global defaults.
      $sel = db_select(LYRIS_PERM_TABLE, 'p')
             ->fields('p')
             ->condition('p.struct', $this->struct_type)
             ->condition('p.rid', $unloaded, 'IN');

      // First get the global defaults.
      $sql = clone($sel);
      $sql->condition('p.parent_eid', 0);
      $this->_populatePerms($sql);

      // If an $eid was provided, get the parent-specific default overrides.
      if ($parent_eid) {
        $sql = clone($sel);
        $sql->condition('p.parent_eid', $parent_eid);
        $this->_populatePerms($sql);
      }
    }
  }

  /**
   * Helper function to get the perms from the DB.
   */
  private function _populatePerms($sql) {
    $res = $sql->execute();

    while ($row = $res->fetchAssoc()) {
      if (!array_key_exists($row['name'], $this->field_perms)) {
        $this->field_perms[$row['name']] = array();
      }
      $prev = (isset($this->field_perms[$row['name']][$row['rid']]['status']) ? $this->field_perms[$row['name']][$row['rid']]['status'] : NULL);
      $this->field_perms[$row['name']][$row['rid']] = array('status' => $row['status'], 'parent_eid' => $row['parent_eid'], 'previous' => $prev);
    }
  }

  /**
   * Load and populate a struct for a Lyris object.  Prepares a complete Lyris
   * struct for push to Lyris.
   */
  public function populateStruct() {
    // Get the API version to load.
    $this->getApiVersion();

    // Load the default struct
    $this->struct = lyris_struct($this->struct_type, 'lyris', $this->api_version);

    // Apply local values to the Lyris Structs
    foreach ($this->struct as $field => $value) {
      // Convert lyris field name to Drupal field name
      $this->_populateStruct($field, $value);
    }
  }

  /**
   * Cyclical helper function to safely populate struct from local data.
   *
   * @param $field
   *   A Lyris struct field.
   */
  private function _populateStruct($field, $value) {
    if (is_array($value)) {
      foreach ($value as $child => $child_value) {
        $child = $field . '__' . $child;
        $this->_populateStruct($child, $child_value);
      }
    }
    else {
      $this->setStructVal($field, $this->localVal($field));
    }
  }

  /**
   * Convert a Lyris field name to a local database field name.
   *
   * @param $field
   *   Lyris field name.
   */
  static function fieldConvertLocal($field) {
    // Handle arrays
    $field = str_replace('][', '__', $field);

    if (!LyrisEntity::lyrisFieldIsLocal($field)) {
      return LYRIS_DB_FIELD_PREFIX . $field;
    }
    else {
      return $field;
    }
  }

  /**
   * Convert field names to the proper Lyris label.
   */
  static function fieldConvertRemote($field) {
    if (stripos($field, LYRIS_DB_FIELD_PREFIX) === 0) {
      return substr($field, strlen(LYRIS_DB_FIELD_PREFIX));
    }
    else {
      return $field;
    }
  }

  /**
   * Determine whether a given fieldname represents a local field or Lyris field.
   */
  static function lyrisFieldIsLocal($field) {
    return (stripos($field, LYRIS_DB_FIELD_PREFIX) === 0);
  }

  /**
   * Checks whether a field's value is serialzied.
   */
  static function is_serialized($val) {
    if (!is_string($val) || trim($val) == "") {
      return FALSE;
    }
    elseif (preg_match("/^(i|s|a|o|d):(.*);/si", $val)) {
      return TRUE;
    }
    return FALSE;
  }

  /**
   * Process token replacements in Lyris struct fields.
   */
  public function tokenize(array $data = array(), array $options = array()) {
    // Prepare this object to be passed in with the data.
    $data[$this->entity_type] = clone($this);

    foreach ($this->struct as $key => &$value) {
      $this->_tokenize($value, $data, $options);
    }
  }

  /**
   * Helper function to process tokens in multi-level fields.
   */
  private function _tokenize(&$value, $data, $options) {
    if (is_array($value)) {
      foreach ($value as $key => &$_value) {
        $this->_tokenize($_value, $data, $options);
      }
    }
    else {
      $value = token_replace($value, $data, $options);
    }
  }
}

/**
 * Entity API controller
 */
class LyrisEntityAPIController extends EntityAPIController {
  /**
   * Constructor
   */
  public function __construct($entityType) {
    parent::__construct($entityType);
  }

  /**
   * Add the standard lyris entity info to each entity.
   */
  public function attachLoad(&$entities, $revision_id = FALSE) {
    foreach ($entities as $entity) {
      $res = db_select(LYRIS_ENTITY_TABLE, 'le')->fields('le')->condition('le.eid', $entity->eid)->condition('le.entity_type', $entity->entity_type)->execute()->fetchAssoc();
      foreach ($res as $field => $val) {
        $entity->{$field} = $val;
      }
    }

    parent::attachLoad($entities, $revision_id);
  }

  /**
   * Implements EntityAPIControllerInterface.
   *
   * @param $transaction
   *   Optionally a DatabaseTransaction object to use. Allows overrides to pass
   *   in their transaction object.
   *
   * Overridden to delete lyris_entity record as well.
   * @see EntityAPIController::delete()
   */
  public function delete($ids, DatabaseTransaction $transaction = NULL) {
    $entities = $ids ? $this->load($ids) : FALSE;
    if (!$entities) {
      // Do nothing, in case invalid or no ids have been passed.
      return;
    }
    // This transaction causes troubles on MySQL, see
    // http://drupal.org/node/1007830. So we deactivate this by default until
    // is shipped in a point release.
    // $transaction = isset($transaction) ? $transaction : db_transaction();

    try {
      $ids = array_keys($entities);

      // Delete the regular entity. (default behavior)
      db_delete($this->entityInfo['base table'])
        ->condition($this->idKey, $ids, 'IN')
        ->execute();

      // ADDITION: Delete our extended info
      db_delete(LYRIS_ENTITY_TABLE)
        ->condition('eid', $ids, 'IN')
        ->condition('entity_type', $this->entityType)
        ->execute();

      // Reset the cache as soon as the changes have been applied.
      $this->resetCache($ids);

      foreach ($entities as $id => $entity) {
        $this->invoke('delete', $entity);
      }
      // Ignore slave server temporarily.
      db_ignore_slave();
    }
    catch (Exception $e) {
      if (isset($transaction)) {
        $transaction->rollback();
      }
      watchdog_exception($this->entityType, $e);
      throw $e;
    }
  }
}
