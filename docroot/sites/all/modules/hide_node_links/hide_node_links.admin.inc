<?php

/**
 * @file
 * Administrative page callbacks for the hide_node_links module.
 *
 * @ingroup hide_node_links
 */

/**
 * Add the link type Hide node links options to the link type's form.
 *
 * Caller is responsible for ensuring hide_node_links_link_bundle_settings_save()
 * is called during submission.
 */
function hide_node_links_add_link_bundle_settings(array &$form, array &$form_state, $entity, $bundle) {
  $entity_info = hide_node_links_get_link_info($entity);
  $bundle_info = hide_node_links_link_bundle_load($entity, $bundle);

  $form['hide_node_links'] = array(
    '#type' => 'fieldset',
    '#title' => t('Node links'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
    '#access' => user_access('hide node links'),
    '#group' => 'additional_settings',
    '#attached' => array(
      'js' => array(
        'vertical-tabs' => drupal_get_path('module', 'hide_node_links') . '/hide_node_links.js',
      ),
    ),
    '#tree' => TRUE,
    '#entity' => $entity,
    '#bundle' => $bundle,
    '#entity_info' => $entity_info,
    '#bundle_info' => $bundle_info,
  );

  // Hack to remove fieldset summary if Vertical tabs is not enabled.
  if (!isset($form['additional_settings'])) {
    unset($form['hide_node_links']['#attached']['js']['vertical-tabs']);
  }

  $form['hide_node_links']['description'] = array(
    '#prefix' => '<div class="description">',
    '#suffix' => '</div>',
    '#markup' => t('Changing these type settings will affect any items of this type that have either inclusion set to default.'),
  );
  $form['hide_node_links']['status'] = array(
    '#type' => 'select',
    '#title' => t('Visibility'),
    '#options' => hide_node_links_get_status_options(),
    '#default_value' => $bundle_info['status'],
  );

  $form += array('#submit' => array());
  array_unshift($form['#submit'], 'hide_node_links_link_bundle_settings_form_submit');

  if (isset($form['submit'])) {
    $form['submit'] += array('#weight' => 40);
  }
  if (isset($form['delete'])) {
    $form['delete'] += array('#weight' => 50);
  }
}

/**
 * Add hide node links options to a form.
 */
function hide_node_links_add_form_link_options(array &$form, $entity, $bundle, $id) {
  $info = hide_node_links_get_link_info($entity);

  if (!$info || empty($info['bundles'][$bundle])) {
    return;
  }

  if (!$link = hide_node_links_link_load($entity, $id)) {
    $link = array();
  }

  $bundle_info = hide_node_links_link_bundle_load($entity, $bundle);
  $link += array(
    'status' => $bundle_info['status'],
    'status_default' => $bundle_info['status'],
    'status_override' => 0,
  );

  $form['hide_node_links'] = array(
    '#type' => 'fieldset',
    '#tree' => TRUE,
    '#title' => t('Node links'),
    '#collapsible' => TRUE,
    '#collapsed' => !$link['status_override'],
    '#access' => user_access('hide node links'),
    '#group' => 'additional_settings',
    '#attached' => array(
      'js' => array(
        'vertical-tabs' => drupal_get_path('module', 'hide_node_links') . '/hide_node_links.js',
      ),
    ),
  );

  // Hack to remove fieldset summary if Vertical tabs is not enabled.
  if (!isset($form['additional_settings'])) {
    unset($form['hide_node_links']['#attached']['js']['vertical-tabs']);
  }

  if ($path = hide_node_links_get_bundle_path($entity, $bundle)) {
    $form['hide_node_links']['description'] = array(
      '#prefix' => '<div class="description">',
      '#suffix' => '</div>',
      '#markup' => t('The default node link visibility settings for this @bundle can be changed <a href="@link-type">here</a>.', array('@bundle' => drupal_strtolower($info['bundle label']), '@link-type' => url($path, array('query' => drupal_get_destination())))),
    );
  }

  // Status field (inclusion/exclusion)
  $form['hide_node_links']['status'] = array(
    '#type' => 'select',
    '#title' => t('Visibility'),
    '#options' => hide_node_links_get_status_options($link['status_default']),
    '#default_value' => $link['status_override'] ? $link['status'] : 'default',
  );
  $form['hide_node_links']['status_default'] = array(
    '#type' => 'value',
    '#value' => $link['status_default'],
  );
  $form['hide_node_links']['status_override'] = array(
    '#type' => 'value',
    '#value' => $link['status_override'],
  );

  // Add the submit handler to adjust the default values if selected.
  $form += array('#submit' => array());
  if (!in_array('hide_node_links_process_form_link_options', $form['#submit'])) {
    array_unshift($form['#submit'], 'hide_node_links_process_form_link_options');
  }
}

/**
 * Get a list of status options.
 *
 * @param $default
 *   Include a 'default' option.
 * @return
 *   An array of options.
 *
 * @see _hide_node_links_translation_strings()
 */
function hide_node_links_get_status_options($default = NULL) {
  $options = array();
  $statuses = array(
    0 => t('Visible'),
    1 => t('Hidden'),
  );

  if (isset($default)) {
    $default = $default ? 1 : 0;
    $options['default'] = t('Default (@value)', array('@value' => drupal_strtolower($statuses[$default])));
  }

  $options += $statuses;

  return $options;
}