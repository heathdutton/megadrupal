<?php

/**
 * Provides a page to perform undelete operations.
 * This is primarily provided for integration with the views module.
 *
 * @param $form
 *   A form array
 * @param $form_state
 *   A form state
 * @param $node
 *   A node object
 *
 * @return
 *   The unsoft delete form.
 */
function entity_soft_delete_undelete_form($form, &$form_state, $node) {
  $function_history = array();
  cf_error_append_history($function_history, __FUNCTION__);

  if (!is_array($form)){
    cf_error_invalid_array($function_history, 'form');
    return cf_page_not_accessible($form_state);
  }

  if (!isset($form['#id'])) $form['#id'] = '';

  if (!is_array($form)){
    cf_error_invalid_array($function_history, 'form');
    return cf_page_not_accessible($form_state, $form['#id']);
  }

  if (cf_is_not_form_state($function_history, 'form_state', $form_state)){
    return cf_page_not_accessible($form_state, $form['#id']);
  }

  $entity_info = _entity_soft_delete_entity_info_from_form($form);

  if (!is_object($entity_info['entity'])){
    cf_error_invalid_object($function_history, 'entity');
    return cf_page_not_accessible($form_state, $form['#id']);
  }

  $entity_bundle_settings = entity_soft_delete_get_entity_bundle_settings(array('type' => $entity_info['entity_type'], 'bundle' => $entity_info['entity_bundle']));
  if ($entity_info['entity']->deleted === FALSE || $entity_bundle_settings['enabled']){
    return cf_page_not_accessible($form_state, $form['#id']);
  }
  if (property_exists($entity_info['entity'], 'title')) {
    $title = $entity_info['title'];
  } else {
    $title = $entity_info['entity_id'];
  }

  if ($entity_info['entity_type'] == 'node') {
    $form['#node'] = $node;

    // Always provide entity id in the same form key as in the entity edit form.
    $form['nid'] = array('#type' => 'value', '#value' => $entity_info['entity_id']);
  } else {
    $form['entity']['#value'] = $entity_info['entity'];

    // Always provide entity id in the same form key as in the entity edit form.
    //$form['nid'] = array('#type' => 'value', '#value' => $entity_info['entity_id']);
  }

  $form['#submit'] = array('entity_soft_delete_undelete_form_submit');

  return confirm_form(
    $form,
    t("Would you like to undelete %title?", array('%title' => $title)),
      $entity_info['view_path'],
    t("Would you like to undelete %title?", array('%title' => $title)),
    t("Yes"),
    t("No")
  );
}

/**
 * Perform the submit operation if the user selected "yes"
 *
 * @param $form
 *   A form array
 * @param $form_state
 *   A form state
 */
function entity_soft_delete_undelete_form_submit($form, &$form_state) {
  $function_history = array();
  cf_error_append_history($function_history, __FUNCTION__);

  if (!is_array($form)){
    cf_error_invalid_array($function_history, 'form');
    return;
  }

  if (cf_is_not_form_state($function_history, 'form_state', $form_state)){
    return;
  }

  if (isset($form_state['values']['confirm'])) {
    $entity_info = _entity_soft_delete_entity_info_from_form($form);

    $result = entity_soft_delete_operate_on_entities('undelete', array($entity_info['entity']), $entity_info['entity_type'], NULL, $function_history);

    if ($result){
      drupal_set_message(t("The content has been undeleted."));
    }

    if (!isset($form_state['redirect'])){
      $form_state['redirect'] = $entity_info['view_path'];
    }
  }

  drupal_redirect_form($form_state);
}
