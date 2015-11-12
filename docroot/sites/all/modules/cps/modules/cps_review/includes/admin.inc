<?php
/**
 * @file
 * CPS review administration pages.
 */

/**
 * List all entities for the given type.
 *
 * @param $view_name
 *   The view to render the reviews admin screen.
 *
 * @return string
 *   A rendered page.
 */
function cps_review_list_page($view_name) {
  $view = views_get_view($view_name);
  if (!$view || !$view->access('default')) {
    return MENU_NOT_FOUND;
  }

  drupal_set_title(t('All CPS reviews'));

  $view->override_path = $_GET['q'];
  return $view->preview('default');
}

/**
 * Field callback to add a new review and list existing reviews.
 *
 * @param string $container_entity_type
 *   The entity type of the containing entity.
 * @param int $container_entity_id
 *   The entity ID this review is part of.
 *
 * @return array
 *   The rendered output of the extra field with keys 'form' and 'reviews'.
 */
function cps_review_widget($container_entity_type, $container_entity) {
  $entity = cps_review_create();

  list($container_entity_id,,) = entity_extract_ids($container_entity_type, $container_entity);
  $entity->container_entity_type = $container_entity_type;
  $entity->container_entity_id = $container_entity_id;

  $form_state = array(
    'entity' => $entity,
    'no_redirect' => TRUE,
    'op' => 'add',
    'args' => array(),
  ) + form_state_defaults();

  form_load_include($form_state, 'inc', 'cps_review', 'includes/forms');
  $output = drupal_build_form('cps_review_edit_form', $form_state);

  if (!empty($form_state['executed'])) {
    // Use the one on $form_state because form caching can change the object.
    $form_state['entity']->save();
    call_user_func_array('drupal_goto', $entity->permalink());
  }

  $build = array(
    '#type' => 'container',
    '#attributes' => array(
      'class' => array(
        'cps-review'
      ),
    ),
  );
  $build['reviews'] = array(
    '#type' => 'fieldset',
    '#title' => t('Reviews'),
    '#attributes' => array(
      'class' => array(
        'cps-review__reviews'
      ),
    ),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
    'reviews' => cps_review_list_reviews($container_entity_type, $container_entity_id),
  );
  $build['form'] = array(
    '#type' => 'fieldset',
    '#title' => t('Add review'),
    '#attributes' => array(
      'class' => array(
        'cps-review__form'
      ),
    ),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
    'form' => $output,
  );

  // Prevent the review form from being displayed.
  $changeset_id = cps_get_current_changeset();
  if ($changeset_id == CPS_PUBLISHED_CHANGESET) {
    $build['form']['#access'] = FALSE;
  }
  else {
    $changeset = cps_changeset_load($changeset_id);
    if (!empty($changeset->published)) {
      $build['form']['#access'] = FALSE;
    }
  }

  // Do not show the reviews part if there are no reviews.
  if (empty($build['reviews']['reviews'])) {
    hide($build['reviews']);
  }

  // Give other modules a chance to allow a different policy.
  drupal_alter('cps_review_widget', $build);

  return $build;
}

/**
 * List all reviews for this entity in order of creation.
 *
 * @param string $container_entity_type
 *   The entity type of the containing entity.
 * @param int $container_entity_id
 *   The entity ID this review is part of.
 *
 * @return array
 *   Render Array with all matching reviews.
 *
 */
function cps_review_list_reviews($container_entity_type, $container_entity_id) {
  $build = array();

  $changeset_id = cps_get_current_changeset();

  $entity_query = new EntityFieldQuery();
  $results = $entity_query
    ->entityCondition('entity_type', 'cps_review')
    ->propertyCondition('container_entity_type', $container_entity_type)
    ->propertyCondition('container_entity_id', $container_entity_id)
    ->propertyCondition('changeset_id', $changeset_id)
    ->propertyCondition('status', 1)
    ->propertyCondition('parent_review_id', 0)
    ->propertyOrderBy('created', 'ASC')
    ->execute();

  if (empty($results['cps_review'])) {
    return array();
  }

  $entities = entity_load('cps_review', array_keys($results['cps_review']));
  $build = entity_view('cps_review', $entities);

  return $build;
}

/**
 * Page callback to reply to an entity.
 *
 * @param CPSReview $review
 *   The review to reply to.
 *
 * @return array
 *   A drupal renderable array.
 */
function cps_review_reply_entity_page(CPSReview $review) {
  $entity = cps_review_create();

  $entity->container_entity_type = $review->container_entity_type;
  $entity->container_entity_id = $review->container_entity_id;
  $entity->parent_review_id = $review->review_id;

  drupal_set_title(t('Reply to a CPS review'));

  $form_state = array(
    'entity' => $entity,
    'no_redirect' => TRUE,
    'op' => 'add',
    'args' => array(),
  ) + form_state_defaults();

  form_load_include($form_state, 'inc', 'cps_review', 'includes/forms');
  $output = drupal_build_form('cps_review_edit_form', $form_state);

  if (!empty($form_state['executed'])) {
    // Use the one on $form_state because form caching can change the object.
    $form_state['entity']->save();
    call_user_func_array('drupal_goto', $entity->permalink());
  }

  return $output;
}

/**
 * Page callback to edit a entity.
 *
 * @param CPSReview $entity
 *   The entity to edit.
 *
 * @return array
 *   A Drupal renderable array.
 */
function cps_review_edit_page(CPSReview $entity) {
  $form_state = array(
    'entity' => $entity,
    'no_redirect' => TRUE,
    'op' => 'edit',
  ) + form_state_defaults();

  form_load_include($form_state, 'inc', 'cps_review', 'includes/forms');
  $output = drupal_build_form('cps_review_edit_form', $form_state);

  if (!empty($form_state['executed'])) {
    // Use the one on $form_state because form caching can actually change the object.
    /** @var CPSReview $entity */
    $entity = $form_state['entity'];
    $entity->save();
    call_user_func_array('drupal_goto', $entity->permalink());
  }

  return $output;
}

/**
 * Page callback to view a entity.
 *
 * @param CPSReview $entity
 *   The entity to view.
 * @param string $view_mode
 *   (optional) The view mode to render. Defaults to 'full'.
 *
 * @return string
 *   A rendered page.
 */
function cps_review_view_page(CPSReview $entity, $view_mode = 'full') {
  $build = $entity->view($view_mode, NULL, TRUE);
  return $build;
}

/**
 * Menu callback via drupal_get_form() -- ask for confirmation of entity deletion
 */
function cps_review_delete_form($form, &$form_state, CPSReview $entity) {
  $form_state['entity'] = $entity;
  $uri = $entity->permalink();
  return confirm_form($form,
    t('Are you sure you want to delete %title?', array('%title' => $entity->label())),
    $uri['path'],
    t('This action cannot be undone.'),
    t('Delete'),
    t('Cancel')
  );
}

/**
 * Form submit handler to execute entity deletion
 */
function cps_review_delete_form_submit($form, &$form_state) {
  if ($form_state['values']['confirm']) {
    /** @var CPSReview $entity */
    $entity = $form_state['entity'];
    $entity->delete();
    drupal_set_message(t('CPS review "%title" has been deleted.', array('%title' => $entity->label())));
  }

  $form_state['redirect'] = 'admin/structure/cps_reviews';
}

/**
 * Page callback to display the publish CPS review form.
 *
 * @param CPSReview $entity
 *
 * @return array
 *   A Drupal renderable array.
 */
function cps_review_publish_page(CPSReview $entity) {
  $form_state = array(
    'entity' => $entity,
  ) + form_state_defaults();

  form_load_include($form_state, 'inc', 'cps_review', 'includes/forms');
  $output = drupal_build_form('cps_review_publish_form', $form_state);
  return $output;
}

/**
 * Page callback to display the unpublish CPS review form.
 *
 * @param CPSReview $entity
 *
 * @return array
 *   A Drupal renderable array.
 */
function cps_review_unpublish_page(CPSReview $entity) {
  $form_state = array(
    'entity' => $entity,
  ) + form_state_defaults();

  form_load_include($form_state, 'inc', 'cps_review', 'includes/forms');
  $output = drupal_build_form('cps_review_unpublish_form', $form_state);
  return $output;
}
