<?php
/**
 * @file
 * recruiter_menu.features.menu_custom.inc
 */

/**
 * Implements hook_menu_default_menu_custom().
 */
function recruiter_menu_menu_default_menu_custom() {
  $menus = array();

  // Exported menu: menu-applicant-menu.
  $menus['menu-applicant-menu'] = array(
    'menu_name' => 'menu-applicant-menu',
    'title' => 'Applicant menu',
    'description' => '',
  );
  // Exported menu: menu-login-menu.
  $menus['menu-login-menu'] = array(
    'menu_name' => 'menu-login-menu',
    'title' => 'Login menu',
    'description' => '',
  );
  // Exported menu: menu-recruiter-menu.
  $menus['menu-recruiter-menu'] = array(
    'menu_name' => 'menu-recruiter-menu',
    'title' => 'Recruiter menu',
    'description' => '',
  );
  // Exported menu: user-menu.
  $menus['user-menu'] = array(
    'menu_name' => 'user-menu',
    'title' => 'User menu',
    'description' => 'The <em>User</em> menu contains links related to the user\'s account, as well as the \'Log out\' link.',
  );
  // Translatables
  // Included for use with string extractors like potx.
  t('Applicant menu');
  t('Login menu');
  t('Recruiter menu');
  t('The <em>User</em> menu contains links related to the user\'s account, as well as the \'Log out\' link.');
  t('User menu');


  return $menus;
}
