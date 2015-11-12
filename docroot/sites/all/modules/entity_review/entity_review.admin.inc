<?php

/**
 * Callback function for displaying the individual project page
 */
function entity_review_view_review($entity) {
  return entity_view('review', array($entity));
}

/**
 * Generates the review action form.
 */
function review_action_form($form, &$form_state, $entity = NULL, $op = 'publish') {
  $review_uri = entity_uri('review', $entity);

  $form['vid'] = array(
    '#type' => 'value',
    '#value' => $entity->vid,
  );

  $form['action'] = array(
    '#type' => 'value',
    '#value' => $op,
  );

  // Make sure the form redirects in the end.
  $form['destination'] = array(
    '#type' => 'hidden',
    '#value' => $review_uri['path'],
  );

  $entity_view = entity_view('review', array($entity));

  return confirm_form(
    $form,
    t('Are you sure you want to !action the review?', array('!action' => $op)),
    $review_uri['path'],
    render($entity_view),
    ucfirst($op),
    t('Cancel')
  );
}

/**
 * Form API submit callback for the form.
 */
function review_action_form_submit(&$form, &$form_state) {
  $values = $form_state['values'];

  $review_revision = entity_revision_load('review', $values['vid']);

  switch ($values['action']) {
    case 'publish':
      $review_revision->publish();

      drupal_set_message(t('The review has been published.'));
      break;

    case 'unpublish':
      $review_revision->unpublish();

      drupal_set_message(t('The review has been unpublished.'));
      break;
  }

  $form_state['redirect'] = $values['destination'];
  cache_clear_all();
}

/**
 * Generates the review editing form.
 */
function review_form($form, &$form_state, $entity = NULL, $op = 'edit', $entity_type = NULL) {
  field_attach_form('review', $entity, $form, $form_state);

  if (isset($entity->id)) {
    $form['id'] = array('#type' => 'value', '#value' => $entity->id);
  }

  $form['uid'] = array('#type' => 'value', '#value' => $entity->uid);
  $form['eid'] = array('#type' => 'value', '#value' => $entity->eid);
  $form['type'] = array('#type' => 'value', '#value' => $entity->type);
  $form['bundle'] = array('#type' => 'value', '#value' => $entity->bundle);

  $form['#submit'][] = 'entity_review_add_form_submit';

  $form['actions'] = array(
    '#type' => 'actions',
    'submit' => array(
      '#type' => 'submit',
      '#value' => t('Save'),
    )
  );

  return $form;
}

/**
 * Generates the review adding form.
 */
function entity_review_add_form($form, &$form_state, $entity_type, $eid) {
  $review_entity = _entity_review_load_or_create($entity_type, $eid);

  return review_form($form, $form_state, $review_entity);
}

/**
 * Form API submit callback for the form.
 */
function entity_review_add_form_submit(&$form, &$form_state) {
  $form_state['entity_type'] = 'review';
  $form_state['review'] = $review = $form['#entity'];

  entity_form_submit_build_entity('review', $review, $form, $form_state);

  $review->save();

  drupal_set_message(t('Thanks! Your review will now be moderated and should be published soon.'));

  // Save and go back.
  $form_state['redirect'] = "review/$review->vid";
}

/**
 * Generates the review manage form.
 */
function entity_review_manage_form($form, &$form_state) {
  $entities_info = entity_get_info();

  $header = array(t('Bundle'), t('Operations'));

  foreach ($entities_info as $entity_type => $entity_info) {
    if ($entity_type != 'review') {
      $form[$entity_type] = array(
        '#type' => 'fieldset',
        '#title' => $entity_info['label'],
        '#collapsible' => TRUE,
        '#collapsed' => TRUE,
      );

      $rows = array();

      foreach ($entity_info['bundles'] as $bundle_name => $bundle) {
        $review_bundle_name = $entity_type . '__' . $bundle_name;

        $rows[] = array(
          $bundle['label'],
          theme('item_list', array('items' => array(
            l(t('Settings'), "admin/structure/reviews/$review_bundle_name/settings"),
            l(t('Manage fields'), "admin/structure/reviews/$review_bundle_name/fields"),
            l(t('Manage display'), "admin/structure/reviews/$review_bundle_name/display"),
          )))
        );
      }

      $form[$entity_type]['table'] = array(
        '#markup' => theme('table', array('header' => $header, 'rows' => $rows)),
      );
    }
  }

  return $form;
}

/**
 * Generates the review manage settings form.
 */
function entity_review_manage_settings_form($form, &$form_state, $bundle) {
  //@TODO: Add title setting, with tokens support.

  $form['entity_review_' . $bundle . '_enable'] = array(
    '#type' => 'checkbox',
    '#title' => t('Enable'),
    '#default_value' => variable_get('entity_review_' . $bundle . '_enable', FALSE),
  );

  $form['entity_review_' . $bundle . '_needs_approval'] = array(
    '#type' => 'checkbox',
    '#title' => t('Needs approval before publishing.'),
    '#default_value' => variable_get('entity_review_' . $bundle . '_needs_approval', FALSE),
  );

  return system_settings_form($form);
}

/**
 * Load or create new review entity.
 *
 * @param $entity_type
 * @param $eid
 *
 * @return object
 *  Review entity.
 */
function _entity_review_load_or_create($entity_type, $eid) {
  $entities = entity_load('review', FALSE, array(
    'type' => $entity_type,
    'eid' => $eid,
    'uid' => $GLOBALS['user']->uid,
  ));

  if (!empty($entities)) {
    return reset($entities);
  }
  else {
    $entity_wrapper = entity_metadata_wrapper($entity_type, $eid);

    return entity_create('review', array(
      'uid' => $GLOBALS['user']->uid,
      'eid' => $eid,
      'type' => $entity_type,
      'bundle' => $entity_type . '__' . $entity_wrapper->getBundle(),
    ));
  }
}
