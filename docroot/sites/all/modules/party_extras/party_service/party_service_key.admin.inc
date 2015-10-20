<?php
/**
 * @file
 * Contains PartyServiceKeyUIController
 */

class PartyServiceKeyUIController extends EntityDefaultUIController {

  /**
   * {@inheritdoc}
   */
  public function hook_menu() {
    $items = array();
    $id_count = count(explode('/', $this->path));
    $service_count = 5;
    $wildcard = isset($this->entityInfo['admin ui']['menu wildcard']) ? $this->entityInfo['admin ui']['menu wildcard'] : '%entity_object';

    $items[$this->path . '/add'] = array(
      'title' => t('Add Key'),
      'page callback' => 'drupal_get_form',
      'page arguments' => array('party_service_key_add_form', $service_count),
      'access callback' => 'entity_access',
      'access arguments' => array('create', $this->entityType),
      'type' => MENU_LOCAL_ACTION,
      'file' => 'party_service_key.admin.inc',
      'file path' => drupal_get_path('module', 'party_service'),
    );

    return $items;
  }
}

/**
 * Form Callback For Adding a Key
 */
function party_service_key_add_form($form, &$form_state, $service) {
  $key = entity_create('party_service_key', array('service' => $service->name));

  $form_state['party_service'] = $form['#party_service'] = $service;
  $form_state['party_service_key'] = $form['#party_service_key'] = $key;

  $form['label'] = array(
    '#title' => t('Label'),
    '#type' => 'textfield',
    '#default_value' => !empty($key->label) ? $key->label : '',
    '#description' => t('The human-readable name of this key.'),
    '#required' => TRUE,
    '#weight' => -6,
  );

  // Add the default field elements.
  $form['apikey'] = array(
    '#type' => 'textfield',
    '#title' => t('API Key'),
    '#description' => t('Unique API Key'),
    '#default_value' => isset($key->apikey) ? $key->apikey : '',
    '#required' => TRUE,
    '#weight' => -5,
  );

  $form['owner'] = array(
    '#type' => 'textfield',
    '#title' => t('Owner'),
    '#description' => t('The PID of the Party that owns this key.'),
    '#required' => TRUE,
    '#weight' => -4,
  );

  $form['from_domain'] = array(
    '#type' => 'textfield',
    '#title' => t('Domain'),
    '#description' => t('The domain this API Key can be used from.'),
    '#weight' => -4,
  );

  $form['status'] = array(
    '#type' => 'checkbox',
    '#title' => t('Enabled'),
  );

  // Add the field related form elements.
  field_attach_form('party_service_key', $key, $form, $form_state);

  $form['actions'] = array(
    '#type' => 'container',
    '#attributes' => array('class' => array('form-actions')),
    '#weight' => 400,
  );

  // We add the form's #submit array to this button along with the actual submit
  // handler to preserve any submit handlers added by a form callback_wrapper.
  $submit = array();
  if (!empty($form['#submit'])) {
    $submit += $form['#submit'];
  }

  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save'),
    '#submit' => $submit + array('party_service_key_form_submit'),
  );

  // We append the validate handler to #validate in case a form callback_wrapper
  // is used to add validate handlers earlier.
  $form['#validate'][] = 'party_service_key_form_validate';
  return $form;
}


/**
 * Form API validate callback for the carkit_key form
 */
function party_service_key_form_validate(&$form, &$form_state) {
  $key = $form_state['party_service_key'];

  // Notify field widgets to validate their data.
  field_attach_form_validate('party_service_key', $key, $form, $form_state);
}


/**
 * Form API submit callback for the carkit_key form.
 *
 * @todo remove hard-coded link
 */
function party_service_key_form_submit(&$form, &$form_state) {
  $key = entity_ui_controller('party_service_key')->entityFormSubmitBuildEntity($form, $form_state);
  $key->save();
  $form_state['redirect'] = "admin/config/services/party_services/manage/{$form['#party_service']->name}/keys";
}
