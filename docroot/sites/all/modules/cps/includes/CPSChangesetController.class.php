<?php
/**
 * @file
 * Contains the controller class for the Changeset entity.
 */

/**
 * Changeset entity controller class.
 */
class CPSChangesetController extends EntityAPIController {
  /**
   * {@inheritdoc}
   */
  public function create(array $values = array()) {
    global $user;
    $values += array(
      'changeset_id' => drupal_hash_base64(uniqid('', TRUE)),
      'name' => '',
      'uid' => $user->uid,
      'created' => REQUEST_TIME,
      'changed' => REQUEST_TIME,
      'status' => 'unpublished',
    );
    return parent::create($values);
  }

  /**
   * Determine if the operation for the entity can be accessed.
   *
   * @param string $op
   *   The operation being performed. One of 'view', 'update', 'create' or
   *   'delete'. It may also be any changeset operation such as 'publish' or
   *   'unpublish'.
   * @param string|CPSChangeset $entity
   *   (optional) entity to check access for. If no entity is given, it will be
   *   determined whether access is allowed for all entities of the given type.
   * @param stdClass|null $account
   *   The user to check for. Leave it to NULL to check for the global user.
   *
   * @return boolean
   *   Whether access is allowed or not. If the entity type does not specify any
   *   access information, NULL is returned.
   *
   * @see entity_access()
   */
  public function access($op, $entity = NULL, $account = NULL) {
    if (!isset($account)) {
      $account = $GLOBALS['user'];
    }
    $access = FALSE;

    // This is just invalid.
    if ($op !== 'create' && !$entity) {
      return FALSE;
    }

    // No operations are valid on this one.
    // None are valid without the administer changesets permission.
    if (user_access('administer changesets', $account) && (!is_object($entity) || $entity->changeset_id != CPS_PUBLISHED_CHANGESET)) {
      switch ($op) {
        case 'create':
        case 'view':
          $access = TRUE;
          break;

        case 'update':
          $access = user_access('edit all changesets', $account) || $entity->uid == $account->uid;
          break;

        case 'delete':
          // Published changesets may not be deleted.
          $access = empty($entity->published) && (user_access('edit all changesets', $account) || $entity->uid == $account->uid);
          break;

        case 'publish':
          // Only items that have not already been published and have active
          // changes may be published.
          $access = user_access('publish changesets', $account) && empty($entity->published) && !empty($entity->changeCount);
          break;

        case 'unpublish':
          // The 'installed' changeset can never be unpublished.
          if ($entity->changeset_id == CPS_INSTALLED_CHANGESET) {
            return FALSE;
          }
          // Only the most recent item can be unpublished.
          $access = user_access('publish changesets', $account) &&  $entity->getPreviousChangeset();
          break;
      }
    }

    // Let other modules get involved,
    // particularly if there are more workflow states.
    drupal_alter('cps_changeset_access', $access, $op, $entity, $account);
    return $access;
  }

  /**
   * {@inheritdoc}
   */
  public function save($entity, DatabaseTransaction $transaction = NULL) {
    $entity->changed = REQUEST_TIME;
    $status = parent::save($entity, $transaction);
    if (isset($entity->oldStatus) && $entity->oldStatus != $entity->status) {
      // Record a status change in the history.
      $record = array(
        'changeset_id' => $entity->changeset_id,
        'previous_status' => $entity->oldStatus,
        'new_status' => $entity->status,
        'uid' => $GLOBALS['user']->uid,
        'timestamp' => REQUEST_TIME,
        'message' => $entity->statusMessage,
      );

      drupal_write_record('cps_changeset_history', $record);
    }
    return $status;
  }

  /**
   * {@inheritdoc}
   */
  protected function attachLoad(&$queried_entities, $revision_id = FALSE) {
    parent::attachLoad($queried_entities, $revision_id);

    // Get a change count for each changeset.
    // @index cps_entity.published_changeset_id
    $result = db_query("SELECT changeset_id, count(*) as count FROM {cps_entity} WHERE published = 0 AND changeset_id IN (:changeset_ids) GROUP BY changeset_id ", array(':changeset_ids' => array_keys($queried_entities)));
    while ($change = $result->fetchObject()) {
      if (isset($queried_entities[$change->changeset_id])) {
        $queried_entities[$change->changeset_id]->changeCount = $change->count;
      }
    }

    // Add all variables changes to the change count.
    foreach ($queried_entities as $item) {
      if (isset($item->variables) && is_array($item->variables)) {
        $item->changeCount += count($item->variables);
      }
    }

    // Load the status change history for each changeset.
    $result = db_query("SELECT * FROM {cps_changeset_history} WHERE changeset_id IN (:changeset_ids)", array(':changeset_ids' => array_keys($queried_entities)));
    while ($history = $result->fetchObject()) {
      $queried_entities[$history->changeset_id]->history[] = $history;
    }
  }

  /**
   * Implements delegated hook_entity_property_info().
   */
  public function hook_entity_property_info() {
    $info = array();
    // Add meta-data about the basic changeset properties.
    $properties = &$info['cps_changeset']['properties'];
    $properties['changeset_id'] = array(
      'label' => t("Changeset ID"),
      'description' => t("The unique ID of the changeset."),
      'setter callback' => 'entity_property_verbatim_set',
      'schema field' => 'changeset_id',
    );
    $properties['is_new'] = array(
      'label' => t("Is new"),
      'type' => 'boolean',
      'description' => t("Whether the changeset is new and not saved to the database yet."),
      'getter callback' => 'entity_metadata_entity_get_properties',
    );
    $properties['name'] = array(
      'label' => t("Name"),
      'description' => t("The title of the changeset."),
      'setter callback' => 'entity_property_verbatim_set',
      'schema field' => 'name',
      'required' => TRUE,
    );
    $properties['description'] = array(
      'label' => t("Description"),
      'description' => t("The description of the changeset."),
      'setter callback' => 'entity_property_verbatim_set',
      'schema field' => 'description',
      'required' => TRUE,
    );
    $properties['url'] = array(
      'label' => t("URL"),
      'description' => t("The URL of the changeset."),
      'getter callback' => 'entity_metadata_entity_get_properties',
      'type' => 'uri',
      'computed' => TRUE,
      'entity views field' => TRUE,
    );
    $properties['edit_url'] = array(
      'label' => t("Edit URL"),
      'description' => t("The URL of the changeset's edit page."),
      'getter callback' => 'cps_changeset_metadata_get_properties',
      'type' => 'uri',
      'computed' => TRUE,
      'entity views field' => TRUE,
    );
    $properties['delete_url'] = array(
      'label' => t("Delete URL"),
      'description' => t("The URL of the changeset's delete page."),
      'getter callback' => 'cps_changeset_metadata_get_properties',
      'type' => 'uri',
      'computed' => TRUE,
      'entity views field' => TRUE,
    );
    $properties['status'] = array(
      'label' => t("Status"),
      'type' => 'text',
      'description' => t("The current state of the changeset."),
      'options list' => 'cps_changeset_get_state_labels',
      'getter callback' => 'entity_property_verbatim_get',
      'setter callback' => 'entity_property_verbatim_set',
      'auto creation' => TRUE,
      'schema field' => 'status',
    );
    $properties['created'] = array(
      'label' => t("Date created"),
      'type' => 'date',
      'description' => t("The date the changeset was posted."),
      'setter callback' => 'entity_property_verbatim_set',
      'setter permission' => 'administer changesets',
      'schema field' => 'created',
    );
    $properties['changed'] = array(
      'label' => t("Date changed"),
      'type' => 'date',
      'schema field' => 'changed',
      'description' => t("The date the changeset was most recently updated."),
    );
    $properties['published'] = array(
      'label' => t("Date published"),
      'type' => 'date',
      'schema field' => 'published',
      'description' => t("The date the changeset was published, if it has been."),
    );
    $properties['author'] = array(
      'label' => t("Author"),
      'type' => 'user',
      'description' => t("The author of the changeset."),
      'getter callback' => 'cps_changeset_metadata_get_properties',
      'setter callback' => 'entity_property_verbatim_set',
      'setter permission' => 'administer changesets',
      'required' => TRUE,
      'schema field' => 'uid',
    );
    $properties['operations'] = array(
      'label' => t("Operations"),
      'description' => t("The available operations for a changeset."),
      'getter callback' => 'cps_changeset_metadata_get_properties',
      'computed' => TRUE,
      'entity views field' => TRUE,
      'type' => 'text',
      'sanitized' => TRUE,
    );
    $properties['change_count'] = array(
      'label' => t("Changes"),
      'description' => t("The number of changes in this changeset"),
      'computed' => TRUE,
      'schema field' => 'changeCount',
      'entity views field' => TRUE,
    );

    return $info;
  }

  /**
   * @{inheritdoc}
   */
  public function buildContent($entity, $view_mode = 'full', $langcode = NULL, $content = array()) {
    /** @var CPSChangeset $entity */
    $build = parent::buildContent($entity, $view_mode, $langcode, $content);

    $account = user_load($entity->uid);
    $build['submitted'] = array(
      '#markup' => t('Created by !username on !datetime', array('!username' => format_username($account), '!datetime' => format_date($entity->created))),
    );

    // Display the changeset status.
    $options = cps_changeset_get_state_labels();
    $build['status'] = array(
      '#type' => 'item',
      '#title' => t('Status'),
      '#prefix' => '<strong>',
      '#markup' => $options[$entity->status],
      '#suffix' => '</strong>',
    );

    $build['description'] = array(
      '#type' => 'item',
      '#title' => t('Description'),
      '#prefix' => '<div class="description">',
      '#markup' => check_plain($entity->description),
      '#suffix' => '</div>',
    );

    // In the full view mode, display changes and history at the bottom.
    if ($view_mode == 'full') {
      $build['changes'] = $entity->renderChangedEntities();
      $build['history'] = $entity->renderHistory();
    }

    return $build;
  }
}
