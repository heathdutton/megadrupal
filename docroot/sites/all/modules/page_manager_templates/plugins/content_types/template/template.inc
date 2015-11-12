<?php

/**
 * @file
 *
 * This is the content type plugin to allow the use of custom templates.
 */

$plugin = array(
  'title' => t('Template'),
  'description' => t('Use a custom template.'),
  'content_types' => array('template'),
  'defaults' => array(
    'module' => '',
    'theme' => '',
    'module_or_theme' => 'module',
    'template' => '',
    'template_path' => '',
    'hook_theme' => TRUE,
  ),
  'category' => t('Template'),
  'single' => TRUE,
  'admin info' => 'page_manager_templates_template_content_type_admin_info',
);

function page_manager_templates_template_content_type_admin_info($subtype, $conf, $context = NULL) {
  $info = new stdClass();
  $info->title = 'Template';
  $info->content = t('You can use a preprocess function titled @prep for your template @template.', array('@prep' => 'template_preprocess_' . $conf['theme_function'], '@template' => $conf['template']));
  return $info;
}

/**
 * 'Edit' callback for the content type.
 */
function page_manager_templates_template_content_type_edit_form($form, &$form_state) {
  $conf = $form_state['conf'];

  $form['hook_theme'] = array(
    '#title' => t('Provide theme function'),
    '#type' => 'checkbox',
    '#description' => t('Page manager templates can create the hook_theme implementation to register your template file. Uncheck this if you will create your own hook_theme implementation for this theme function.'),
    '#default_value' => $conf['hook_theme'],
  );

  $form['template'] = array(
    '#title' => t('Template name'),
    '#type' => 'textfield',
    '#description' => t('The name of the template or theme function. If you will rely on Page Manager Templates for your hook_theme implementation, enter the name of the new template file excluding its the .tpl.php extension. If you are handling your own hook_theme implementation, enter the name of the theme function.'),
    '#default_value' => $conf['template'],
  );

  ctools_include('dependent');
  $form['module_or_theme'] = array(
    '#title' => t('Will a module or theme contain your template file?'),
    '#type' => 'select',
    '#options' => array('module' => 'module', 'theme' => 'theme'),
    '#default_value' => $conf['module_or_theme'],
    '#dependency' => array('edit-hook-theme' => array(TRUE)),
  );
  $result = db_query("SELECT name FROM {system} WHERE type = 'theme' ORDER BY name ASC");
  $themes = array();
  foreach ($result as $theme) {
    $themes[$theme->name] = $theme->name;
  }
  $form['theme'] = array(
    '#title' => t('Theme containing your template file'),
    '#type' => 'select',
    '#options' => $themes,
    '#default_value' => $conf['theme'],
    '#dependency' => array('edit-hook-theme' => array(TRUE), 'edit-module-or-theme' => array('theme')),
    '#dependency_count' => 2,
  );

  $form['module'] = array(
    '#title' => t('Module containing your template file'),
    '#type' => 'select',
    '#options' => module_list(),
    '#default_value' => $conf['module'],
    '#dependency' => array('edit-hook-theme' => array(TRUE), 'edit-module-or-theme' => array('module')),
    '#dependency_count' => 2,
  );

  $form['template_path'] = array(
    '#title' => t('Sub-directory'),
    '#type' => 'textfield',
    '#default_value' => $conf['template_path'],
    '#dependency' => array('edit-hook-theme' => array(TRUE)),
    '#description' => t('Optional path to the template within the chosen module or theme. e.g. templates'),
  );

  $form['#submit'][] = 'drupal_theme_rebuild';

  return $form;
}

/**
 * The submit form stores the data in $conf.
 */
function page_manager_templates_template_content_type_edit_form_submit($form, &$form_state) {
  foreach (array_keys($form_state['plugin']['defaults']) as $key) {
    if (isset($form_state['values'][$key])) {
      $form_state['conf'][$key] = $form_state['values'][$key];
    }
  }
  $form_state['conf']['template_path'] = $form_state['values']['template_path'];
  $form_state['conf']['theme_function'] = $form_state['values']['template'];
  if ($form_state['values']['hook_theme']) {
    $form_state['conf']['base'] = $form_state['values']['theme'];
    if ($form_state['values']['module_or_theme'] == 'module') {
      $form_state['conf']['base'] = $form_state['values']['module'];
    }
    $form_state['conf']['theme_function'] = $form_state['conf']['base'] . '_template_' . str_replace("-", "_", $form_state['values']['template']);

    // Reset the page manager templates registry.
    cache_clear_all('page_manager_templates_registry', 'cache');
    _page_manager_templates_check_registry();
  }
}

/**
 * Put the output for your content type in the pane object's content.
 */
function page_manager_templates_template_content_type_render($subtype, $conf, $args, $context) {
  $pane = new stdClass();
  if ($conf['hook_theme']) {
    _page_manager_templates_check_registry();
  }
  $pane->content = theme($conf['theme_function'], $context);
  return $pane;
}
