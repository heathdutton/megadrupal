<?php
/**
 * @file
 * Administrative functionality for the Emediate module
 */

/**
 * Form for general Emediate settings.
 */
function emediate_settings_form() {
  $form = array();

  $form['general'] = array(
    '#type'  => 'fieldset',
    '#title' => t('General settings'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
  );

  $form['general']['emediate_enabled'] = array(
    '#title' => 'Enable Emediate content units.',
    '#description' => 'Enabling this will output banners from the Emediate system.',
    '#type' => 'checkbox',
    '#default_value' => variable_get('emediate_enabled', EMEDIATE_ENABLED),
  );
  $form['general']['emediate_external_js'] = array(
    '#title' => 'Path to external EAS script.',
    '#description' => 'If blank, emediate/js/EAS_tag.1.0.js from this webserver will be used.',
    '#type' => 'textfield',
    '#default_value' => variable_get('emediate_external_js', ''),
  );

  return system_settings_form($form);
}

/**
 * Form for administering content units.
 */
function emediate_content_units_form() {
  $build = array();

  $rows = array();
  $header = array(t('Name'), t('Content unit ID'), t('Label'), t('Operations'));
  $content_units = emediate_load_all();
  foreach ($content_units as $content_unit) {
    $rows[] = array(
      'name' => $content_unit->name,
      'cuid' => $content_unit->cuid,
      'label' => $content_unit->label,
      'operation' => l(t('delete'), "admin/config/system/emediate/content-unit/delete/$content_unit->name"),
    );
  }

  $form = array();
  $form['name'] = array(
    '#title' => t('Name'),
    '#type' => 'textfield',
    '#size' => 48,
    '#maxlength' => 40,
    '#default_value' => '',
    '#description' => t('Machine name of the Emediate content unit (unique).'),
    '#required' => TRUE,
    '#weight' => 10,
  );
  $form['cuid'] = array(
    '#title' => t('Content unit ID'),
    '#type' => 'textfield',
    '#size' => 48,
    '#maxlength' => 40,
    '#default_value' => '',
    '#description' => t('Enter an Emediate content unit ID.'),
    '#required' => TRUE,
    '#weight' => 20,
  );
  $form['label'] = array(
    '#title' => t('Content unit label'),
    '#type' => 'textfield',
    '#size' => 48,
    '#maxlength' => 40,
    '#default_value' => '',
    '#description' => t('Enter the label of the Emediate content unit.'),
    '#required' => TRUE,
    '#weight' => 30,
  );
  $form['actions'] = array(
    '#type' => 'actions',
    '#weight' => 50,
  );
  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Add'),
    '#validate' => array('emediate_content_units_form_validate_add'),
    '#submit' => array('emediate_content_units_form_submit_add'),
  );


  $build['emediate_content_units_form'] = $form;
  $build['emediate_content_units_table'] = array(
    '#theme' => 'table',
    '#header' => $header,
    '#rows' => $rows,
    '#weight' => 100,
  );

  return $build;
}

/**
 * Validate handler for adding new content units.
 */
function emediate_content_units_form_validate_add($form, &$form_state) {
  $cuid = $form_state['values']['cuid'];
  if (!is_numeric($cuid)) {
    form_set_error('cuid', t('Content unit ID must be an integer'));
  }

  $name = $form_state['values']['name'];
  if (emediate_load($name)) {
    form_set_error('name', t('Content unit name must be unique'));
  }
}

/**
 * Submit handler for adding new content units.
 */
function emediate_content_units_form_submit_add($form, &$form_state) {
  $content_unit = new stdClass();
  $content_unit->name = $form_state['values']['name'];
  $content_unit->cuid = $form_state['values']['cuid'];
  $content_unit->label = $form_state['values']['label'];
  emediate_save($content_unit);
}

/**
 * Content unit deletion confirm page.
 */
function emediate_content_unit_delete($form, &$form_state, $content_unit) {
  $form['content_unit'] = array(
    '#type' => 'value',
    '#value' => $content_unit,
  );
  return confirm_form($form, t('Are you sure you want to delete %name (%cuid)?', array('%cuid' => $content_unit->cuid, '%name' => $content_unit->name)), 'admin/config/system/emediate/content-unit', t('This action cannot be undone.'), t('Delete'), t('Cancel'));
}

/**
 * Process content unit delete form submissions.
 */
function emediate_content_unit_delete_submit($form, &$form_state) {
  $content_unit = $form_state['values']['content_unit'];
  emediate_delete($content_unit);
  drupal_set_message(t('The Emediate content unit %name (%cuid) was deleted.', array('%cuid' => $content_unit->cuid, '%name' => $content_unit->name)));
  $form_state['redirect'] = 'admin/config/system/emediate/content-unit';
}
