<?php

/**
 * @file Helper pages of cat_items.
 *
 * @author Oleksiy Kalinichenko <aexchecker@gmail.com>
 */

/**
 * Form for editing CAT item.
 */
function cat_ui_edit_page($cat_item) {
  $page = array(
    'content' => array(
      '#prefix' => '<div id="cat-ui-wrapper" class="clearfix">',
      '#suffix' => '</div>',
    )
  );

  // Save cat_item in cache.
  cat_object_cache_set($cat_item->machine_name, $cat_item);

  $links[] = array(
    '#type' => 'link',
    '#title' => t('Basic'),
    '#href' => "admin/structure/cat/edit/{$cat_item->machine_name}/ajax/1",
    '#attributes' => array(
      'class' => array(
        'use-ajax',
        'active',
      ),
    ),
  );

  $links[] = array(
    '#type' => 'link',
    '#title' => t('Arguments'),
    '#href' => "admin/structure/cat/edit/{$cat_item->machine_name}/ajax/2",
    '#attributes' => array(
      'class' => array(
        'use-ajax',
      ),
    ),
  );

  $links[] = array(
    '#type' => 'link',
    '#title' => t('Selection rules'),
    '#href' => "admin/structure/cat/edit/{$cat_item->machine_name}/ajax/3",
    '#attributes' => array(
      'class' => array(
        'use-ajax',
      ),
    ),
  );

  $links[] = array(
    '#type' => 'link',
    '#title' => t('CAT item settings'),
    '#href' => "admin/structure/cat/edit/{$cat_item->machine_name}/ajax/4",
    '#attributes' => array(
      'class' => array(
        'use-ajax',
      ),
    ),
  );

  $rendered_links = array();
  foreach ($links as $link) {
    $rendered_links[] = render($link);
  }

  $page['content']['menu'] = array(
    '#prefix' => '<div id="cat-ui-menu">',
    '#markup' => theme('item_list', array('items' => $rendered_links)),
    '#suffix' => '</div>',
  );

  $form_state = array(
    'storage' => array(
      'machine_name' => $cat_item->machine_name,
      'step' => 1,
    ),
    'build_info' => array(
      'args' => array(),
    ),
  );

  // Add form inc file, also see module_load_include()
  form_load_include($form_state, 'inc', 'cat_ui', 'cat_ui.add_form');
  form_load_include($form_state, 'inc', 'cat_ui', 'cat_ui.edit_form');

  $form = drupal_build_form('cat_ui_edit_form', $form_state);
  $form['#attached']['library'][] = array('system', 'ui.autocomplete');
  $form['#attached']['library'][] = array('system', 'drupal.ajax');

  $page['#attached']['css'][] = drupal_get_path('module', 'cat_ui') . '/cat_ui.css';

  $page['content']['edit_form'] = array(
    '#markup' => render($form)
  );

  return $page;
}

/**
 * Menu item title callback for CAT item edit.
 */
function cat_ui_edit_page_title($cat_item) {
  if (!empty($cat_item->name)) {
    return t('Edit CAT item "!name"', array('!name' => $cat_item->name));
  }
  else {
    return t('Edit CAT item');
  }
}

/**
 * Disables CAT item.
 */
function cat_ui_disable_cat_page($cat_item) {
  ctools_include('export');
  ctools_export_crud_disable('cat_items', $cat_item);
  cache_clear_all('cat_items', 'cache', TRUE);

  drupal_set_message(t('The CAT item "%name" was disabled.', array('%name' => $cat_item->name)));

  drupal_goto('admin/structure/cat');
}

/**
 * Enables CAT item.
 */
function cat_ui_enable_cat_page($cat_item) {
  ctools_include('export');
  ctools_export_crud_enable('cat_items', $cat_item);
  cache_clear_all('cat_items', 'cache', TRUE);

  drupal_set_message(t('The CAT item "%name" was enabled', array('%name' => $cat_item->name)));

  drupal_goto('admin/structure/cat');
}

/**
 * Form for CAT item clone.
 */
function cat_ui_clone_cat_page($cat_item) {
  unset($cat_item->path_id);

  $cat_item->export_type = NULL;
  $cat_item->machine_name .= '_clone';
  $cat_item->name = t('!cat_item_name (clone)', array('!cat_item_name' => filter_xss($cat_item->name)));

  cat_object_cache_set($cat_item->machine_name, $cat_item);

  $form_state = array(
    'storage' => array(
      'machine_name' => $cat_item->machine_name,
    ),
    'build_info' => array(
      'args' => array(),
    ),
  );

  // Add form inc file, also see module_load_include()
  form_load_include($form_state, 'inc', 'cat_ui');
  form_load_include($form_state, 'inc', 'cat_ui', 'cat_ui.add_form');
  form_load_include($form_state, 'inc', 'cat_ui', 'cat_ui.edit_form');

  return drupal_build_form('cat_ui_add_form', $form_state);
}

/**
 * Menu item title callback for CAT item export.
 */
function cat_ui_export_page_title($cat_item) {
  if (is_object($cat_item)) {
    return t('Export CAT item "!name"', array('!name' => $cat_item->name));
  }
  else {
    return t('Export CAT item');
  }
}

/**
 * Form for object export.
 */
function cat_ui_export_form($form, $form_state, $cat_item) {
  ctools_include('export');

  $code = ctools_export_crud_export('cat_items', $cat_item);
  $lines = substr_count($code, "\n");

  $form['export'] = array(
    '#title' => t('Export CAT item data'),
    '#type' => 'textarea',
    '#value' => $code,
    '#rows' => $lines,
    '#description' => t('Copy the export text and paste it into import area.'),
  );

  return $form;
}

/**
 * AJAX callback for arguments configuration in path.
 * This is help function for SECOND STEP.
 *
 * @param string $path_name
 * @param string $keyword
 *
 * @return array AJAX commands array.
 */
function cat_ui_configure_arguments_page($path_name, $keyword) {
  ctools_include('modal');

  $form_state = array(
    'storage' => array(
      'path_name' => $path_name,
      'keyword' => $keyword,
    ),
    'build_info' => array(
      'args' => array(),
    ),
  );

  $form = drupal_build_form('cat_ui_argument_context_form', $form_state);

  $form_state = array(
    'title' => t('Choose argument'),
  );

  return array(
    '#type' => 'ajax',
    '#commands' => ctools_modal_form_render($form_state, $form),
  );
}

/**
 * AJAX form that allows to match arguments with contexts.
 * This is helper form for FIRST and SECOND steps.
 *
 * @param array $form
 * @param array $form_state
 *
 * @return array Drupal form array.
 */
function cat_ui_argument_context_form($form, &$form_state) {
  // Add form inc file, also see module_load_include()
  form_load_include($form_state, 'inc', 'cat_ui');

  $form['#prefix'] = '<div id="cat-ui-argument-context-form-wrapper">';
  $form['#suffix'] = '</div>';

  $form['#tree'] = TRUE;

  // Load values from form storage.
  $path_name = $form_state['storage']['path_name'];
  $keyword = $form_state['storage']['keyword'];

  if (!($cat_item = cat_object_cache_get($path_name))) {
    return array();
  }

  // Get current form step.
  if (!isset($form_state['storage']['step'])) {
    $form_state['storage']['step'] = 1;
  }

  // Include ctools library for contexts.
  ctools_include('context');

  switch ($form_state['storage']['step']) {
    // Show form with selection radios.
    case 1 :
      // Get ctools plugins for arguments.
      $options = array();
      $plugins = ctools_get_arguments();
      foreach ($plugins as $id => $plugin) {
        if (empty($plugin['no ui'])) {
          $options[$id] = $plugin['title'];
        }
      }

      asort($options);

      $form['argument'] = array(
        '#type' => 'radios',
        '#options' => (array('' => t('No context selected')) + $options),
      );

      // Apply default value for argument.
      if (isset($cat_item->arguments[$keyword]['argument'])) {
        $form['argument']['#default_value'] = $cat_item->arguments[$keyword]['argument'];
      }

      // Attach buttons to form.
      _cat_ui_form_attach_buttons($form, array('next'), 'cat_ui_argument_context_form');

      break;

    case 2 :
      // Load argument's plugin.
      $plugins = ctools_get_arguments();
      $plugin = $plugins[$form_state['storage']['argument']];

      $form['settings']['identifier'] = array(
        '#title' => t('Context identifier'),
        '#type' => 'textfield',
        '#default_value' => $plugin['title'],
      );

      // Apply plugin's settings form to previously builded form.
      if ($function = (string) ctools_plugin_get_function($plugin, 'settings form')) {
        $settings_form = $settings_form_state = array();
        $function($settings_form, $settings_form_state);
        $form['settings'] += $settings_form['settings'];
      }

      // Apply argument's stored values as default form values.
      if (!empty($cat_item->arguments[$keyword]['settings'])) {
        foreach ($cat_item->arguments[$keyword]['settings'] as $name => $value) {
          if (isset($form['settings'][$name])) {
            $form['settings'][$name]['#default_value'] = $value;
          }
        }
      }

      // Attach buttons to the form.
      _cat_ui_form_attach_buttons($form, array('prev', 'finish'), 'cat_ui_argument_context_form');

      break;
  }

  return $form;
}

/**
 * AJAX submit callback for contexts selection form.
 *
 * @param array $form
 * @param array $form_state
 */
function cat_ui_argument_context_form_submit($form, &$form_state) {
  $values = $form_state['values'];
  $actions = $form_state['values']['actions'];
  $keyword = $form_state['storage']['keyword'];
  $path_name = $form_state['storage']['path_name'];

  if (!($cat_item = cat_object_cache_get($path_name))) {
    return;
  }

  // Save keyword's argument.
  if (!empty($values['argument'])) {
    // Unset argument's settings and selection rules if it was changed.
    if (!empty($cat_item->arguments[$keyword]['argument'])) {
      if ($cat_item->arguments[$keyword]['argument'] != $values['argument']) {
        // Unset arguments settings.
        unset($cat_item->arguments[$keyword]['settings']);

        // Search for selection rules for old argument.
        if (!empty($cat_item->access['plugins'])) {
          foreach ($cat_item->access['plugins'] as $key => $access) {
            if ($access['context'] == $keyword) {
              unset($cat_item->access['plugins'][$key]);
            }
          }
        }
      }
    }

    // Save new argument.
    $cat_item->arguments[$keyword]['argument'] = $values['argument'];

    // Save argument in storage.
    $form_state['storage']['argument'] = $values['argument'];
  }

  // Save argument's settings.
  if (!empty($values['settings'])) {
    $cat_item->arguments[$keyword]['settings'] = $values['settings'];
  }

  // Save CAT item in cache.
  cat_object_cache_set($path_name, $cat_item);

  // Switch current step.
  if (isset($actions['next']) && $values['op'] == $actions['next']) {
    $form_state['storage']['step']++;
  }
  elseif (isset($actions['prev']) && $values['op'] == $actions['prev']) {
    $form_state['storage']['step']--;
  }
  elseif (isset($actions['finish']) && $values['op'] == $actions['finish']) {
    $form_state['rebuild'] = FALSE;
  }

  $form_state['rebuild'] = TRUE;
}

/**
 * AJAX callback for contexts selection form.
 *
 * @param array $form
 * @param array $form_state
 *
 * @return array mixed
 */
function cat_ui_argument_context_form_ajax_callback($form, $form_state) {
  if (!form_get_errors()) {
    $values = $form_state['values'];
    $actions = $form_state['values']['actions'];
    $path_name = $form_state['storage']['path_name'];

    if (isset($actions['finish']) && $values['op'] == $actions['finish']) {
      ctools_include('modal');

      // Update second step of CAT item add form.
      $form_state = array(
        'storage' => array(
          'step' => 2,
          'machine_name' => $path_name,
        ),
        'build_info' => array(
          'args' => array(),
        ),
      );

      $form = drupal_build_form('cat_ui_add_form', $form_state);

      $commands[] = ajax_command_replace('#cat-ui-argument-table', drupal_render($form['table']));
      $commands[] = ctools_modal_command_dismiss();

      return array(
        '#type' => 'ajax',
        '#commands' => $commands,
      );
    }
  }

  return $form;
}

/**
 * AJAX callback for autocomplete path.
 */
function cat_ui_autocomplete_path() {
  $matches = array();

  if (!empty($_SERVER['REQUEST_URI'])) {
    $autocomplete_path = ltrim($_SERVER['REQUEST_URI'], '/');
    $autocomplete_path = str_replace('cat/autocomplete', '', $autocomplete_path);

    if ($real_path = cat_path_router($autocomplete_path)) {
      // Select drupal paths
      $result = db_select('menu_router', 'mr')
        ->fields('mr', array('path', 'load_functions'))
        ->condition('mr.path', db_like($real_path) . '%', 'LIKE')
        ->range(0, 30)
        ->execute();

      // add drupal paths to $matches
      foreach ($result as $path) {
        if (!empty($path->load_functions)) {
          $load_functions = unserialize($path->load_functions);
          $load_path = explode('/', $path->path);

          foreach ($load_functions as $arg => $load_function) {
            if (is_array($load_function)) {
              $load_function = array_keys($load_functions[$arg]);
              $load_function = array_shift($load_function);
              if ($load_function === 'pm_arg_load') {
                // try replace path keywords.
                if ($load_path[0] === 'node' && $arg == 1) {
                  $load_function = 'node_load';
                }
              }
            }
            if (is_string($load_function)) {
              $load_path[$arg] = '%' . substr($load_function, 0, -5); // remove '_load'
            }
          }

          $load_path = implode('/', $load_path);
          $matches[$load_path] = $load_path;
        }
        else {
          $matches[$path->path] = $path->path;
        }
      }
    }
  }

  drupal_json_output($matches);
}

/**
 * AJAX callback for settings form for arguments.
 *
 * @param string $path_name
 * @param string $keyword
 *
 * @return array AJAX commands array.
 */
function cat_ui_change_arguments_page($path_name, $keyword) {
  ctools_include('modal');

  $cat_item = cat_object_cache_get($path_name);

  // Set form values for second step of context settings form.
  $form_state = array(
    'storage' => array(
      'step' => 2,
      'path_name' => $path_name,
      'keyword' => $keyword,
      'argument' => $cat_item->arguments[$keyword]['argument'],
    ),
    'build_info' => array(
      'args' => array(),
    ),
  );

  $form = drupal_build_form('cat_ui_argument_context_form', $form_state);

  $form_state = array(
    'title' => t('Choose argument'),
  );

  return array(
    '#type' => 'ajax',
    '#commands' => ctools_modal_form_render($form_state, $form),
  );
}

