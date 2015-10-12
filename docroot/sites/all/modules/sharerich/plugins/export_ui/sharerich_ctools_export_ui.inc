<?php

/**
 * @file
 * A Ctools Export UI plugin for Sharerich.
 */
$plugin = array(
  'schema' => 'sharerich_sets', // As defined in hook_schema().
  'access' => 'administer sharerich', // Define a permission users must have to access these pages.
  // Define the menu item.
  'menu' => array(
    'menu item' => 'sharerich',
    'menu title' => 'Sharerich',
    'menu description' => 'Administer Sharerich sets.',
  ),
  // Define user interface texts.
  'title singular' => t('set'),
  'title plural' => t('sets'),
  'title singular proper' => t('Sharerich set'),
  'title plural proper' => t('Sharerich sets'),
  // Define the class to use as a handler for DFP ad tags.
  'handler' => array(
     'class' => 'sharerich_set_ui',
     'parent' => 'ctools_export_ui',
  ),
  // Define the names of the functions that provide the add/edit forms.
  'form' => array(
    'settings' => 'sharerich_ctools_export_ui_form',
  ),
);

/**
 * Define the preset add/edit form.
 */
function sharerich_ctools_export_ui_form(&$form, &$form_state) {
  $item = ($form_state['op'] == 'add' ? $form_state['item'] : $form_state['item']->raw);

  // Generate the list of available services.
  $services = sharerich_get_services();

  $form['settings'] = array(
    '#type' => 'vertical_tabs',
  );

  $form['general'] = array(
    '#type' => 'fieldset',
    '#title' => t('General settings'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
    '#group' => 'settings',
  );

  $form['available_services'] = array(
    '#type' => 'fieldset',
    '#title' => t('Services'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
    '#group' => 'settings',
  );

  $form['custom_markup'] = array(
    '#type' => 'fieldset',
    '#title' => t('Custom Markup'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
    '#group' => 'settings',
    '#description' => '<strong>' . t('In order to edit the markup, you need to enable the services in the "Services tab".') . '</strong>',
  );

  // Use our dragable table theme function.
  $form['available_services']['services'] = array(
    '#theme' => 'sharerich_manage_services',
    '#tree' => TRUE,
  );

  foreach ($services as $service_name) {

    $form['available_services']['services'][$service_name]['service'] = array(
      '#markup' => $service_name,
    );

    $form['available_services']['services'][$service_name]['enabled'] = array(
      '#type' => 'checkbox',
      '#default_value' => $item->services[$service_name]['enabled'],
    );

    // Store the buttons markup along other data.
    $form['available_services']['services'][$service_name]['markup'] = array(
      '#type' => 'hidden',
      '#value' => sharerich_get_service_content($service_name, $item->machinename),
    );

    // The field contains sort info (weights).
    $form['available_services']['services'][$service_name]['weight'] = array(
      '#type' => 'weight',
      '#title' => t('Weight'),
      '#title_display' => 'invisible',
      '#default_value' => $item->services[$service_name]['weight'],
    );
  }

  // Sort the services.
  uasort($form['available_services']['services'], 'drupal_sort_weight');

  $form['general']['name'] = array(
    '#type' => 'textfield',
    '#title' => t('Set name'),
    '#required' => TRUE,
    '#maxlength' => 64,
    '#default_value' => $item->name,
    '#description' => t('Example: Article Top'),
    '#weight' => -2,
  );
  $form['general']['machinename'] = array(
    '#type' => 'machine_name',
    '#title' => t('Machine name'),
    '#maxlength' => 64,
    '#default_value' => $item->machinename,
    '#description' => t('Only use letters, numbers and underscores. Example: article_top'),
    '#machine_name' => array(
      'exists' => 'sharerich_set_name_exists',
      'source' => array('general', 'name'),
    ),
    '#weight' => -1,
  ) + $form['info']['machinename'];
  unset($form['info']);

  $form['general']['block'] = array(
    '#type' => 'checkbox',
    '#title' => t('Create a block for this button set.'),
    '#description' => '',
    '#default_value' => $item->block,
  );

  $form['general']['title'] = array(
    '#type' => 'textfield',
    '#title' => t('Title'),
    '#description' => t('This is the title that appears above the share buttons.'),
    '#default_value' => !empty($item->title) ? $item->title : '',
    '#states' => array(
      'visible' => array(
        ':input[name="block"]' => array('checked' => TRUE),
      ),
    ),
  );

  // Custom widgets.
  foreach ($services as $service_name) {
    $var_name = 'sharerich_custom_' . $service_name;

    $form['custom_markup'][$var_name] = array(
      '#type' => 'textarea',
      '#title' => t('Custom code for %name', array('%name' => $service_name)),
      '#description' => '',
      '#default_value' => sharerich_get_service_content($service_name, $item->machinename),
      '#states' => array(
        'invisible' => array(
          ':input[name="services[' . $service_name . '][enabled]"]' => array('checked' => FALSE),
        ),
      ),
    );
  }
}

function sharerich_ctools_export_ui_form_submit(&$form, &$form_state) {
  $services_values = &$form_state['values']['services'];
  uasort($services_values, 'drupal_sort_weight');

  // Rewrite the valuees from the custom service code (coming from default).
  foreach ($services_values as $key => &$service) {
    $custom_key = 'sharerich_custom_' . $key;

    if (empty($form_state['values'][$custom_key])) {
      continue;
    }

    $service['markup'] = $form_state['values'][$custom_key];
  }
}

// Theme function for sharerich_manage_services().
function theme_sharerich_manage_services($variables) {
  $form = $variables['form'];
  $rows = array();

  foreach (element_children($form) as $service_identifier) {
    $form[$service_identifier]['weight']['#attributes']['class'] = array('services-order-weight');
    $rows[] = array(
      'data' => array(
        array('class' => array('service-cross')),
        drupal_render($form[$service_identifier]['service']),
        drupal_render($form[$service_identifier]['enabled']),
        drupal_render($form[$service_identifier]['weight']),
      ),
      'class' => array('draggable'),
    );
  }

  $header = array('', t('Service'), t('Enabled'), t('Weight'));
  $output = drupal_render($form['note']);
  $output .= theme('table', array('header' => $header, 'rows' => $rows, 'attributes' => array('id' => 'service-order')));
  $output .= drupal_render_children($form);

  drupal_add_tabledrag('service-order', 'order', 'sibling', 'services-order-weight');

  return $output;
}

/**
 * Check if the given machinename is unique in the dfp_tags table.
 */
function sharerich_set_name_exists($machinename) {
  $select = db_select('sharerich_sets', 's');
  $select->addExpression('COUNT(*)');
  $select->condition('s.machinename', $machinename);

  return $select->execute()->fetchField();
}
