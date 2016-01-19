<?php

/**
 * @file
 * Admin page callbacks for the simple_cache module.
 */

/**
 * Form builder; Configure simple_cache settings.
 *
 * @ingroup forms
 * @see system_settings_form()
 */
function simple_cache_admin_settings() {
  $options = array(
  0 => t('All pages except those listed'),
  1 => t('Only the listed pages'),
  );
  $form['pages'] = array(
    '#type' => 'fieldset',
    '#title' => t('SimpleCache Pages'),
    '#collapsible' => TRUE,
  );
  $form['pages']['simple_cache_cacheability_option'] = array(
    '#type' => 'radios',
    '#title' => t('Cache specific pages'),
    '#options' => $options,
    '#default_value' => variable_get('simple_cache_cacheability_option', 0),
  );
  $form['pages']['simple_cache_cacheability_pages'] = array(
    '#type' => 'textarea',
    '#title' => 'Pages',
    '#default_value' => variable_get('simple_cache_cacheability_pages', SIMPLE_CACHE_NOCACHE_DEFAULT),
    '#description' => t("Specify pages by using their paths. Enter one path per line. The '*' character is a wildcard. Example paths are %blog for the blog page and %blog-wildcard for every personal blog. %front is the front page.", array('%blog' => 'blog', '%blog-wildcard' => 'blog/*', '%front' => '<front>')),
  );

  $form['roles'] = array(
    '#type' => 'fieldset',
    '#title' => t('SimpleCache Roles'),
    '#collapsible' => TRUE,
  );
  $form['roles']['simple_cache_roles'] = array(
    '#title' => t('Enable caching for specified user roles'),
    '#description' => t('Unchecked roles and the admin account (uid #1) will never be cached.'),
    '#type' => 'checkboxes',
    '#required' => TRUE,
    '#options' => _simple_cache_get_roles(TRUE),
    '#default_value' => variable_get('simple_cache_roles', array(0 =>1)),
  );
  
  $form['debug'] = array(
    '#type' => 'fieldset',
    '#title' => t('Simplecache Debugging/Development'),
    '#description' => t("Debug mode will notify user if the page is served from cache."),
    '#collapsible' => TRUE,
  );
  $form['debug']['simple_cache_debug'] = array(
    '#title' => t('Enable debug mode.'),
    '#type' => 'checkbox',
    '#default_value' => variable_get('simple_cache_debug', 0),
  );
  return system_settings_form($form);
}

/**
 * Helper function, get simple_cache user roles
 * @param $all_roles = return all user roles
 * @return array of user roles
 */
function _simple_cache_get_roles($all_roles = FALSE) {
  $roles = user_roles();
  $roles[DRUPAL_AUTHENTICATED_RID] .= ' ' . t('(without additional roles)');
  if ($all_roles) {
    return $roles;
  }
  else {
    $simple_cache_roles = array();
    foreach (variable_get('simple_cache_roles', array(0 =>1)) as $key) {
      $simple_cache_roles[$key] = $roles[$key];
    }
    return $simple_cache_roles;
  }
}