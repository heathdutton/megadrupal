<?php
/**
 * @file
 * Page generation callbacks the Commerce Bulk Product Creation settings
 * pages.
 */

/**
 * Form constructor for the commerce_bpc settings pages.
 *
 * Paths:
 * - admin/commerce/config/commerce_bpc
 * - admin/commerce/config/commerce_bpc/patterns
 * - admin/commerce/products/types/PRODUCT_TYPE/commerce_bpc
 * - admin/commerce/products/types/PRODUCT_TYPE/commerce_bpc/patterns
 *
 * @see commerce_bpc_menu()
 * @see commetce_bpc_settings_form()
 * @ingroups forms
 */
function commerce_bpc_pattern_settings_form($form, &$form_state, $type = NULL) {
  $form['patterns'] = array(
    '#type' => 'fieldset',
    '#title' => t('Patterns'),
    '#description' => t('Specify the patterns used for the generation of SKUs and titles during bulk creation.'),
    '#collapsible' => FALSE,
    '#collapsed' => FALSE,
    '#access' => user_access('configure commerce bpc'),
    '#tree' => FALSE,
  );

  $form['patterns']['sku_pattern'] = array(
    '#type' => 'textfield',
    '#title' => t('SKU Pattern'),
    '#description' => t('Enter the pattern that should be used to generate SKUs. You should use the patterns below.'),
    '#size' => 160,
    '#maxlength' => 255,
    '#default_value' => commerce_bpc_setting('default', 'sku_pattern', $type),
  );
  $form['patterns']['sku_separator'] = array(
    '#type' => 'textfield',
    '#title' => t('Separator for combination field values in SKU'),
    '#description' => t('This string will be used to separate the values of various combination field. Thus if there are two combination fields "size" and "color", and for a particular combination, those have the values "M" and "red", respectively, with a separator value of "-", the string returned by [bulk_defaults:sku_combination_values] will be "M-red".'),
    '#size' => 3,
    '#maxlength' => 255,
    '#default_value' => commerce_bpc_setting('default', 'sku_separator', $type),
  );

  $form['patterns']['title_pattern'] = array(
    '#type' => 'textfield',
    '#title' => t('Title pattern'),
    '#description' => t('Enter the pattern that should be used to generate titles. You should use the patterns below.'),
    '#size' => 160,
    '#maxlength' => 255,
    '#default_value' => commerce_bpc_setting('default', 'title_pattern', $type),
  );

  $form['patterns']['title_separator'] = array(
    '#type' => 'textfield',
    '#title' => t('Separator for combination field labels in titles'),
    '#description' => t('This string will be used to separate the value-labels of various combination fields. Thus if there are two combination fields "size" and "color", and for a particular combination, those have the values "Medium" and "red", respectively, with a separator value of ", ", the string returned by [commerce_bpc:default_title_combination_values] will be "Medium, red".'),
    '#size' => 3,
    '#maxlength' => 255,
    '#default_value' => commerce_bpc_setting('default', 'title_separator', $type),
  );
  $bpc_tokens = commerce_bpc_token_info();


  $form['tokens'] = _commerce_bpc_show_tokens('bulk_defaults', $bpc_tokens['tokens']['bulk_defaults']);
  if ($type) {
    $product_type_tokens = commerce_bpc_token_info_by_product_type($type);
    $form['product_tokens'] = _commerce_bpc_show_tokens('bulk_product', $product_type_tokens);
  }


  return commerce_bpc_settings_form($form, 'default', $type);
}
/**
 * Form constructor for the commerce_bpc display node settings page.
 *
 * Paths:
 * - admin/commerce/config/commerce_bpc/display_nodes
 * - admin/commerce/products/types/PRODUCT_TYPE/commerce_bpc/display_nodes
 *
 * @see commerce_bpc_menu()
 * @see commerce_bpc_display_node_settings_validate()
 * @see commetce_bpc_settings_form()
 * @ingroups forms
 */
function commerce_bpc_display_node_settings_form($form, &$form_state, $type = NULL) {

  $form = array();

  $form['creation_type'] = array(
    '#type' => 'fieldset',
    '#title' => t('Display node creation'),
    '#description' => t('How should the creation of display nodes be handled?'),
    '#collapsible' => FALSE,
    '#collapsed' => FALSE,
    '#access' => user_access('configure commerce bpc'),
  );

  $form['creation_type']['creation_method'] = array(
    '#type' => 'radios',
    '#title' => t('Display node creation'),
    '#title_display' => 'invisible',
    '#description' => t('<strong>Note:</strong> The rest of the settings available on this page depends on what you select here.'),
    '#options' => array(
      'wizard' => t('Provide a "Save and create display" link on the Bulk creation form that takes the user to a pre-populated node creation form.'),
      'auto' => t('Silently create a display node automatically that references all the created products.'),
      'onetoone' => t('Create a display node automatically for every product created.'),
      'none' => t('Do not include any display node functionality.'),
    ),
    '#default_value' => commerce_bpc_setting('display', 'creation_method', $type),
  );


  $form['auto_settings'] = array(
    '#type' => 'fieldset',
    '#title' => t('Settings for created display nodes'),
    '#collapsible' => FALSE,
    '#collapsed' => FALSE,
    '#access' => user_access('configure commerce bpc'),
    '#states' => array(
      'invisible' => array(
        ':input[name="commerce_bpc_display_creation_method"]' => array('value' => 'none'),
      ),
    ),
  );
  $required_if_auto = array(
    'optional' => array(
      ':input[name="creation_method"]' => array('value' => 'wizard'),
    ),
  );
  $node_types = commerce_bpc_get_node_types();
  if (!empty($node_types)) {
    $form['auto_settings']['auto_content_type'] = array(
      '#type' => 'select',
      '#title' => t('Content type of the created display node'),
      '#description' => t('The setting <strong>--Allow user to select node type--</strong> is only valid for manual node creation.'),
      '#multiple' => FALSE,
      '#options' => array('_none' => t('--Allow user to select node type--')) + $node_types,
      '#default_value' => commerce_bpc_setting('display', 'auto_content_type', $type),
      '#states' => $required_if_auto,
    );
  }
  else {
    $form['auto_settings']['auto_content'] = array(
      '#markup' => '<p><strong>' . t('In order to auto-generate display nodes for all product types, you must create at least one content type that has a product reference field that can reference all product types. Visit the <a href="@content_type_path">content type page</a> to add such a field to a content type.',
      array(
        '@content_type_path' => '/admin/structure/types',
      )
      ) . '</strong></p>',
    );
    $form['auto_settings']['no_content_types_available'] = array(
      '#type' => 'value',
      '#value' => TRUE,
    );
  }
  $form['auto_settings']['auto_node_title_pattern'] = array(
    '#type' => 'textfield',
    '#title' => t('Pattern for the title of the created display node'),
    '#description' => t('You can use the token %title_fragment_token for the title fragment entered by the user on the bulk creation form.', array(
           '%title_fragment_token' => '[bulk_defaults:entered_title]',
    )),
    '#size' => 60,
    '#maxlength' => 255,
    '#default_value' => commerce_bpc_setting('display', 'auto_node_title_pattern', $type),
    '#states' => $required_if_auto,
  );

  $tokens = token_info();
  $product_tokens = $tokens['tokens']['commerce-product'];
  $form['auto_settings']['tokens'] = _commerce_bpc_show_tokens('commerce-product', $product_tokens);
  // Only show if we create one display per product.
  $form['auto_settings']['tokens']['#states'] = array(
    'visible' => array(
      ':input[name="display_creation_method"]' => array('value' => 'onetoone'),
      ),

  );

  $form['auto_settings']['auto_redirect'] = array(
    '#type' => 'radios',
    '#title' => t('After successful bulk creation, send the user to ...'),
    '#options' => array(
      'product listing' => t('... the list of products.'),
      'display node'    => t('... the newly created display node.'),
      'custom'          => t('... a custom location.'),
    ),
    '#default_value' => commerce_bpc_setting('display', 'auto_redirect', $type),
  );
  $form['auto_settings']['redirection_settings'] = array(
    '#type' => 'fieldset',
    '#title' => t('Custom redirect path'),
    '#collapsible' => FALSE,
    '#collapsed' => FALSE,
    '#access' => user_access('configure commerce bpc'),
    '#states' => array(
      'visible' => array(
        ':input[name="auto_redirect"]' => array('value' => 'custom'),
      ),
    ),
  );


  $form['auto_settings']['redirection_settings']['auto_redirect_custom'] = array(
    '#type' => 'textfield',
    '#title' => t('Custom redirect path'),
    '#size' => 80,
    '#maxlength' => 255,
    '#title_display' => 'invisible',
    '#description' => 'Enter any valid drupal path.',
    '#default_value' => commerce_bpc_setting('display', 'auto_redirect_custom', $type),
    '#states' => array(
      'visible' => array(
        ':input[name="display_auto_redirect"]' => array('value' => 'custom'),
      ),
      'required' => array(
        ':input[name="display_auto_redirect"]' => array('value' => 'custom'),
      ),
    ),
  );

  return commerce_bpc_settings_form($form, 'display', $type);
}

/**
 * Form validation handler for commerce_bpc_display_node_settings_form().
 */
function commerce_bpc_display_node_settings_form_validate($form, &$form_state) {
  $fv = &$form_state['values'];
  switch ($fv['creation_method']) {
    case 'auto':
    case 'onetoone':
      // Notify the user if there is no content type available.
      if (!empty($fv['no_content_types_available'])) {
        form_set_error('auto_content_type', 'You cannot activate auto-creation of display nodes if there are no appropriate display node types. See below for details.');
      }
      // Make sure all the required fields have values.
      if (empty($fv['auto_content_type']) || $fv['auto_content_type'] == '_none') {
        form_set_error('auto_content_type', 'You have to specify a content type to automatically create display nodes.');
      }
      if (empty($fv['auto_node_title_pattern'])) {
        form_set_error('auto_node_title_pattern', 'For auto-creation of display nodes to work, you have to specify a pattern for the node title.');
      }
      if ($fv['auto_redirect'] == 'custom') {
        if (empty($fv['auto_redirect_custom'])) {
          form_set_error('auto_redirect', 'Please specify a custom redirect location.');
        }
        elseif (!drupal_valid_path($fv['auto_redirect_custom'])) {
          form_set_error('auto_redirect_custom', 'Please specify a valid path as custom redirect location');
        }
      }
      break;
  }
}
/**
 * BPC-specific wrapper for system_settings_form.
 */
function commerce_bpc_settings_form($form, $settings_group, $product_type = NULL) {
  $form['settings_group'] = array(
    '#type' => 'value',
    '#value' => $settings_group,
  );

  if (!empty($product_type)) {
    $form['product_type'] = array(
      '#type' => 'value',
      '#value' => $product_type,
    );
    // For product-type specific forms, we add a checkbox for overriding,
    // and hide all form elements if it is unchecked.
    foreach ($form as $key => $element) {
      $form[$key]['#states']['visible'][':input[name="override"]'] = array('checked' => TRUE);
    }
    $types = commerce_product_types();
    $form['override'] = array(
      '#type' => 'checkbox',
      '#title' => t('Override settings for type %label', array('%label' => $types[$product_type]['name'])),
      '#default_value' => commerce_bpc_setting($settings_group, 'override', $product_type),
      '#weight' => -1,
    );
  }
  $form = system_settings_form($form);
  // Override the submit handler, so we can do pre- and suffixing.
  $form['#submit'] = array('commerce_bpc_settings_form_submit');
  return $form;
}

/**
 * Form submission handler for commerce_bpc_settings_form().
 */
function commerce_bpc_settings_form_submit($form, &$form_state) {
  form_state_values_clean($form_state);
  $fv = &$form_state['values'];

  $group = $fv['settings_group'];
  unset($fv['settings_group']);

  $type = isset($fv['product_type']) ? $fv['product_type'] : NULL;
  unset($fv['product_type']);

  foreach ($fv as $key => $value) {
    commerce_bpc_setting_set($group, $key, $value, $type);
  }
  drupal_set_message(t('The configuration options have been saved.'));

}
