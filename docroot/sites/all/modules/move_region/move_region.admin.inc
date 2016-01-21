<?php

/**
 * @file
 * Administration interface for the Move Region module.
 */

/**
 * Menu callback; Lists all the available configurations.
 */
function move_region_list() {
  $themes = list_themes();

  // Build a table of all the existing configurations.
  $rows = array();
  foreach (move_region_load_multiple() as $configuration) {
    $rows[] = array(
      check_plain($themes[$configuration->theme]->info['name']),
      check_plain(t($themes[$configuration->theme]->info['regions'][$configuration->source])),
      check_plain(t($themes[$configuration->theme]->info['regions'][$configuration->destination])),
      l(t('Edit'), 'admin/config/content/move-region/configure/' . $configuration->id) . '<br />' . l(t('Delete'), 'admin/config/content/move-region/delete/' . $configuration->id),
    );
  }

  $page = array();

  if (!empty($rows)) {
    $page['config_list'] = array(
      '#theme' => 'table',
      '#header' => array(t('Theme'), t('Source'), t('Destination'), t('Actions')),
      '#rows' => $rows,
    );
  }

  return $page;
}

/**
 * Menu callback; Form builder for the configuration edit form.
 *
 * @param integer $id
 *   The unique ID of the configuration. Leave empty to create a new
 *   configuration.
 */
function move_region_configure_form($form, &$form_state, $id = NULL) {
  drupal_theme_initialize();

  $config = move_region_load($id);

  // Check if the theme has been passed in with ajax.
  if (isset($form_state['values']['theme'])) {
    $selected_theme = $form_state['values']['theme'];
  }
  else {
    $selected_theme = !empty($config->theme) ? $config->theme : variable_get('theme_default', 'bartik');
  }

  // Build a list of enabled themes.
  $themes = array();
  foreach (list_themes() as $theme) {
    if (!empty($theme->status) || ($selected_theme && $theme->name == $selected_theme)) {
      $themes[$theme->name] = check_plain($theme->info['name']);
    }
  }

  $regions = system_region_list($selected_theme, REGIONS_VISIBLE);

  // Build the form.
  $form = array();

  $form['id'] = array(
    '#type' => 'value',
    '#value' => !empty($config->id) ? $config->id : NULL,
  );

  $form['theme'] = array(
    '#title' => t('Theme'),
    '#type' => 'select',
    '#options' => $themes,
    '#default_value' => $selected_theme,
    '#ajax' => array(
      'callback' => 'move_region_configure_form_js',
      'wrapper' => 'regions-wrapper',
    ),
  );

  $form['regions'] = array(
    '#prefix' => '<div id="regions-wrapper">',
    '#suffix' => '</div>',
  );

  $form['regions']['source'] = array(
    '#title' => t('Source'),
    '#description' => t('The region that contains the content to be moved.'),
    '#type' => 'select',
    '#options' => $regions,
    '#default_value' => !empty($config->source) ? $config->source : NULL,
  );

  $form['regions']['destination'] = array(
    '#title' => t('Destination'),
    '#description' => t('The region to which the content should be moved.'),
    '#type' => 'select',
    '#options' => $regions,
    '#default_value' => !empty($config->destination) ? $config->destination : NULL,
  );

  $form['config']['pages'] = array(
    '#title' => t('Pages'),
    '#description' => t("Specify pages by using their paths. Enter one path per line. The '*' character is a wildcard. Example paths are %blog for the blog page and %blog-wildcard for every personal blog. %front is the front page.", array('%blog' => 'blog', '%blog-wildcard' => 'blog/*', '%front' => '<front>')),
    '#type' => 'textarea',
    '#default_value' => !empty($config->pages) ? $config->pages : '',
  );

  $form['actions'] = array('#type' => 'actions');
  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save configuration'),
  );

  if (!empty($id)) {
    $form['actions']['delete'] = array(
      '#type' => 'submit',
      '#value' => t('Delete'),
      '#weight' => 2,
      '#submit' => array('move_region_delete_submit'),
    );
  }

  $form['actions']['cancel'] = array(
    '#type' => 'link',
    '#title' => t('Cancel'),
    '#href' => 'admin/config/content/move-region',
    '#weight' => 3,
  );

  return $form;
}

/**
 * Validate handler for the configuration edit form.
 */
function move_region_configure_form_validate($form, &$form_state) {
  // The source and destination should be different.
  if ($form_state['values']['source'] == $form_state['values']['destination']) {
    form_set_error('source', t('The source and destination regions should be different.'));
  }

  // Each source-destination combination should be unique. Check if the current
  // combination already exists in the database.
  $query = db_select('move_region', 'mr')
    ->condition('mr.theme', $form_state['values']['theme'])
    ->condition('mr.source', $form_state['values']['source'])
    ->condition('mr.destination', $form_state['values']['destination']);
  // Allow to overwrite an existing configuration.
  if (!empty($form_state['values']['id'])) {
    $query->condition('mr.id', $form_state['values']['id'], '<>');
  }
  if ($query->countQuery()->execute()->fetchField()) {
    form_set_error('source', t('A configuration with this source and destination already exists. Please choose a different combination.'));
  }
}

/**
 * Submit handler for the configuration edit form.
 */
function move_region_configure_form_submit($form, &$form_state) {
  db_merge('move_region')
    ->key(array('id' => $form_state['values']['id']))
    ->fields(array(
      'theme' => $form_state['values']['theme'],
      'source' => $form_state['values']['source'],
      'destination' => $form_state['values']['destination'],
      'pages' => trim($form_state['values']['pages']),
    ))
    ->execute();

  drupal_set_message(t('Your configuration has been saved.'));
  $form_state['redirect'] = 'admin/config/content/move-region';
}

/**
 * Ajax handler for returning the regions in a theme.
 */
function move_region_configure_form_js($form, $form_state) {
  return $form['regions'];
}

/**
 * Button submit function: handle the 'Delete' button on the configuration form.
 */
function move_region_delete_submit($form, &$form_state) {
  $form_state['redirect'] = array(
    'admin/config/content/move-region/delete/' . $form_state['values']['id'],
    array('query' => drupal_get_destination()),
  );
}

/**
 * Menu callback; Confirmation form for deleting a configuration.
 */
function move_region_delete_confirm($form, &$form_state, $id) {
  // Pass the id to the submit handler.
  $form['id'] = array(
    '#type' => 'value',
    '#value' => $id,
  );
  return confirm_form($form,
    t('Are you sure you want to delete the configuration?'),
    'admin/config/content/move-region',
    t('This action cannot be undone.'),
    t('Delete'));
}

/**
 * Submit handler; Deletes a configuration.
 */
function move_region_delete_confirm_submit($form, &$form_state) {
  db_query('DELETE FROM {move_region} WHERE id = :id', array(':id' => $form_state['values']['id']));
  drupal_set_message(t('The configuration has been deleted.'));
  // The destination parameter may be set to the edit form of the configuration
  // which has just been removed, so make sure to unset it.
  unset($_GET['destination']);
  $form_state['redirect'] = 'admin/config/content/move-region';
}
