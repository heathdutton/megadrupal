<?php

/**
 * Page callback to configure the ECK Clone settings.
 */
function eck_clone_settings_form($form, &$form_state) {
  $form['basic'] = array(
    '#type' => 'fieldset',
    '#title' => t('General settings'),
  );
  $form['basic']['eck_clone_method'] = array(
    '#type' => 'radios',
    '#title' => t('Method to use when cloning an ECK Entity'),
    '#options' => array(
      ECK_CLONE_PREPOPULATE => t('Pre-populate the entity form fields'),
      ECK_CLONE_SAVEEDIT => t('Save as a new entity then edit')
    ),
    '#default_value' => variable_get('eck_clone_method', ECK_CLONE_PREPOPULATE),
  );

  // Flag to set what page to use for cloning entities.
  $form['basic']['eck_clone_admin_edit_form'] = array(
    '#type' => 'checkbox',
    '#title' => t('Use administration forms to edit the entities.'),
    '#default_value' => variable_get('eck_clone_admin_edit_form', FALSE),
  );

  $form_state['eck_clone_admin_edit_form'] = variable_get('eck_clone_admin_edit_form', FALSE);

  // @todo - Configuration for what entities to clone.

  $form = system_settings_form($form);

  // Hook our own submit after the default submit.
  $form['#submit'][] = 'eck_clone_settings_form_submit';

  return $form;
}

/**
 * Submit of settings form.
 */
function eck_clone_settings_form_submit($form, &$form_state) {
  // Rebuild the menu in case flag was changed.
  if ($form_state['eck_clone_admin_edit_form'] != $form_state['values']['eck_clone_admin_edit_form']) {
    menu_rebuild();
  }
}

