<?php
/**
 * @file
 * Admin page callbacks for the assetic module.
 */

/**
 * Menu callback for the admin form.
 *
 * @see assetic_menu()
 */
function assetic_admin_form($form, &$form_state) {
  $form['#tree'] = TRUE;

  $form['filters'] = array(
    '#type' => 'item',
    '#theme' => 'assetic_admin_form_filter',
  );

  // Retrieve available filters and load all configured filters for
  // existing filters.
  $form['#filters'] = assetic_get_filter_info();

  foreach ($form['#filters'] as $alias => $filter) {
    $form['filters'][$alias]['filter'] = array(
      '#markup' => $filter['title'],
    );

    $form['filters'][$alias]['alias'] = array(
      '#type' => 'value',
      '#value' => $alias,
    );

    $form['filters'][$alias]['status'] = array(
      '#type' => 'checkbox',
      '#title' => t('Status of @filter', array('@filter' => $filter['title'])),
      '#title_display' => 'invisible',
      '#default_value' => $filter['status'],
      '#parents' => array('filters', $alias, 'status'),
    );

    $form['filters'][$alias]['config'] = array(
      '#markup' => $filter['admin'] !== NULL ? l(t('configure'), 'admin/config/development/assetic/' . $alias) : '-',
    );

    $form['filters'][$alias]['weight'] = array(
      '#type' => 'weight',
      '#title' => t('Weight for @filter', array('@filter' => $filter['title'])),
      '#title_display' => 'invisible',
      '#default_value' => $filter['weight'],
    );

    $form['filters'][$alias]['#weight'] = $filter['weight'];
  }

  $form = system_settings_form($form);
  // Use our own submit handler instead of system_settings_form_submit.
  $form['#submit'] = array('assetic_admin_form_submit');

  return $form;
}

/**
 * Submit handler of the admin form.
 *
 * @see assetic_admin_form()
 */
function assetic_admin_form_submit($form, &$form_state) {
  // Remove unnecessary values.
  form_state_values_clean($form_state);

  $filters = assetic_get_filter_info();

  foreach ($form_state['values']['filters'] as $alias => $filter) {
    $filter = drupal_array_merge_deep($filters[$alias], $filter);
    assetic_filter_save($filter);
  }

  // Clear all cached data so the changes are visible.
  assetic_filters_reset();

  drupal_set_message(t('The filter ordering has been saved.'));
}

/**
 * Page callback for the admin form of individual filter configuration.
 *
 * @see assetic_menu()
 */
function assetic_admin_filter_page($filter) {
  if ($filter['admin'] === NULL) {
    return drupal_not_found();
  }

  return drupal_get_form($filter['admin']);
}

/**
 * Title callback for the admin form of individual filter configuration.
 *
 * @see assetic_menu()
 */
function assetic_admin_filter_title($filter) {
  return t('!filter Configuration', array(
    '!filter' => $filter['title'],
  ));
}
