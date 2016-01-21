<?php

function commerce_option_set_form($form, &$form_state, $option_set, $op = 'edit') {



  if ($op == 'clone') {
    $option_set->name .= ' (cloned)';
    $option_set->set_id = '';
  }

  $form['name'] = array(
    '#title' => t('Name'),
    '#type' => 'textfield',
    '#default_value' => $option_set->name,
    '#description' => t('The human-readable name of this option set.'),
    '#required' => TRUE,
    '#size' => 30,
  );
  
  // Machine-readable type name.
  $form['set_id'] = array(
    '#type' => 'machine_name',
    '#default_value' => isset($option_set->set_id) ? $option_set->set_id : '',
    '#maxlength' => 32,
    '#disabled' => $option_set->isLocked() && $op != 'clone',
    '#machine_name' => array(
      'exists' => 'commerce_option_get_sets',
      'source' => array('name'),
    ),
    '#description' => t('A unique machine-readable name for this option set. It must only contain lowercase letters, numbers, and underscores.'),
  );
  
  $form['description'] = array(
    '#type' => 'textarea',
    '#default_value' => $option_set->description,
    '#title' => t('Description'),
    '#description' => t('This description is shown to the admin user to indicate if this option set is useful for the current purpose.'),
  );
  

  $form['data']['#tree'] = TRUE;
  
  $form['actions'] = array('#type' => 'actions');
  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save option set'),
    '#weight' => 40,
  );
  

  if (!$option_set->isLocked() && $op != 'add') {
    $form['actions']['delete'] = array(
      '#type' => 'submit',
      '#value' => t('Delete option set'),
      '#weight' => 45,
      '#limit_validation_errors' => array(),
      '#submit' => array('commerce_option_set_form_submit_delete')
    );
  }
  return $form;


}




/**
 * Form submit handler for the option set
 */
function commerce_option_set_form_submit($form, &$form_state) {
  $option_set = entity_ui_form_submit_build_entity($form, $form_state);
  
  
  commerce_option_set_save($option_set);
  
  drupal_set_message(t('Option set saved.'));

  // Redirect based on the button clicked.
  if ($form_state['clicked_button']['#parents'][0] == 'save_continue') {
    $form_state['redirect'] = 'admin/commerce/products/option-sets/' . strtr($option_set['set_id'], '_', '-') . '/fields';
  }
  else {
    $form_state['redirect'] = 'admin/commerce/products/option-sets';
  }
}




function commerce_option_set_form_delete_submit($form, &$form_state) {
  $form_state['redirect'] = 'admin/commerce/config/option-sets/' . $form_state['option_set']->set_id . '/delete';
}



/**
 * Form callback: confirmation form for deleting a option set.
 *
 * @param $option_set
 *   The option set array to be deleted.
 *
 * @see confirm_form()
 */
function commerce_option_set_delete_form($form, &$form_state, $option_set) {
  $form_state['option_set'] = $option_set;

  // Ensure this include file is loaded when the form is rebuilt from the cache.
  $form_state['build_info']['files']['form'] = drupal_get_path('module', 'commerce_product_ui') . '/includes/commerce_product_ui.forms.inc';

  $form['#submit'][] = 'commerce_option_set_delete_form_submit';

  $form = confirm_form($form,
    t('Are you sure you want to delete the %name option set?', array('%name' => $option_set['name'])),
    'admin/commerce/config/option-sets',
    '<p>' . t('This action cannot be undone.') . '</p>',
    t('Delete'),
    t('Cancel'),
    'confirm'
  );

  return $form;
}

/**
 * Submit callback for commerce_option_set_delete_form().
 */
function commerce_option_set_delete_form_submit($form, &$form_state) {
  $option_set = $form_state['option_set'];

  commerce_option_set_delete($option_set['set_id']);

  drupal_set_message(t('The option set %name has been deleted.', array('%name' => $option_set['name'])));
  watchdog('commerce_option', 'Deleted option set %name.', array('%name' => $option_set['name']), WATCHDOG_NOTICE);

  $form_state['redirect'] = 'commerce_option_set_delete';
}

















