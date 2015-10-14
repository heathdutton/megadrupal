<?php

/**
 * @file
 * Provides the Views' administrative interface.
 */

/**
 * Form builder for beanstag_admin form.
 *
 * @see system_settings_form()
 */
function beanstag_admin() {
  $form = array();

  $form['beanstag_override'] = array(
    '#type' => 'checkbox',
    '#title' => t('Override'),
    '#default_value' => variable_get('beanstag_override', 0),
    '#description' => t('Override existing meta tags.'),
    '#required' => FALSE,
  );

  $form['beanstag_enable_meta_robots'] = array(
    '#type' => 'checkbox',
    '#title' => t('Enable meta robots'),
    '#default_value' => variable_get('beanstag_enable_meta_robots', 0),
    '#description' => t('Enable meta robots in BeansTag.'),
    '#required' => FALSE,
  );

  $form['beanstag_enable_meta_canonical'] = array(
    '#type' => 'checkbox',
    '#title' => t('Enable meta canonical'),
    '#default_value' => variable_get('beanstag_enable_meta_canonical', 0),
    '#description' => t('Enable meta canonical in BeansTag.'),
    '#required' => FALSE,
  );

  return system_settings_form($form);
}

/**
 * Menu callback; BeansTag listing.
 *
 * @param string $keys
 *   The string which the user input for searching the alias.
 *
 * @return array
 *   The built forms.
 */
function beanstag_list($keys = NULL) {
  // Add the filter form above the overview table.
  $build['path_admin_filter_form'] = drupal_get_form('beanstag_filter_form', $keys);

  // Render the beanstag listview with bulk delete form.
  $build['bulk_delete_form'] = drupal_get_form('beanstag_bulk_delete_form', $keys);

  return $build;
}

/**
 * Menu callback; handles pages for creating and editing BeansTag.
 *
 * @ingroup forms
 */
function beanstag_load_admin_form() {
  $prev_path = (isset($_SESSION['last_request_page']) && isset($_SESSION['last_request_page'])) ? $_SESSION['last_request_page'] : '';
  $beanstag = beanstag_load($prev_path);
  if (empty($beanstag)) {
    $beanstag = array(
      'path_alias' => $prev_path,
      'page_title' => '',
      'meta_keywords' => '',
      'meta_description' => '',
      'meta_robots' => '',
      'meta_canonical' => '',
      'id' => NULL,
    );
  }
  $output = drupal_get_form('beanstag_admin_form', $beanstag);

  return $output;
}

/**
 * Return a form for BeansTag editing.
 *
 * @ingroup forms
 * @see beanstag_admin_form_submit()
 * @see beanstag_admin_form_validate()
 */
function beanstag_admin_form($form, &$form_state, $form_data = NULL) {
  if (empty($form_data)) {
    $form_data['path_alias'] = '';
    $form_data['page_title'] = '';
    $form_data['meta_keywords'] = '';
    $form_data['meta_description'] = '';
    $form_data['meta_robots'] = '';
    $form_data['meta_canonical'] = '';
    $form_data['id'] = NULL;
  }

  // Define a fieldset.
  $form['beanstag'] = array(
    '#type' => 'fieldset',
    '#title' => t('BeansTag'),
  );

  $description  = t('Enter the path alias which you want to apply the BeansTag. (ex. node/1)');
  $description .= '<br>';
  $description .= t('You can append * at the end of the path alias as a wildcard. (ex. node/*)');
  $description .= '<br>';
  $description .= t('But please note that this * wildcard only works at end of the path alias.');

  $form['beanstag']['path_alias'] = array(
    '#type' => 'textfield',
    '#title' => t('Path Alias'),
    '#description' => $description,
    '#size' => '160',
    '#maxlength' => '255',
    '#default_value' => $form_data['path_alias'],
    '#required' => TRUE,
  );

  $form['beanstag']['page_title'] = array(
    '#type' => 'textfield',
    '#title' => t('Page Title'),
    '#description' => t('Enter the page title. Enter <i>&lt;none&gt;<i> if you only want to display the site name.'),
    '#size' => '160',
    '#maxlength' => '255',
    '#default_value' => $form_data['page_title'],
    '#required' => TRUE,
  );

  $form['beanstag']['meta_keywords'] = array(
    '#type' => 'textarea',
    '#title' => t('Meta Keywords'),
    '#description' => t('Enter the meta keywords in comma separated format.'),
    '#maxlength' => '255',
    '#default_value' => $form_data['meta_keywords'],
    '#required' => FALSE,
  );

  $form['beanstag']['meta_description'] = array(
    '#type' => 'textarea',
    '#title' => t('Meta Description'),
    '#description' => t('Enter the meta description.'),
    '#size' => '255',
    '#maxlength' => '255',
    '#default_value' => $form_data['meta_description'],
    '#required' => FALSE,
  );

  if (variable_get('beanstag_enable_meta_robots', 0) == 1) {
    $form['beanstag']['meta_robots'] = array(
      '#type' => 'textarea',
      '#title' => t('Meta Robots'),
      '#description' => t('Enter the meta robots.'),
      '#size' => '255',
      '#maxlength' => '255',
      '#default_value' => $form_data['meta_robots'],
      '#required' => FALSE,
    );
  }
  else {
    $form['beanstag']['meta_robots'] = array(
      '#type' => 'hidden',
      '#title' => t('Meta Robots'),
      '#description' => t('Enter the meta robots.'),
      '#size' => '255',
      '#maxlength' => '255',
      '#default_value' => $form_data['meta_robots'],
      '#required' => FALSE,
    );
  }

  if (variable_get('beanstag_enable_meta_canonical', 0) == 1) {
    $form['beanstag']['meta_canonical'] = array(
      '#type' => 'textarea',
      '#title' => t('Meta Canonical'),
      '#description' => t('Enter the meta canonical.'),
      '#size' => '255',
      '#maxlength' => '255',
      '#default_value' => $form_data['meta_canonical'],
      '#required' => FALSE,
    );
  }
  else {
    $form['beanstag']['meta_canonical'] = array(
      '#type' => 'hidden',
      '#title' => t('Meta Canonical'),
      '#description' => t('Enter the meta canonical.'),
      '#size' => '255',
      '#maxlength' => '255',
      '#default_value' => $form_data['meta_canonical'],
      '#required' => FALSE,
    );
  }

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Submit'),
  );

  $path_id = isset($form_state['input']['id']) ? $form_state['input']['id'] : $form_data['id'];

  if ($path_id) {
    $form['id'] = array(
      '#type' => 'hidden',
      '#value' => $path_id,
    );
    $form['actions']['delete'] = array(
      '#type' => 'submit',
      '#value' => t('Delete'),
      '#submit' => array('beanstag_admin_form_delete_submit'),
    );
  }

  if (module_exists('token')) {
    $form['token_tree'] = array(
      '#theme' => 'token_tree',
      '#token_types' => array('node', 'term', 'site'),
    );
  }
  else {
    $form['token_tree'] = array(
      '#markup' => '<p>' . t('Enable the <a href="@drupal-token">Token module</a> to view the available token browser.', array('@drupal-token' => 'http://drupal.org/project/token')) . '</p>',
    );
  }

  return $form;
}

/**
 * Validate BeansTag form.
 *
 * @ingroup forms
 */
function beanstag_admin_form_validate($form, &$form_state) {
  // Bypass path checking if a wildcard exists.
  if (strpos($form_state['values']['path_alias'], '*') === FALSE) {
    // Check if the path exists.
    if (!drupal_valid_path($form_state['values']['path_alias'])) {
      // Not a system URL.
      if (!drupal_lookup_path('source', $form_state['values']['path_alias'])) {
        // Not a path alias.
        form_set_error('path_alias', t('The path does not exist.'));
        return;
      }
    }
  }

  // Check if path alias already existed.
  $path_id = isset($form_state['values']['id']) ? $form_state['values']['id'] : NULL;
  if ($form_state['values']['path_alias'] && _beanstag_exists($form_state['values']['path_alias'], $path_id)) {
    form_set_error('path_alias', t('Path alias already exists.'));
  }
}

/**
 * Submit function for the BeansTag editing form.
 *
 * @ingroup forms
 */
function beanstag_admin_form_submit($form, &$form_state) {
  global $user;

  if (!empty($form_state['values']['id'])) {
    // Update an existing record.
    $schema = drupal_get_schema('beanstag');
    $status = drupal_write_record('beanstag', $form_state['values'], 'id');

    if ($status === SAVED_UPDATED) {
      $message = t('Your BeansTag (id = %id) is updated successfully.', array('%id' => check_plain($form_state['values']['id'])));
      drupal_set_message($message);
    }
  }
  else {
    // Insert a new record.
    $id = db_insert('beanstag')
      ->fields(array(
          'uid'               => $user->uid,
          'path_alias'        => $form['beanstag']['path_alias']['#value'],
          'page_title'        => $form['beanstag']['page_title']['#value'],
          'meta_keywords'     => $form['beanstag']['meta_keywords']['#value'],
          'meta_description'  => $form['beanstag']['meta_description']['#value'],
          'meta_robots'       => $form['beanstag']['meta_robots']['#value'],
          'meta_canonical'    => $form['beanstag']['meta_canonical']['#value'],
        )
      )
      ->execute();
    $message = t("Your BeansTag (id = $id) is created successfully.");
    drupal_set_message($message);
  }

  $form_state['redirect'] = 'admin/structure/beanstag';
}

/**
 * Submit function for the 'Delete' button on the beanstag editing form.
 *
 * @ingroup forms
 */
function beanstag_admin_form_delete_submit($form, &$form_state) {
  $destination = array();
  if (isset($_GET['destination'])) {
    $destination = drupal_get_destination();
    unset($_GET['destination']);
  }
  $form_state['redirect'] = array('admin/structure/beanstag/delete/' . $form_state['values']['id'], array('query' => $destination));
}

/**
 * Menu callback; confirms deleting a beanstag.
 *
 * @ingroup forms
 * @see beanstag_admin_delete_confirm_submit()
 */
function beanstag_admin_delete_confirm($form, &$form_state, $beanstag) {
  if (user_access('administer beanstag')) {
    $form_state['beanstag'] = $beanstag;
    return confirm_form(
      $form,
      t('Are you sure you want to delete BeansTag %title?',
      array('%title' => $beanstag['path_alias'])),
      'admin/structure/beanstag'
    );
  }
  return array();
}

/**
 * Execute beanstag deletion.
 *
 * @ingroup forms
 */
function beanstag_admin_delete_confirm_submit($form, &$form_state) {
  if ($form_state['values']['confirm']) {
    $query = db_delete('beanstag')
      ->condition('id', $form_state['beanstag']['id'])
      ->execute();
    $form_state['redirect'] = 'admin/structure/beanstag';
  }
}

/**
 * Return a form to filter beanstags.
 *
 * @ingroup forms
 * @see beanstag_filter_form_submit()
 */
function beanstag_filter_form($form, &$form_state, $keys = '') {
  $form['#attributes'] = array('class' => array('search-form'));
  $form['basic'] = array(
    '#type' => 'fieldset',
    '#title' => t('Filter aliases'),
    '#attributes' => array('class' => array('container-inline')),
  );
  $form['basic']['filter'] = array(
    '#type' => 'textfield',
    '#title' => t('Path alias'),
    '#title_display' => 'invisible',
    '#default_value' => $keys,
    '#maxlength' => 128,
    '#size' => 25,
  );
  $form['basic']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Filter'),
    '#submit' => array('beanstag_filter_form_submit_filter'),
  );
  if ($keys) {
    $form['basic']['reset'] = array(
      '#type' => 'submit',
      '#value' => t('Reset'),
      '#submit' => array('beanstag_filter_form_submit_reset'),
    );
  }
  return $form;
}

/**
 * Process filter form submission when the Filter button is pressed.
 *
 * @ingroup forms
 */
function beanstag_filter_form_submit_filter($form, &$form_state) {
  $form_state['redirect'] = 'admin/structure/beanstag/list/' . trim($form_state['values']['filter']);
}

/**
 * Process filter form submission when the Reset button is pressed.
 *
 * @ingroup forms
 */
function beanstag_filter_form_submit_reset($form, &$form_state) {
  $form_state['redirect'] = 'admin/structure/beanstag/list';
}

/**
 * Return a form for bulk delete BeansTag.
 *
 * @ingroup forms
 * @see beanstag_bulk_delete_form_submit()
 */
function beanstag_bulk_delete_form($form, $form_state, $keys = '') {

  $query = db_select('beanstag')->extend('TableSort');

  if ($keys) {
    // Replace wildcards with PDO wildcards.
    $query->condition(db_or()
      ->condition('path_alias', '%' . preg_replace('!\*+!', '%', $keys) . '%', 'LIKE')
      ->condition('page_title', '%' . preg_replace('!\*+!', '%', $keys) . '%', 'LIKE')
      ->condition('meta_keywords', '%' . preg_replace('!\*+!', '%', $keys) . '%', 'LIKE')
      ->condition('meta_description', '%' . preg_replace('!\*+!', '%', $keys) . '%', 'LIKE')
      ->condition('meta_robots', '%' . preg_replace('!\*+!', '%', $keys) . '%', 'LIKE')
      ->condition('meta_canonical', '%' . preg_replace('!\*+!', '%', $keys) . '%', 'LIKE'));
  }

  $beanstags = $query
    ->fields('beanstag')
    ->extend('PagerDefault')
    ->limit(50)
    ->execute();

  $form['pager'] = array(
    '#theme' => 'pager',
    '#weight' => 10,
  );

  $header = array(
    'path_alias' => t('Path Alias'),
    'page_title' => t('Page Title'),
    'meta_keywords' => t('Meta Keywords'),
    'meta_description' => t('Meta Description'),
    'meta_robots' => t('Meta Robots'),
    'meta_canonical' => t('Meta Canonical'),
    'operations' => t('Operations'),
  );

  $options = array();
  $destination = drupal_get_destination();
  foreach ($beanstags as $beanstag) {

    $operations = array();
    $operations['edit'] = array(
      'title' => t('edit'),
      'href' => "admin/structure/beanstag/edit/$beanstag->id",
      'query' => $destination,
    );
    $operations['delete'] = array(
      'title' => t('delete'),
      'href' => "admin/structure/beanstag/delete/$beanstag->id",
      'query' => $destination,
    );

    $options[$beanstag->id] = array(
      'path_alias' => sprintf('<a href="%s">%s</a>', base_path() . check_plain($beanstag->path_alias), check_plain($beanstag->path_alias)),
      'page_title' => sprintf('<a href="%s">%s</a>', base_path() . check_plain($beanstag->path_alias), check_plain($beanstag->page_title)),
      'meta_keywords' => check_plain($beanstag->meta_keywords),
      'meta_description' => check_plain($beanstag->meta_description),
      'meta_robots' => check_plain($beanstag->meta_robots),
      'meta_canonical' => check_plain($beanstag->meta_canonical),
      'operations' => array(
        'data' => array(
          '#theme' => 'links',
          '#links' => $operations,
          '#attributes' => array('class' => array('links', 'inline', 'nowrap')),
        ),
      ),
    );

    // Remove disabled fields.
    if (variable_get('beanstag_enable_meta_robots', 0) == 0) {
      unset($options[$beanstag->id]['meta_robots']);
    }
    if (variable_get('beanstag_enable_meta_canonical', 0) == 0) {
      unset($options[$beanstag->id]['meta_canonical']);
    }
  }

  // Remove disabled headers.
  if (variable_get('beanstag_enable_meta_robots', 0) == 0) {
    unset($header['meta_robots']);
  }
  if (variable_get('beanstag_enable_meta_canonical', 0) == 0) {
    unset($header['meta_canonical']);
  }

  $form['table'] = array(
    '#type' => 'tableselect',
    '#header' => $header,
    '#options' => $options,
    '#empty' => t('No BeansTag available. <a href="@link">Add BeansTag</a>.', array('@link' => url('admin/structure/beanstag/add'))),
  );

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Delete Selected'),
    '#submit' => array('beanstag_bulk_delete_form_submit'),
  );

  return $form;
}

/**
 * Submit function for the 'Delete' button on the BeansTag bulk delete form.
 *
 * @ingroup forms
 * @see beanstag_bulk_delete_form_confirm()
 */
function beanstag_bulk_delete_form_submit($form, &$form_state) {
  if (count($form['table']['#value']) == 0) {
    drupal_set_message(t('Please select at least one BeansTag.'), 'error');
    return FALSE;
  }

  $destination = array();
  if (isset($_GET['destination'])) {
    $destination = drupal_get_destination();
    unset($_GET['destination']);
  }

  $_SESSION['selected_beanstag'] = $form['table']['#value'];
  $form_state['redirect'] = array('admin/structure/beanstag/delete/selected', array('query' => $destination));

}

/**
 * Menu callback; confirms deleting the selected BeansTag.
 *
 * @ingroup forms
 * @see beanstag_bulk_delete_form_confirm_submit()
 */
function beanstag_bulk_delete_form_confirm($form, &$form_state) {
  if (user_access('administer beanstag')) {
    return confirm_form(
      $form,
      t('Are you sure you want to delete the selected BeansTag?'),
      'admin/structure/beanstag'
    );
  }
  return array();
}

/**
 * Execute selected BeansTag deletion.
 *
 * @ingroup forms
 */
function beanstag_bulk_delete_form_confirm_submit($form, &$form_state) {
  foreach ($_SESSION['selected_beanstag'] as $beanstag_id) {
    $query = db_delete('beanstag')
      ->condition('id', $beanstag_id)
      ->execute();
    $form_state['redirect'] = 'admin/structure/beanstag';
  }
}
