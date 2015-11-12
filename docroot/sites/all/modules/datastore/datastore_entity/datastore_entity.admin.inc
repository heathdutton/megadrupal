<?php

/**
 * @file
 * Provides datastore domain admin ui.
 */

/**
 * UI controller.
 */
class DataStoreDomainUIController extends EntityDefaultUIController {

  /**
   * Implements hook_menu().
   */
  public function hook_menu() {
    $items = parent::hook_menu();
    $items[$this->path]['description'] = 'Manage datastore domains.';
    return $items;
  }
}

/**
 * Generates the datastore domain editing form.
 */
function datastore_domain_form($form, &$form_state, $datastore_domain, $op = 'edit') {

  if ($op == 'clone') {
    $datastore_domain->label .= ' (cloned)';
    $datastore_domain->domain .= '_clone';
  }

  $form['label'] = array(
    '#title' => t('Label'),
    '#type' => 'textfield',
    '#default_value' => isset($datastore_domain->label) ? $datastore_domain->label : '',
    '#description' => t('The human-readable name of this domain.'),
    '#required' => TRUE,
    '#size' => 30,
  );

  // Machine-readable type name.
  $form['domain'] = array(
    '#type' => 'machine_name',
    '#default_value' => isset($datastore_domain->domain) ? $datastore_domain->domain : '',
    '#maxlength' => 16, // limit to 16 chars, to provide another 16 charts for extra field namespace
    '#machine_name' => array(
      'exists' => 'datastore_entity_get_domains',
      'source' => array('label'),
    ),
    '#description' => t('A unique machine-readable name for this datastore domain. It must only contain lowercase letters, numbers, and underscores.'),
  );

  $form['actions'] = array('#type' => 'actions');
  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save datastore domain'),
    '#weight' => 40,
  );

  $form['actions']['delete'] = array(
    '#type' => 'submit',
    '#value' => t('Delete domain type'),
    '#weight' => 45,
    '#limit_validation_errors' => array(),
    '#submit' => array('datastore_domain_form_submit_delete')
  );

  return $form;
}

/**
 * Form API submit callback for the type form.
 */
function datastore_domain_form_submit(&$form, &$form_state) {
  $datastore_domain = entity_ui_form_submit_build_entity($form, $form_state);
  // Save and go back.
  $datastore_domain->save();

  drupal_set_message(t('Stored datastore domain @label.', array('@label' => $datastore_domain->label)));
  $form_state['redirect'] = 'admin/structure/datastore_domains';
}

/**
 * Form API submit callback for the delete button.
 */
function datastore_domain_form_submit_delete(&$form, &$form_state) {
  $form_state['redirect'] = 'admin/structure/datastore_domains/manage/' . $form_state['datastore_domain']->domain . '/delete';
}