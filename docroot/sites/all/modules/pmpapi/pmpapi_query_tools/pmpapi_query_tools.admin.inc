<?php

/**
 * @file
 * Basic admin forms, validators, and submit handlers.
 */

/**
 * Form constructor for the PMPAPI query admin form.
 *
 * @see pmpapi_query_tools_admin_config_validate()
 * @see pmpapi_query_tools_admin_config_submit()
 *
 * @ingroup forms
 */
function pmpapi_query_tools_admin_config($form, &$form_state, $query = '') {

  $form['query_name'] = array(
    '#type' => 'textfield',
    '#title' => t('Query name'),
    '#size' => 12,
    '#maxlength' => 12,
    '#required' => TRUE,
    '#disabled' => $query,
    '#default_value' => ($query) ? $query : '',
    '#description' => t('Only numbers, letters and spaces allowed. Maximum 12 characters.'),
  );

  $options = pmpapi_query_tools_get_query($query);
  $saved = ($options) ? $options : array();

  $form['query_parameters'] = array(
    '#type' => 'fieldset',
    '#title' => t('Query parameters'),
  );

  $form['query_parameters']['creator'] = array(
    '#type' => 'select',
    '#title' => t('Creator'),
    '#description' => t('Limit query to the chosen creator(s).'),
    '#options' => array(0 => t('All')) + array_flip(pmpapi_get_creators()),
    '#multiple' => TRUE,
    '#default_value' => !empty($saved['creator']) ? $saved['creator'] : array(),
  );

  $form['query_parameters']['limit'] = array(
    '#type' => 'select',
    '#title' => t('Limit'),
    '#description' => t('Maximum number of docs pulled per query run.'),
    '#options' => drupal_map_assoc(array(1, 5, 10, 15, 20)),
    '#default_value' => !empty($saved['limit']) ? $saved['limit'] : 5,
  );

  $form['query_parameters']['profile'] = array(
    '#type' => 'checkboxes',
    '#title' => t('Profiles'),
    '#multiple' => TRUE,
    '#description' => t('Limit query to the checked profiles.'),
    '#options' => drupal_map_assoc(pmpapi_get_profile_list()),
    '#default_value' => !empty($saved['profile']) ? $saved['profile'] : array(),
  );

  $text_t_args = array('@syntax' => url('http://www.lucenetutorial.com/lucene-query-syntax.html'));
  $form['query_parameters']['text'] = array(
    '#type' => 'textfield',
    '#title' => t('Search text'),
    '#size' => 60,
    '#description' => t("Limit query to search snippet (using the <a href='@syntax'>Lucene query syntax</a>).", $text_t_args),
    '#default_value' => !empty($saved['text']) ? $saved['text'] : '',
  );

  $form['query_parameters']['tag'] = array(
    '#type' => 'textfield',
    '#title' => t('Tags'),
    '#size' => 60,
    '#description' => t('Limit query to the listed tags (space-separated).'),
    '#default_value' => !empty($saved['tag']) ? $saved['tag'] : '',
  );

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save settings'),
  );

  return $form;
}

/**
 * Form validation handler for pmpapi_query_tools_admin_config().
 *
 * @see pmpapi_query_tools_admin_config_submit()
 */
function pmpapi_query_tools_admin_config_validate($form, &$form_state) {
  $name_pattern = '/^[a-zA-Z0-9 ]+$/';
  if (!preg_match($name_pattern, $form_state['values']['query_name'])) {
    form_set_error('query_name', t('Only numbers, letters, and spaces allowed in query name.'));
  }

  // The form should prevent this, but adding this just in case.
  if (strlen($form_state['values']['query_name']) > 12) {
    form_set_error('query_name', t('Query name has a limit of 12 characters.'));
  }
}

/**
 * Form submission handler for pmpapi_query_tools_admin_config().
 *
 * @see pmpapi_query_tools_admin_config_validate()
 */
function pmpapi_query_tools_admin_config_submit($form, &$form_state) {
  $options = array(
    'creator' => $form_state['values']['creator'],
    'limit' => $form_state['values']['limit'],
    'profile' => $form_state['values']['profile'],
    'tag' => $form_state['values']['tag'],
    'text' => $form_state['values']['text'],
  );

  $name = $form_state['values']['query_name'];
  pmpapi_query_tools_merge_query($name, $options);
  $form_state['redirect'] = array('admin/config/services/pmp/queries');
  drupal_set_message(t('Your query settings have been saved.'));
}

/**
 * Page callback: Displays a table of saved queries.
 *
 * @return string
 *   An HTML table of queries and associated operations, or empty string, if no 
 *    queries exist.
 *
 * @see pmpapi_query_tools_menu()
 */
function pmpapi_query_tools_list_queries() {
  if ($queries = pmpapi_query_tools_get_queries()) {
    $variables['header'] = array(
      t('Query'),
      array(
        'data' => t('Operations'),
        'colspan' => '2',
      ),
    );
    foreach ($queries as $i => $query) {
      $edit = l('Edit', 'admin/config/services/pmp/queries/edit/' . $i);
      $delete = l('Delete', 'admin/config/services/pmp/queries/delete/' . $i);
      $variables['rows'][] = array($i, $edit, $delete);
    }
    return theme('table', $variables);
  }
  else {
    return '';
  }
}

/**
 * Form constructor for the query delete form.
 *
 * @see pmpapi_query_tools_remove_query_form_submit()
 *
 * @ingroup forms
 */
function pmpapi_query_tools_remove_query_form($form, &$form_state, $query) {
  $question = "Are you sure you want to delete the query: $query ?";
  return confirm_form(array(), $question, 'admin/config/services/pmp/queries');
}

/**
 * Form submission handler for pmpapi_query_tools_remove_query_form().
 */
function pmpapi_query_tools_remove_query_form_submit($form, &$form_state) {
  $query = arg(6);
  pmpapi_query_tools_delete_query($query);
  drupal_set_message(t('The query %query has been deleted.', array('%query' => $query)));
  $form_state['redirect'] = 'admin/config/services/pmp/queries';
}