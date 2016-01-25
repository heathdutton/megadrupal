<?php

/**
 * @file
 * Commerce Marketplace Withdraw entity types UI controllers.
 */

/**
 * UI controller for cmp_funds_withdraw_method_type.
 */
class CommerceMarketplaceFundsWithdrawMethodTypeUIController extends EntityDefaultUIController {
  /**
   * Overrides hook_menu() defaults.
   */
  public function hook_menu() {
    $items = parent::hook_menu();
    $items[$this->path]['description'] = 'Manage withdraw method entity types, including adding
      and removing fields and the display of fields.';
    return $items;
  }
}

/**
 * Generates the withdraw method type editing form.
 */
function cmp_funds_withdraw_method_type_form($form, &$form_state, $method_type, $op = 'edit') {

  if ($op == 'clone') {
    $method_type->label .= ' (cloned)';
    $method_type->type = '';
  }

  $form['label'] = array(
    '#title' => t('Label'),
    '#type' => 'textfield',
    '#default_value' => $method_type->label,
    '#description' => t('The human-readable name of this model type.'),
    '#required' => TRUE,
    '#size' => 30,
  );
  // Machine-readable type name.
  $form['type'] = array(
    '#type' => 'machine_name',
    '#default_value' => isset($method_type->type) ? $method_type->type : '',
    '#maxlength' => 32,
    '#machine_name' => array(
      'exists' => 'commerce_marketplace_funds_withdraw_get_types',
      'source' => array('label'),
    ),
    '#description' => t('A unique machine-readable name for this model type. It must only contain lowercase letters, numbers, and underscores.'),
  );

  $form['actions'] = array('#type' => 'actions');
  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save withdraw method type'),
    '#weight' => 40,
  );

  return $form;
}

/**
 * Form API submit callback for the type form.
 */
function cmp_funds_withdraw_method_type_form_submit(&$form, &$form_state) {
  $method_type = entity_ui_form_submit_build_entity($form, $form_state);
  $method_type->save();
  $form_state['redirect'] = 'admin/commerce/marketplace_funds/withdraw';
}

/**
 * Form API submit callback for the delete button.
 */
function cmp_funds_withdraw_method_type_form_submit_delete(&$form, &$form_state) {
  $form_state['redirect'] = 'admin/commerce/marketplace_funds/withdraw/manage/' . $form_state['cmp_funds_withdraw_method_type']->type . '/delete';
}


