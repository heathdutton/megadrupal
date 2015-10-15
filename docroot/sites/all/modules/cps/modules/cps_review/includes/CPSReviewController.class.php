<?php
/**
 * @file
 * Contains the controller class for the CPS review entity.
 */

/**
 * CPS review entity controller class.
 */
class CPSReviewController extends EntityAPIController {
  /**
   * {@inheritdoc}
   */
  public function create(array $values = array()) {
    global $user;
    $values += array(
      'changeset_id' => cps_get_current_changeset(),
      'uid' => $user->uid,
      'title' => '',
      'created' => REQUEST_TIME,
      'changed' => REQUEST_TIME,
      'status' => TRUE,
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
   * @param string|CPSReview $entity
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
    // None are valid without the administer reviews permission.
    if (user_access('administer cps reviews', $account)) {
      switch ($op) {
        case 'create':
          $access = TRUE;
          break;
        case 'view':
          $access = $entity->status == ENTITY_PUBLISHED || user_access('view unpublished cps reviews', $account);
          break;

        case 'update':
          $access = user_access('edit all cps reviews', $account) || $entity->uid == $account->uid;
          break;

        case 'delete':
          // Reviews in published changesets may not be deleted.
          $changeset = cps_changeset_load($entity->changeset_id);
          $access = $changeset && empty($changeset->published);
          break;

        case 'publish':
          $access = user_access('publish cps reviews', $account) && $entity->status == ENTITY_UNPUBLISHED;
          break;

        case 'unpublish':
          $access = user_access('publish cps reviews', $account) && $entity->status == ENTITY_PUBLISHED;
          $access = $access || ($account->uid == $entity->uid);
          break;
      }
    }

    // Let other modules get involved,
    drupal_alter('cps_review_access', $access, $op, $entity, $account);
    return $access;
  }

  /**
   * {@inheritdoc}
   */
  public function save($entity, DatabaseTransaction $transaction = NULL) {
    $entity->changed = REQUEST_TIME;
    return parent::save($entity, $transaction);
  }

  /**
   * {@inheritdoc}
   */
  protected function attachLoad(&$queried_entities, $revision_id = FALSE) {
    parent::attachLoad($queried_entities, $revision_id);
  }

  /**
   * Implements delegated hook_entity_property_info().
   */
  public function hook_entity_property_info() {
    $info = array();
    // Add meta-data about the basic CPS review properties.
    $properties = &$info['cps_review']['properties'];
    $properties['changeset_id'] = array(
      'label' => t("Associated Changeset ID"),
      'description' => t("The ID of the associated changeset."),
      'setter callback' => 'entity_property_verbatim_set',
      'schema field' => 'changeset_id',
    );
    $properties['title'] = array(
      'label' => t("Title"),
      'description' => t("The title of the CPS review."),
      'setter callback' => 'entity_property_verbatim_set',
      'schema field' => 'title',
    );
    $properties['is_new'] = array(
      'label' => t("Is new"),
      'type' => 'boolean',
      'description' => t("Whether the CPS review is new and not saved to the database yet."),
      'getter callback' => 'entity_metadata_entity_get_properties',
    );
    $properties['url'] = array(
      'label' => t("URL"),
      'description' => t("The URL of the CPS review."),
      'getter callback' => 'entity_metadata_entity_get_properties',
      'type' => 'uri',
      'computed' => TRUE,
      'entity views field' => TRUE,
    );
    $properties['edit_url'] = array(
      'label' => t("Edit URL"),
      'description' => t("The URL of the CPS review's edit page."),
      'getter callback' => 'cps_review_metadata_get_properties',
      'type' => 'uri',
      'computed' => TRUE,
      'entity views field' => TRUE,
    );
    $properties['delete_url'] = array(
      'label' => t("Delete URL"),
      'description' => t("The URL of the CPS review's delete page."),
      'getter callback' => 'cps_review_metadata_get_properties',
      'type' => 'uri',
      'computed' => TRUE,
      'entity views field' => TRUE,
    );
    $properties['status'] = array(
      'label' => t("Status"),
      'type' => 'boolean',
      'description' => t("The current state of the CPS review."),
      'getter callback' => 'entity_property_verbatim_get',
      'setter callback' => 'entity_property_verbatim_set',
      'auto creation' => TRUE,
      'schema field' => 'status',
    );
    $properties['created'] = array(
      'label' => t("Date created"),
      'type' => 'date',
      'description' => t("The date the CPS review was posted."),
      'setter callback' => 'entity_property_verbatim_set',
      'setter permission' => 'administer cps reviews',
      'schema field' => 'created',
    );
    $properties['changed'] = array(
      'label' => t("Date changed"),
      'type' => 'date',
      'schema field' => 'changed',
      'description' => t("The date the CPS review was most recently updated."),
    );
    $properties['author'] = array(
      'label' => t("Author"),
      'type' => 'user',
      'description' => t("The author of the CPS review."),
      'getter callback' => 'cps_review_metadata_get_properties',
      'setter callback' => 'entity_property_verbatim_set',
      'setter permission' => 'administer cps reviews',
      'required' => TRUE,
      'schema field' => 'uid',
    );
    $properties['operations'] = array(
      'label' => t("Operations"),
      'description' => t("The available operations for a CPS review."),
      'getter callback' => 'cps_review_metadata_get_properties',
      'computed' => TRUE,
      'entity views field' => TRUE,
      'type' => 'text',
      'sanitized' => TRUE,
    );

    return $info;
  }

  /**
   * @{inheritdoc}
   */
  public function buildContent($entity, $view_mode = 'full', $langcode = NULL, $content = array()) {
    /** @var CPSReview $entity */
    $build = array();
    $build['body'] = parent::buildContent($entity, $view_mode, $langcode, $content);

    $account = user_load($entity->uid);
    $build['title'] = array(
      '#markup' => check_plain($entity->title),
      '#access' => variable_get('cps_review_use_title', 0) == 1,
      '#weight' => -10,
    );
    $uri = $entity->permalink();
    $build['submitted'] = array(
      '#markup' => t('Submitted by !username on !datetime - !permalink', array(
        '!username' => l(format_username($account), 'user/' . $account->uid),
        '!datetime' => format_date($entity->created),
        '!permalink' => l(t('Permalink'), $uri['path'], $uri['options']),
        )
      ),
      '#weight' => -5,
    );
    $build['links'] = array(
      '#type' => 'container',
      '#weight' => 10,
      '#attributes' => array(
        'class' => array(
          'entity-cps-review__operations',
        ),
      ),
      'operations' => cps_review_get_operations($entity),
    );
    $build['thread'] = array(
      '#type' => 'container',
      '#attributes' => array(
        'class' => array(
          'entity-cps-review__thread',
        ),
      ),
      '#weight' => 20,
      'thread' => cps_review_get_thread($entity),
    );
    return $build;
  }
}
