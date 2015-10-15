<?php

/**
 * @file
 *
 */

/**
 * Page callback for arguments configuration in path.
 */
function path_metatags_ui_configure_arguments_page($path_name, $keyword) {

  // Include ctools modal plugin.
  ctools_include('modal');

  // Set form state values for context selections.
  $form_state = array(
    'storage' => array(
      'path_name' => $path_name,
      'keyword' => $keyword
    )
  );

  // Load context selections form.
  $form = drupal_build_form('path_metatags_ui_argument_context_form', $form_state);

  // Wrap form into ctools modal.
  $commands = ctools_modal_form_render(array('title' => t('Choose argument')), $form);
  return array('#type' => 'ajax', '#commands' => $commands);
}

/**
 * Page callback for settings form for arguments.
 *
 * @param $path_name
 * @param $keyword
 * @return array
 */
function path_metatags_ui_change_arguments_page($path_name, $keyword) {
  // Include ctools modal plugin.
  ctools_include('modal');

  $metatags = path_metatags_object_cache_get($path_name);

  // Set form values for second step of context settings form.
  $form_state = array(
    'storage' => array(
      'step' => 2,
      'path_name' => $path_name,
      'keyword' => $keyword,
      'argument' => $metatags->arguments[$keyword]['argument'],
    ),
  );

  $form = drupal_build_form('path_metatags_ui_argument_context_form', $form_state);
  $commands = ctools_modal_form_render(array('title' => t('Choose argument')), $form);
  return array('#type' => 'ajax', '#commands' => $commands);
}

/**
 * Form for editing path metatags.
 */
function path_metatags_ui_edit_page($path_metatags) {

  $page['content']['#prefix'] = '<div id="path-metatags-ui-wrapper" class="clearfix">';
  $page['content']['#suffix'] = '</div>';

  // Save form in cache.
  path_metatags_object_cache_set($path_metatags->machine_name, $path_metatags);

  $links[] = array(
    '#type' => 'link',
    '#title' => t('Basic'),
    '#href' => 'admin/structure/path-metatags/edit/' . $path_metatags->machine_name . '/ajax/1',
    '#attributes' => array('class' => array('use-ajax', 'active')),
  );

  $links[] = array(
    '#type' => 'link',
    '#title' => t('Arguments'),
    '#href' => 'admin/structure/path-metatags/edit/' . $path_metatags->machine_name . '/ajax/2',
    '#attributes' => array('class' => array('use-ajax')),
  );

  $links[] = array(
    '#type' => 'link',
    '#title' => t('Selection rules'),
    '#href' => 'admin/structure/path-metatags/edit/' . $path_metatags->machine_name . '/ajax/3',
    '#attributes' => array('class' => array('use-ajax')),
  );

  $links[] = array(
    '#type' => 'link',
    '#title' => t('Metatags'),
    '#href' => 'admin/structure/path-metatags/edit/' . $path_metatags->machine_name . '/ajax/4',
    '#attributes' => array('class' => array('use-ajax')),
  );

  $rendered_links = array();
  foreach ($links as $link) {
    $rendered_links[] = render($link);
  }

  $page['content']['menu'] = array(
    '#markup' => '<div id="path-metatags-ui-menu">' . theme('item_list', array('items' => $rendered_links)) . '</div>',
  );

  $form_state = array(
    'storage' => array(
      'machine_name' => $path_metatags->machine_name,
      'step' => 1,
    )
  );

  $form = drupal_build_form('path_metatags_ui_edit_form', $form_state);
  $page['content']['edit_form'] = array(
    '#markup' => render($form),
  );

  $page['content']['#attached']['library'][] = array('system', 'drupal.ajax');

  return $page;
}

/**
 * Ajax callback for path_metatags EDIT links.
 */
function path_metatags_ui_edit_form_ajax_links($metatags, $step) {

  $form_state = array(
    'storage' => array(
      'machine_name' => $metatags->machine_name,
      'step' => $step,
    )
  );

  $form = drupal_build_form('path_metatags_ui_edit_form', $form_state);
  $commands = array();
  $commands[] = ajax_command_replace('#path-metatags-ui-form-wrapper', render($form));
  $commands[] = ajax_command_invoke('#path-metatags-ui-menu li a', 'removeClass', array('active'));
  $commands[] = ajax_command_invoke('#path-metatags-ui-menu li a:eq(' . --$step . ')', 'addClass', array('active'));
  return array('#type' => 'ajax', '#commands' => $commands);
}

/**
 * Disables path metatags.
 */
function path_metatags_ui_disable_metatags_page($path_metatags) {
  ctools_include('export');
  ctools_export_crud_disable('path_metatags', $path_metatags);
  cache_clear_all('path_metatags', 'cache', TRUE);

  drupal_set_message(t('Metatags %name was disabled', array('%name' => $path_metatags->name)));

  drupal_goto('admin/structure/path-metatags');
}

/**
 * Enables path metatags.
 */
function path_metatags_ui_enable_metatags_page($path_metatags) {

  // Update metatags in database.
  ctools_include('export');
  ctools_export_crud_enable('path_metatags', $path_metatags);
  cache_clear_all('path_metatags', 'cache', TRUE);

  drupal_set_message(t('Metatags %name was enabled', array('%name' => $path_metatags->name)));

  drupal_goto('admin/structure/path-metatags');
}

/**
 * AJAX callback for tokens autocomplete.
 */
function path_metatags_ui_tokens_ajax_autocomplete($path_metatags, $string) {

  // Check if path metatags object is loaded.
  if (empty($path_metatags)) {
    return;
  }

  // Build context substitutions.
  $contexts = array();
  if (!empty($path_metatags->arguments)) {

    // Include ctools library for contexts.
    ctools_include('context');

    foreach ($path_metatags->arguments as $keyword => $arg) {

      // Load argument plugin with its settings.
      $argument = ctools_get_argument($arg['argument']);
      if (isset($arg['settings'])) {
        $argument += $arg['settings'];
        $argument['id'] = 1;
      }

      // Load entity context.
      $contexts[] = ctools_context_get_context_from_argument($argument, $keyword, TRUE);
    }
  }

  $context_keywords = array();
  foreach ($contexts as $context) {
    $type = array_pop($context->type);
    $context_keywords[$context->original_argument] = $type;
  }

  // Load class for autocomplete ui.
  $matches = PathMetatagsUIAutocomplete::processAutocomplete($string, $context_keywords);

  drupal_json_output($matches);
}
