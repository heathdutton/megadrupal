<?php

/**
 * @file
 * Plugin to provide access control based upon views results.
 */

/**
 * Access plugin for views.
 */
$plugin = array(
  'title' => t("Views: has result"),
  'description' => t('Control access by checking if a views content pane returns a result.'),
  'callback' => 'views_content_visibility_views_ctools_access_check',
  'default' => array('view' => NULL, 'view_settings' => array()),
  'settings form' => 'views_content_visibility_views_ctools_access_settings',
  'settings form submit' => 'views_content_visibility_views_ctools_access_settings_submit',
  'summary' => 'views_content_visibility_views_ctools_access_summary',
  'all contexts' => TRUE,
);

/**
 * Settings form for access plugin.
 */
function views_content_visibility_views_ctools_access_settings($form, &$form_state, $conf) {
  ctools_include('content');
  $context = $form_state['display']->context;
  $plugin = ctools_get_content_type('views_panes');
  $views = views_content_views_panes_content_type_content_types(NULL);

  $options = array();
  // Find valid views. I can't believe there isn't a high level ctoosl function
  // for this? Am I doing something wrong? Or can't I find it?
  foreach ($views as $name => $view) {
    $invalid = FALSE;
    foreach ($view['required context'] as $rc) {
      if (empty($rc)) {
        continue;
      }
      $found = FALSE;
      foreach ($context as $c) {
        if (in_array($rc->keywords, $c->type)) {
          $found = TRUE;
          break;
        }
      }
      if (!$found) {
        $invalid = TRUE;
        break;
      }
    }
    if (!$invalid) {
      $options[$name] = $view['title'];
    }
  }

  // Select view to check.
  $options = array(NULL => t('-- Please select a view --')) + $options;
  $form['settings']['view'] = array(
    '#title' => t('View'),
    '#type' => 'select',
    '#required' => TRUE,
    '#description' => t('Select the view to check.'),
    '#options' => $options,
    '#default_value' => $conf['view'],
  );

  // Select views settings through ajax.
  $form['settings']['view_settings'] = array(
    '#tree' => TRUE,
  );

  foreach ($views as $name => $view) {
    $form['settings']['view_settings'][$name] = array(
      '#type' => 'fieldset',
      '#title' => t('Settings'),
      '#tree' => TRUE,
    );
    $subform = &$form['settings']['view_settings'][$name];

    $subform['context'] = ctools_context_selector($context, $view['required context'], array());
    $old_subtype_name = $form_state['subtype_name'];
    $form_state['subtype_name'] = $name;
    $form_state['conf'] = isset($conf['view_settings'][$name]) ? $conf['view_settings'][$name] : array();
    $subform += views_content_views_panes_content_type_edit_form(array(), $form_state);
    $form_state['subtype_name'] = $old_subtype_name;

    // Some settings arent necessary for an access check.
    unset($subform['link_to_view']);
    unset($subform['more_link']);
    unset($subform['use_pager']);
    unset($subform['pager_id']);
    unset($subform['fields_override']);
    unset($subform['feed_icons']);

    $subform['#states'] = array(
      'visible' => array(
        ':input[name="settings[view]"]' => array('value' => $name),
      ),
      'enabled' => array(
        ':input[name="settings[view]"]' => array('value' => $name),
      ),
    );
    foreach ($subform as $key => &$value) {
      if (is_array($value) && isset($value['#type'])) {
        $value['#states'] = $subform['#states'];
      }
    }
  }
  return $form;
}

/**
 * Filters values to store less.
 */
function views_content_visibility_views_ctools_access_settings_submit($form, &$form_state) {
  $view = $form_state['values']['settings']['view'];
  $form_state['values']['settings']['view_settings'] = array(
    $view => $form_state['values']['settings']['view_settings'][$view],
  );
}

/**
 * Check for access.
 */
function views_content_visibility_views_ctools_access_check($conf, $contexts) {
  if (!is_array($contexts)) {
    $contexts = array($contexts);
  }
  $not = !empty($conf['not']);
  $view = $conf['view'];
  $conf = $conf['view_settings'][$view];

  // Never use pager for access check.
  $conf['use_pager'] = FALSE;

  if (isset($conf['context'])) {
    foreach ($conf['context'] as $context_name) {
      if (empty($contexts[$context_name])) {
        return FALSE;
      }
    }
  }

  list($name, $display) = explode('-', $view);
  $view = views_get_view($name);
  if (empty($view)) {
    return;
  }
  $plugin = ctools_get_content_type('views_panes');

  $view->set_display($display);
  views_content_views_panes_add_defaults($conf, $view);

  if (!$view->display_handler->access($GLOBALS['user']) || empty($view->display_handler->panel_pane_display)) {
    return;
  }

  $view->display_handler->set_pane_conf($conf);

  $args = array();
  $arguments = $view->display_handler->get_option('arguments');

  $context_keys = isset($conf['context']) ? $conf['context'] : array();
  foreach ($view->display_handler->get_argument_input() as $id => $argument) {
    switch ($argument['type']) {
      case 'context':
        $key = array_shift($context_keys);
        if (isset($contexts[$key])) {
          if (strpos($argument['context'], '.')) {
            list($context, $converter) = explode('.', $argument['context'], 2);
            $args[] = ctools_context_convert_context($contexts[$key], $converter, array('sanitize' => FALSE));
          }
          else {
            $args[] = $contexts[$key]->argument;
          }
        }
        else {
          $args[] = isset($arguments[$id]['exception']['value']) ? $arguments[$id]['exception']['value'] : 'all';
        }
        break;

      case 'fixed':
        $args[] = $argument['fixed'];
        break;

      case 'panel':
        $args[] = isset($panel_args[$argument['panel']]) ? $panel_args[$argument['panel']] : NULL;
        break;

      case 'user':
        $args[] = (isset($conf['arguments'][$id])  && $conf['arguments'][$id] !== '') ? ctools_context_keyword_substitute($conf['arguments'][$id], array(), $contexts) : NULL;
        break;

      case 'wildcard':
        // Put in the wildcard.
        $args[] = isset($arguments[$id]['wildcard']) ? $arguments[$id]['wildcard'] : '*';
        break;

      case 'none':
      default:
        // Put in NULL.
        // The views.module knows what to do with NULL (or missing) arguments.
        $args[] = NULL;
        break;
    }
  }

  // Remove any trailing NULL arguments as these are non-args:
  while (count($args) && end($args) === NULL) {
    array_pop($args);
  }

  $view->set_arguments($args);

  $allow = $view->display_handler->get_option('allow');

  if (!empty($conf['path'])) {
    $conf['path'] = ctools_context_keyword_substitute($conf['path'], array(), $contexts);
  }
  if ($allow['path_override'] && !empty($conf['path'])) {
    $view->override_path = $conf['path'];
  }
  elseif ($path = $view->display_handler->get_option('inherit_panels_path')) {
    $view->override_path = $_GET['q'];
  }

  if ($allow['items_per_page'] && isset($conf['items_per_page'])) {
    $view->set_items_per_page($conf['items_per_page']);
  }

  if ($allow['offset']) {
    $view->set_offset($conf['offset']);
  }

  if ($allow['exposed_form'] && !empty($conf['exposed'])) {
    foreach ($conf['exposed'] as $filter_name => $filter_value) {
      if (!is_array($filter_value)) {
        $conf['exposed'][$filter_name] = ctools_context_keyword_substitute($filter_value, array(), $contexts);
      }
    }
    $view->set_exposed_input($conf['exposed']);
  }

  $stored_feeds = drupal_add_feed();

  $view->preview = TRUE;
  $view->pre_execute($args);
  $view->build();
  $rows = $view->build_info['count_query']->countQuery()->execute()->fetchField();

  return $not ? !$rows : $rows;
}

/**
 * Provide a summary description based upon the checked terms.
 */
function views_content_visibility_views_ctools_access_summary($conf, $context) {
  list($name, $display) = explode('-', $conf['view']);
  $view = views_get_view($name);
  if (empty($view)) {
    return t('@view not found!', array('@view' => $conf['view']));
  }
  return t('View: @view - @display has results', array(
    '@view' => $view->human_name,
    '@display' => $view->display[$display]->display_title,
  ));
}
