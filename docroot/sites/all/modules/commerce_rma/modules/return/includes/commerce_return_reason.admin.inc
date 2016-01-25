<?php
/**
 * @file
 * Commerce return reason admin UI class.
 */

/**
 * UI controller.
 */
class CommerceReturnReasonUIController extends EntityDefaultUIController {
  /**
   * Overridden EntityDefaultUIController::hook_menu().
   */
  public function hook_menu() {
    // Get parent menu items.
    $items = parent::hook_menu();

    // We don't need those operations.
    unset($items[$this->path . '/clone']);
    unset($items[$this->path . '/list']);
    unset($items[$this->path . '/add']);

    $items[$this->path . '/add/product'] = array(
      'title' => 'Add return reason filtered by products',
      'page callback' => 'entity_ui_get_form',
      'page arguments' => array(
        $this->entityType,
        commerce_return_reason_new('product'),
        'add'
      ),
      'access callback' => 'entity_access',
      'access arguments' => array('create', $this->entityType),
      'type' => MENU_LOCAL_ACTION,
    );

    $items[$this->path . '/add/product-type'] = array(
      'title' => 'Add return reason filtered by product types',
      'page callback' => 'entity_ui_get_form',
      'page arguments' => array(
        $this->entityType,
        commerce_return_reason_new('product_type'),
        'add'
      ),
      'access callback' => 'entity_access',
      'access arguments' => array('create', $this->entityType),
      'type' => MENU_LOCAL_ACTION,
    );

    // The CommerceReturnReason default path is added as a tab on the return
    // admin page.
    $items[$this->path]['type'] = MENU_LOCAL_TASK;
    $items[$this->path]['title'] = 'Customer reasons';

    return $items;
  }
}

/**
 * @param $form
 *   The form array.
 * @param $form_state
 *   The form state array.
 * @param CommerceReturnReason $return_reason
 *   A fully loaded CommerceReturnReason object.
 * @param string $op
 *   The operation to proceed.
 *
 * @return array
 *   The form array to be rendered.
 */
function commerce_return_reason_form($form, &$form_state, CommerceReturnReason $return_reason, $op = 'edit') {
  $form = array();

  $return_reason_wrapper = entity_metadata_wrapper('commerce_return_reason', $return_reason);

  $form['label'] = array(
    '#type' => 'textfield',
    '#title' => t('Label'),
    '#description' => t('The return reasons label.'),
    '#length' => 60,
    '#weight' => -10,
    '#default_value' => !empty($return_reason->label) ? $return_reason->label : '',
    '#required' => TRUE,
  );

  $form['name'] = array(
    '#type' => 'machine_name',
    '#default_value' => !empty($return_reason->name) ? $return_reason->name : '',
    '#maxlength' => 128,
    '#weight' => -9,
    '#machine_name' => array(
      'exists' => '_commerce_return_reason_name_exists',
      'source' => array('label'),
    ),
  );

  $form['description'] = array(
    '#type' => 'textarea',
    '#title' => t('Description'),
    '#description' => t('A short description for return reasons.'),
    '#weight' => -8,
    '#default_value' => !empty($return_reason->description) ? $return_reason->description : '',
  );

  // Call the widgets from the fields attached on the bundle.
  $form_state['commerce_return_reason'] = $return_reason;
  field_attach_form('commerce_return_reason', $return_reason, $form, $form_state);

  if (isset($form['return_reason_product_type'])) {
    $form['return_reason_product_type'][$form['return_reason_product_type']['#language']] = array_replace(
      $form['return_reason_product_type'][$form['return_reason_product_type']['#language']],
      array(
        '#multiple' => TRUE,
        '#options' => commerce_product_type_options_list(),
        '#size' => 15,
        '#default_value' => $return_reason_wrapper->return_reason_product_type->value(),
      )
    );
  }

  $form['return_reason_taxonomy_term'][$form['return_reason_taxonomy_term']['#language']] = array_replace(
    $form['return_reason_taxonomy_term'][$form['return_reason_taxonomy_term']['#language']],
    array(
      '#multiple' => TRUE,
      '#options' => commerce_return_reasons_options_list(),
      '#size' => 15,
      '#description' =>
        t('Pick up the customer reason to be displayed regarding the selected conditions above.') .
        l(t(' Add a new term.'), 'admin/structure/taxonomy/return_reason', array('absolute' => TRUE)),
    )
  );

  // Attach the action buttons.
  $form['action'] = array(
    '#type' => 'actions',
    'submit' => array(
      '#type' => 'submit',
      '#value' => t('@operation customer reasons', array('@operation' => ucfirst($op))),
      '#weight' => 40,
    ),
    'delete' => array(
      '#type' => 'submit',
      '#value' => t('remove customer reasons'),
      '#weight' => 45,
      '#submit' => array('commerce_return_reason_form_submit_delete')
    )
  );

  return $form;
}

/**
 * Form API validation callback for the type form.
 *
 * @param array $form
 *   The form array.
 * @param array $form_state
 *   The reference to the form state array.
 */
function commerce_return_reason_form_validate($form, &$form_state) {
  field_attach_form_validate('commerce_return_reason', $form_state['commerce_return_reason'], $form, $form_state);

  if (!empty($form_state['values']['name'])) {
    form_set_value($form['name'], $form_state['values']['name'], $form_state);
  }
}

/**
 * Form API submit callback for the type form.
 *
 * @param array $form
 *   The reference to the form array.
 * @param array $form_state
 *   The reference to the form state array.
 */
function commerce_return_reason_form_submit(&$form, &$form_state) {
  $commerce_return_reason = entity_ui_form_submit_build_entity($form, $form_state);
  $commerce_return_reason->save();
  $form_state['redirect'] = 'admin/commerce/store/returns/reasons';
}


/**
 * Checks if a Return Reason machine name is taken.
 *
 * @param string $value
 *   The machine name.
 *
 * @return bool
 *   Whether or not the field machine name is taken.
 */
function _commerce_return_reason_name_exists($value) {
  return entity_load_single('commerce_return_reason', $value);
}