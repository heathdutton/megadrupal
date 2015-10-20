<?php
/**
 * @file
 * Contains the class for the Changeset entity.
 */

/**
 * Class to handle changeset entities.
 */
class CPSChangeset extends Entity {
  /**
   * The database identifier of this changeset, which will be a sha256.
   *
   * @var string
   */
  public $changeset_id = NULL;

  /**
   * The human readable name of this changeset, used as the entity label.
   *
   * @var string
   */
  public $name = NULL;

  /**
   * The user ID of the author/owner of the changeset.
   *
   * @var int
   */
  public $uid = NULL;

  /**
   * The administrative description of this changeset.
   *
   * @var string
   */
  public $description = NULL;

  /**
   * The current status of this changeset.
   *
   * This can be 'published', 'archived' or 'live' for the special live
   * changeset. Other modules can add additional status strings.
   *
   * @var string
   */
  public $status = NULL;

  /**
   * The date of publication of this changeset as a Unix Timestamp.
   *
   * @var int
   */
  public $published = 0;

  /**
   * TRUE if the changeset is locked into the selector in the IIB bar.
   */
  public $lock_in_select = FALSE;
  
  /**
   * An array of variables that are changed as part of this changeset.
   *
   * @var array
   */
  public $variables = array();

  /**
   * The history of this changeset as loaded from cps_changeset_history.
   *
   * @var array
   */
  public $history = array();

  /**
   * The original status of the changeset, if it has been changed.
   *
   * @var string
   */
  public $oldStatus = NULL;

  /**
   * A message that may be set when the changeset status is changed, to later
   * be saved to the history.
   *
   * @var string
   */
  public $statusMessage = NULL;

  /**
   * Contains the previous status after the changeset status is changed.
   *
   * @var string|null
   */
  protected $previousChangeset = NULL;
  /**
   * The number of changed entities in the changeset.
   *
   * @var int
   */
  public $changeCount = 0;

  /**
   * A list of changed entities for the changeset, grouped by entity type.
   *
   * @var array
   */
  protected $changedEntities = NULL;

  /**
   * {@inheritdoc}
   */
  protected function defaultLabel() {
    return $this->name;
  }

  /**
   * {@inheritdoc}
   */
  protected function defaultUri() {
    return array('path' => 'admin/structure/changesets/' . $this->identifier());
  }

  /**
   * Get a list of changed entities for the changeset.
   *
   * @return array
   *   An array of entities that have revisions for this changeset,
   *   grouped by entity typed.
   */
  public function getChangedEntities() {
    if (!isset($this->changedEntities)) {
      $this->changedEntities = array();
      $result = db_query("SELECT * FROM {cps_entity} WHERE changeset_id = :changeset AND published = 0", array(':changeset' => $this->changeset_id));

      $entities = array();
      $entity_info = entity_get_info();

      // Load each of the changes individually; we have to do them this way
      // instead of in a batch because we are loading specific revisions.
      while ($change = $result->fetchObject()) {
        // Not sure if this can happen.
        if (empty($entity_info[$change->entity_type])) {
          continue;
        }

        $entities[$change->entity_type][] = $change->entity_id;
      }

      cps_override_changeset($this->changeset_id);
      foreach ($entities as $entity_type => $ids) {
        // Tell it to load items for this changeset
        $this->changedEntities[$entity_type] = entity_load($entity_type, $ids, array(), TRUE);

        // Now tell it to go back to loading items from the selected changeset.
      }
      cps_override_changeset(NULL);
    }
    return $this->changedEntities;
  }

  /**
   * Removes an entity from the changeset by deleting its associated revision.
   *
   * @param string $entity_type
   *   The type of entity being removed, for example 'node'.
   * @param object $entity
   *   The entity being removed. The revision associated with the provided
   *   entity object is the one that will be deleted, so calling code should
   *   make sure that the loaded revision actually corresponds to the one from
   *   the correct changeset when providing an entity to this function.
   *
   * @return bool
   *   TRUE if the entity was removed from the changeset, or FALSE if it could
   *   not be removed.
   */
  public function removeEntity($entity_type, $entity) {
    list($entity_id, $revision_id) = entity_extract_ids($entity_type, $entity);

    cps_override_changeset(CPS_PUBLISHED_CHANGESET);
    $success = entity_revision_delete($entity_type, $revision_id);
    cps_override_changeset(NULL);
    if (!$success) {
      return FALSE;
    }

    module_invoke_all('cps_remove_changeset', $this, $entity_type, $entity);
    // We also need to remove any field collections that point to our entity from the changeset.
    $entities = $this->getChangedEntities();
    if (!empty($entities['field_collection_item'])) {
      /** @var FieldCollectionItemEntity $fc_entity */
      foreach ($entities['field_collection_item'] as $fc_entity) {
        if ($fc_entity->hostEntityType() == $entity_type) {
          $host = $fc_entity->hostEntity();
          list($host_entity_id) = entity_extract_ids($entity_type, $host);
          if ($host_entity_id == $entity_id) {
            cps_override_changeset(CPS_PUBLISHED_CHANGESET);
            entity_revision_delete('field_collection_item', $fc_entity);
            cps_override_changeset(NULL);
          }
        }
      }
    }

    return TRUE;
  }

  /**
   * Figure out the previous changeset in order to see if this changeset can be unpublished.
   *
   * @return string|false
   *   The changeset id or false if there is not a previous changeset.
   */
  public function getPreviousChangeset() {
    // unpublished changesets do not have a previous changeset.
    if (empty($this->published)) {
      return FALSE;
    }
    if (!isset($this->previousChangeset)) {
      $this->previousChangeset = FALSE;
      $result = db_query("SELECT changeset_id FROM {cps_changeset} ORDER BY published DESC LIMIT 2")->fetchCol();
      if (count($result) == 2 && $result[0] == $this->changeset_id) {
        $this->previousChangeset = $result[1];
      }
    }

    return $this->previousChangeset;
  }

  /**
   * Set the status of the changeset.
   *
   * This should always be called so that the orignal status can be preserved.
   * When the changeset is saved, that change can be acted on via hook_entity_update.
   *
   * @param string $status
   *   The status to set. Should be one of the statuses available from
   *   cps_changeset_get_states().
   * @param string $message
   *   An optional message to be stored and potentially later used for
   *   a notification.
   */
  public function setStatus($status, $message = NULL) {
    $this->oldStatus = $this->status;
    $this->status = $status;
    $this->statusMessage = $message;
  }

  /**
   * Generate a Drupal build array of changed entities for rendering.
   *
   * @return array
   *   A Drupal render array.
   */
  function renderChangedEntities() {
    $build = array(
      '#attached' => array(
        'js' => array(ctools_attach_js('collapsible-div')),
        'css' => array(ctools_attach_css('collapsible-div')),
      )
    );

    // Display a list of changed entities to make it easy to examine what
    // changes this set contains.
    $changes = $this->getChangedEntities();

    if (!empty($changes)) {
      $entity_info = entity_get_info();
      foreach ($changes as $entity_type => $changed_entities) {
        $build[$entity_type] = array(
          '#type' => 'item',
          '#title' => t('Changed %type', array('%type' => $entity_info[$entity_type]['label'])),
          '#prefix' => '<table class="cps-changed-entities">',
          '#suffix' => '</table>',
        );

        foreach ($changed_entities as $entity_id => $change) {
          ctools_include('diff', 'cps');
          $build[$entity_type][$entity_id] = cps_render_changed_entity($entity_type, $change, $this);
        }
      }
    }
    else {
      $build['#markup'] = '<p class="cps-changeset-no-changes">' . t('This site version has no changes.') . '</p>';
    }

    return $build;
  }

  /**
   * Render a table for the changeset's history.
   *
   * @return array
   */
  function renderHistory() {
    $statuses = cps_changeset_get_state_labels();
    $account = user_load($this->uid);
    $items = array(
      array(
        t('Created'),
        format_date($this->created),
        format_username($account),
        '',
      )
    );

    foreach ($this->history as $item) {
      $status = isset($statuses[$item->new_status]) ? $statuses[$item->new_status] : '';
      // Special status for unpublishing.
      if ($item->new_status == 'unpublished' && $item->previous_status == CPS_ARCHIVED_STATUS) {
        $status = t('Unpublished');
      }

      drupal_alter('cps_changeset_history_status', $status, $this, $item);
      $account = user_load($item->uid);
      $items[] = array(
        $status,
        format_date($item->timestamp),
        format_username($account),
        $item->message,
      );
    }

    $build = array(
      '#title' => t('Site version history'),
      '#type' => 'item',
      '#theme' => 'table',
      '#header' => array(t('Status'), t('Date'), t('Author'), t('Message')),
      '#rows' => $items
    );

    return $build;
  }
}
