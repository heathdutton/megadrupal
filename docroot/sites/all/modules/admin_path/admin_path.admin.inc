<?php

/**
 * @file
 * Administrative page callbacks for the Administrative pages module.
 */

/**
 * Form builder for the admin path configuration form.
 */
function admin_path_admin() {
  $patterns = path_get_admin_paths();

  $form['admin'] = array(
    '#type' => 'textarea',
    '#title' => t('Administrative pages'),
    '#default_value' => $patterns['admin'],
    '#description' => t("Specify pages by using their paths. Enter one path per line. The '*' character is a wildcard. Example paths are %blog for the blog page and %blog-wildcard for every personal blog. %front is the front page.", array('%blog' => 'blog', '%blog-wildcard' => 'blog/*', '%front' => '<front>')),
  );

  $form['non_admin'] = array(
    '#type' => 'textarea',
    '#title' => t('Exclusions'),
    '#default_value' => $patterns['non_admin'],
    '#description' => t("Specify pages by using their paths. Enter one path per line. The '*' character is a wildcard. Example paths are %blog for the blog page and %blog-wildcard for every personal blog. %front is the front page.", array('%blog' => 'blog', '%blog-wildcard' => 'blog/*', '%front' => '<front>')),
  );

  $form['actions'] = array('#type' => 'actions');
  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save configuration'),
  );
  $form['actions']['restore'] = array(
    '#type' => 'submit',
    '#value' => t('Restore defaults'),
    '#submit' => array('admin_path_admin_reset_submit'),
  );

  return $form;
}

/**
 * Form validation handler for the admin path configuration form.
 *
 * @see admin_path_admin()
 * @see admin_path_admin_submit()
 */
function admin_path_admin_validate($form, &$form_state) {

}

/**
 * Form submission handler for the admin path configuration form.
 *
 * @see admin_path_admin()
 * @see admin_path_admin_validate()
 */
function admin_path_admin_submit($form, &$form_state) {
  $paths = array();
  foreach (array('admin' => TRUE, 'non_admin' => FALSE) as $field => $as_admin) {
    foreach (explode("\n", $form_state['values'][$field]) as $path) {
      $path = trim($path);
      if ($path) {
        $paths[$path] = $as_admin;
      }
    }
  }
  variable_set('admin_path', $paths);
}
/**
 * Button submit function: handle the 'Delete' button on the node form.
 */
function admin_path_admin_reset_submit($form, &$form_state) {
  $form_state['redirect'] = array('admin/config/user-interface/admin-path/reset');
}

/**
 * Form builder for the admin path reset form.
 */
function admin_path_reset_confirm($form, &$form_state) {
  return confirm_form($form,
    t('Are you sure you want to reset the administrative pages configuration to the global defaults?'),
    'admin/config/user-interface/admin-path',
    t('Resetting will remove all changes to the administrative pages configuration. This action cannot be undone.'),
    t('Reset'), t('Cancel'));

}

/**
 * Reset admin path configuration.
 *
 * @see admin_path_reset_confirm()
 */
function admin_path_reset_confirm_submit($form, &$form_state) {
  variable_del('admin_path');
  $form_state['redirect'] = 'admin/config/user-interface/admin-path';
}