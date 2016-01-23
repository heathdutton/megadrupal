<?php

/**
 * Menu callback; present an administrative review listing.
 */
function entity_review_content($type = 'new') {
  $edit = $_POST;

  if (isset($edit['operation']) && ($edit['operation'] == 'delete') && isset($edit['reviews']) && $edit['reviews']) {
    return drupal_get_form('entity_review_multiple_delete_confirm');
  }
  else {
    return drupal_get_form('entity_review_content_overview', $type);
  }
}

/**
 * Form builder for the review overview administration form.
 *
 * @param $arg
 *   Current path's fourth component: the type of overview form
 *   ('approval' or 'new').
 *
 * @ingroup forms
 * @see entity_review_content_overview_validate()
 * @see entity_review_content_overview_submit()
 */
function entity_review_content_overview($form, &$form_state, $arg) {
  // Build an 'Update options' form.
  $form['options'] = array(
    '#type' => 'fieldset',
    '#title' => t('Update options'),
    '#attributes' => array('class' => array('container-inline')),
  );

  if ($arg == 'approval') {
    $options['publish'] = t('Publish the selected reviews');
  }
  else {
    $options['unpublish'] = t('Unpublish the selected reviews');
  }
  $options['delete'] = t('Delete the selected reviews');

  $form['options']['operation'] = array(
    '#type' => 'select',
    '#title' => t('Operation'),
    '#title_display' => 'invisible',
    '#options' => $options,
    '#default_value' => 'publish',
  );
  $form['options']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Update'),
  );

  $header = array(
    'eid' => array('data' => t('Posted in'), 'field' => 'eid'),
    'type' => array('data' => t('Entity type'), 'field' => 'type'),
    'bundle' => array('data' => t('Bundle')),
    'uid' => array('data' => t('User'), 'field' => 'uid'),
    'timestamp' => array('data' => t('Updated'), 'field' => 'timestamp', 'sort' => 'desc'),
    'created' => array('data' => t('Created'), 'field' => 'created', 'sort' => 'desc'),
    'operations' => array('data' => t('Operations')),
  );

  $query = db_select('entity_review', 'r')->extend('PagerDefault')->extend('TableSort');

  if ($arg == 'approval') { // Unpublished.
    $query->join('entity_review_revision', 'v', 'v.id = r.id AND (v.vid <> r.vid OR r.status = 0)');
  }
  else { // Published.
    $query->join('entity_review_revision', 'v', 'v.id = r.id AND v.vid = r.vid');
    $query->condition('r.status', 1);
  }

  $query->fields('v', array('vid'));

  $query->limit(50);
  $query->orderByHeader($header);

  // Build a table listing the appropriate reviews.
  $options = array();

  foreach ($query->execute()->fetchCol() as $vid) {
    $review = entity_revision_load('review', $vid);
    $review_uri = entity_uri('review', $review);

    $entities = entity_load($review->type, array($review->eid));
    $entity = $entities[$review->eid];
    $entity_wrapper = entity_metadata_wrapper($review->type, $entity);

    $entity_info = $entity_wrapper->entityInfo();
    $entity_uri = entity_uri($review->type, $entity);

    $options[$review->vid] = array(
      'eid' => array(
        'data' => array(
          '#type' => 'link',
          '#title' => $entity_wrapper->label(),
          '#href' => $entity_uri['path'],
        ),
      ),
      'type' => $entity_info['label'],
      'bundle' => $entity_wrapper->getBundle(),
      'uid' => theme('username', array('account' => user_load($review->uid))),
      'timestamp' => format_date($review->timestamp, 'short'),
      'created' => format_date($review->created, 'short'),
      'operations' => array(
        'data' => array(
          '#type' => 'link',
          '#title' => t('view'),
          '#href' => $review_uri['path'],
        ),
      ),
    );
  }

  $form['reviews'] = array(
    '#type' => 'tableselect',
    '#header' => $header,
    '#options' => $options,
    '#empty' => t('No reviews available.'),
  );

  $form['pager'] = array('#theme' => 'pager');

  return $form;
}

/**
 * Validate entity_review_content_overview form submissions.
 */
function entity_review_content_overview_validate($form, &$form_state) {
  $form_state['values']['reviews'] = array_diff($form_state['values']['reviews'], array(0));
  // We can't execute any 'Update options' if no reviews were selected.
  if (count($form_state['values']['reviews']) == 0) {
    form_set_error('', t('Select one or more reviews to perform the update on.'));
  }
}

/**
 * Process entity_review_content_overview form submissions.
 *
 * Execute the chosen 'Update option' on the selected reviews, such as
 * publishing, unpublishing or deleting.
 */
function entity_review_content_overview_submit($form, &$form_state) {
  $operation = $form_state['values']['operation'];
  $vids = $form_state['values']['reviews'];

  foreach ($vids as $vid) {
    $review_revision = entity_revision_load('review', $vid);

    switch ($operation) {
      case 'publish':
        $review_revision->publish();
        break;

      case 'unpublish':
        $review_revision->unpublish();
        break;
    }
  }

  drupal_set_message(t('The update has been performed.'));
  $form_state['redirect'] = 'admin/content/reviews';
  cache_clear_all();
}

/**
 * List the selected reviews and verify that the admin wants to delete them.
 *
 * @param $form_state
 *   An associative array containing the current state of the form.
 * @return
 *   TRUE if the reviews should be deleted, FALSE otherwise.
 * @ingroup forms
 * @see entity_review_multiple_delete_confirm_submit()
 */
function entity_review_multiple_delete_confirm($form, &$form_state) {
  $edit = $form_state['input'];

  $form['reviews'] = array(
    '#prefix' => '<ul>',
    '#suffix' => '</ul>',
    '#tree' => TRUE,
  );

  foreach ($edit['reviews'] as $vid) {
    $review = entity_revision_load('review', $vid);
    $entity_wrapper = entity_metadata_wrapper('review', $review);

    $form['reviews'][$review->vid] = array(
      '#type' => 'hidden',
      '#value' => $review->vid,
      '#prefix' => '<li>',
      '#suffix' => $entity_wrapper->label() . '</li>');
  }

  $form['operation'] = array('#type' => 'hidden', '#value' => 'delete');

  if (!count($edit['reviews'])) {
    drupal_set_message(t('There do not appear to be any reviews to delete, or your selected review was deleted by another administrator.'));
    drupal_goto('admin/content/reviews');
  }
  else {
    return confirm_form($form,
      t('Are you sure you want to delete these reviews?'),
      'admin/content/reviews', t('This action cannot be undone.'),
      t('Delete reviews'), t('Cancel'));
  }
}

/**
 * Process entity_review_multiple_delete_confirm form submissions.
 */
function entity_review_multiple_delete_confirm_submit($form, &$form_state) {
  if ($form_state['values']['confirm']) {
    foreach ($form_state['values']['reviews'] as $vid) {
      $review_revision = entity_revision_load('review', $vid);

      // If revision is current - unpublish first.
      if ($review_revision->isDefaultRevision()) {
        $review_revision->unpublish();
      }

      entity_revision_delete('review', $vid);
    }

    cache_clear_all();

    $count = count($form_state['values']['reviews']);

    watchdog('content', 'Deleted @count reviews.', array('@count' => $count));
    drupal_set_message(format_plural($count, 'Deleted 1 review.', 'Deleted @count reviews.'));
  }

  $form_state['redirect'] = 'admin/content/reviews';
}
