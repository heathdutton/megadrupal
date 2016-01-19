<?php

/**
 * @file
 * Page callbacks and form callbacks for admin
 * Commerce Restrict Area pages.
 */

/**
 * Commerce Restrict Area admin form.
 */
function commerce_restrict_area_settings_form($form, &$form_state) {
  drupal_add_css(drupal_get_path('module', 'commerce_restrict_area') . '/includes/commerce_restrict_area.admin.css');

  $commerce_restrict_area_product_type_settings = variable_get('commerce_restrict_area_product_type_settings', array());
  $commerce_restrict_area_restrict_settings = variable_get('commerce_restrict_area_restrict_settings', array());

  $enabled = array();
  $names = array();
  $names['default'] = 'Default';

  if (!empty($form_state['values']['commerce_restrict_area_product_type_settings'])) {
    foreach ($form_state['values']['commerce_restrict_area_product_type_settings'] as $type => $values) {
      if ($values['state'] && ($values['restrict_type'] == 'default' || $values['restrict_type'] == 'same')) {
        $enabled[$type] = $values['restrict_type'];
      }
    }
  }


  $form['#tree'] = TRUE;
  $form['commerce_restrict_area_product_type_settings'] = array(
    '#type' => 'fieldset',
    '#title' => t('Enable Restrict Area management for these product types'),
    '#description' => t('Note that disabling Restrict Area management removes the Restrict Area field from the product type, deleting any existing Restrict Area data for that product type.'),
    '#collapsible' => TRUE,
  );

  // Create a checkbox for each product type, set with the current state.
  foreach (commerce_product_types() as $type => $product_type) {
    if (!empty($commerce_restrict_area_product_type_settings[$type]['state']) && $commerce_restrict_area_product_type_settings[$type]['state']
       && ($commerce_restrict_area_product_type_settings[$type]['restrict_type'] == 'same' || $commerce_restrict_area_product_type_settings[$type]['restrict_type'] == 'same_all')) {
      $enabled[$type] = $commerce_restrict_area_product_type_settings[$type]['restrict_type'];
    }
    $names[$type] = $product_type['name'];


    $form['commerce_restrict_area_product_type_settings'][$type]['state'] = array(
      '#type' => 'checkbox',
      '#title' => theme('commerce_restrict_area_product_type', array('product_type' => $product_type)),
      '#default_value' => !empty($commerce_restrict_area_product_type_settings[$type]['state']) ? $commerce_restrict_area_product_type_settings[$type]['state'] : 0,
      '#ajax' => array(
        'callback' => 'commerce_restrict_area_ajax_callback',
        'wrapper' => 'commerce-restrict-area-restrict-settings',
        'method' => 'replace',
        'effect' => 'fade',
      ),
    );
    $form['commerce_restrict_area_product_type_settings'][$type]['restrict_type'] = array(
      '#type' => 'select',
      '#options' => array(
        'default_all' => t('Use the default restrictions for all products'),
        'default' => t('Use the default restrictions for each marked product'),
        'same_all' => t('All products have its own restrictions'),
        'same' => t('All marked products have its own restrictions'),
      ),
      '#default_value' => !empty($commerce_restrict_area_product_type_settings[$type]['restrict_type']) ? $commerce_restrict_area_product_type_settings[$type]['restrict_type'] : 'default_all',
      '#states' => array(
        'invisible' => array(
          ':input[name="commerce_restrict_area_product_type_settings[' . $type . '][state]"]' => array('checked' => FALSE),
        ),
      ),
      '#ajax' => array(
        'callback' => 'commerce_restrict_area_ajax_callback',
        'wrapper' => 'commerce-restrict-area-restrict-settings',
        'method' => 'replace',
        'effect' => 'fade',
      ),
    );
  }

  if (!empty($form_state['values']['commerce_restrict_area_product_type_settings'])) {
    $enabled = array();
    foreach ($form_state['values']['commerce_restrict_area_product_type_settings'] as $type => $values) {
      if ($values['state'] && ($values['restrict_type'] == 'same' || $values['restrict_type'] == 'same_all')) {
        $enabled[$type] = $values['restrict_type'];
      }
    }
  }


  $form['commerce_restrict_area_restrict_settings'] = array(
    '#type' => 'vertical_tabs',
    '#prefix' => '<div id="commerce-restrict-area-restrict-settings">',
    '#suffix' => '</div>',
    '#default_tab' => 'edit-commerce_restrict_area_default_restrict_settings',
    '#tree' => TRUE,
  );

  // Necessary for country_get_list().
  require_once DRUPAL_ROOT . '/includes/locale.inc';
  $countries = country_get_list();

  // Get commerce profiles.
  $profiles = commerce_customer_profile_types();
  foreach ($profiles as $key => $profile) {
    $addresses[$key] = $profile['description'];
  }
  reset($profiles);

  // Add Default to enabled products.
  $enabled = array_merge(array('default' => 'Default'), $enabled);
  foreach ($enabled as $type => $value) {
    $form['commerce_restrict_area_restrict_settings'][$type] = array(
      '#type' => 'fieldset',
      '#title' => t('%name restrictions', array('%name' => $names[$type])),
      '#tree' => TRUE,
    );
    $form['commerce_restrict_area_restrict_settings'][$type]['countries'] = array(
      '#title' => t('Enable Restrict Area filtering for these countries'),
      '#type' => 'select',
      '#multiple' => TRUE,
      '#options' => $countries,
      '#default_value' => empty($commerce_restrict_area_restrict_settings[$type]['countries']) ? '' : $commerce_restrict_area_restrict_settings[$type]['countries'],
      '#ajax' => array(
        'callback' => 'commerce_restrict_area_ajax_callback',
        'wrapper' => 'commerce-restrict-area-restrict-settings',
        'method' => 'replace',
        'effect' => 'fade',
      ),
      '#size' => 10,
    );
    if (!empty($form_state['values']['commerce_restrict_area_restrict_settings'][$type]['countries'])) {
      $enable_countries = $form_state['values']['commerce_restrict_area_restrict_settings'][$type]['countries'];
    }
    elseif (!empty($commerce_restrict_area_restrict_settings[$type]['countries'])) {
      $enable_countries = $commerce_restrict_area_restrict_settings[$type]['countries'];
    }
    else {
      $enable_countries = array();
    }
    foreach ($enable_countries as $country) {
      $form['commerce_restrict_area_restrict_settings'][$type][$country] = array(
        '#type' => 'fieldset',
        '#title' => t('%name restrictions', array('%name' => $countries[$country])),
        '#tree' => TRUE,
        '#collapsible' => TRUE,
        '#collapsed' => TRUE,
      );
      if (count($profiles) > 1) {
        $form['commerce_restrict_area_restrict_settings'][$type][$country]['addresses'] = array(
          '#type' => 'radios',
          '#title' => t('Addresses to filter'),
          '#description' => t('Which addresses should be filtered.'),
          '#default_value' => empty($commerce_restrict_area_restrict_settings[$type][$country]['addresses']) ? key($addresses) : $commerce_restrict_area_restrict_settings[$type][$country]['addresses'],
          '#options' => $addresses,
          '#required' => TRUE,
        );
      }
      else {
        $form['commerce_restrict_area_restrict_settings'][$type][$country]['addresses'] = array(
          '#type' => 'value',
          '#value' => key($profiles),
        );
      }
      $form['commerce_restrict_area_restrict_settings'][$type][$country]['mode'] = array(
        '#type' => 'radios',
        '#title' => t('Filter mode'),
        '#description' => t('Whether to "blacklist" or "whitelist" address. If blacklisted, address in the filter will be prevented from completing checkout. If whitelisted, <em>only</em> address that are in the filter will be allowed to checkout'),
        '#default_value' => empty($commerce_restrict_area_restrict_settings[$type][$country]['mode']) ? 'blacklist' : $commerce_restrict_area_restrict_settings[$type][$country]['mode'],
        '#options' => array(
          'blacklist' => t('Blacklist'),
          'whitelist' => t('Whitelist'),
        ),
        '#required' => TRUE,
      );
      $form['commerce_restrict_area_restrict_settings'][$type][$country]['postal_code'] = array(
        '#type' => 'textarea',
        '#title' => t('Postal codes filter (Recommended)'),
        '#description' => t('Postal codes to be filtered, comma seperated. Partial postal codes are supported, e.g. PL, EX5.'),
        '#default_value' => empty($commerce_restrict_area_restrict_settings[$type][$country]['postal_code']) ? '' : drupal_strtoupper(preg_replace('/\s+/', ' ', $commerce_restrict_area_restrict_settings[$type][$country]['postal_code'])),
      );
      $form['commerce_restrict_area_restrict_settings'][$type][$country]['locality'] = array(
        '#type' => 'textarea',
        '#title' => t('Localities filter'),
        '#description' => t('Localities to be filtered, comma seperated. If locality addressfield plugin have ISO format support, better in ISO format. Otherwise is not recommend use this filter.'),
        '#default_value' => empty($commerce_restrict_area_restrict_settings[$type][$country]['locality']) ? '' : drupal_strtoupper(preg_replace('/\s+/', ' ', $commerce_restrict_area_restrict_settings[$type][$country]['locality'])),
      );
      $form['commerce_restrict_area_restrict_settings'][$type][$country]['administrative_area'] = array(
        '#type' => 'textarea',
        '#title' => t('Administrative areas filter'),
        '#description' => t('Administrative areas to be filtered, comma seperated. If administrative area addressfield plugin have ISO format support, better in ISO format. Otherwise is not recommend use this filter.'),
        '#default_value' => empty($commerce_restrict_area_restrict_settings[$type][$country]['administrative_area']) ? '' : drupal_strtoupper(preg_replace('/\s+/', ' ', $commerce_restrict_area_restrict_settings[$type][$country]['administrative_area'])),
      );
    }
  }

  $form['#validate'][] = 'commerce_restrict_area_settings_form_validate';
  $form['#submit'][] = 'commerce_restrict_area_settings_form_submit';

  return system_settings_form($form);
}

/**
 * Validate callback for the Commerce Restrict Area admin form.
 */
function commerce_restrict_area_settings_form_validate($form, &$form_state) {
  foreach ($form_state['values']['commerce_restrict_area_product_type_settings'] as $type => $values) {
    if (!$values['state']) {
      unset($form_state['values']['commerce_restrict_area_product_type_settings'][$type]);
      unset($form_state['values']['commerce_restrict_area_restrict_settings'][$type]);
    }
    elseif ($values['restrict_type'] == 'default' || $values['restrict_type'] == 'default_all') {
      unset($form_state['values']['commerce_restrict_area_restrict_settings'][$type]);
    }
  }
}

/**
 * Submit callback for the Commerce Restrict Area admin form.
 */
function commerce_restrict_area_settings_form_submit($form, &$form_state) {
  foreach ($form_state['values']['commerce_restrict_area_product_type_settings'] as $type => $values) {
    $enable = $values['state'];
    $restriction = $values['restrict_type'];

    $instance = field_info_instance('commerce_product', COMMERCE_RESTRICT_AREA_FIELD_NAME, $type);
    $currently_enabled = commerce_restrict_area_product_type_enabled($type);

    if ($enable && !$currently_enabled && ($restriction == 'default' || $restriction == 'same')) {
      // Create the instance.
      commerce_restrict_area_create_instance(COMMERCE_RESTRICT_AREA_FIELD_NAME, 'list_boolean', FALSE, 'commerce_product', $type, t('Restrict Area'));
      drupal_set_message(t('Restrict Area field has been added to the %type product type.', array('%type' => $type)));
    }
    elseif ($enable && $currently_enabled && ($restriction != 'default' && $restriction != 'same')) {
      // Remove the instance.
      field_delete_instance($instance);
      drupal_set_message(t('Restrict Area field has been removed on the %type product type', array('%type' => $type)));
    }
    elseif (!$enable && $currently_enabled && ($restriction != 'default' && $restriction != 'same')) {
      // Remove the instance.
      field_delete_instance($instance);
      drupal_set_message(t('Restrict Area management has been disabled on the %type product type', array('%type' => $type)));
    }
    elseif (!$enable && $currently_enabled) {
      drupal_set_message(t('Restrict Area management has been disabled on the %type product type', array('%type' => $type)));
    }
  }
}

/**
 * Ajax callback.
 */
function commerce_restrict_area_ajax_callback($form, &$form_state) {
  return $form['commerce_restrict_area_restrict_settings'];
}

/**
 * Builds an overview of a product type for display to an administrator.
 *
 * @param array $variables
 *   An array of variables used to generate the display; by default includes the
 *   product_type key with a value of the product type info array.
 */
function theme_commerce_restrict_area_product_type($variables) {
  $product_type = $variables['product_type'];

  // Build the actual output.
  $output = check_plain($product_type['name']);
  $output .= ' <small> (Type: ' . check_plain($product_type['type']) . ')</small>';
  $output .= '<div class="description">' . filter_xss_admin($product_type['description']) . '</div>';
  return $output;
}

/**
 * Commerce Restrict Area rules form.
 */
function commerce_restrict_area_settings_rules_form($form, &$form_state) {
  $options = array('show plugin' => FALSE);

  RulesPluginUI::$basePath = 'admin/commerce/config/restrict-area/rules';

  $content['commerce_restrict_area_settings_rules_enabled']['title']['#markup'] = '<h3>' . t('Enabled rules for restrict areas') . '</h3>';
  $conditions = array(
    'event' => 'commerce_restrict_area_validation',
    'plugin' => 'reaction rule',
    'active' => TRUE,
  );
  $content['commerce_restrict_area_settings_rules_enabled']['rules'] = RulesPluginUI::overviewTable($conditions, $options);
  $content['commerce_restrict_area_settings_rules_enabled']['rules']['#empty'] = t('There are no active rules for restrict areas.');
  $content['commerce_restrict_area_settings_rules_disabled']['title']['#markup'] = '<h3>' . t('Disabled rules for restrict areas') . '</h3>';
  $conditions['active'] = FALSE;
  $content['commerce_restrict_area_settings_rules_disabled']['rules'] = RulesPluginUI::overviewTable($conditions, $options);
  $content['commerce_restrict_area_settings_rules_disabled']['rules']['#empty'] = t('There are no disabled rules for restrict areas.');

  // Store the function name in the content array to make it easy to alter the
  // contents of this page.
  $content['#page_callback'] = 'commerce_restrict_area_settings_rules_form';

  return $content;
}
