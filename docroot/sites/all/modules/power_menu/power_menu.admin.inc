<?php

/**
 * @file
 *
 * This contains all the admin stuff of the module
 */

/**
 * Form definition for Powe Menu configuration.
 */
function power_menu_configuration_form() {
  // Description for power menu settings page
  // We're re-using classes from the menu module.
  $form['#attached']['css'] = array(drupal_get_path('module', 'menu') . '/menu.css');

  $form['handlers'] = array(
    '#type' => 'fieldset',
    '#title' => t('Handler settings'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
  );

  $menus = menu_get_menus();

  $form['handlers']['menus'] = array(
    '#type' => 'checkboxes',
    '#options' => $menus,
    '#title' => t('Availabe Menus'),
    '#default_value' => variable_get('power_menu_handlers_menus', array()),
    '#description' => t('Choose the menus on which the path sould set.'),
  );

  $form['handlers']['breadcrumb'] = array(
    '#type' => 'fieldset',
    '#title' => t('Breadcrumb settings'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
  );

  $form['handlers']['breadcrumb']['set_breadcrumb'] = array(
    '#type' => 'checkbox',
    '#title' => t('Set breadcrumb'),
    '#default_value' => variable_get('power_menu_handlers_breadcrumb', TRUE),
    '#description' => t('Should the power menu also set the breadcrumb. This works only when the system path is used in a menu.'),
  );
  $form['handlers']['breadcrumb']['set_breadcrumb_title'] = array(
    '#type' => 'checkbox',
    '#title' => t('Add title to breadcrumb'),
    '#default_value' => variable_get('power_menu_handlers_breadcrumb_title', TRUE),
    '#description' => t('Display title in breadcrumbs. This works only when the entity has a title attribute ($entity->title).'),
  );

  $settings = variable_get('power_menu_handlers_settings', array());

  // Get the menu_handlers plugins
  $handlers = power_menu_get_menu_handlers();
  $delta = count($handlers);

  // Default message if no handlers.
  if ($delta == 0) {
    $form['handlers']['handlers_order'] = array(
      '#markup' => '<p>' . t('No power menu menu_handlers plugins exists!') . '</p>',
    );
  }
  else {
    $form['handlers']['handlers_order'] = array(
      '#tree' => TRUE,
      '#theme' => 'power_menu_plugins_order',
    );
    foreach ($handlers as $key => $handler) {

      $form['handlers']['handlers_order'][$key] = array(
        'title' => array(
          '#markup' => '<strong>' . check_plain($handler['title']) . '</strong>',
        ),
        'description' => array(
          '#markup' => check_plain($handler['description']),
        ),
        'enabled' => array(
          '#type' => 'checkbox',
          '#default_value' => isset($settings[$key]['enabled']) ? $settings[$key]['enabled'] : FALSE,
        ),
        'weight' => array(
          '#type' => 'weight',
          '#default_value' => isset($settings[$key]['weight']) ? $settings[$key]['weight'] : 0,
          '#delta' => max($delta, 5),
          '#id' => 'edit-handler-' . $key,
        ),
      );

      // Add edit link
      if ($handler['has edit form']) {
        $form['handlers']['handlers_order'][$key]['operations'] = array(
          'edit-link' => array(
            '#type' => 'link',
            '#title' => t('edit'),
            '#href' => 'admin/config/search/power_menu/handler/edit/' . $key,
          ),
        );
      }
      else {
        $form['handlers']['handlers_order'][$key]['operations'] = array(
          'edit-link' => array(
            '#type' => 'markup',
          ),
        );
      }
    }

    $form['submit'] = array(
      '#type' => 'submit',
      '#value' => t('Save'),
    );
  }

  return $form;
}

/**
 * Handles form submission for power_menu_configuration_form().
 */
function power_menu_configuration_form_submit($form, &$form_state) {
  $handler_enabled = FALSE;

  $settings = array();

  foreach ($form_state['values']['handlers_order'] as $key => $vaues) {
    $settings[$key]['enabled'] = $vaues['enabled'];
    $settings[$key]['weight'] = $vaues['weight'];

    if ($vaues['enabled']) {
      $handler_enabled = TRUE;
    }
  }

  $menus = array();
  $menu_enabled = FALSE;

  foreach ($form_state['values']['menus'] as $key => $vaue) {
    if ($key === $vaue) {
      $menus[] = $vaue;
      $menu_enabled = TRUE;
    }
  }

  variable_set('power_menu_handlers_settings', $settings);
  variable_set('power_menu_handlers_menus', $menus);
  // Is a handler and a menu enabled?
  variable_set('power_menu_handlers_enabled', $handler_enabled && $menu_enabled ? TRUE : FALSE);

  variable_set('power_menu_handlers_breadcrumb', $form_state['values']['set_breadcrumb']);
  variable_set('power_menu_handlers_breadcrumb_title', $form_state['values']['set_breadcrumb_title']);

  drupal_set_message(t('The new Power Menu settings has been saved.'));
}

/**
 * Returns HTML for the power menu plugins order form.
 */
function theme_power_menu_plugins_order($variables) {
  $element = $variables['element'];

  drupal_add_tabledrag('power-menu-plugins', 'order', 'sibling', 'plugin-weight');

  $variables = array(
    'header' => array(
      t('Handlers'),
      t('Description'),
      array(
        'data' => t('Enabled'),
        'class' => array('checkbox'),
      ),
      array(
        'data' => t('Operations'),
        //'colspan' => '2',
      ),
      t('Weight'),
    ),
    'rows' => array(),
    'attributes' => array('id' => 'power-menu-plugins'),
  );

  // Generate table of draggable menu names.
  foreach (element_children($element) as $rule) {
    // Add special classes to be used for tabledrag.js.
    $element[$rule]['weight']['#attributes']['class'] = array('plugin-weight');
    // Render the title, enabled, and weight columns.
    $data = array(
      drupal_render($element[$rule]['title']),
      drupal_render($element[$rule]['description']),
      array(
        'data' => drupal_render($element[$rule]['enabled']),
        'class' => array('checkbox', 'menu-enabled'),
      ),
    );

    // Render the operations links.
    foreach (element_children($element[$rule]['operations']) as $op) {
      $data[] = array(
        'data' => drupal_render($element[$rule]['operations'][$op]),
        'class' => array('menu-operations'),
      );
    }
    $data[] = drupal_render($element[$rule]['weight']);

    $variables['rows'][] = array(
      'data' => $data,
      'class' => array('draggable'),
    );
  }

  return theme('table', $variables);
}

/**
 * Gets the settings form from handler plugins.
 *
 *
 * @param $form_id
 * @param $form_state
 * @param $handler
 *   The key of the handler which form should presented
 * @return
 *   A Drupal form array
 */
function power_menu_configuration_handler_form($form_id, $form_state, $handler) {

  $handlers = power_menu_get_menu_handlers();

  if (!empty($handlers[$handler])) {
    $handler = $handlers[$handler];

    drupal_set_title(t('Power Menu settings for the \'@title\'', array('@title' => $handler['title'])));

    $instance = power_menu_plugin_get_handler_instance($handler);

    if ($instance) {
      $form = $instance->configurationForm();

      $form['buttons']['submit'] = array(
        '#type' => 'submit',
        '#value' => t('Save'),
      );

      // Cancel button
      $form['buttons']['cancel'] = array(
        '#type' => 'submit',
        '#value' => t('Cancel'),
        '#submit' => array('power_menu_configuration_handler_cancel'),
        '#validate' => array(),
      );

      return $form;
    }
  }
}

/**
 * Process handler form validation.
 */
function power_menu_configuration_handler_form_validate(&$elements, &$form_state, $form_id = NULL) {

  $instance = power_menu_plugin_get_handler_instance_by_name(arg(6));

  if ($instance) {
    // Call form validation for given handler instance
    $instance->configurationFormValidate($elements, $form_state, $form_id);
  }
}

/**
 * Process handler form submissions.
 */
function power_menu_configuration_handler_form_submit($form, &$form_state) {
  $instance = power_menu_plugin_get_handler_instance_by_name(arg(6));

  if ($instance) {
    // Call form validation for given handler instance
    $instance->configurationFormSubmit($form, $form_state);
  }

  // No errors? Redirect to Power Menu overview
  if (!form_get_errors()) {
    drupal_set_message(t('Handler settings updated.'));
    drupal_goto('admin/config/search/power_menu');
  }
}

/**
 * Submit callback to cancel handler configuration.
 */
function power_menu_configuration_handler_cancel($form, &$form_state) {
  drupal_goto('admin/config/search/power_menu');
}

/**
 * Form definition for menu fileds configuration.
 */
function power_menu_fields_configuration_form() {

  $form['fields'] = array(
    '#type' => 'fieldset',
    '#title' => t('Fields settings'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
  );

  $header = array(
    'menu_name' => t('Menu name'),
    'operation_field' => t('Fields'),
    'operation_display' => t('Display'),
  );

  $options = array();
  $default_values = array();

  $menus = menu_get_menus();
  $selected_menus = variable_get('power_menu_fields_menus', array());

  foreach ($menus as $key => $value) {

    $options[$key] = array(
      'menu_name' => $value,
      'operation_field' => array(
        'data' => array(
          '#type' => 'link',
          '#title' => t('Manage fields'),
          '#href' => 'admin/config/search/power_menu/fields/' . power_menu_create_machine_name($key) . '/fields',
        ),
      ),
      'operation_display' => array(
        'data' => array(
          '#type' => 'link',
          '#title' => t('Manage display'),
          '#href' => 'admin/config/search/power_menu/fields/' . power_menu_create_machine_name($key) . '/display',
        ),
      ),
    );

    // Add manage fields/display link when menu is selected
    if (in_array($key, $selected_menus)) {
      $default_values[$key] = TRUE;

      $options[$key]['operation_field'] = array(
        'data' => array(
          '#type' => 'link',
          '#title' => t('Manage fields'),
          '#href' => 'admin/config/search/power_menu/fields/' . power_menu_create_machine_name($key) . '/fields',
        ),
      );
      $options[$key]['operation_display'] = array(
        'data' => array(
          '#type' => 'link',
          '#title' => t('Manage display'),
          '#href' => 'admin/config/search/power_menu/fields/' . power_menu_create_machine_name($key) . '/display',
        ),
      );

    }
    else {
      $default_values[$key] = FALSE;
      $options[$key]['operation_field'] = '';
      $options[$key]['operation_display'] = '';
    }
  }

  $form['fields']['menus'] = array(
    '#type' => 'tableselect',
    '#header' => $header,
    '#options' => $options,
    '#empty' => t('No menus available.'),
    '#multiple' => TRUE,
    '#default_value' => $default_values,
    '#js_select' => FALSE,
  );

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save'),
  );

  return $form;
}

/**
 * Handles form submission for power_menu_fields_configuration_form().
 */
function power_menu_fields_configuration_form_submit($form, &$form_state) {
  $menus = array();

  foreach ($form_state['values']['menus'] as $key => $vaue) {
    if ($key === $vaue) {
      $menus[] = $vaue;
    }
  }

  variable_set('power_menu_fields_menus', $menus);

  // Clear the caches that the manage fields link is added
  entity_info_cache_clear();
  menu_rebuild();
  drupal_set_message(t('The new Power Menu fields settings has been saved.'));
}

