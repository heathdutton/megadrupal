<?php

/**
 * @file
 * White label entity editing UI.
 */

/**
 * While UI controller.
 */
class WhileUIController extends EntityBundleableUIController {

  /**
   * Provides definitions for implementing hook_menu().
   */
  public function hook_menu() {
    $items = parent::hook_menu();

    // Extend the 'add' path.
    $items[$this->path . '/add']['title'] = 'Add ' . while_get_string('entity label');
    return $items;
  }

  /**
   * Operation form submit callback.
   */
  public function operationFormSubmit($form, &$form_state) {
    parent::operationFormSubmit($form, $form_state);
    $form_state['redirect'] = while_get_string('admin menu path content');
  }
}

/**
 * Generates the while editing form.
 */
function while_form($form, &$form_state, Entity $while, $op = 'edit') {

  // Needed by entity_form_field_validate().
  $form['type'] = array('#type' => 'value', '#value' => $while->type);

  $form['label'] = array(
    '#type' => 'textfield',
    '#title' => t('Label'),
    '#default_value' => isset($while->label) ? $while->label : '',
    '#description' => t('The label associated with the @while.', array('@while' => while_get_string('entity label'))),
    '#required' => TRUE,
    '#weight' => -15,
  );

  $form['created'] = array(
    '#type' => 'textfield',
    '#title' => t('Creation date'),
    '#default_value' => isset($while->created) ? format_date($while->created, 'short') : '',
    '#description' => t('The date when the @while was created.', array('@while' => while_get_string('entity label'))),
    '#required' => TRUE,
    '#weight' => -14,
  );

  $form['status'] = array(
    '#type' => 'checkbox',
    '#title' => t('Activate this @while', array('@while' => while_get_string('entity label'))),
    '#default_value' => !empty($while->status),
    '#weight' => -10,
  );

  $form['options']['#tree'] = TRUE;
  if ($while->type()->data['supports_revisions'] && empty($while->is_new)) {
    $form['options']['create_revision'] = array('#type' => 'checkbox',
      '#title' => t('Create a new revision'),
      '#default_value' => TRUE,
      '#description' => t('Create a new revision for this edit.'),
    );
  }

  $form['actions'] = array('#type' => 'actions');
  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save'),
    '#weight' => 40,
  );

  $form['actions']['delete'] = array(
    '#type' => 'submit',
    '#value' => t('Delete'),
    '#weight' => 45,
    '#limit_validation_errors' => array(),
    '#submit' => array('while_form_submit_delete'),
    '#access' => $op != 'add' && $op != 'clone'
  );

  field_attach_form('while', $while, $form, $form_state);

  return $form;
}

/**
 * while editing form validation callback.
 */
function while_form_validate(&$form, &$form_state) {
  entity_form_field_validate('while', $form, $form_state);
}

/**
 * Form API submit callback for the while form.
 */
function while_form_submit(&$form, &$form_state) {
  $date_format = variable_get('date_format_short', 'm/d/Y - H:i');
  $date = date_parse_from_format($date_format, $form_state['values']['created']);
  $form_state['values']['created'] = mktime($date['hour'], $date['minute'], $date['second'], $date['month'], $date['day'], $date['year']);
  $while = entity_ui_form_submit_build_entity($form, $form_state);
  // Save and go back.
  $while->save();
  $form_state['redirect'] = while_get_string('admin menu path content');
}

/**
 * Form API submit callback for the delete button.
 */
function while_form_submit_delete(&$form, &$form_state) {
  $form_state['redirect'] = 'admin/content/whiles/' . $form_state['while']->id . '/delete';
}