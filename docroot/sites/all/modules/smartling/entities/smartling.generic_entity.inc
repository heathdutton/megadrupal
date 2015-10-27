<?php

/**
 * @file
 * Functions for entity.
 */
function smartling_language_neutral_form_element() {
  $form['smartling'] = array(
    '#title' => t('Smartling management'),
    '#type' => 'fieldset',
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
    '#weight' => 100,
    '#group' => 'additional_settings',
    '#attributes' => array('id' => array('smartling_fieldset')),
    '#attached' => array(
      'css' => array(
        drupal_get_path('module', 'smartling') . '/css/smartling_entity_settings.css' => array(
          'type' => 'file',
        ),
      ),
    ),
    '#modal' => TRUE,
  );

  $form['smartling']['content'] = array(
    '#type' => 'container',
  );

  $form['smartling']['content']['lang'] = array(
    '#type' => 'item',
    '#title' => '',
    '#markup' => t('This entity is in the Neutral Language so it can\'t be translated right now. Please convert it into some language (for example english) in order to turn on such possibility.'),
  );
  return $form;
}
/**
 * Implements hook_form_alter().
 */
function smartling_form_alter(&$form, $form_state, $form_id) {
  //print $form_id;
  if (!in_array($form_id, array('fieldable_panels_panes_entity_edit_form', ))) {
    return;
  }

  if (!isset($form['#entity'])) {
    return;
  }

  $entity_type = $form['#entity_type'];
  $bundle = $form['#bundle'];

  if (!smartling_translate_fields_configured($bundle, $entity_type)) {
    drupal_set_message(t('Please follow the page <a href="@link">"Smartling settings"</a> to setup Smartling configuration.', array('@link' => url('admin/config/regional/smartling'))), 'warning');
  }
  $entity = $form['#entity'];
  $wp = entity_metadata_wrapper($entity_type, $entity);
  $id = $wp->getIdentifier();
  if (!empty($id)) {
    if (($entity->language != LANGUAGE_NONE)) {
      $form = array_merge($form, smartling_get_entity_settings_form($form, $form_state));
    }
    else {
      $frm = smartling_language_neutral_form_element();
      $form = array_merge($form, $frm);
    }
  }
}

/**
 * Entity smartling form.
 *
 * @param array $form
 *   FAPI array.
 * @param array $form_state
 *   FAPI array.
 *
 * @return array
 *   Return FAPI array for Entity smartling form.
 */
function smartling_get_entity_settings_form(array $form, array &$form_state) {
  return drupal_container()->get('smartling.forms.generic_entity_settings')->buildForm($form, $form_state);
}

/**
 * Entity smartling form - Form Submit.
 *
 * @param array $form
 *   FAPI array.
 * @param array $form_state
 *   FAPI array.
 */
function smartling_get_entity_settings_form_submit(array $form, array &$form_state) {
  drupal_container()->get('smartling.forms.generic_entity_settings')->submitForm($form, $form_state);
  unset($_GET['destination']);
}
