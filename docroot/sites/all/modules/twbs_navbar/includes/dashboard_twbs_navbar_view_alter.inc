<?php

/**
 * Implements hook_twbs_navbar_view_alter().
 */
function dashboard_twbs_navbar_view_alter(&$build) {
  if (user_access('access dashboard')) {
    $build['navbar_nav']['dashboard'] = array(
      '#theme' => 'link',
      '#text' => t('Dashboard'),
      '#path' => 'admin/dashboard',
      '#options' => array(
        'html' => TRUE,
        'attributes' => array(
          'class' => array(
            'twbs-navbar-icon',
            'twbs-navbar-icon-dashboard',
          ),
        ),
      ),
    );
  }
}
