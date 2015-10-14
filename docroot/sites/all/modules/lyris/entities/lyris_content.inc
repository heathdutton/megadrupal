<?php
/**
 * @file
 * Separate include file to house lyris_content entity functionality.
 */
/**
 * Instantiate a new, empty Lyris Content object.  This is a wrapper for
 * lyris_content_load as well as a preparatory mechanism for new content.
 */
function lyris_content($parent_eid = 0) {
  $content = lyris_entity_create('lyris_content', $parent_eid);

  // Inherit fields from list.
  if ($parent_eid) {
    $list = lyris_list_load($parent_eid);

    if (is_object($list)) {
      $content->setLocalVal('ListName',    $list->localVal('ListName'));
      $content->setLocalVal('HeaderFrom',  $list->localVal('DefaultFrom'));
      $content->setLocalVal('HeaderTo',    $list->localVal('DefaultTo'));
      $content->setLocalVal('NativeTitle', $list->localVal('DefaultSubject'));
    }
  }

  return $content;
}

/**
 * Access callback for Lyris content.
 *
 * $param $op
 *   The operation being requested.  Values are:
 *   - View
 *   - Create
 *   - Update
 *   - Delete
 * $param $entity
 *   The entity object or string representing the entity the operation is being
 *   performed on.
 */
function lyris_content_access($op, $entity = NULL) {
  global $user;

  // If the user has the global administrative permission, grant them
  // permission.
  if (user_access('administer lyris content')) {
    return TRUE;
  }

  switch ($op) {
    case 'view':
      return TRUE;
      break;

    case 'create':
      $bundles = lyris_get_lists();

      // If we are checking a specific entity or entity type, get the bundle
      // to verify against.
      if ($entity) {
        $bundle = (is_object($entity) ? $entity->localVal('ListName') : $entity);
        return user_access('create lyris content for ' . $bundle);
      }
      // If no entity or entity type was supplied, see if the user has
      // permission to create an entity of any bundle.
      else {
        foreach ($bundles as $list) {
          if (user_access('create lyris content for ' . $list[llf('ListName')])) {
            return TRUE;
          }
        }
      }
      break;

    case 'update':
      return (user_access('edit any lyris content for ' . $lyris_content->localVal('ListName')) ||
              (user_access('edit own lyris content for ' . $lyris_content->localVal('ListName')) && $lyris_content->uid == $user->uid));
      break;

    case 'delete':
      return (user_access('delete any lyris content for ' . $lyris_content->localVal('ListName')) ||
              (user_access('delete own lyris content for ' . $lyris_content->localVal('ListName')) && $lyris_content->uid == $user->uid));
      break;
  }
}

/**
 * Delete lyris content.
 */
function lyris_content_delete($eid, $delete_lyris = FALSE) {
  lyris_content_delete_multiple(array($eid), $delete_lyris);
}

/**
 * Delete multiple pieces of lyris content.
 */
function lyris_content_delete_multiple($eids, $delete_lyris = FALSE) {
  $transaction = db_transaction();

  if (!empty($eids)) {
    $content = lyris_content_load_multiple($eids);

    try {
      foreach ($content as $eid => $lyris_content) {
        // Call delete hooks
        module_invoke_all('lyris_content_delete', $lyris_content);
        module_invoke_all('entity_delete', $lyris_content, 'lyris_content');
        field_attach_delete('lyris_content', $lyris_content);
      }

      // Delete from the DB
      db_delete(LYRIS_CONTENT_TABLE)
        ->condition('eid', $eids, 'IN')
        ->execute();

      // Delete from Lyris
      if ($delete_lyris) {
        $list = lyris_list_load($lyris_content->localVal('ListName'));
        $lyris = lyris(NULL, $list->server);
        $lyris_content->populateStruct();
        $lyris->deleteContent($lyris_content);
      }
    }
    catch (Exception $e) {
      $transaction->rollback();
      watchdog_exception('lyris_content', $e);
      throw $e;
    }

    // Clear the caches.
    entity_get_controller('lyris_content')->resetCache();
  }
}

/**
 * Wrapper to load a Lyris Content entity.
 */
function lyris_content_load($eid, $reset = FALSE) {
  return lyris_entity_load_entity('lyris_content', $eid, $reset);
}

/**
 * Wrapper to load multiple Lyris Content entities.
 */
function lyris_content_load_multiple($eids = array(), $conditions = array(), $reset = FALSE) {
  return lyris_entity_load_multiple('lyris_content', $eids, $conditions, $reset);
}

/**
 * Load a piece of content from the ContentID.
 */
function lyris_content_load_contentID($ContentID) {
  $eid = db_select(LYRIS_CONTENT_TABLE, 'lc')->condition(llf('ContentID'), $ContentID)->fields('lc', array('eid'))->execute()->fetchField();
  return lyris_content_load($eid);
}

/**
 * Save a Lyris Content entity.
 *
 * @see node_save().
 */
function lyris_content_save(LyrisContent $lc) {
  return $lc->save();
}

/**
 * Menu Callback for Lyris Content page title.
 */
function lyris_content_title(LyrisContent $lc) {
  return $lc->localVal('Title');
}

/**
 * Lyris Content Entity Callback Path.
 */
function lyris_content_uri($lyris_content) {
  return array(
    'path' => 'lyris/' . $lyris_content->eid,
  );
}

/**
 * Entity class for Lyris Content.
 */
class LyrisContent extends LyrisEntity {
  public $eid;
  public $uid;
  public $changed;
  public $synced;

  /**
   * Constructor
   */
  public function __construct(array $values = array()) {
    global $user;

    // If this is new then set the type first.
    if (isset($values['is_new'])) {
      $this->type = isset($values['type']) ? $values['type'] : '';
    }

    // Default the user to the active user.  It may be overridden when a piece
    // of content is loaded from the DB.
    $this->uid = $user->uid;
    parent::__construct('ContentStruct', 'lyris_content', $values);
  }

  /**
   * Implements LyrisEntityInterface::getDisplayName().
   */
  public function getDisplayName() {
    return $this->localVal('Title');
  }

  /**
   * Update or insert Lyris Content.
   */
  public function save() {
    // Are we inserting or updating?
    if (!isset($this->is_new)) {
      $this->is_new = empty($this->eid);
    }

    // Save the content
    if ($this->is_new) {
      $this->setLocalVal('DateCreated', REQUEST_TIME);
    }

    return parent::save();
  }

  /**
   * Push the content to Lyris.
   */
  public function pushToLyris() {
    // Get the list data so we know which Lyris server to run on.
    $list = lyris_list_load($this->localVal('ListName'));

    // Get a connection to the Lyris server for the list housing this content.
    $lyris = lyris(user_load($this->changer), $list->server);

    // Content cannot be created in Lyris via the API in API version 1.5.2a or
    // older.  Mailings will be generated and sent directly from the Drupal
    // entities
    if ($list->api_version == '1.5.2a') {
      return;
    }

    // Ensure some content has been set.
    if (!isset($this->struct['DocParts'])) {
      drupal_set_message(t('No body value has been set.'), 'error');
      return FALSE;
    }

    // Process any tokens in Lyris struct fields.
    $data['lyris_list'] = lyris_list_load($this->localVal('ListName'));
    $this->tokenize($data);

    // Update or Insert?
    if ($cid = $this->localVal('ContentID')) {
      $this->setStructVal('ContentID', $cid);
      if ($lyris->updateContent($this)) {
        $this->setSynced(SAVED_UPDATED);
        return SAVED_UPDATED;
      }
    }
    else {
      if ($cid = $lyris->createContent($this)) {
        $this->setStructVal('ContentID', $cid);
        $this->setLocalVal('ContentID', $cid);
        $this->setLocalVal('DateCreated', time());
        $this->setSynced(SAVED_NEW);
        return SAVED_NEW;
      }
    }
  }

  /**
   * Add a DocParts section.  DocParts is an array of multiple DocPart arrays.
   */
  public function addDocPart($docpart) {
    // Ensure all Lyris DocPart parts are set for transmission.
    $defaults = array(
      'Body'         => '',
      'MimePartName' => $this->localVal('DocParts__MimePartName'),
      'CharSetID'    => $this->localVal('DocParts__CharSetID'),
      'Encoding'     => $this->localVal('DocParts__Encoding'),
    );
    $docpart += $defaults;
    $this->struct['DocParts'][] = $docpart;
  }
}

/**
 * Entity API Controller for Lyris Content.
 */
class LyrisContentEntityAPIController extends LyrisEntityAPIController {}
