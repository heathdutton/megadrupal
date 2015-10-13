<?php
/**
 * @file
 * Contains administrative forms of the commerce checkout multilane UI module.
 */

/**
 * Form for adding new checkout lanes.
 */
function commerce_checkout_multilane_ui_lane_form($form, &$form_state) {
  $form['title'] = array(
    '#type' => 'textfield',
    '#title' => t('Title'),
    '#required' => TRUE,
  );

  $form['lane_id'] = array(
    '#type' => 'machine_name',
    '#title' => t('Lane Id'),
    '#maxlength' => 15,
    '#machine_name' => array(
      'exists' => 'commerce_checkout_multilane_ui_lane_exists',
      'source' => array('title'),
    ),
    '#required' => TRUE,
  );

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Create lane'),
    '#suffix' => l(t('Cancel'), 'admin/commerce/config/checkout/form'),
  );

  return $form;
}

/**
 * Submit callback for the add-lane form.
 */
function commerce_checkout_multilane_ui_lane_form_submit($form, &$form_state) {
  $lanes = variable_get('commerce_checkout_multilane_lanes', array());
  $lanes[$form_state['values']['lane_id']] = $form_state['values']['title'];
  variable_set('commerce_checkout_multilane_lanes', $lanes);

  menu_rebuild();

  $form_state['redirect'] = 'admin/commerce/config/checkout/form/manage/' . $form_state['values']['lane_id'];
}

/**
 * Form constructor for the custom checkout lane deletion form.
 *
 * @param string $lane_id
 *   The lane id of the lane to be deleted.
 *
 * @see commerce_checkout_multilane_ui_lane_delete_form_submit()
 */
function commerce_checkout_multilane_ui_lane_delete_form($form, &$form_state, $lane_id) {
  $form_state['lane_id'] = $lane_id;

  $lanes = variable_get('commerce_checkout_multilane_lanes', array());
  if (isset($lanes[$lane_id])) {
    return confirm_form($form, t('Are you sure you want to delete the checkout lane %name?', array('%name' => $lanes[$lane_id])), 'admin/commerce/config/checkout/form/manage/' . $lane_id, '', t('Delete'), t('Cancel'));
  }
  else {
    drupal_goto('admin/commerce/config/checkout/form');
  }
}

/**
 * Form submission handler for commerce_checkout_multilane_ui_lane_delete_form().
 *
 * @see commerce_checkout_multilane_ui_lane_delete_form()
 */
function commerce_checkout_multilane_ui_lane_delete_form_submit($form, &$form_state) {
  $lane_id = $form_state['lane_id'];

  $lanes = variable_get('commerce_checkout_multilane_lanes', array());
  if (isset($lanes[$lane_id])) {
    $title = $lanes[$lane_id];

    unset($lanes[$lane_id]);
    variable_set('commerce_checkout_multilane_lanes', $lanes);

    $separator = variable_get('commerce_checkout_multilane_separator', COMMERCE_CHECKOUT_MULTILANE_DEFAULT_SEPARATOR);
    $lane_suffix = $separator . $lane_id;
    db_delete('commerce_checkout_pane')
      ->condition('pane_id', '%' . db_like($lane_suffix), 'LIKE')
      ->execute();

    menu_rebuild();

    drupal_set_message(t('The custom lane %name has been removed.', array('%name' => $title)));
  }

  $form_state['redirect'] = 'admin/commerce/config/checkout/form';
}

/**
 * Checkout sequence form.
 */
function commerce_checkout_multilane_ui_checkout_settings_form($form, &$form_state, $lane_id) {
  drupal_set_title(t('Checkout settings (@lane_title lane)', array(
    '@lane_title' => commerce_checkout_multilane_ui_checkout_settings_title($lane_id),
  )));

  $form['lane_id'] = array(
    '#type' => 'value',
    '#value' => $lane_id,
  );

  $form['#theme'] = 'commerce_checkout_builder_form';

  // Load an array of all available checkout pages.
  $checkout_pages = commerce_checkout_pages();

  // Add a "disabled" pseudo-page.
  $checkout_pages['disabled'] = array('page_id' => 'disabled', 'title' => t('Disabled'), 'name' => t('Disabled'));

  $form['checkout_pages'] = array(
    '#type' => 'value',
    '#value' => $checkout_pages,
  );

  $checkout_pages_options = array();

  // Create arrays for checkout panes in each of the pages.
  foreach (array_keys($checkout_pages) as $page_id) {
    $form['page'][$page_id]['panes'] = array('#tree' => TRUE);

    // Build the options list for selecting the pane's checkout page.
    $checkout_pages_options[$page_id] = $checkout_pages[$page_id]['name'];
  }

  // Loop through all the checkout panes on the site.
  $panes = commerce_checkout_multilane_ui_panes($lane_id);

  foreach ($panes as $pane_id => $checkout_pane) {
    // Determine a checkout pane's current checkout page.
    $page_id = $checkout_pane['enabled'] ? $checkout_pane['page'] : 'disabled';

    // If the page no longer exists, place the pane on the first page.
    if (empty($checkout_pages[$page_id])) {
      reset($checkout_pages);
      $page_id = key($checkout_pages);
    }

    // Add the pane's name to the form array.
    $form['page'][$page_id]['panes'][$pane_id]['name'] = array(
      '#markup' => check_plain($checkout_pane['name']),
    );

    // Add the select field for the pane's checkout page.
    $form['page'][$page_id]['panes'][$pane_id]['page'] = array(
      '#type' => 'select',
      '#options' => $checkout_pages_options,
      '#default_value' => $checkout_pane['page'],
      '#attributes' => array('class' => array('checkout-pane-page checkout-pane-page-'. $checkout_pane['page'])),
    );

    // Add the select field for the pane's weight.
    $form['page'][$page_id]['panes'][$pane_id]['weight'] = array(
      '#type' => 'weight',
      '#delta' => 20,
      '#default_value' => $checkout_pane['weight'],
      '#attributes' => array('class' => array('checkout-pane-weight checkout-pane-weight-'. $checkout_pane['page'])),
    );

    // Add a configuration link for the pane.
    $form['page'][$page_id]['panes'][$pane_id]['ops'] = array(
      '#markup' => l(t('configure'), 'admin/commerce/config/checkout/form/manage/' . $lane_id . '/pane/'. $pane_id),
    );
  }

  $form['actions'] = array(
    '#type' => 'actions',
    '#tree' => FALSE,
  );
  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save configuration'),
  );

  $custom_lanes = variable_get('commerce_checkout_multilane_lanes', array());
  if (isset($custom_lanes[$lane_id])) {
    $form['actions']['submit']['#suffix'] = l(t('Delete'), 'admin/commerce/config/checkout/form/delete/' . $lane_id);
  }

  return $form;
}

/**
 * Validation handler for the checkout builder form.
 */
function commerce_checkout_multilane_ui_checkout_settings_form_validate(&$form, &$form_state) {
  $lane_id = $form_state['values']['lane_id'];

  $pages = commerce_checkout_pages();

  // Get unmodified checkout panes.
  $panes = commerce_checkout_multilane_ui_panes($lane_id, TRUE);

  // Get saved checkout panes. If an error is detected, we'll need it to
  // reference the original form structure so we can accurately locate the
  // elements in the form and set their values.
  $saved_panes = commerce_checkout_multilane_ui_panes($lane_id);

  // Loop through each configured panes.
  foreach ($form_state['values']['panes'] as $pane_id => $checkout_pane) {
    // Check if pane should be locked but is configured on the wrong checkout page.
    if (!empty($panes[$pane_id]['locked']) && $checkout_pane['page'] != $panes[$pane_id]['page']) {
      $element = &$form['page'][$saved_panes[$pane_id]['page']]['panes'][$pane_id]['page'];

      if ($panes[$pane_id]['page'] == 'disabled') {
        drupal_set_message(t('%pane must be configured before it can be enabled. It has been moved back to a disabled position.', array('%pane' => $panes[$pane_id]['title'])), 'warning');
      }
      else {
        drupal_set_message(t('%pane is locked in the code to the %page page and was repositioned accordingly.', array('%pane' => $panes[$pane_id]['title'], '%page' => $pages[$panes[$pane_id]['page']]['name'])), 'warning');
      }

      form_set_value($element, $panes[$pane_id]['page'], $form_state);
    }
  }
}


/**
 * Submit handler for the checkout builder form's save button.
 */
function commerce_checkout_multilane_ui_checkout_settings_form_submit($form, &$form_state) {
  // Loop through each of the checkout panes.
  if (!empty($form_state['values']['panes'])) {
    foreach ($form_state['values']['panes'] as $pane_id => $data) {
      // Load and update the checkout pane array.
      $checkout_pane = commerce_checkout_multilane_ui_pane_load($pane_id);
      $checkout_pane['weight'] = $data['weight'];

      // Accommodate the "Disabled" pseudo-page in the form.
      if ($data['page'] == 'disabled') {
        $checkout_pane['enabled'] = FALSE;
        $checkout_pane['page'] = 'disabled';
      }
      else {
        $checkout_pane['enabled'] = TRUE;
        $checkout_pane['page'] = $data['page'];
      }

      commerce_checkout_pane_save($checkout_pane);
    }
  }

  drupal_set_message(t('Checkout pane positions saved.'));
}

/**
 * Build the configuration form for a checkout pane.
 */
function commerce_checkout_multilane_ui_pane_settings_form($form, &$form_state, $lane_id, $checkout_pane) {
  // Build the form array with the bare minimum fields.
  $form['lane_id'] = array(
    '#type' => 'value',
    '#value' => $lane_id,
  );
  $form['checkout_pane'] = array(
    '#type' => 'value',
    '#value' => $checkout_pane,
  );

  $form['display'] = array(
    '#type' => 'fieldset',
    '#title' => t('Display settings'),
    '#description' => t('These settings are common to all checkout panes and affect their appearance on the checkout form.'),
  );
  $form['display']['collapsibility'] = array(
    '#type' => 'radios',
    '#title' => t('Checkout form fieldset display'),
    '#description' => t('Checkout panes are rendered on the checkout form in individual fieldsets.') .'<br />'. t('Specify here how the fieldset for this pane will appear.'),
    '#options' => array(
      'default' => t('Display this pane in a non-collapsible fieldset.'),
      'collapsible' => t('Display this pane in a collapsible fieldset.'),
      'collapsed' => t('Display this pane in a collapsed fieldset.'),
      'none' => t('Do not display this pane in a fieldset.'),
    ),
    '#default_value' => $checkout_pane['fieldset'] ? ($checkout_pane['collapsible'] ? ($checkout_pane['collapsed'] ? 'collapsed' : 'collapsible') : 'default') : 'none',
  );

  // If the checkout pane has a review callback, allow the user to include the
  // pane on the review checkout pane.
  $form['display']['review'] = array(
    '#type' => 'checkbox',
    '#title' => t('Include this pane on the Review checkout pane.'),
    '#default_value' => $checkout_pane['review'],
    '#access' => (boolean) commerce_checkout_pane_callback($checkout_pane, 'review'),
  );

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save configuration'),
    '#submit' => array('commerce_checkout_multilane_ui_pane_settings_form_save_submit'),
  );
  $form['reset'] = array(
    '#type' => 'submit',
    '#value' => t('Reset to defaults'),
    '#suffix' => l(t('Cancel'), 'admin/commerce/config/checkout/form/manage/' . $lane_id),
    '#submit' => array('commerce_checkout_multilane_ui_pane_settings_form_reset_submit'),
  );

  return $form;
}

/**
 * Submit handler for the checkout pane settings form's save button.
 */
function commerce_checkout_multilane_ui_pane_settings_form_save_submit($form, &$form_state) {
  // Load and update the pane with values from the form.
  $checkout_pane = commerce_checkout_multilane_ui_pane_load($form_state['values']['checkout_pane']['pane_id']);

  // Update the fieldset collapsibility variables.
  switch ($form_state['values']['collapsibility']) {
    case 'collapsible':
      $checkout_pane['fieldset'] = TRUE;
      $checkout_pane['collapsible'] = TRUE;
      $checkout_pane['collapsed'] = FALSE;
      break;

    case 'collapsed':
      $checkout_pane['fieldset'] = TRUE;
      $checkout_pane['collapsible'] = TRUE;
      $checkout_pane['collapsed'] = TRUE;
      break;

    case 'none':
      $checkout_pane['fieldset'] = FALSE;
      $checkout_pane['collapsible'] = FALSE;
      $checkout_pane['collapsed'] = FALSE;
      break;

    case 'default':
    default:
      $checkout_pane['fieldset'] = TRUE;
      $checkout_pane['collapsible'] = FALSE;
      $checkout_pane['collapsed'] = FALSE;
      break;
  }

  // Update the pane's review page visibility.
  $checkout_pane['review'] = $form_state['values']['review'];

  // Save the changes.
  commerce_checkout_pane_save($checkout_pane);

  drupal_set_message(t('Checkout pane saved.'));

  // Redirect to the main checkout form builder page on save.
  $form_state['redirect'] = 'admin/commerce/config/checkout/form/manage/' . $form_state['values']['lane_id'];
}

/**
 * Submit handler for the checkout pane settings form's reset button.
 */
function commerce_checkout_multilane_ui_pane_settings_form_reset_submit($form, &$form_state) {
  // Reset the display settings for the checkout pane.
  commerce_checkout_pane_reset($form_state['values']['checkout_pane']['pane_id']);
  drupal_set_message(t('Checkout pane reset.'));
}
