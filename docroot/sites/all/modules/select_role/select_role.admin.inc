<?php
/**
 * @file
 *
 * Administration of select_role.
 */

/**
 * Return form of admin form.
 */
function select_role_admin_form($form, &$form_state) {
  $roles = user_roles(TRUE);
  unset($roles[DRUPAL_AUTHENTICATED_RID]);
  $form['select_role'] = array(
    '#title' => t('Global Select Role'),
    '#type' => 'checkboxes',
    '#options' => $roles,
    '#description' => t('User who have the role(s), need to select single role only. This configuration effected for all user, if you need setting per user, visit user/*/edit'),
    '#default_value' => array_keys(variable_get('select_role_global', array())),
  );
  $form['select_role_exclude'] = array(
    '#type' => 'textarea',
    '#title' => t('Excluded Pages'),
    '#default_value' => variable_get('select_role_exclude', SELECT_ROLE_EXCLUDE),
    '#cols' => 40,
    '#rows' => 5,
    '#description' => t("Indicates which pages will be ignored (no select role checks). Example: Your custom logout page. <br />Enter one page per line as Drupal paths. The '*' character is a wildcard. <br />Example paths are '<em>blog</em>' for the blog page and '<em>blog/*</em>' for every personal blog. '<em>&lt;front&gt;</em>' is the front page."),
  );
  $form['actions'] = array('#type' => 'actions');
  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save'),
  );
  return $form;
}

/**
 * Form submit handler for the select_role_admin_form() form.
 */
function select_role_admin_form_submit($form, &$form_state) {
  $select_role = array_filter($form_state['values']['select_role']);
  $select_role_exclude = $form_state['values']['select_role_exclude'];
  if (!empty($select_role)) {
    // Build labels.
    $roles = user_roles();
    foreach ($select_role as $rid => $label) {
      $select_role[$rid] = $roles[$rid];
    }
  }

  variable_set('select_role_global', $select_role);
  variable_set('select_role_exclude', $select_role_exclude);
  drupal_set_message(t('The selected role has been updated.'));
}
