<?php

/**
 * @file
 * Content type plugin to expose rendered content.
 */

$plugin = array(
  'title' => t('Dynamic pane'),
  'defaults' => array(),
  'category' => t('Dynamic panes'),
  'no title override' => TRUE,
  'all contexts' => TRUE,
);

/**
 * Returns an edit form for a entity.
 */
function dynamic_panes_dynamic_panes_content_type_edit_form($form, &$form_state) {
  $conf = $form_state['conf'];
  $form['conf'] = array(
    '#tree' => TRUE,
  );

  ctools_include('plugins');
  $options = array();
  $plugins = ctools_get_plugins('dynamic_panes', 'dynamic_panes_layout_providers');
  foreach ($plugins as $plugin) {
    if ($class = ctools_plugin_get_class($plugin, 'handler')) {
      $reflection = new ReflectionClass($class);
      if ($reflection->isSubclassOf('\Drupal\dynamic_panes\LayoutProvider')) {
        $options[$plugin['name']] = $plugin['title'];
      }
    }
  }

  $form['conf']['layout_provider'] = array(
    '#type' => 'select',
    '#title' => t('Layout provider.'),
    '#default_value' => isset($conf['layout_provider']) ? $conf['layout_provider'] : 0,
    '#options' => $options,
    '#required' => TRUE,
  );

  $form['conf']['level'] = array(
    '#type' => 'select',
    '#title' => t('Level'),
    '#options' => _dynamic_panes_get_config_levels(),
    '#multiple' => TRUE,
    '#default_value' => isset($conf['level']) ? $conf['level'] : NULL,
    '#empty_option' => t('- All -'),
    '#description' => t('Choose blocks of which level should be rendered.'),
  );

  $form['conf']['static_region'] = array(
    '#type' => 'select',
    '#title' => t('Static region'),
    '#options' => dynamic_panes_get_static_regions(),
    '#default_value' => isset($conf['static_region']) ? $conf['static_region'] : NULL,
    '#empty_option' => t('- None -'),
    '#description' => t('Choose blocks of which level should be rendered.'),
  );

  // TODO: Move to plugins as this module shouldn't know about layouts.
  $form['conf']['do_not_render_inheritable'] = array(
    '#type' => 'checkbox',
    '#title' => t('Do not render blocks from parent layouts.'),
    '#default_value' => isset($conf['do_not_render_inheritable']) ? $conf['do_not_render_inheritable'] : 0,
    '#description' => t("If checked then blocks from parent layots wouldn't be rendered."),
  );

  return $form;
}

/**
 * Admin info callback.
 */
function dynamic_panes_dynamic_panes_content_type_admin_info($subtype, $conf) {
  $block = new stdClass();
  $block->title = t('Content from <em>@type</em> region (@region_name) of levels: <em>@level</em>.', array(
    '@type' => dynamic_panes_region_is_static($conf) ? t('static') : t('dynamic'),
    '@level' => !empty($conf['level']) ? implode(', ', $conf['level']) : 'all',
    '@region_name' => dynamic_panes_get_region_title($conf),
  ));
  $block->content = t('This pane renders content from static or dynamic regions.');
  return $block;
}

/**
 * Submit callback for dynamic_panes_dynamic_panes_content_type_edit_form.
 */
function dynamic_panes_dynamic_panes_content_type_edit_form_submit(&$form, &$form_state) {
  if (isset($form_state['values']['conf'])) {
    $form_state['conf'] = $form_state['values']['conf'];
  }
}

/**
 * Checks if region is static.
 */
function dynamic_panes_region_is_static($conf) {
  return !empty($conf['static_region']);
}

/**
 * Gets region.
 */
function dynamic_panes_get_region($conf) {
  return dynamic_panes_region_is_static($conf) ? $conf['static_region'] : $conf['region'];
}

/**
 * Gets region title.
 */
function dynamic_panes_get_region_title($conf) {
  if (dynamic_panes_region_is_static($conf)) {
    $static_regions = dynamic_panes_get_static_regions();
    return isset($static_regions[$conf['static_region']]) ? $static_regions[$conf['static_region']] : t('None');
  }
  else {
    return t('current');
  }
}

/**
 * Implements hook_PLUGIN_content_type_render().
 */
function dynamic_panes_dynamic_panes_content_type_render($entity_type, $conf, $panel_args, $contexts) {
  $block = new stdClass();
  $block->content = dynamic_panes_get_content($contexts, $conf);

  return $block;
}
