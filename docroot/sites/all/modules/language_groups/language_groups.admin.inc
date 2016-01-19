<?php
/**
 * @file
 * Admin settings page for Language Groups.
 */

/**
 * Provides administration overview page for language groups.
 */
function language_groups_groups_overview() {
  $groups = variable_get('language_groups', array());
  $header = array(t('Name'), t('Languages'), t('Operations'));
  $rows = array();

  $caption = t('The following groups will be available as selection rules for panels variants.');

  foreach ($groups as $name => $group) {
    $ops = array(
      l(t('Edit'), 'admin/config/regional/language/groups/' . $name . '/edit', array('query' => array('destination' => 'admin/config/regional/language/groups'))),
      l(t('Delete'), 'admin/config/regional/language/groups/' . $name . '/delete', array('query' => array('destination' => 'admin/config/regional/language/groups'))),
    );

    $rows[] = array(
      $name,
      implode(',', $group),
      implode(' | ', $ops),
    );
  }
  return theme('table', array('header' => $header, 'rows' => $rows, 'caption' => $caption));
}

/**
 * FormAPI callback to build the 'config_add' form.
 */
function language_groups_config_add_form($form, &$form_state) {
  $form['name'] = array(
    '#title' => 'Name',
    '#type' => 'textfield',
    '#default_value' => '',
    '#description' => 'The unique ID for this Language Group. This must contain only lower case letters, numbers and underscores.',
    '#required' => 1,
    '#maxlength' => 255,
  );
  $form['langcodes'] = array(
    '#title' => 'Language Codes',
    '#type' => 'textfield',
    '#default_value' => '',
    '#description' => 'A comma separated list of language codes to be included in this group.',
    '#required' => 1,
    '#maxlength' => 255,
  );

  $form['actions']['#type'] = 'actions';
  $form['actions']['save'] = array(
    '#type' => 'submit',
    '#value' => t('Add'),
  );
  $form['actions']['cancel'] = array(
    '#type' => 'link',
    '#title' => t('Cancel'),
    '#href' => isset($_GET['destination']) ? $_GET['destination'] : 'admin/config/regional/language/groups',
  );
  return $form;
}

/**
 * FormAPI callback to validate the 'config_add' form.
 */
function language_groups_config_add_form_validate($form, &$form_state) {
  // Check for string identifier sanity.
  if (!preg_match('!^[a-z0-9_-]+$!', $form_state['values']['name'])) {
    form_set_error('name', t('The name can only consist of lowercase letters, underscores, dashes, and numbers.'));
    return;
  }

  // Validate that the new name is not used and is in the correct format.
  $language_groups = variable_get('language_groups', array());
  $new_name = $form_state['values']['name'];
  if (isset($language_groups[$new_name])) {
    form_set_error('name', t('The entered name is already in use.'));
  }

  // Validate that the languages exist.
  $langcodes = explode(',', $form_state['values']['langcodes']);
  $languages = array_keys(language_list());
  foreach ($langcodes as $langcode) {
    if (!in_array($langcode, $languages)) {
      form_set_error('langcodes', t('The language code %input is not valid.', array('%input' => $langcode)));
    }
  }

}

/**
 * FormAPI callback to save the 'config_add' form.
 */
function language_groups_config_add_form_submit($form, &$form_state) {
  $groups = variable_get('language_groups', array());

  $groups[$form_state['values']['name']] = explode(',', $form_state['values']['langcodes']);

  variable_set('language_groups', $groups);

  $form_state['redirect'] = 'admin/config/regional/language/groups';
}

/**
 * FormAPI callback to build the edit language group form.
 */
function language_groups_config_edit_form($form, &$form_state, $group) {
  $groups = variable_get('language_groups', array());

  if (!isset($groups[$group])) {
    drupal_set_message('Group not found.', 'error');
    return;
  }

  $form = language_groups_config_add_form($form, $form_state);
  $form['name']['#default_value'] = isset($group) ? $group : '';
  $form['langcodes']['#default_value'] = isset($groups[$group]) ? implode(',', $groups[$group]) : '';
  $form['name']['#disabled'] = TRUE;

  $form['actions']['save']['#value'] = t('Update');

  $form['#validate'][] = 'language_groups_config_edit_form_validate';
  $form['#submit'][] = 'language_groups_config_add_form_submit';

  return $form;
}

/**
 * FormAPI callback to validate the edit language group form.
 */
function language_groups_config_edit_form_validate($form, &$form_state) {
  // Validate that the languages exist.
  $langcodes = explode(',', $form_state['values']['langcodes']);
  $languages = array_keys(language_list());
  foreach ($langcodes as $langcode) {
    if (!in_array($langcode, $languages)) {
      form_set_error('langcodes', t('The language code %input is not valid.', array('%input' => $langcode)));
    }
  }
}

/**
 * FormAPI callback to build the delete language groups form.
 */
function language_groups_delete_form($form, &$form_state, $group) {
  $form_state['language_groups']['group'] = $group;
  $form['delete'] = array(
    '#value' => 'This action will permanently remove this item.',
  );
  $form['actions']['#type'] = 'actions';
  $form['actions']['save'] = array(
    '#type' => 'submit',
    '#value' => t('Delete'),
  );
  $form['actions']['cancel'] = array(
    '#type' => 'link',
    '#title' => t('Cancel'),
    '#href' => isset($_GET['destination']) ? $_GET['destination'] : 'admin/config/regional/language/groups',
  );
  return $form;
}

/**
 * FormAPI callback for submission of delete language groups form.
 */
function language_groups_delete_form_submit($form, &$form_state) {
  $groups = variable_get('language_groups', array());
  if (isset($groups[$form_state['language_groups']['group']])) {
    unset($groups[$form_state['language_groups']['group']]);
    variable_set('language_groups', $groups);
  }
}
