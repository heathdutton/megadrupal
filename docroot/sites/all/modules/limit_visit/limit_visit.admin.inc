<?php
/**
 * @file
 * Administrative pages.
 */

/**
 * Generate the admin page.
 *
 * @return array
 *   A renderable array.
 */
function limit_visit_admin() {
  return array(
    'redirect' => drupal_get_form('limit_visit_admin_redirect'),
    'rules' => drupal_get_form('limit_visit_admin_form'),
  );
}

/**
 * Manage the limit visit redirect.
 *
 * @see limit_visit_admin_redirect_validate()
 * @see limit_visit_admin_redirect_submit()
 *
 * @ingroup forms
 */
function limit_visit_admin_redirect($form, &$form_state) {
  $form = array();
  $form['redirect'] = array(
    '#type' => 'fieldset',
    'message' => array(
      '#type' => 'markup',
      '#markup' => t('Enter the path to where users should be redirected to after they exceed their limit. Leave it blank to use your standard 403 page.'),
    ),
    'value' => array(
      '#type' => 'textfield',
      '#default_value' => variable_get("limit_visit_redirect", ""),
      '#element_validate' => array('limit_visit_admin_path_validate'),
    ),
    'submit' => array(
      '#type' => 'submit',
      '#value' => t('Save redirect'),
    ),
  );

  return $form;
}

/**
 * Element validation for paths.
 */
function limit_visit_admin_path_validate($element, &$form_state) {
  if (!drupal_valid_path($element['#value']) && !drupal_lookup_path('source', $element['#value']) && !empty($element['#value'])) {
    form_error($element, t('@path is not a valid path.', array('@path' => $element['#value'])));
  }
}

/**
 * Form submission for limit_visit_admin_redirect().
 *
 * @see limit_visit_admin_redirect()
 */
function limit_visit_admin_redirect_submit($form, &$form_state) {
  variable_set("limit_visit_redirect", $form_state['values']['value']);
}

/**
 * Manage the limit visit views.
 *
 * @see limit_visit_admin_validate()
 * @see limit_visit_admin_submit()
 *
 * @ingroup forms
 */
function limit_visit_admin_form($form, &$form_state) {
  $form['rules'] = array('#tree' => TRUE);
  $rules = NULL;

  // Load from cache.
  if ($cache = cache_get('limit_visit_rules')) {
    $rules = $cache->data;
  }

  // Load from db and store in cache.
  if (!isset($rules)) {
    $qry = db_select('limit_visit_rules', 'lvr');
    $qry->fields('lvr');
    $rules = $qry->execute()->fetchAllAssoc('lvid');
    cache_set('limit_visit_rules', $rules);
  }

  foreach ($rules as $lvid => $rule) {
    $form['rules'][$lvid]['path'] = array(
      '#type' => 'textfield',
      '#default_value' => $rule->path,
      '#maxlength' => 255,
      '#size' => 30,
      '#required' => TRUE,
    );
    $form['rules'][$lvid]['allowed'] = array(
      '#type' => 'textfield',
      '#default_value' => $rule->allowed,
      '#maxlength' => 255,
      '#size' => 30,
      '#required' => TRUE,
    );
    $form['rules'][$lvid]['delete'] = array(
      '#type' => 'checkbox',
      '#title' => t('Delete'),
      '#title_display' => 'invisible',
      '#default_value' => FALSE,
    );
  }
  // Provide fields to create a new state.
  $new_rule['path'] = array(
    '#title' => t('New rule'),
    '#type' => 'textfield',
    '#description' => t('Enter the path for the new rule.'),
    '#maxlength' => 255,
    '#size' => 30,
  );
  $new_rule['allowed'] = array(
    '#title' => t('Allowed visits per hour'),
    '#type' => 'textfield',
    '#description' => t('Enter the number of visits allowed per hour.'),
    '#maxlength' => 255,
    '#size' => 30,
  );

  $form['rules'][] = $new_rule;

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save Rules'),
  );

  return $form;
}

/**
 * A theming function for the admin form.
 *
 * @param array $variables
 *   Variables for this thing.
 *
 * @ingroup themeable
 */
function theme_limit_visit_admin_form(array $variables) {
  $form = $variables['form'];

  $header = array(
    t('Path'),
    t('Visits allowed per hour'),
    array('data' => t('Delete'), 'class' => array('checkbox')),
  );

  $rows = array();
  foreach (element_children($form['rules']) as $key) {
    $element = &$form['rules'][$key];

    $row = array(
      'data' => array(),
    );
    $row['data']['path'] = drupal_render($element['path']);
    $row['data']['allowed'] = drupal_render($element['allowed']);
    $row['data']['delete'] = drupal_render($element['delete']);

    $rows[] = $row;
  }

  $output = theme('table', array(
    'header' => $header,
    'rows' => $rows,
    'attributes' => array('id' => 'limit_visit-admin'),
  ));
  $output .= drupal_render_children($form);

  return $output;
}

/**
 * Form validation for limit_visit_admin_form().
 *
 * @see limit_visit_admin_form()
 */
function limit_visit_admin_form_validate($form, &$form_state) {
  foreach ($form_state['values']['rules'] as $key => $info) {
    if (strlen($info['allowed']) > 0 && !is_numeric($info['allowed'])) {
      form_set_error('rules][' . $key . '][allowed', t('Allowed visits per hour must be numeric.'));
      return FALSE;
    }
  }
}

/**
 * Submit admin updates.
 *
 * @see limit_visit_admin_form()
 */
function limit_visit_admin_form_submit($form, &$form_state) {
  foreach ($form_state['values']['rules'] as $key => $info) {
    if (!empty($info['delete'])) {
      db_delete('limit_visit_rules')
        ->condition('lvid', $key)
        ->execute();
      drupal_set_message(t('The rule for path %path has been deleted.', array('%path' => $info['path'])));
    }
    elseif (!empty($info['path']) && !empty($info['allowed'])) {
      db_merge('limit_visit_rules')
        ->key(array('lvid' => $key))
        ->fields(array(
          'path' => $info['path'],
          'allowed' => $info['allowed'],
        ))
        ->execute();
    }
  }
  cache_clear_all('limit_visit', 'cache', TRUE);

  drupal_set_message(t('Your settings have been saved.'));
}
