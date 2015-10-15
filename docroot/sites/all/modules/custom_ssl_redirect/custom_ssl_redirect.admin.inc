<?php
/**
 * @file
 * Adminitration functions of custom_ssl_redirect module.
 */

/**
 * Administration settings form.
 */
function custom_ssl_redirect_admin_form($form, $form_state) {
  $form = array();
  $form['secure'] = array(
    '#type' => 'textarea',
    '#title' => t('List of secured paths'),
    '#description' => t('Please enter the list of path regular expressions (without leading and trailing slashes), one expression per line. These paths will be forced to use HTTPS, all other paths will be forced to use HTTP (except ignored paths, see below). Use <em>&lt;front&gt;</em> for front page.'),
    '#default_value' => implode("\n", variable_get('custom_ssl_redirect_secure_paths', array())),
  );
  $form['ignore'] = array(
    '#type' => 'textarea',
    '#title' => t('List of ignored paths'),
    '#description' => t('Please enter the list of path regular expressions (without leading and trailing slashes), one expression per line. These paths will use currently used protocol (required for e.g., for correct AJAX requests). Use <em>&lt;front&gt;</em> for front page.'),
    '#default_value' => implode("\n", variable_get('custom_ssl_redirect_ignore_paths', array())),
  );
  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save configuration'),
  );
  return $form;
}

/**
 * Administration settings form submit callback.
 */
function custom_ssl_redirect_admin_form_submit($form, $form_state) {
  variable_set('custom_ssl_redirect_secure_paths', array_filter(preg_split('/[\n\r]+/', $form_state['values']['secure'])));
  variable_set('custom_ssl_redirect_ignore_paths', array_filter(preg_split('/[\n\r]+/', $form_state['values']['ignore'])));
  drupal_set_message(t('Configuration is successfully saved.'));
}
