<?php

/**
 * @file
 * This file contains functions needed to add the webform-7.x-4.x progressbar
 * to webform-7.x-3.x.
 *
 * Code is largely copied from the webform module.
 */

/**
 * Implements hook_theme().
 */
function webform_steps_w3_theme() {
  $theme['webform_progressbar'] = array(
    'variables' => array('node' => NULL, 'page_num' => NULL, 'page_count' => NULL, 'page_labels' => array()),
    'template' => 'templates/webform-progressbar',
  );
  return $theme;
}

/**
 * Implements hook_theme_registry_alter().
 */
function webform_steps_w3_theme_registry_alter(&$theme) {
  $webform_path = '/webform/templates';
  $theme_path = &$theme['webform_form']['path'];
  if (!isset($theme_path) || substr($theme_path, -strlen($webform_path)) == $webform_path) {
    $theme_path = drupal_get_path('module', 'webform_steps_w3') . '/templates';
  }
}

/**
 * Prepare for theming of the webform progressbar.
 */
function template_preprocess_webform_progressbar(&$vars) {
  // Add CSS used by the progress bar.
  drupal_add_css(drupal_get_path('module', 'webform_steps_w3') . '/css/webform_steps_w3.css');

  $vars['progressbar_page_number'] = $vars['node']->webform['progressbar_page_number'];
  $vars['progressbar_percent'] = $vars['node']->webform['progressbar_percent'];
  $vars['progressbar_bar'] = $vars['node']->webform['progressbar_bar'];
  $vars['progressbar_pagebreak_labels'] = $vars['node']->webform['progressbar_pagebreak_labels'];
  $vars['percent'] = ($vars['page_num'] - 1) / ($vars['page_count'] - 1) * 100;
}

/**
 * Add the progressbar to a webform_client_form().
 */
function webform_steps_w3_progressbar(&$form, &$form_state) {
  $node = $form['#node'];

  if (isset($form['submitted'])) {
    $page_count = $form_state['webform']['page_count'];
    $page_num = $form_state['webform']['page_num'];
    if ($page_count > 1) {
      $page_labels = webform_steps_w3_page_labels($node, $form_state);
      $form['progressbar'] = array(
        '#theme' => 'webform_progressbar',
        '#node' => $node,
        '#page_num' => $page_num,
        '#page_count' => count($page_labels),
        '#page_labels' => $page_labels,
        '#weight' => -100
      );
    }
  }
}

/**
 * Find the label of a given page based on page breaks.
 *
 * @param $webform
 *   The webform node.
 * @param $form_state
 *   The form's state, if available
 * @return array
 *   An array of all page labels, indexed by page number.
 */
function webform_steps_w3_page_labels($node, $form_state = array()) {
  $page_count = 1;
  $page_labels = array();
  $page_labels[0] = t($node->webform['progressbar_label_first']);
  foreach ($node->webform['components'] as $component) {
    if ($component['type'] == 'pagebreak') {
      $page_labels[$page_count] = $component['name'];
      $page_count++;
    }
  }
  return $page_labels;
}

/**
 * Implements hook_form_FORM_ID_alter().
 */
function webform_steps_w3_form_webform_configure_form_alter(&$form, &$form_state) {
  $node = $form['#node'];
  /* Start progress bar settings form */
  $form['progressbar'] = array(
    '#type' => 'fieldset',
    '#title' => t('Progress bar'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
    '#weight' => 15,
  );
  $progress_bar_style = array();
  foreach (array('page_number', 'percent', 'bar', 'pagebreak_labels') as $name) {
    if (!empty($node->webform['progressbar_' . $name])) {
      $progress_bar_style[] = 'progressbar_' . $name;
    }
  }
  $form['progressbar']['webform_progressbar_style']  = array(
    '#type' => 'checkboxes',
    '#title' => t('Progress bar style'),
    '#options' => array(
      'progressbar_bar' => t('Show progress bar'),
      'progressbar_page_number' => t('Show page number as number of completed (i.e. Page 1 of 10)'),
      'progressbar_percent' => t('Show percentage completed (i.e. 10%)'),
      'progressbar_pagebreak_labels' => t('Show page labels from page break components'),
    ),
    '#default_value' => $progress_bar_style,
    '#description' => t('Choose how the progress bar should be displayed for multi-page forms.'),
  );
  $form['progressbar']['progressbar_label_first'] = array(
    '#type' => 'textfield',
    '#title' => t('First page label'),
    '#default_value' => $node->webform['progressbar_label_first'],
    '#maxlength' => 255,
    '#description' => t('The first page label in the progress bar. Subseqent pages are titled by their page break label.'),
    '#states' => array(
      'visible' => array(
        ':input[name="webform_progressbar_style[progressbar_pagebreak_labels]"]' => array('checked' => TRUE),
      ),
    ),
  );
  /* End progress bar settings form */

  // Add our own submit-handler right after webform_configure_submit().
  $submit = array();
  foreach ($form['#submit'] as $handler) {
    $submit[] = $handler;
    if ($handler == 'webform_configure_form_submit') {
      $submit[] = 'webform_steps_w3_webform_configure_form_submit';
    }
  }
  $form['#submit'] = $submit;
}

/**
 * Submit callback for webform_steps_w3_webform_configure_form_alter().
 */
function webform_steps_w3_webform_configure_form_submit($form, &$form_state) {
  $node = $form['#node'];
  // Set the progress bar preferences.
  $progress_bar_settings = array_filter($form_state['values']['webform_progressbar_style']);
  $node->webform['progressbar_page_number'] = (int) in_array('progressbar_page_number', $progress_bar_settings);
  $node->webform['progressbar_percent'] = (int) in_array('progressbar_percent', $progress_bar_settings);
  $node->webform['progressbar_bar'] = (int) in_array('progressbar_bar', $progress_bar_settings);
  $node->webform['progressbar_pagebreak_labels'] = (int) in_array('progressbar_pagebreak_labels', $progress_bar_settings);
  $node->webform['progressbar_label_first'] = $form_state['values']['progressbar_label_first'];
}

/**
 * Implements hook_webform_node_defaults_alter().
 */
function webform_steps_w3_webform_node_defaults_alter(&$defaults) {
  $defaults += webform_steps_w3_node_defaults();
}

/**
 * Default settings for the progressbar.
 */
function webform_steps_w3_node_defaults() {
  $progress_bar_defaults = webform_steps_w3_variable_get('webform_progressbar_style');
  $defaults = array(
    'progressbar_page_number' => in_array('progressbar_page_number', $progress_bar_defaults) ? '1' : '0',
    'progressbar_percent' => in_array('progressbar_percent', $progress_bar_defaults) ? '1' : '0',
    'progressbar_bar' => in_array('progressbar_bar', $progress_bar_defaults) ? '1' : '0',
    'progressbar_pagebreak_labels' => in_array('progressbar_pagebreak_labels', $progress_bar_defaults) ? '1' : '0',
    'progressbar_label_first' => webform_steps_w3_variable_get('webform_progressbar_label_first'),
  );
  return $defaults;
}

/**
 * Implements hook_node_load().
 */
function webform_steps_w3_node_load($nodes, $types) {
  // Quick check to see if we need to do anything at all for these nodes.
  $webform_types = webform_variable_get('webform_node_types');
  if (count(array_intersect($types, $webform_types)) == 0) {
    return;
  }
  $defaults = webform_steps_w3_node_defaults();

  // Select all webforms that match these node IDs.
  $q = db_select('webform', 'w');
  $q->leftJoin('webform_steps_w3_progressbar', 'settings', 'w.nid=settings.nid');
  $q->addfield('settings', 'nid', 'has_settings');
  $result = $q->fields('w', array('nid'))
    ->fields('settings', array_keys($defaults))
    ->condition('w.nid', array_keys($nodes), 'IN')
    ->execute()
    ->fetchAllAssoc('nid', PDO::FETCH_ASSOC);

  foreach ($result as $nid => $settings) {
    // Add progressbar settings to each node.
    if ($settings['has_settings']) {
      unset($settings['has_settings']);
      $nodes[$nid]->webform += $settings;
    }
    else {
      $nodes[$nid]->webform += $defaults;
    }
  }
}

/**
 * Implements hook_node_insert().
 */
function webform_steps_w3_node_insert($node) {
  if (!isset($node->webform)) {
    return;
  }
  $record = array_intersect_key($node->webform, webform_steps_w3_node_defaults() + array('nid' => TRUE));
  db_merge('webform_steps_w3_progressbar')
    ->key(array('nid' => $node->nid))
    ->fields($record)
    ->execute();
}

/**
 * Implements hook_node_update().
 */
function webform_steps_w3_node_update($node) {
  webform_steps_w3_node_insert($node);
}

/**
 * Implements hook_node_delete().
 */
function webform_steps_w3_node_delete($node) {
  if (!isset($node->webform)) {
    return;
  }
  db_delete('webform_steps_w3_progressbar')->condition('nid', $node->nid)->execute();
}

/**
 * Webform progressbar variables with default values.
 */
function webform_steps_w3_variable_get($variable) {
  switch ($variable) {
    case 'webform_progressbar_style':
      $result = variable_get('webform_progressbar_style', array('progressbar_bar', 'progressbar_pagebreak_labels', 'progressbar_include_confirmation'));
      break;
    case 'webform_progressbar_label_first':
      $result = variable_get('webform_progressbar_label_first', t('Start'));
      break;
    case 'webform_progressbar_label_confirmation':
      $result = variable_get('webform_progressbar_label_confirmation', t('Complete'));
      break;
  }
  return $result;
}

/**
 * Implements hook_form_FORM_ID_alter().
 */
function webform_steps_w3_form_webform_admin_settings_alter(&$form, &$form_state) {
  $form['#submit'][] = 'webform_steps_w3_admin_settings_submit';
  $form['progressbar'] = array(
    '#type' => 'fieldset',
    '#title' => t('Progress bar'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );
  $form['progressbar']['webform_progressbar_style']  = array(
    '#type' => 'checkboxes',
    '#title' => t('Progress bar style'),
    '#options' => array(
      'progressbar_bar' => t('Show progress bar'),
      'progressbar_page_number' => t('Show page number as number of completed (i.e. Page 1 of 10)'),
      'progressbar_percent' => t('Show percentage completed (i.e. 10%)'),
      'progressbar_pagebreak_labels' => t('Show page labels from page break components'),
      'progressbar_include_confirmation' => t('Include confirmation page in progress bar'),
    ),
    '#default_value' => webform_steps_w3_variable_get('webform_progressbar_style'),
    '#description' => t('Choose how the progress bar should be displayed for multi-page forms.'),
  );
  $form['progressbar']['webform_progressbar_label_first'] = array(
    '#type' => 'textfield',
    '#title' => t('First page label'),
    '#default_value' => webform_steps_w3_variable_get('webform_progressbar_label_first'),
    '#maxlength' => 255,
  );
}

/**
 * Submit handler for webform_steps_w3_form_webform_admin_settings_alter().
 */
function webform_steps_w3_webform_admin_settings_submit($form, &$form_state) {
  // Trim out empty options in the progress bar options.
  $form_state['values']['webform_progressbar_style'] = array_keys(array_filter($form_state['values']['webform_progressbar_style']));
}
