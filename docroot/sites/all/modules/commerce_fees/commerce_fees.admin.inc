<?php

/**
 * @file
 * Contains the administrative page and form callbacks for
 * the Commerce Fees module.
 */

/**
 * Displays all fee types defined in a table.
 */
function commerce_fees_overview() {
  drupal_add_css(drupal_get_path('module', 'commerce_fees') . '/theme/commerce_fees.admin.css');

  $header = array(
    t('Title'),
    t('Operations'),
  );

  $rows = array();

  $fee_types = commerce_fees_types();
  uasort($fee_types, 'drupal_sort_weight');

  // Loop through all of the items to include in the overview.
  foreach ($fee_types as $name => $item) {
    // Build the operation links for the current item.
    $arg = strtr($name, '_', '-');

    $links = menu_contextual_links('commerce-fees', 'admin/commerce/config/fees', array($arg));

    // Add the item's row to the table's rows array.
    $rows[] = array(
      theme('commerce_fees_admin_overview', array('fee_type' => $item)),
      theme('links', array('links' => $links, 'attributes' => array('class' => 'links inline operations'))),
    );
  }

  // If no items are defined...
  if (empty($rows)) {
    // Add a standard empty row with a link to add a new item.
    $empty_text = t('There are no fee types enabled.');

    $rows[] = array(
      array(
        'data' => $empty_text,
        'colspan' => 2,
      ),
    );
  }

  return theme('table', array('header' => $header, 'rows' => $rows));
}

/**
 * Builds an overview of a fee type for display to an administrator.
 *
 * @param array $variables
 *   An array of variables used to generate the display; by default includes the
 *   fee_type key with a value of the fee type info array.
 *
 * @ingroup themeable
 */
function theme_commerce_fees_admin_overview($variables) {
  $fee_type = $variables['fee_type'];

  // Build the actual output.
  $output = check_plain($fee_type['title']);
  $output .= ' <small> (Machine name: ' . check_plain($fee_type['name']) . ')</small>';
  $output .= '<div class="description">' . filter_xss_admin($fee_type['description']) . '</div>';

  return $output;
}

/**
 * Builds the form for adding and editing fee types.
 */
function commerce_fees_form($form, &$form_state, $fee_type) {
  $form['#attached']['css'][] = drupal_get_path('module', 'commerce_fees') . '/theme/commerce_fees.admin.css';

  // Store the initial fee type in the form state.
  $form_state['fee_type'] = $fee_type;

  $form['fee_type'] = array('#tree' => TRUE);

  $form['fee_type']['title'] = array(
    '#type' => 'textfield',
    '#title' => t('Title'),
    '#default_value' => $fee_type['title'],
    '#description' => t('The administrative title of this fee type. It is recommended that this title begin with a capital letter and contain only letters, numbers, and spaces.'),
    '#required' => TRUE,
    '#size' => 32,
    '#maxlength' => 255,
    '#field_suffix' => ' <small id="edit-fee-type-title-suffix">' . t('Machine name: @name', array('@name' => $fee_type['name'])) . '</small>',
  );

  if (empty($fee_type['name'])) {
    $form['fee_type']['name'] = array(
      '#type' => 'machine_name',
      '#title' => t('Machine name'),
      '#default_value' => $fee_type['name'],
      '#maxlength' => 32,
      '#required' => TRUE,
      '#machine_name' => array(
        'exists' => 'commerce_fees_load',
        'source' => array('fee_type', 'title'),
      ),
      '#description' => t('The machine-name of this fee type. This name must contain only lowercase letters, numbers, and underscores. It must be unique.'),
    );
  }
  else {
    $form['fee_type']['name'] = array(
      '#type' => 'value',
      '#value' => $fee_type['name'],
    );
  }

  $form['fee_type']['display_title'] = array(
    '#type' => 'textfield',
    '#title' => t('Display title'),
    '#default_value' => $fee_type['display_title'],
    '#description' => t('The front end display title of this fee type shown to customers. Leave blank to default to the <em>Title</em> from above.'),
    '#size' => 32,
  );

  $form['fee_type']['description'] = array(
    '#type' => 'textarea',
    '#title' => t('Description'),
    '#description' => t('Describe this fee type if necessary. The text will be displayed in the fee type overview table.'),
    '#default_value' => $fee_type['description'],
    '#rows' => 3,
  );

  $form['actions'] = array(
    '#type' => 'container',
    '#attributes' => array('class' => array('form-actions')),
    '#weight' => 40,
  );

  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save fee type'),
  );

  if (!empty($form_state['fee_type']['name'])) {
    $form['actions']['delete'] = array(
      '#type' => 'submit',
      '#value' => t('Delete fee type'),
      '#suffix' => l(t('Cancel'), 'admin/commerce/config/fees'),
      '#submit' => array('commerce_fees_form_delete_submit'),
      '#weight' => 45,
    );
  }
  else {
    $form['actions']['submit']['#suffix'] = l(t('Cancel'), 'admin/commerce/config/fees');
  }

  return $form;
}

/**
 * Submit handler: saves the new or updated fee type.
 */
function commerce_fees_form_submit($form, &$form_state) {
  $fee_type = $form_state['fee_type'];

  // Update the shipping service array with values from the form.
  foreach (array('name', 'title', 'display_title', 'description') as $key) {
    $fee_type[$key] = $form_state['values']['fee_type'][$key];
  }

  // Save the fee type.
  $op = commerce_fees_save($fee_type);

  if (!$op) {
    drupal_set_message(t('The fee type failed to save properly. Please review the form and try again.'), 'error');
    $form_state['rebuild'] = TRUE;
  }
  else {
    drupal_set_message(t('Fee type saved.'));
    $form_state['redirect'] = 'admin/commerce/config/fees';
  }
}

/**
 * Submit handler: redirects to the fee type delete confirmation form.
 *
 * @see commerce_fees_form()
 */
function commerce_fees_form_delete_submit($form, &$form_state) {
  $form_state['redirect'] = 'admin/commerce/config/fees/' . strtr($form_state['fee_type']['name'], '_', '-') . '/delete';
}

/**
 * Displays the edit form for an existing fee type.
 *
 * @param string $name
 *   The machine-name of the fee type to edit.
 */
function commerce_fees_edit_page($name) {
  return drupal_get_form('commerce_fees_form', commerce_fees_load($name));
}

/**
 * Builds the form for deleting fee types.
 */
function commerce_fees_delete_form($form, &$form_state, $fee_type) {
  $form_state['fee_type'] = $fee_type;

  $form = confirm_form($form,
    t('Are you sure you want to delete the <em>%title</em> fee type?', array('%title' => $fee_type['title'])),
    'admin/commerce/config/fees',
    '<p>' . t('This action cannot be undone.') . '</p>',
    t('Delete'),
    t('Cancel'),
    'confirm'
  );

  return $form;
}

/**
 * Submit callback for commerce_fees_delete_form().
 */
function commerce_fees_delete_form_submit($form, &$form_state) {
  $fee_type = $form_state['fee_type'];

  commerce_fees_delete($fee_type['name']);

  drupal_set_message(t('The fee type <em>%title</em> has been deleted.', array('%title' => $fee_type['title'])));
  watchdog('commerce_fees', 'Deleted fee type <em>%title</em>.', array('%title' => $fee_type['title']), WATCHDOG_NOTICE);

  $form_state['redirect'] = 'admin/commerce/config/fees';
}

/**
 * Displays the delete confirmation form for an existing fee type.
 *
 * @param string $name
 *   The machine-name of the fee type to delete.
 */
function commerce_fees_delete_page($name) {
  return drupal_get_form('commerce_fees_delete_form', commerce_fees_load($name));
}
