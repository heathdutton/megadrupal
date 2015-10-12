<?php
/**
 * @file
 * Separate include file to house lyris_list entity functionality.
 */
/**
 * Sets the default values for a list bundle.  List defaults are compiled in the
 * following order:
 *
 * 1. Default struct data from lyris.structs.inc.
 * 2. Site level defaults set from the Defaults form.
 *
 * @param $info
 *   An object or array containing values to override the defaults.
 *
 * @return
 *   A node type object, with missing values in $info set to their defaults.
 */
function lyris_list() {
  return lyris_entity_create('lyris_list');
}

/**
 * Access callback for Lyris lists.
 */
function lyris_list_access($op, $list, $account = NULL) {
  if (user_access('administer lyris')) {
    return TRUE;
  }

  $account = (is_object($account) ? $account : lyris_global_user());
  $list = (is_object($list) ? $list : lyris_list_load($list));
  $lyris = lyris($account, $list->server);
  $ListName = $list->localVal('ListName');

  switch ($op) {
    case 'view':
      if (user_access('view any lyris list') || $lyris->userListAccess($ListName)) {
        return TRUE;
      }
      break;

    case 'create':
      // Not likely to be used
      break;


    case 'import':
      if (variable_get('lyris_mode', LYRIS_MODE_DEFAULT) == LYRIS_MODE_OFFLINE) {
        return FALSE;
      }
      else {
        return (user_access('import lyris lists') && $lyris->userListAccess($ListName));
      }
      break;

    case 'update':
      return (user_access('edit any lyris lists') || $lyris->userListAccess($ListName));
      break;

    case 'delete':

      break;

    case 'manage members':
      return $lyris->userListAccess($ListName);
      break;

    case 'remote':
      return $lyris->userListAccess($ListName);
      break;
  }
}

/**
 * Delete an association to a Lyris list and optionally delete the list from
 * Lyris completely.
 *
 * @param $ListName
 *   The Lyris list name.
 * @param $delete_lyris
 *   Boolean value whether to delete from Lyris.
 *
 * @return
 *   One of four options:
 *   - LYRIS_LIST_DELETE_LOCAL   (Local was deleted)
 *   - LYRIS_LIST_DELETE_LYRIS   (Lyris was deleted)
 *   - LYRIS_LIST_DELETE_BOTH    (Both local and lyris were deleted)
 *   - LYRIS_LIST_DELETE_FAILURE (Neither were deleted)
 *
 *   If implementing this function, make sure you evaluate the result you are
 *   expecting.  For example, if you are not deleting from Lyris, you should
 *   expect either LYRIS_LIST_DELETE_LOCAL or LYRIS_LIST_DELETE_FAILURE but not
 *   LYRIS_LIST_DELETE_BOTH.
 */
function lyris_list_delete($ListName, $delete_lyris = FALSE) {
  // Moved into controller.inc
}

/**
 * Load a Lyris List from the DB.
 */
function lyris_list_load($eid, $reset = FALSE) {
  // If we were given a ListName instead of an eid, get the eid.
  if (!is_numeric($eid)) {
    $row = db_select(LYRIS_LIST_TABLE, 'll')->fields('ll', array('eid'))->condition('ll.' . llf('ListName'), $eid)->execute()->fetchAssoc();
    $eid = $row['eid'];
  }
  return lyris_entity_load_entity('lyris_list', $eid, $reset);
}

/**
 * Load multiple lists from Lyris.
 */
function lyris_list_load_multiple($eids = array(), $conditions = array(), $reset = FALSE) {
  return lyris_entity_load_multiple('lyris_list', $eids, $conditions, $reset);
}

/**
 * Validate the existence of a list name.
 */
function lyris_list_name_exists($listname) {
  $lyris = lyris('admin');
  return $lyris->listExists($listname);
}

/**
 * Callback for menu list title.
 */
function lyris_list_page_title($list) {
  return $list->title;
}

/**
 * Return a list prefix with separator if one is set.
 */
function lyris_list_prefix() {
  $prefix = variable_get('lyris_list_prefix');
  return ($prefix ? $prefix . '_' : '');
}

/**
 * Save a List bundle.
 *
 * @param $info
 *   A list object to save
 */
function lyris_list_save(LyrisList $list) {
  return $list->save();
}

/**
 * List URI Callback
 */
function lyris_list_uri($list) {
  return 'lyris/list/' . $list->eid;
}

/**
 * Entity class for Lyris Lists.
 */
class LyrisList extends LyrisEntity implements LyrisEntityInterface {
  public $title;
  public $template = '';
  public $template_format;
  public $origin;
  public $server;

  // Valid type so of lists
  private $allowed_types = array('marketing', 'announcement-moderated', 'discussion-moderated', 'discussion-unmoderated');

  // The lyris list type (of allowed types)
  public $type = 'marketing';

  // The default topic for the list.
  public $topic = NULL;

  /**
   * Constructor
   *
   * The current Lyris object is passed in to instantiate the connection to Lyris
   * so the connected user is inherited and will become the admin of any new
   * lists.
   */
  public function __construct(array $values = array()) {
    // If this is new then set the type first.
    if (isset($values['is_new'])) {
      $this->type = isset($values['type']) ? $values['type'] : '';
      $this->server = variable_get('lyris_mode', LYRIS_MODE_DEFAULT);
    }

    // Set the default input format if one has not been assigned.
    if (!$this->template_format) {
      $this->template_format = filter_default_format();
    }

    parent::__construct('ListStruct', 'lyris_list', $values);
  }

  /**
   * Implements LyrisEntityInterface::getDisplayName().
   */
  public function getDisplayName() {
    return $this->title;
  }

  /**
   * Entity API.
   */
  /**
   * Save the list locally.
   */
  public function save() {
    // Are we renaming the list
    $existing_name = (!empty($this->old_name) ? $this->old_name : $this->localVal('ListName'));

    // Check Drupal for existence.
    $this->is_new = !(bool) $this->eid;

    // Ensure all fields are in the correct format for local storage.
    $this->translateFieldSet('SubscriptionReports');

    // Save the entity
    $status = parent::save();

    if ($status == SAVED_UPDATED) {
      if (!empty($this->old_name) && $this->old_name != $this->localVal('ListName')) {
        field_attach_rename_bundle('lyris_content', $this->old_name, $this->localVal('ListName'));
      }
    }
    else {
      field_attach_create_bundle('lyris_content', $this->localVal('ListName'));
      watchdog('lyris', 'New lyris list %name created.', array('%name' => $this->localVal('ListName')));
    }

    return $status;
  }

  /**
   * Push a list to Lyris.
   */
  public function pushToLyris() {
    $lyris = lyris(user_load($this->changer), $this->server);

    // Process any tokens in Lyris struct fields.
    $this->tokenize();

    if ($this->structVal('ListID')) {
      if ($lyris->updateList($this)) {
        $this->setSynced(SAVED_UPDATED);
        return SAVED_UPDATED;
      }
    }
    else {
      if ($lyris->createList($this)) {
        $this->setSynced(SAVED_NEW);
        return SAVED_NEW;
      }
    }

    return FALSE;
  }

  /**
   * Determine whether this list has been loaded.
   *
   * @param $system
   *   The system (Local or Lyris) to validate.  If NULL both are validated.
   *
   * @return
   *   Boolean determining whether the object was loaded.

  public function isLoaded($system = NULL) {
    switch ($system) {
      case 'local':
        return (is_numeric($this->localVal('ListID')) && $this->localVal('ListID') > 0);
        break;

      case 'lyris':
        return (!empty($this->struct) && is_numeric($this->struct['ListID']) && $this->struct['ListID'] > 0);
        break;

      default:
        return ($this->isLoaded('local') && $this->isLoaded('lyris'));
        break;
    }
  }*/
}

/**
 * List Entity API Controller
 */
class LyrisListEntityAPIController extends LyrisEntityAPIController {
  /**
   * Constructor
   */
  public function __construct($entityType) {
    parent::__construct($entityType);
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

    try {
      // Delete the list bundles
      foreach ($entities as $entity) {
        field_attach_delete_bundle('lyris_content', $entity->localVal('ListName'));
      }
    }
    catch (Exception $e) {
      if (isset($transaction)) {
        $transaction->rollback();
      }
      watchdog_exception($this->entityType, $e);
      throw $e;
    }

    parent::delete($ids, $transaction);
  }
}
