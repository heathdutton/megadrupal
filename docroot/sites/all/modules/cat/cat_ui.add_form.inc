<?php

/**
 * @file The "Add form" of cat_items.
 *
 * @author Oleksiy Kalinichenko <aexchecker@gmail.com>
 */

/**
 * Provides multistep form for create/edit CAT item.
 *
 * @param array $form
 * @param array $form_state
 * @param bool $show_navigation
 * @param string $class
 *
 * @return array
 */
function cat_ui_add_form($form, &$form_state, $show_navigation = TRUE, $class = '') {
  // Add form inc file, also see module_load_include()
  form_load_include($form_state, 'inc', 'cat_ui');
  form_load_include($form_state, 'inc', 'cat_ui', 'cat_ui.add_form');

  // Add UI ajax wraper
  $form['#prefix'] = '<div id="cat-ui-form-wrapper" class="' . $class . '">';
  $form['#suffix'] = '</div>';

  if (!isset($form_state['storage']['step'])) {
    $form_state['storage']['step'] = 1;
  }

  $step = $form_state['storage']['step'];

  if ($show_navigation) {
    $navigation = array(
      t('Basic settings'),
      t('Choose arguments'),
      t('Selection rules'),
      t('CAT settings'),
    );
    $navigation[$step - 1] = '<span class="current-nav-item">' . $navigation[$step - 1] . '</span>';
    $form['navigation'] = array(
      '#type' => 'item',
      '#markup' => implode(' » ', $navigation),
    );
  }

  $form_build_function = '';
  switch ($step) {
    case 1 :
      $form_build_function = 'basic_settings';
      break;

    case 2 :
      $form_build_function = 'arguments_selection';
      break;

    case 3 :
      $form_build_function = 'selection_rules';
      break;

    case 4 :
      $form_build_function = 'cat_settings';
      break;
  }

  $form['#validate'][] = 'cat_ui_add_form_validate';
  $form['#submit'][] = 'cat_ui_add_form_submit';
  $form['#attached']['css'][] = drupal_get_path('module', 'cat_ui') . '/cat_ui.css';

  // Build form according to current step.
  $function = "_cat_ui_form_step_{$step}_{$form_build_function}";
  if (function_exists($function)) {
    $function($form, $form_state);
  }

  return $form;
}

/**
 * Validate callback for cat_ui_add_form().
 *
 * @param array $form
 * @param array $form_state
 */
function cat_ui_add_form_validate($form, $form_state) {
  if (!empty($form_state['storage']['machine_name'])) {
    $machine_name = $form_state['storage']['machine_name'];
  }
  elseif (!empty($form_state['values']['machine_name'])) {
    $machine_name = $form_state['values']['machine_name'];
  }

  if (!form_get_errors() && $form_state['build_info']['form_id'] == 'cat_ui_add_form') {
    if (!empty($machine_name) && ($cat_item = cat_load_by_name($machine_name))) {
      $message = t('CAT item <a href="@edit_url">%name</a> with the same machine name %machine_name already exists.', array(
        '@edit_url' => url('admin/structure/cat/edit/' . $cat_item->machine_name),
        '%name' => filter_xss($cat_item->name),
        '%machine_name' => $cat_item->machine_name,
      ));

      form_set_error('', $message);
    }
  }
}

/**
 * Submit callback for cat_ui_add_form().
 *
 * @param array $form
 * @param array $form_state
 */
function cat_ui_add_form_submit($form, &$form_state) {
  $values = $form_state['values'];

  if (!empty($form_state['storage']['machine_name']) && !empty($values['machine_name'])) {
    // Move temporary cache to new storage if machine name was changed..
    if ($form_state['storage']['machine_name'] != $values['machine_name']) {
      $cat_item = cat_object_cache_get($form_state['storage']['machine_name']);
      cat_object_cache_clear($form_state['storage']['machine_name']);
      cat_object_cache_set($values['machine_name'], $cat_item);
      $form_state['storage']['machine_name'] = $values['machine_name'];
    }
  }
  elseif (!empty($values['machine_name'])) {
    // Save machine name in form storage.
    $form_state['storage']['machine_name'] = $values['machine_name'];
  }

  $machine_name = $form_state['storage']['machine_name'];
  if (!($cat_item = cat_object_cache_get($machine_name))) {
    $cat_item = array();
  }
  $form_state['values']['cat_original'] = $cat_item;
  $cat_item = array_merge((array) $cat_item, $values);
  cat_object_cache_set($machine_name, $cat_item);

  // Switch current step.
  if (isset($values['next']) && $values['op'] == $values['next']) {
    $form_state['storage']['step']++;
  }
  elseif (isset($values['prev']) && $values['op'] == $values['prev']) {
    $form_state['storage']['step']--;
  }
  elseif (isset($values['finish']) && $values['op'] == $values['finish']) {
    $form_state['rebuild'] = FALSE;
  }

  $form_state['rebuild'] = TRUE;
}

/**
 * AJAX callback for cat_ui_add_form().
 *
 * @param array $form
 * @param array $form_state
 *
 * @return array
 */
function cat_ui_add_form_ajax_callback($form, $form_state) {
  // Simple reload form.
  return $form;
}

/**
 * FIRST STEP of the cat_ui_add_form().
 * Provide form with basic settings.
 *
 * @param array $form
 * @param array $form_state
 */
function _cat_ui_form_step_1_basic_settings(&$form, &$form_state) {
  $form['name'] = array(
    '#type' => 'textfield',
    '#title' => t('CAT item definition'),
    '#description' => t('Human-readable name of CAT item.'),
    '#required' => TRUE,
    '#maxlength' => 64,
  );

  $form['machine_name'] = array(
    '#type' => 'machine_name',
    '#maxlength' => 64,
    '#machine_name' => array(
      'exists' => 'cat_load_by_name',
      'source' => array('name'),
    ),
  );

  $form['path_original'] = array(
    '#type' => 'textfield',
    '#title' => t('Path'),
    '#description' => t('The URL non-aliased path where CAT items will be shown. You may create named placeholders for variable parts of the path by using %name for required elements. For example: "node/%node/foo" or "forum/%forum". These named placeholders can be turned into contexts on the arguments form.'),
    '#required' => TRUE,
    '#maxlength' => 255,
    '#autocomplete_path' => 'cat/autocomplete',
    '#field_prefix' => (url(NULL, array('absolute' => TRUE)) . (variable_get('clean_url', FALSE) ? '' : '?q=')),
  );
  
  $form['catch_subpaths'] = array(
    '#type' => 'checkbox',
    '#title' => t('Catch subpaths'),
    '#description' => t('If this is checked, this cat item will match for all sub paths of the entered path above.'),
  );

  $form['category'] = array(
    '#type' => 'textfield',
    '#title' => t('Category'),
    '#description' => t("The category that this cat-item will be grouped into on the CAT list form. Only upper and lower-case alphanumeric characters are allowed. If left blank, defaults to 'CAT items'."),
    '#maxlength' => 64,
  );

  $form['#validate'][] = '_cat_ui_form_step_1_basic_settings_validate';
  $form['#submit'][] = '_cat_ui_form_step_1_basic_settings_submit';

  if (!empty($form_state['storage']['machine_name'])) {
    _cat_ui_form_apply_default_values($form, $form_state['storage']['machine_name']);
  }

  _cat_ui_form_attach_buttons($form, array('next'), 'cat_ui_add_form');
}

/**
 * Validate function for basic settings form (first step).
 *
 * @param array $form
 * @param array $form_state
 */
function _cat_ui_form_step_1_basic_settings_validate(&$form, &$form_state) {
  $path_url = drupal_parse_url($form_state['values']['path_original']);
  $path = explode('/', $path_url['path']);
  $correct_path = array();
  foreach ($path as $position => $bit) {
    if (!($trimmed_bit = trim($bit))) {
      continue;
    }
    $correct_path[] = $trimmed_bit;
    if ($trimmed_bit[0] == '%' && $position == 0) {
      form_error($form['path'], t('The first element in a path may not be dynamic.'));
    }
  }

  // Save correct path.
  $form_state['values']['path'] = implode('/', $correct_path);
  $form_state['values']['path_original'] = $form_state['values']['path_original'];
  $form_state['values']['path_router'] = cat_path_router($path_url['path']);
  $form_state['values']['path_pattern'] = cat_path_pattern($path_url['path']);
  $form_state['values']['path_query'] = $path_url['query'];

  $arguments = array();
  $with_keywords = variable_get('cat_item_with_keywords', FALSE);

  // Add drupal path arguments
  foreach ($correct_path as $position => $bit) {
    if ($with_keywords) {
      if ($bit === '%') {
        form_error($form['path'], t('Invalid arg <em>%</em>. All arguments must be named with keywords.'));
      }
      elseif (substr($bit, 0, 1) === '%') {
        $bit_name = substr($bit, 1);
        $arguments[$bit_name] = array(
          'argument_name' => $bit_name,
          'argument_type' => 'path',
          'position' => $position,
        );
      }
    }
    else {
      if (substr($bit, 0, 1) === '%') {
        $arguments[$position] = array(
          'argument_name' => substr($bit, 1),
          'argument_type' => 'path',
          'position' => $position,
        );
      }
    }
  }

  // Add $_GET (query) arguments
  foreach ($path_url['query'] as $position => $bit) {
    if (substr($bit, 0, 1) === '%') {
      $arguments[$position] = array(
        'argument_name' => substr($bit, 1),
        'argument_type' => 'query',
        'position' => $position,
      );
    }
  }

  // Save keywords.
  $form_state['values']['arguments'] = $arguments;
}

/**
 * Submit function for basic settings form (first step).
 *
 * @param array $form
 * @param array $form_state
 *
 * @return mixed
 */
function _cat_ui_form_step_1_basic_settings_submit($form, &$form_state) {
  if ($cat_item = cat_object_cache_get($form_state['storage']['machine_name'])) {
    if (!isset($cat_item->arguments)) {
      $cat_item->arguments = array();
    }

    $cat_original = !empty($form_state['values']['cat_original']) ? $form_state['values']['cat_original'] : false;  //this is needed as we don't have the arguments setting here, so we will add them
    $arguments = array();

    foreach ($form_state['values']['arguments'] as $position => $argument) {
      if (!empty($cat_original->arguments[$position])) {
        $arguments[$position] = array_merge($cat_original->arguments[$position], $argument);
      }
      else {
        $arguments[$position] = $argument;
      }
    }

    $cat_item->arguments = $arguments;
    $cat_item->catch_subpaths = $form_state['values']['catch_subpaths'];

    cat_object_cache_set($form_state['storage']['machine_name'], $cat_item);
  }
}

/**
 * SECOND STEP of the cat_ui_add_form().
 * Provide form for arguments selection.
 *
 * @param array $form
 * @param array $form_state
 *
 * @return mixed
 */
function _cat_ui_form_step_2_arguments_selection(&$form, &$form_state) {
  if (!($cat_item = cat_object_cache_get($form_state['storage']['machine_name']))) {
    return FALSE;
  }

  $form['table'] = array(
    '#theme' => 'cat_ui_form_step_arguments_selection_table',
    '#cat-path' => $cat_item->path,
    'argument' => array(),
  );

  foreach ($cat_item->arguments as $keyword => $argument) {
    $context_title = t('No context assigned');
    if (!empty($argument['argument'])) {
      ctools_include('context');
      $plugin = ctools_get_argument($argument['argument']);
      if (!empty($argument['settings']['identifier'])) {
        $context_title = $argument['settings']['identifier'];
      }
      elseif (!empty($plugin['title'])) {
        $context_title = $plugin['title'];
      }
    }

    $form['table']['argument'][$keyword]['#keyword'] = $argument['argument_name'];
    $form['table']['argument'][$keyword]['#keyword-type'] = $argument['argument_type'];
    $form['table']['argument'][$keyword]['#position'] = t('!position of !type', array(
      '!position' => $argument['position'],
      '!type' => $argument['argument_type'],
    ));
    $form['table']['argument'][$keyword]['#context'] = $context_title;
    $form['table']['argument'][$keyword]['change-url'] = array(
      '#type' => 'hidden',
      '#value' => url("admin/structure/cat/arguments/settings/{$cat_item->machine_name}/{$keyword}", array('absolute' => TRUE)),
      '#attributes' => array(
        'class' => array(
          "page-manager-context-{$keyword}-change-url",
        ),
      ),
    );

    $form['table']['argument'][$keyword]['change'] = array(
      '#type' => 'submit',
      '#value' => t('Change'),
      '#attributes' => array(
        'class' => array(
          'ctools-use-modal',
        ),
      ),
      '#id' => "page-manager-context-{$keyword}-change",
    );

    if (!empty($plugin)) {
      $form['table']['argument'][$keyword]['settings-url'] = array(
        '#type' => 'hidden',
        '#value' => url("admin/structure/cat/arguments/change/{$cat_item->machine_name}/{$keyword}", array('absolute' => TRUE)),
        '#attributes' => array(
          'class' => array(
            "page-manager-context-{$keyword}-settings-url",
          ),
        ),
      );

      $form['table']['argument'][$keyword]['settings'] = array(
        '#type' => 'submit',
        '#value' => t('Settings'),
        '#id' => "page-manager-context-{$keyword}-settings",
        '#attributes' => array(
          'class' => array(
            'ctools-use-modal',
          ),
        ),
      );
    }
  }

  _cat_ui_form_attach_buttons($form, array('next', 'prev'), 'cat_ui_add_form');
}

/**
 * THIRD STEP.
 * Provide form with selection rules.
 *
 * @param array $form
 * @param array $form_state
 *
 * @return mixed
 */
function _cat_ui_form_step_3_selection_rules(&$form, &$form_state) {
  if (!($cat_item = cat_object_cache_get($form_state['storage']['machine_name']))) {
    return $cat_item;
  }

  if (!empty($cat_item->access)) {
    $form_state['access'] = $cat_item->access;
  }
  else {
    $form_state['access'] = array();
  }

  ctools_include('modal');
  ctools_include('ajax');
  ctools_include('context');
  ctools_include('context-access-admin');
  ctools_modal_add_plugin_js(ctools_get_access_plugins());

  // Build form states for selection rules.
  $contexts = cat_get_contexts_from_arguments($cat_item->arguments, TRUE);
  $form_state['module'] = 'cat_ui';
  $form_state['callback argument'] = $form_state['storage']['machine_name'];
  $form_state['no buttons'] = TRUE;
  $form_state['contexts'] = $contexts;

  $form['markup'] = array(
    '#prefix' => '<div class="description">',
    '#markup' => t('If there is more than one %item on a page, when the page is visited each variant is given an opportunity to be displayed. Starting from the first %item and working to the last, each one tests to see if its selection rules will pass. The first %item that meets its criteria (as specified below) will be used.', array('%item' => t('cat_item'))),
    '#suffix' => '</div>',
  );

  $form = ctools_access_admin_form($form, $form_state);

  _cat_ui_form_attach_buttons($form, array('next', 'prev'), 'cat_ui_add_form');
}

/**
 * FOURTH STEP.
 * Provide form for CAT item settings.
 *
 * @param array $form
 * @param array $form_state
 * @return mixed
 */
function _cat_ui_form_step_4_cat_settings(&$form, &$form_state) {
  if (!($cat_item = cat_object_cache_get($form_state['storage']['machine_name']))) {
    return FALSE;
  }

  $form['translatable'] = array(
    '#type' => 'checkbox',
    '#title' => t('Translatable'),
    '#default_value' => !empty($cat_item->translatable),
  );

  // Add help message about cat_i18n module.
  if (module_exists('cat_i18n')) {
    $form['translatable']['#description'] = t('Every CAT item will be available for translation.');
  }
  else {
    $form['translatable']['#disabled'] = TRUE;
    $form['translatable']['#description'] = t('You have to enable <a href="@modules_url">@module_name</a> module if you need translatable CAT items.', array(
      '@module_name' => 'CAT item translation',
      '@modules_url' => url('admin/modules'),
    ));
  }

  $form['reusable'] = array(
    '#type' => 'checkbox',
    '#title' => t('Reusable'),
    '#default_value' => isset($cat_item->reusable) ? $cat_item->reusable : 0,
    '#description' => t('This CAT item will be available for the reusing in another CAT items.'),
  );

  if (module_exists('token')) {
    $form['token_tree'] = array(
      '#title' => t('Replacement patterns'),
      '#type' => 'fieldset',
      '#collapsible' => TRUE,
      '#collapsed' => TRUE,
      '#description' => t('Prefer raw-text replacements for text to avoid problems with HTML entities!'),
    );

    $token_tree = array(
      'token_types' => 'all',
      'click_insert' => FALSE,
      'show_restricted' => TRUE,
    );
    
    if (!empty($cat_item->arguments)) {
      //get the argument names and decide their keywords
      $token_tree['token_types'] = array();
      foreach ($cat_item->arguments as $position => $arg) {
        $token_tree['token_types'][] = $arg['argument_name'];
      }
    }

    $form['token_tree']['token_help'] = array(
      '#markup' => theme('token_tree', $token_tree)
    );
  }

  // Remove tabs
  $form['cat_remove'] = array('#tree' => TRUE);

  // Initialize default amount of rows for titles and paths.
  if (empty($form_state['storage']['cat_remove_count'])) {
    $form_state['storage']['cat_remove_count'] = 0;
  }

  // If cat_item already has titles and paths we should build appropriate amount of rows.
  if (!empty($cat_item->cat_remove)) {
    $count = count($cat_item->cat_remove);
    if ($count > $form_state['storage']['cat_remove_count']) {
      $form_state['storage']['cat_remove_count'] = $count;
    }
  }

  $cat_remove = array_keys($cat_item->cat_remove);
  $path_original_len = drupal_strlen($cat_item->path_original);
  if ($form_state['storage']['cat_remove_count']) {
    for ($i = 0; $i < $form_state['storage']['cat_remove_count']; $i++) {
      // Unique hash required for updating form values in browser when deleting title-and-path row.
      $unique_hash = md5(rand() . 'cat_remove');

      $form['cat_remove'][$unique_hash]['path'] = array(
        '#type' => 'textfield',
        '#default_value' => '',
        '#maxlength' => 255,
        '#size' => 36,
        '#field_prefix' => $cat_item->path_original . '/',
      );

      $form['cat_remove'][$unique_hash]['remove_settings'] = array(
        '#type' => 'checkboxes',
        '#default_value' => '-1',
        '#options' => array(
          'tab' => t('Remove if tab'),
          'action' => t('Remove if action'),
        ),
      );

      if (isset($cat_remove[$i])) {
        if (isset($cat_item->cat_remove[$cat_remove[$i]]['path_original'])) {
          $path_original = $cat_item->cat_remove[$cat_remove[$i]]['path_original'];
          $path_original = substr($path_original, $path_original_len + 1);
          $form['cat_remove'][$unique_hash]['path']['#default_value'] = $path_original;
        }
        if (isset($cat_item->cat_remove[$cat_remove[$i]]['remove_settings'])) {
          $form['cat_remove'][$unique_hash]['remove_settings']['#default_value'] = $cat_item->cat_remove[$cat_remove[$i]]['remove_settings'];
        }
      }

      $form['cat_remove'][$unique_hash]['weight'] = array(
        '#type' => 'textfield',
        '#default_value' => $i,
      );

      $form['cat_remove'][$unique_hash]['delete'] = array(
        '#name' => 'delete_' . $unique_hash,
        '#type' => 'submit',
        '#value' => t('Delete'),
        '#ajax' => array(
          'callback' => '_cat_ui_form_step_4_cat_settings_ajax',
          'wrapper' => 'cat-ui-form-wrapper',
          'progress' => array(
            'type' => 'throbber',
            'message' => t('Delete ...'),
          ),
        ),
      );

    }
  }

  // Button allows add new empty title-and-path row.
  $form['cat_remove_more'] = array(
    '#type' => 'submit',
    '#value' => t('Add existed path'),
    '#ajax' => array(
      'callback' => '_cat_ui_form_step_4_cat_settings_ajax',
      'wrapper' => 'cat-ui-form-wrapper',
    ),
  );

  // NEW TABS
  // table for menegment CAT item Tabs
  $form['cat_tabs'] = array('#tree' => TRUE);

  // Initialize default amount of rows for titles and paths.
  if (empty($form_state['storage']['cat_tabs_count'])) {
    $form_state['storage']['cat_tabs_count'] = 0;
  }

  // If cat_item already has titles and paths we should build appropriate amount of rows.
  if (!empty($cat_item->cat_tabs)) {
    $count = count($cat_item->cat_tabs);
    if ($count > $form_state['storage']['cat_tabs_count']) {
      $form_state['storage']['cat_tabs_count'] = $count;
    }
  }

  $cat_tabs = array_keys($cat_item->cat_tabs);
  if ($form_state['storage']['cat_tabs_count']) {
    for ($i = 0; $i < $form_state['storage']['cat_tabs_count']; $i++) {
      // Unique hash required for updating form values in browser when deleting title-and-path row.
      $unique_hash = md5(rand() . 'cat_tabs');

      $form['cat_tabs'][$unique_hash] = array();

      $element = &$form['cat_tabs'][$unique_hash];

      $element['title'] = array(
        '#type' => 'textfield',
        '#maxlength' => 255,
        '#size' => 27,
        '#field_prefix' => t('Title'),
      );

      $element['reusing'] = array(
        '#type' => 'container',
        '#attributes' => array(
          'class' => array(
            'cat-reusing-wrapper'
          )
        ),
      );

      $element['reusing']['machine_name'] = array(
        '#type' => 'machine_name',
        '#maxlength' => 255,
        '#size' => 27,
        '#machine_name' => array(
          'source' => array(
            'path',
            'title',
          ),
          'exists' => '_cat_ui_form_step_4_cat_settings_machine_name_exists',
        ),
        '#description' => '',
        '#title' => '',
        '#required' => FALSE,
        '#field_prefix' => t('Name'),
        '#field_suffix' => '(' . t('machine name') . ')',
      );

      $element['reusing']['reusable'] = array(
        '#type' => 'checkbox',
        '#title' => t('Reusable'),
      );

      $element['path'] = array(
        '#type' => 'textfield',
        '#maxlength' => 255,
        '#size' => 27,
        '#autocomplete_path' => 'cat/autocomplete',
        '#field_prefix' => t('Path'),
        '#attributes' => array(
          'placeholder' => 'path/arg1/arg2',
        ),
      );

      $element['path_query'] = array(
        '#type' => 'textfield',
        '#size' => 27,
        '#field_prefix' => t('Query'),
        '#attributes' => array(
          'placeholder' => 'param1=val1&param2=val2',
        ),
      );

      $element['path_destination'] = array(
        '#type' => 'checkbox',
        '#title' => t('Tab has "Destination"'),
      );

      $element['css_id'] = array(
        '#type' => 'textfield',
        '#size' => 27,
        '#field_prefix' => 'Id',
      );

      $element['css_class'] = array(
        '#type' => 'textfield',
        '#size' => 27,
        '#field_prefix' => 'Class',
      );

      $element['active'] = array(
        '#type' => 'checkbox',
        '#title' => t('Tab has "Active" state'),
      );

      if (isset($cat_tabs[$i]) && isset($cat_item->cat_tabs[$cat_tabs[$i]])) {
        $cat_tab = &$cat_item->cat_tabs[$cat_tabs[$i]];

        if (isset($cat_tab['title'])) {
          $element['title']['#default_value'] = $cat_tab['title'];
        }

        if (isset($cat_tab['machine_name'])) {
          $element['reusing']['machine_name']['#default_value'] = $cat_tab['machine_name'];
        }

        if (isset($cat_tab['reusable'])) {
          $element['reusing']['reusable']['#default_value'] = $cat_tab['reusable'];
        }

        if (isset($cat_tab['path_original'])) {
          $element['path']['#default_value'] = $cat_tab['path_original'];
        }

        if (isset($cat_tab['path_query'])) {
          $element['path_query']['#default_value'] = urldecode(drupal_http_build_query($cat_tab['path_query']));
        }

        if (isset($cat_tab['path_destination'])) {
          $element['path_destination']['#default_value'] = $cat_tab['path_destination'];
        }

        if (isset($cat_tab['css_id'])) {
          $element['css_id']['#default_value'] = $cat_tab['css_id'];
        }

        if (isset($cat_tab['css_class'])) {
          $element['css_class']['#default_value'] = $cat_tab['css_class'];
        }

        if (!empty($cat_tab['active'])) {
          $element['active']['#default_value'] = TRUE;
        }

        if (!empty($cat_tab['reusabled']) && !empty($cat_tab['reusabled_machine_name'])) {
          $element['title']['#disabled'] = TRUE;
          $element['path']['#disabled'] = TRUE;
          $element['path_query']['#disabled'] = TRUE;

          $element['reusing']['machine_name']['#default_value'] = '';
          $element['reusing']['machine_name']['#access'] = FALSE;
          $element['reusing']['reusable']['#default_value'] = '0';
          $element['reusing']['reusable']['#access'] = FALSE;

          list($parent_cat_tab, ) = explode('|', $cat_tab['reusabled_machine_name'], 2);
          $parent_cat_tab = cat_load_by_name($parent_cat_tab);
          $element['reusing']['reusabled_message'] = array(
            '#type' => 'markup',
            '#markup' => t('This %item has been inherited from the !cat_item (%cat_item_machine_name).', array(
              '%item' => t('tab'),
              '!cat_item' => l($parent_cat_tab->name, "admin/structure/cat/edit/{$parent_cat_tab->machine_name}"),
              '%cat_item_machine_name' => $parent_cat_tab->machine_name,
            )),
          );

          $element['reusing']['reusabled'] = array(
            '#type' => 'hidden',
            '#value' => '1'
          );

          $element['reusing']['reusabled_machine_name'] = array(
            '#type' => 'hidden',
            '#value' => $cat_tab['reusabled_machine_name']
          );
        }
      }

      $element['weight'] = array(
        '#type' => 'textfield',
        '#default_value' => $i,
      );

      $element['delete'] = array(
        '#name' => 'delete_' . $unique_hash,
        '#type' => 'submit',
        '#value' => t('Delete'),
        '#ajax' => array(
          'callback' => '_cat_ui_form_step_4_cat_settings_ajax',
          'wrapper' => 'cat-ui-form-wrapper',
          'progress' => array(
            'type' => 'throbber',
            'message' => t('Delete ...'),
          ),
        ),
      );
    }
  }

  $reusable_cat_tabs = _cat_ui_form_step_4_cat_settings_get_reusable_tabs($cat_item->machine_name);

  // Add reusable cat items
  $form['cat_tabs_reusable'] = array();

  if (!empty($reusable_cat_tabs)) {
    $options = array('new-item' => t('New local tab'));

    $options += $reusable_cat_tabs;

    $form['cat_tabs_reusable'] = array(
      '#type' => 'select',
      '#options' => $options,
      '#default_value' => 'new-item',
    );
  }

  // Button allows add new empty title-and-path row.
  $form['cat_tabs_more'] = array(
    '#type' => 'submit',
    '#value' => t('Add local tab'),
    '#ajax' => array(
      'callback' => '_cat_ui_form_step_4_cat_settings_ajax',
      'wrapper' => 'cat-ui-form-wrapper',
    ),
  );

  $form['cat_actions'] = array('#tree' => TRUE);

  // Initialize default amount of rows for titles and paths.
  if (empty($form_state['storage']['cat_actions_count'])) {
    $form_state['storage']['cat_actions_count'] = 0;
  }

  // If cat_item already has titles and paths we should build appropriate amount of rows.
  if (!empty($cat_item->cat_actions)) {
    $count = count($cat_item->cat_actions);
    if ($count > $form_state['storage']['cat_actions_count']) {
      $form_state['storage']['cat_actions_count'] = $count;
    }
  }

  $cat_actions = array_keys($cat_item->cat_actions);

  if ($form_state['storage']['cat_actions_count']) {
    for ($i = 0; $i < $form_state['storage']['cat_actions_count']; $i++) {
      // Unique hash required for updating form values in browser when deleting title-and-path row.
      $unique_hash = md5(rand() . 'cat_actions');

      $form['cat_actions'][$unique_hash]['title'] = array(
        '#type' => 'textfield',
        '#default_value' => '',
        '#maxlength' => 255,
        '#size' => 27,
      );

      $form['cat_actions'][$unique_hash]['path'] = array(
        '#type' => 'textfield',
        '#default_value' => '',
        '#maxlength' => 255,
        '#size' => 27,
        '#autocomplete_path' => 'cat/autocomplete',
        '#field_prefix' => t('Path'),
        '#attributes' => array(
          'placeholder' => 'path/arg1/arg2',
        ),
      );

      $form['cat_actions'][$unique_hash]['path_query'] = array(
        '#type' => 'textfield',
        '#default_value' => '',
        '#size' => 27,
        '#field_prefix' => t('Query'),
        '#attributes' => array(
          'placeholder' => 'param1=val1&param2=val2',
        ),
      );

      $form['cat_actions'][$unique_hash]['path_destination'] = array(
        '#type' => 'checkbox',
        '#title' => t('Action has "Destination"'),
        '#default_value' => FALSE,
      );

      $form['cat_actions'][$unique_hash]['css_id'] = array(
        '#type' => 'textfield',
        '#default_value' => '',
        '#size' => 27,
        '#field_prefix' => 'Id',
      );

      $form['cat_actions'][$unique_hash]['css_class'] = array(
        '#type' => 'textfield',
        '#default_value' => '',
        '#size' => 27,
        '#field_prefix' => 'Class',
      );

      $form['cat_actions'][$unique_hash]['active'] = array(
        '#type' => 'checkbox',
        '#title' => t('Action has "Active" state'),
        '#default_value' => FALSE,
      );

      if (isset($cat_actions[$i])) {
        if (isset($cat_item->cat_actions[$cat_actions[$i]]['title'])) {
          $form['cat_actions'][$unique_hash]['title']['#default_value'] = $cat_item->cat_actions[$cat_actions[$i]]['title'];
        }
        if (isset($cat_item->cat_actions[$cat_actions[$i]]['path'])) {
          $form['cat_actions'][$unique_hash]['path']['#default_value'] = $cat_item->cat_actions[$cat_actions[$i]]['path'];
        }
        if (isset($cat_item->cat_actions[$cat_actions[$i]]['path_query'])) {
          $form['cat_actions'][$unique_hash]['path_query']['#default_value'] = urldecode(drupal_http_build_query($cat_item->cat_actions[$cat_actions[$i]]['path_query']));
        }
        if (isset($cat_item->cat_actions[$cat_actions[$i]]['path_destination'])) {
          $form['cat_actions'][$unique_hash]['path_destination']['#default_value'] = $cat_item->cat_actions[$cat_actions[$i]]['path_destination'];
        }
        if (isset($cat_item->cat_actions[$cat_actions[$i]]['css_id'])) {
          $form['cat_actions'][$unique_hash]['css_id']['#default_value'] = $cat_item->cat_actions[$cat_actions[$i]]['css_id'];
        }
        if (isset($cat_item->cat_actions[$cat_actions[$i]]['css_class'])) {
          $form['cat_actions'][$unique_hash]['css_class']['#default_value'] = $cat_item->cat_actions[$cat_actions[$i]]['css_class'];
        }
        if (isset($cat_item->cat_actions[$cat_actions[$i]]['active'])) {
          $form['cat_actions'][$unique_hash]['active']['#default_value'] = TRUE;
        }
      }

      $form['cat_actions'][$unique_hash]['weight'] = array(
        '#type' => 'textfield',
        '#default_value' => $i,
      );

      $form['cat_actions'][$unique_hash]['delete'] = array(
        '#name' => 'delete_' . $unique_hash,
        '#type' => 'submit',
        '#value' => t('Delete'),
        '#ajax' => array(
          'callback' => '_cat_ui_form_step_4_cat_settings_ajax',
          'wrapper' => 'cat-ui-form-wrapper',
          'progress' => array(
            'type' => 'throbber',
            'message' => t('Delete ...'),
          ),
        ),
      );
    }
  }

  // Button allows add new empty title-and-path row.
  $form['cat_actions_more'] = array(
    '#type' => 'submit',
    '#value' => t('Add local action'),
    '#ajax' => array(
      'callback' => '_cat_ui_form_step_4_cat_settings_ajax',
      'wrapper' => 'cat-ui-form-wrapper',
    ),
  );

  // Active trail path
  $form['active_trail_path'] = array(
    '#type' => 'textfield',
    '#title' => t('Active trail path'),
    '#maxlength' => 255,
    '#default_value' => !empty($cat_item->active_trail_path) ? $cat_item->active_trail_path : NULL,
    '#description' => t('The active trail path for this CAT item.'),
    '#required' => FALSE,
  );

  // Attached elements
  $form['#attached']['library'][] = array('system', 'ui.autocomplete');
  $form['#attached']['library'][] = array('system', 'ui.button');

  $form['#attached']['js'][] = array(
    'type' => 'setting',
    'data' => array(
      'CatUI' => array(
        'machineName' => $form_state['storage']['machine_name'],
      ),
    ),
  );

  $form['#validate'][] = '_cat_ui_form_step_4_cat_settings_validate';
  $form['#submit'][] = '_cat_ui_form_step_4_cat_settings_submit';

  _cat_ui_form_attach_buttons($form, array('prev', 'finish'), 'cat_ui_add_form');

  if (isset($form_state['storage']['machine_name'])) {
    _cat_ui_form_apply_default_values($form, $form_state['storage']['machine_name'], array(
      'cat_tabs',
      'cat_tabs_reusable',
      'cat_tabs_more',
      'cat_actions',
      'cat_actions_more',
      'cat_remove',
      'cat_remove_more',
    ));
  }

  // there is we can provide extended settings
  drupal_alter('cat_settings_form_custom', $form, $cat_item);
}

/**
 * Ajax callback for ADD MORE button.
 *
 * @param array $form
 * @param array $form_state
 * @return mixed
 */
function _cat_ui_form_step_4_cat_settings_ajax($form, $form_state) {
  // Simple form reload.
  return $form;
}

/**
 * Return reusable item.
 *
 * @param string $machine_name
 *   Exclude current machine name.
 *
 * @param string $type
 *
 * @return array
 */
function _cat_ui_reusable_items($concat_machine_name = NULL, $type = NULL) {
  $reusable_items = array();

  if (!$concat_machine_name || !in_array($type, array('cat_tabs', 'cat_actions'))) {
    return $reusable_items;
  }

  list($input_cat_mname, $input_item_mname) = explode('|', $concat_machine_name, 2);

  if (!empty($input_cat_mname) && empty($input_item_mname)) {
    // all subitems
    foreach (cat_get_reusable() as $cat_item_machine_name => $cat_item_types) {
      if ($cat_item_machine_name == $input_cat_mname) {
        foreach ($cat_item_types[$type] as $cat_item) {
          $cat_item['css_id'] = '';
          $cat_item['css_class'] = '';
          $cat_item['active'] = FALSE;
          $cat_item['reusable'] = FALSE;
          $cat_item['reusabled'] = TRUE;
          $cat_item['reusabled_machine_name'] = "{$cat_item_machine_name}|{$cat_item['machine_name']}";
          $cat_item['machine_name'] = '';

          $reusable_items[] = $cat_item;
        }
      }
    }
  }
  else {
    // sepecial subitem
    foreach (cat_get_reusable() as $cat_item_machine_name => $cat_item_types) {
      if ($cat_item_machine_name == $input_cat_mname) {
        foreach ($cat_item_types as $cat_item_type => $cat_items) {
          foreach ($cat_items as $cat_item) {
            if ($cat_item['machine_name'] == $input_item_mname) {
              $cat_item['css_id'] = '';
              $cat_item['css_class'] = '';
              $cat_item['active'] = FALSE;
              $cat_item['reusable'] = FALSE;
              $cat_item['reusabled'] = TRUE;
              $cat_item['reusabled_machine_name'] = "{$cat_item_machine_name}|{$cat_item['machine_name']}";
              $cat_item['machine_name'] = '';

              $reusable_items[] = $cat_item;
            }
          }
        }
      }
    }
  }

  return $reusable_items;
}

/**
 * Validate function for CAT item settings form (FOURTH STEP).
 *
 * @param array $form
 * @param array $form_state
 *
 * @return mixed
 */
function _cat_ui_form_step_4_cat_settings_validate($form, $form_state) {
  $values = &$form_state['values'];

  // Do not validate form if DELETE button was submitted.
  if (!array_search(t('Delete'), $values, TRUE)) {
    // Every existed path should contain value.
    if (!empty($values['cat_remove'])) {
      $error_cat_remove_path = FALSE;
      foreach ($values['cat_remove'] as $key => $cat_remove) {
        if (empty($cat_remove['path'])) {
          if (!$error_cat_remove_path) {
            $error_cat_remove_path = TRUE;
            form_set_error("cat_remove][{$key}][path", t('Path should contain value.'));
          }
          else {
            form_set_error("cat_remove][{$key}][path");
          }
        }
      }
    }

    // Every local tab's title and path should contain value.
    if (!empty($values['cat_tabs'])) {
      $error_cat_tab_title = FALSE;
      $error_cat_tab_path = FALSE;
      $error_cat_tab_reusable = FALSE;

      foreach ($values['cat_tabs'] as $key => $cat_tab) {
        if (empty($cat_tab['title'])) {
          if (!$error_cat_tab_title) {
            $error_cat_tab_title = TRUE;
            form_set_error("cat_tabs][{$key}][title", t('Title must contain value.'));
          }
          else {
            form_set_error("cat_tabs][{$key}][title");
          }
        }

        if (empty($cat_tab['path'])) {
          if (!$error_cat_tab_path) {
            $error_cat_tab_path = TRUE;
            form_set_error("cat_tabs][{$key}][path", t('Path must contain value.'));
          }
          else {
            form_set_error("cat_tabs][{$key}][path");
          }
        }

        if (!empty($cat_tab['reusing']['reusable']) && empty($cat_tab['reusing']['machine_name'])) {
          if (!$error_cat_tab_reusable) {
            $error_cat_tab_reusable = TRUE;
            form_set_error("cat_tabs][{$key}][reusing][machine_name", t('The machine-readable name must contain value, if you want reuse this item.'));
          }
          else {
            form_set_error("cat_tabs][{$key}][reusing][machine_name");
          }
        }
      }
    }

    // Every local action's title and path should contain value.
    if (!empty($values['cat_actions'])) {
      $error_cat_tab_title = FALSE;
      $error_cat_tab_path = FALSE;

      foreach ($values['cat_actions'] as $key => $cat_action) {
        if (empty($cat_action['title'])) {
          if (!$error_cat_tab_title) {
            $error_cat_tab_title = TRUE;
            form_set_error("cat_actions][{$key}][title", t('Title must contain value.'));
          }
          else {
            form_set_error("cat_actions][{$key}][title");
          }
        }

        if (empty($cat_action['path'])) {
          if (!$error_cat_tab_path) {
            $error_cat_tab_path = TRUE;
            form_set_error("cat_actions][{$key}][path", t('Path must contain value.'));
          }
          else {
            form_set_error("cat_actions][{$key}][path");
          }
        }
      }
    }

    // Check active trail path
    if (!empty($values['active_trail_path'])) {
      // @TODO: needs add check active trail path
    }
  }
}

/**
 * Submit callback for CAT item settings form (FOURTH STEP).
 *
 * @param array $form
 * @param array $form_state
 */
function _cat_ui_form_step_4_cat_settings_submit($form, &$form_state) {
  $values = &$form_state['values'];

  // Check dosen't set value
  $default_values = array(
    'cat_remove' => array(),
    'cat_tabs' => array(),
    'cat_actions' => array(),
    'active_trail_path' => '',
  );
  foreach ($default_values as $var => $val) {
    if (!isset($values[$var])) {
      $values[$var] = $val;
    }
  }

  // Get CAT item
  $machine_name = $form_state['storage']['machine_name'];
  $cat_item = cat_object_cache_get($machine_name);
  $reusable_items = array(
    'cat_tabs' => array(),
    'cat_actions' => array(),
  );

  // Remove row when DELETE button submitted.
  $delete = array_search(t('Delete'), $values, TRUE);
  if ($delete !== FALSE) {
    $deleted_key = drupal_substr($delete, 7);
    if (isset($values['cat_remove'][$deleted_key])) {
      unset($values['cat_remove'][$deleted_key]);
      unset($form['cat_remove'][$deleted_key]);
      $form_state['storage']['cat_remove_count']--;
      $form_state['rebuild'] = TRUE;
    }
    elseif (isset($values['cat_tabs'][$deleted_key])) {
      unset($values['cat_tabs'][$deleted_key]);
      unset($form['cat_tabs'][$deleted_key]);
      $form_state['storage']['cat_tabs_count']--;
      $form_state['rebuild'] = TRUE;
    }
    elseif (isset($values['cat_actions'][$deleted_key])) {
      unset($values['cat_actions'][$deleted_key]);
      unset($form['cat_actions'][$deleted_key]);
      $form_state['storage']['cat_actions_count']--;
      $form_state['rebuild'] = TRUE;
    }
  }

  // Action for ADD MORE button.
  if (isset($values['cat_remove_more']) && $values['op'] == $values['cat_remove_more']) {
    $form_state['storage']['cat_remove_count']++;
    $form_state['rebuild'] = TRUE;
  }
  elseif (isset($values['cat_tabs_more']) && $values['op'] == $values['cat_tabs_more']) {
    $cat_tabs_count = $form_state['storage']['cat_tabs_count'];
    $form_state['rebuild'] = TRUE;

    if (!empty($values['cat_tabs_reusable'])) {
      // @TODO: add check input data
      $reusable_items['cat_tabs'] = _cat_ui_reusable_items($values['cat_tabs_reusable'], 'cat_tabs');
      $form_state['storage']['cat_tabs_count'] += count($reusable_items['cat_tabs']);
    }

    unset($values['cat_tabs_reusable']);

    if ($cat_tabs_count == $form_state['storage']['cat_tabs_count']) {
      $form_state['storage']['cat_tabs_count'] ++;
    }
  }
  elseif (isset($values['cat_actions_more']) && $values['op'] == $values['cat_actions_more']) {
    $form_state['storage']['cat_actions_count']++;
    $form_state['rebuild'] = TRUE;
  }

  $cat_remove = array();
  if (!empty($values['cat_remove'])) {
    // Sort CAT items based on weights changed by tabledrag.
    usort($values['cat_remove'], 'drupal_sort_weight');
    // Rebuild CAT titles and paths.
    foreach ($values['cat_remove'] as $cat_remove_tab) {
      if (!empty($cat_remove_tab['path'])) {
        $path_original = trim($cat_remove_tab['path'], '/');
        $path_original = $cat_item->path_original . '/' . $path_original;
        $cat_remove[] = array(
          'path' => cat_path_router($path_original),
          'path_router' => cat_path_router($path_original),
          'path_original' => $path_original,
          'path_pattern' => cat_path_pattern($path_original),
          'remove_settings' => $cat_remove_tab['remove_settings'],
        );
      }
    }
  }

  $cat_tabs = array();
  if (!empty($values['cat_tabs'])) {
    // Sort CAT items based on weights changed by tabledrag.
    usort($values['cat_tabs'], 'drupal_sort_weight');
    // Rebuild CAT titles and paths.
    foreach ($values['cat_tabs'] as $cat_tab) {
      $cat_tabs[] = array(
        'path' => cat_path_router($cat_tab['path']),
        'path_router' => cat_path_router($cat_tab['path']),
        'path_original' => $cat_tab['path'],
        'path_pattern' => cat_path_pattern($cat_tab['path']),
        'path_query' => cat_parse_url_query($cat_tab['path_query']),
        'path_destination' => !empty($cat_tab['path_destination']),
        'title' => $cat_tab['title'],
        'css_id' => $cat_tab['css_id'],
        'css_class' => $cat_tab['css_class'],
        'active' => !empty($cat_tab['active']),
        'reusable' => !empty($cat_tab['reusing']['reusable']) && empty($cat_tab['reusing']['reusabled']),
        'machine_name' => $cat_tab['reusing']['machine_name'],
        'reusabled' => !empty($cat_tab['reusing']['reusabled']),
        'reusabled_machine_name' => $cat_tab['reusing']['reusabled_machine_name'],
      );
    }
  }
  $cat_tabs = array_merge($cat_tabs, $reusable_items['cat_tabs']);

  $cat_actions = array();
  if (!empty($values['cat_actions'])) {
    // Sort CAT items based on weights changed by tabledrag.
    usort($values['cat_actions'], 'drupal_sort_weight');
    // Rebuild CAT titles and paths.
    foreach ($values['cat_actions'] as $cat_action) {
      $cat_actions[] = array(
        'path' => cat_path_router($cat_action['path']),
        'path_router' => cat_path_router($cat_action['path']),
        'path_original' => $cat_action['path'],
        'path_pattern' => cat_path_pattern($cat_action['path']),
        'path_query' => cat_parse_url_query($cat_action['path_query']),
        'path_destination' => !empty($cat_action['path_destination']),
        'title' => $cat_action['title'],
        'css_id' => $cat_action['css_id'],
        'css_class' => $cat_action['css_class'],
        'active' => !empty($cat_action['active']),
      );
    }
  }
  $cat_actions = array_merge($cat_actions, $reusable_items['cat_actions']);

  // Update titles and paths in cache.
  $cat_item->cat_remove = $cat_remove;
  $cat_item->cat_tabs = $cat_tabs;
  $cat_item->cat_actions = $cat_actions;
  $cat_item->path = cat_path_router($cat_item->path_original);
  $cat_item->active_trail_path = $values['active_trail_path'];

  // Possibility to implement custom CAT item settings.
  $custom = array();
  drupal_alter('cat_settings_form_submit_custom', $custom, $form_state);
  if (!empty($custom)) {
    $cat_item->custom = $custom;
  }

  cat_object_cache_set($machine_name, $cat_item);
}

/**
 * Return list of the tabs, which avalable for reusing.
 *
 * @param string $machine_name
 *   Exclude current machine name.
 *
 * @return array
 */
function _cat_ui_form_step_4_cat_settings_get_reusable_tabs($machine_name = NULL) {
  $reusable_cat_tabs = array();

  foreach (cat_get_reusable() as $cat_item_machine_name => $cat_item) {
    if ($cat_item_machine_name != $machine_name) {
      $cat_item_loaded = cat_item_load($cat_item_machine_name);

      if (!empty($cat_item['cat_tabs'])) {
        $cat_item_name = t('!cat_name (!machine_name)', array(
          '!cat_name' => $cat_item_loaded->name,
          '!machine_name' => $cat_item_machine_name,
        ));

        if (!empty($cat_item['reusable'])) {
          $key = $cat_item_machine_name;

          $value = t('!cat_name (!machine_name) => All tabs as set', array(
            '!cat_name' => $cat_item_loaded->name,
            '!machine_name' => $cat_item_machine_name,
          ));

          $reusable_cat_tabs[$cat_item_name][$key] = $value;
        }

        foreach ($cat_item['cat_tabs'] as $cat_tab) {
          $key = "{$cat_item_machine_name}|{$cat_tab['machine_name']}";

          $value = "{$cat_item_loaded->name} ({$cat_item_machine_name}) => ";
          $value .= "{$cat_tab['title']} ({$cat_tab['machine_name']}, {$cat_tab['path_original']})";

          $reusable_cat_tabs[$cat_item_name][$key] = $value;
        }
      }
    }
  }

  return $reusable_cat_tabs;
}

/**
 * Callbak for checking whether a submitted machine name value already exists.
 * The submitted value is passed as argument. In most cases, an existing API
 * or menu argument function can be re-used. The callback is only invoked,
 * if the submitted value differs from the element's #default_value.
 *
 * @param string $machine_name
 *
 * @return bool
 */
function _cat_ui_form_step_4_cat_settings_machine_name_exists($machine_name) {
  // @TODO: we need to add checking whether a submitted
  // machine name value already exists.

  return FALSE;
}

/**
 * AJAX callback for last step of CAT itme creating.
 * Saves new CAT item.
 *
 * @param array $form
 * @param array $form_state
 *
 * @return array
 */
function _cat_ui_add_form_finish_ajax_callback($form, &$form_state) {
  if (!form_get_errors()) {
    // Save new CAT item from cache to permanent storage.
    $cat_item = cat_object_cache_get($form_state['storage']['machine_name']);
    cat_save($cat_item);

    drupal_set_message(t('New CAT item "%name" was created.', array('%name' => filter_xss($cat_item->name))));

    ctools_include('ajax');

    // Ajax responder not required since CTools 7.x-1.0 but this line should be
    // added to keep module works properly.
    ctools_add_js('ajax-responder');

    $commands = array();
    $commands[] = ctools_ajax_command_redirect('admin/structure/cat');

    return array(
      '#type' => 'ajax',
      '#commands' => $commands,
    );
  }

  return $form;
}

