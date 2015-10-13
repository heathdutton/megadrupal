<?php

/**
 * Defines the settings form for the module.
 */
function commerce_redhen_settings_form($form, &$form_state) {
  $form = array();

  $form['redhen_membership'] = array(
    '#type' => 'fieldset',
    '#title' => t('Redhen Membership Settings'),
    '#prefix' => '<div id="redhen-commerce-membership-settings">',
    '#suffix' => '</div>',
  );

  // Get an array of all Commerce product types.
  $product_types = commerce_product_types();
  $options = array();
  foreach($product_types as $machine_name => $info) {
    $options[$machine_name] = $info['name'];
  }

  $form['redhen_membership']['redhen_product_types'] = array(
    '#type' => 'checkboxes',
    '#title' => t('Product Types'),
    '#description' => t('Enable product types to be used to grant Redhen Memberships to Contacts.'),
    '#options' => $options,
    '#default_value' => variable_get('commerce_redhen_selected_product_types', array()),
  );

  if (module_exists('commerce_custom_product') && module_exists('views')) {
    $form['redhen_membership']['redhen_product_types']['#ajax'] = array(
      'callback' => 'commerce_redhen_settings_form_ajax',
      'wrapper' => 'redhen-commerce-membership-settings',
      'method' => 'replace',
      'effect' => 'fade',
    );

    if (!isset($form_state['values'])) {
      $selected_product_types = variable_get('commerce_redhen_selected_product_types', array());
    }
    else {
      $selected_product_types = $form_state['values']['redhen_product_types'];
    }
    // Manipulate previously selected options to show a subset of available product types.
    $options = array_intersect_key($options, array_flip($selected_product_types));
    $form['redhen_membership']['redhen_product_types_org_membership'] = array(
      '#type' => 'checkboxes',
      '#title' => t('Memberships for Organizations as well as Contacts'),
      '#description' => t('Allow selected product types to be used to grant Redhen Memberships to Organizations or Contacts.'),
      '#options' => $options,
      '#default_value' => variable_get('commerce_redhen_selected_product_types_org_membership', array()),
    );
  }
  else {
    $form['redhen_membership']['redhen_product_types_org_membership'] = array(
      '#markup' => t('Download and Install !commerce_custom_product and !views to configure RedHen memberships for Organizations.', array(
        '!commerce_custom_product' => l('Commerce Customizable Products', 'https://www.drupal.org/project/commerce_custom_product', array('attributes' => array('target' => '_blank'))),
        '!views' => l('Views', 'https://www.drupal.org/project/views', array('attributes' => array('target' => '_blank'))),
      )),
    );
  }

  $form['actions'] = array(
    '#type' => 'container',
  );

  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save configuration')
  );

  return $form;
}

/**
 * Ajax callback;  Commerce redhen settings.
 */
function commerce_redhen_settings_form_ajax($form, $form_state) {
  return $form['redhen_membership'];
}

/**
 * Submit handler for the commerce_redhen_settings_form().
 */
function commerce_redhen_settings_form_submit($form, &$form_state) {
  $selected_product_types = array();
  foreach($form_state['values']['redhen_product_types'] as $machine_name => $product_type) {
    // If a product type has been selected, add the relevant fields to it.
    if ($product_type) {
      drupal_set_message(t('The <em>@product_type</em> product type now has the required fields to grant memberships.',
        array(
          '@product_type' => $product_type
          )));

      $selected_product_types[] = $product_type;

      // Add an entityreference field to the product type.

      // Create the field if it does not yet exist.
      if (!field_info_field('commerce_redhen_membership')) {
        $field = array(
          'field_name' => 'commerce_redhen_membership',
          'type' => 'entityreference',
          'cardinality' => 1,
          'settings' => array(
            'target_type' => 'redhen_membership_type',
          ),
        );

        field_create_field($field);
      }

      // Create the instance if it does not yet exist for this product type.
      if (!field_info_instance('commerce_product', 'commerce_redhen_membership', $product_type)) {
        $instance = array(
          'field_name' => 'commerce_redhen_membership',
          'entity_type' => 'commerce_product',
          'label' => 'Redhen Membership Type',
          'bundle' => $product_type,
          'widget' => array(
            'type' => 'options_select',
          ),
        );

        field_create_instance($instance);
      }

      // Add an interval field to the product type.

      // Create the field if it does not yet exist.
      if (!field_info_field('commerce_redhen_interval')) {
        $field = array(
          'field_name' => 'commerce_redhen_interval',
          'type' => 'interval',
          'cardinality' => 1,
        );

        field_create_field($field);
      }

      // Create the instance if it does not yet exist for this product type.
      if (!field_info_instance('commerce_product', 'commerce_redhen_interval', $product_type)) {
        $instance = array(
          'field_name' => 'commerce_redhen_interval',
          'entity_type' => 'commerce_product',
          'label' => 'Length of Membership',
          'bundle' => $product_type,
        );

        field_create_instance($instance);
      }

    } else {
      // If not selected, then remove the field isntances from the product type.
      if ($instance = field_info_instance('commerce_product', 'commerce_redhen_membership', $machine_name)) {
        field_delete_instance($instance);
      }

      if ($instance = field_info_instance('commerce_product', 'commerce_redhen_interval', $machine_name)) {
        field_delete_instance($instance);
      }
    }
  }

  $org_membership_product_types = array();
  if (isset($form_state['values']['redhen_product_types_org_membership'])) {
    foreach($form_state['values']['redhen_product_types_org_membership'] as $machine_name => $product_type) {
      // If a product type has been selected, add the relevant fields to it.
      if ($product_type) {
        drupal_set_message(t('The <em>@product_type</em> line item type now has the required fields to grant memberships to organizations.',
          array(
            '@product_type' => $product_type
          )));

        $org_membership_product_types[] = $product_type;

        // Create line item type.
        if (!array_key_exists('course_voucher', commerce_custom_product_commerce_line_item_type_info())) {
          $line_item_type = array(
            'name' => $product_type,
            'description' => 'A customizable product line item type',
            'product' => TRUE,
            'add_form_submit_value' => t('Add product'),
            'base' => 'commerce_product_line_item',
          );

          commerce_custom_product_line_item_type_save($line_item_type);
        }

        // Add an entityreference field to the line item type.

        // Create the field if it does not yet exist.
        if (!field_info_field('commerce_redhen_organization_ref')) {
          $field = array(
            'field_name' => 'commerce_redhen_organization_ref',
            'type' => 'entityreference',
            'cardinality' => 1,
            'settings' => array(
              'target_type' => 'redhen_org',
              'handler' => 'views',
              'handler_settings' => array(
                'view' => array(
                  'view_name' => 'entity_reference_redhen_organizations',
                  'display_name' => 'entityreference_1',
                  'args' => array(),
                ),
                'behaviors' => array(
                  'views-select-list' => array(
                    'status' => 0,
                  ),
                ),
              ),
            ),
          );

          field_create_field($field);
        }

        // Create the instance if it does not yet exist for this product type.
        if (!field_info_instance('commerce_line_item', 'commerce_redhen_organization_ref', $product_type)) {
          $instance = array(
            'field_name' => 'commerce_redhen_organization_ref',
            'entity_type' => 'commerce_line_item',
            'label' => 'Organization',
            'bundle' => $product_type,
            'widget' => array(
              'type' => 'options_select',
            ),
            'commerce_cart_settings' => array(
              'field_access' => 1,
            ),
          );

          field_create_instance($instance);
        }
      }
    }
    variable_set('commerce_redhen_selected_product_types_org_membership', $org_membership_product_types);
  }

  // Save the selection, so we can set the default values in the form.
  variable_set('commerce_redhen_selected_product_types', $selected_product_types);
  drupal_set_message(t('The settings have been saved.'));
}
