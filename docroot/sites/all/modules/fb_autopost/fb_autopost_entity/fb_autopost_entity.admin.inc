<?php

/**
 * @file
 * Facebook publication type editing UI.
 */

/**
 * UI controller.
 */
class FacebookPublicationTypeUIController extends EntityDefaultUIController {

  /**
   * Overrides hook_menu() defaults.
   */
  public function hook_menu() {
    $items = parent::hook_menu();
    $items[$this->path]['description'] = 'Manage Facebook publications, including fields.';
    return $items;
  }
}

/**
 * Generates the Facebook publication type editing form.
 */
function facebook_publication_type_form($form, &$form_state, $facebook_publication_type, $op = 'edit') {

  if ($op == 'clone') {
    $facebook_publication_type->label .= ' (cloned)';
    $facebook_publication_type->type = '';
  }

  $form['label'] = array(
    '#title' => t('Label'),
    '#type' => 'textfield',
    '#default_value' => $facebook_publication_type->label,
    '#description' => t('The human-readable name of this facebook publication type.'),
    '#required' => TRUE,
    '#size' => 30,
  );
  // Machine-readable type name.
  $form['type'] = array(
    '#type' => 'machine_name',
    '#default_value' => isset($facebook_publication_type->type) ? $facebook_publication_type->type : '',
    '#maxlength' => 32,
    '#disabled' => $facebook_publication_type->isLocked() && $op != 'clone',
    '#machine_name' => array(
      'exists' => 'facebook_publication_get_types',
      'source' => array('label'),
    ),
    '#description' => t('A unique machine-readable name for this Facebook publication type. It must only contain lowercase letters, numbers, and underscores.'),
  );

  $form['description'] = array(
    '#type' => 'textarea',
    '#title' => t('Description'),
    '#description' => t('Write a short description about this Facebook publication type.'),
  );

  $form['actions'] = array('#type' => 'actions');
  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save Facebook publication type'),
    '#weight' => 40,
  );

  if (!$facebook_publication_type->isLocked() && $op != 'add' && $op != 'clone') {
    $form['actions']['delete'] = array(
      '#type' => 'submit',
      '#value' => t('Delete Facebook publication type'),
      '#weight' => 45,
      '#limit_validation_errors' => array(),
      '#submit' => array('facebook_publication_type_form_submit_delete'),
    );
  }
  return $form;
}

/**
 * Form API submit callback for the type form.
 */
function facebook_publication_type_form_submit(&$form, &$form_state) {
  $facebook_publication_type = entity_ui_form_submit_build_entity($form, $form_state);
  // Save and go back.
  $facebook_publication_type->save();
  $form_state['redirect'] = 'admin/structure/facebook-publications';
}

/**
 * Form API submit callback for the delete button.
 */
function facebook_publication_type_form_submit_delete(&$form, &$form_state) {
  $form_state['redirect'] = 'admin/structure/facebook-publications/manage/' . $form_state['facebook_publication_type_form_submit_delete_type']->type . '/delete';
}
