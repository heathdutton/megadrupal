<?php

/**
 * @file
 * Share text selections on social networks admin pages.
 */

/**
 * Share text selections config form.
 */
function share_selection_config_form($form, &$form_state) {
  $content_types = entity_get_info('node');
  $content_types_options = array();
  $content_types_options_default = array();
  $share_by = array(
    'paths' => t('Paths'),
    'content_types' => t('Content types'),
  );
  $exclude_paths = 'admin/*';
  foreach ($content_types['bundles'] as $bundle_machine_name => $bundle) {
    $content_types_options[$bundle_machine_name] = $bundle['label'];
    $content_types_options_default[$bundle_machine_name] = $bundle_machine_name;
  }
  $form['share_selection_paths_or_content'] = array(
    '#type' => 'radios',
    '#options' => $share_by,
    '#title' => t('Show by paths or content types'),
    '#description' => t('Select how you want to control the places to share selection'),
    '#default_value' => variable_get('share_selection_paths_or_content', 'paths'),
  );
  // Share by content options.
  $form['by_content'] = array(
    '#type' => 'fieldset',
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
    '#title' => t('By content options'),
    '#states' => array(
      'visible' => array(
        ':input[name="share_selection_paths_or_content"]' => array('value' => 'content_types'),
      ),
    ),
  );
  $form['by_content']['share_selection_content_types'] = array(
    '#type' => 'checkboxes',
    '#options' => $content_types_options,
    '#title' => t('Content types'),
    '#description' => t('Content types where links will be shown'),
    '#default_value' => variable_get('share_selection_content_types', $content_types_options_default),
  );
  // Share by paths options.
  $paths_behaviors = array(
    0 => t('All pages except those listed'),
    1 => t('Only the listed pages'),
  );
  $form['by_paths'] = array(
    '#type' => 'fieldset',
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
    '#title' => t('By paths options'),
    '#states' => array(
      'visible' => array(
        ':input[name="share_selection_paths_or_content"]' => array('value' => 'paths'),
      ),
    ),
  );
  $form['by_paths']['share_selection_paths_behavior'] = array(
    '#type' => 'radios',
    '#options' => $paths_behaviors,
    '#title' => t('Show on specific pages'),
    '#default_value' => variable_get('share_selection_paths_behavior', 0),
  );
  $form['by_paths']['share_selection_paths'] = array(
    '#type' => 'textarea',
    '#default_value' => variable_get('share_selection_paths', $exclude_paths),
    '#description' => t("Specify pages by using their paths. Enter one path per line. The '*' character is a wildcard. Example paths are %blog for the blog page and %blog-wildcard for every personal blog. %front is the front page.", array('%blog' => 'blog', '%blog-wildcard' => 'blog/*', '%front' => '<front>')),
  );
  // Exclude roles.
  $user_roles = user_roles();
  $form['exclude_roles'] = array(
    '#type' => 'fieldset',
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
    '#title' => t('Exclude Roles'),
  );
  $form['exclude_roles']['share_selection_exclude_roles'] = array(
    '#type' => 'checkboxes',
    '#options' => $user_roles,
    '#description' => t("The selected roles won't see the Share Selection buttons."),
    '#default_value' => variable_get('share_selection_exclude_roles', array()),
  );
  // Display options.
  $display_options = array(
    'image' => t('Only image'),
    'text' => t('Only text'),
    'image_and_text' => t('Image and text'),
  );
  $form['display_options'] = array(
    '#type' => 'fieldset',
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
    '#title' => t('Styling options'),
  );
  $form['display_options']['share_selection_images_replacement_path'] = array(
    '#type' => 'textfield',
    '#title' => t('Images replacement path'),
    '#default_value' => variable_get('share_selection_images_replacement_path', 'sites/default/files/share-selection'),
  );
  $form['display_options']['share_selection_display_style'] = array(
    '#type' => 'select',
    '#title' => t('Style'),
    '#options' => $display_options,
    '#default_value' => variable_get('share_selection_display_style', 'image'),
  );
  return system_settings_form($form);
}

/**
 * Share text selections services form.
 */
function share_selection_config_services_form() {
  $settings = array();

  $services = share_selection_get_links(NULL, TRUE);
  $settings['show'] = variable_get('share_selection_show', NULL);
  $settings['weight'] = variable_get('share_selection_weight', NULL);
  $settings['custom'] = variable_get('share_selection_custom', NULL);

  $form['share_selection'] = array('#theme' => 'share_selection_services_drag_table');
  $form['share_selection']['share_selection_show'] = array('#tree' => TRUE);
  $form['share_selection']['share_selection_weight'] = array('#tree' => TRUE);
  // Custom service options.
  $form['share_selection_custom'] = array(
    '#type' => 'fieldset',
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
    '#title' => t('Custom services options'),
    '#description' => t('Set the custom options per service.'),
    '#tree' => TRUE,
  );
  foreach ($services as $service_id => $service) {
    $icon_path = drupal_get_path('module', $service['module']) . '/images/' . $service['icon'];
    $icon = array(
      'path' => isset($service['icon']) ? $icon_path : '',
    );
    $weight = isset($settings['weight'][$service_id]) ? $settings['weight'][$service_id] : 0;

    $form['share_selection']['share_selection_show'][$service_id] = array(
      '#service' => ucwords(str_replace('_', ' ', $service['module'])),
      '#weight' => $weight,
      '#type' => 'checkbox',
      '#title' => theme('image', $icon) . " " . t('Show %name', array('%name' => $service['name'])),
      '#return_value' => 1,
      '#default_value' => isset($settings['show'][$service_id]) ? $settings['show'][$service_id] : 0,
    );
    $form['share_selection']['share_selection_weight'][$service_id] = array(
      '#type' => 'weight',
      '#delta' => 100,
      '#default_value' => $weight,
    );

    if (isset($service['custom_options'])) {
      $form['share_selection_custom'][$service_id] = array(
        '#type' => 'fieldset',
        '#collapsible' => TRUE,
        '#collapsed' => FALSE,
        '#title' => $service['name'],
      );
      foreach ($service['custom_options'] as $option_key => $option_label) {
        $form['share_selection_custom'][$service_id][$option_key] = array(
          '#type' => 'textfield',
          '#title' => $option_label,
          '#default_value' => isset($settings['custom'][$service_id][$option_key]) ? $settings['custom'][$service_id][$option_key] : '',
          '#size' => '60',
        );
      }
    }
  }
  $form['replacement_tokens'] = array(
    '#type' => 'fieldset',
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
    '#title' => t('Replacement tokens'),
  );
  $form['replacement_tokens']['tokens'] = array(
    '#theme' => 'token_tree',
    '#token_types' => array('node'),
    '#global_types' => TRUE,
    '#click_insert' => TRUE,
  );
  return system_settings_form($form);
}

/**
 * Theming function for build a draggable service table.
 */
function theme_share_selection_services_drag_table($variables) {
  $form = $variables['form'];
  $table['rows'] = array();

  foreach (element_children($form['share_selection_show']) as $service_id) {
    $service = $form['share_selection_show'][$service_id];
    $col = array();

    $service['name'] = array('#markup' => $service['#title']);
    $col[] = drupal_render($service['name']);

    $service['service'] = array('#markup' => $service['#service']);
    $col[] = drupal_render($service['service']);

    $service['show'] = array(
      '#type' => 'checkbox',
      '#checked' => $service['#default_value'],
      '#id' => $service['#id'],
      '#name' => $service['#name'],
    );
    $col[] = drupal_render($service['show']);

    $service['weight'] = $form['share_selection_weight'][$service_id];
    $service['weight']['#attributes']['class'] = array('service-weight');
    $col[] = drupal_render($service['weight']);

    $table['weights'][] = $service['#weight'];
    $table['rows'][] = array('data' => $col, 'class' => array('draggable'));
  }

  $table['header'] = array(t('Service Name'), t('Group'), t('Show'), t('Weight'));

  if (!empty($table['rows'])) {
    array_multisort($table['weights'], $table['rows']);
  }

  drupal_add_tabledrag('share_selection', 'order', 'sibling', 'service-weight');

  $params = array(
    '#theme' => 'table',
    '#header' => $table['header'],
    '#rows' => $table['rows'],
    '#attributes' => array('id' => 'share_selection'),
    '#empty' => t('No services available.'),
  );

  return theme('table', $params);
}
