<?php
/**
 * @file
 * recruiter_menu.features.menu_links.inc
 */

/**
 * Implements hook_menu_default_menu_links().
 */
function recruiter_menu_menu_default_menu_links() {
  $menu_links = array();

  // Exported menu link: menu-applicant-menu:recruiter
  $menu_links['menu-applicant-menu:recruiter'] = array(
    'menu_name' => 'menu-applicant-menu',
    'link_path' => 'recruiter',
    'router_path' => 'recruiter',
    'link_title' => 'For Recruiters',
    'options' => array(
      'attributes' => array(
        'title' => '',
      ),
    ),
    'module' => 'menu',
    'hidden' => '0',
    'external' => '0',
    'has_children' => '0',
    'expanded' => '0',
    'weight' => '50',
  );
  // Exported menu link: menu-applicant-menu:resume
  $menu_links['menu-applicant-menu:resume'] = array(
    'menu_name' => 'menu-applicant-menu',
    'link_path' => 'resume',
    'router_path' => 'resume',
    'link_title' => 'My resume',
    'options' => array(
      'attributes' => array(
        'title' => '',
      ),
    ),
    'module' => 'menu',
    'hidden' => '0',
    'external' => '0',
    'has_children' => '0',
    'expanded' => '1',
    'weight' => '-49',
  );
  // Exported menu link: menu-applicant-menu:search/job
  $menu_links['menu-applicant-menu:search/job'] = array(
    'menu_name' => 'menu-applicant-menu',
    'link_path' => 'search/job',
    'router_path' => 'search/job',
    'link_title' => 'Job search',
    'options' => array(
      'attributes' => array(
        'title' => '',
      ),
    ),
    'module' => 'menu',
    'hidden' => '0',
    'external' => '0',
    'has_children' => '1',
    'expanded' => '1',
    'weight' => '-48',
  );
  // Exported menu link: menu-login-menu:user/login
  $menu_links['menu-login-menu:user/login'] = array(
    'menu_name' => 'menu-login-menu',
    'link_path' => 'user/login',
    'router_path' => 'user/login',
    'link_title' => 'Login / Register',
    'options' => array(
      'attributes' => array(
        'title' => '',
      ),
      'alter' => TRUE,
    ),
    'module' => 'menu',
    'hidden' => '0',
    'external' => '0',
    'has_children' => '0',
    'expanded' => '0',
    'weight' => '0',
  );
  // Exported menu link: menu-recruiter-menu:<front>
  $menu_links['menu-recruiter-menu:<front>'] = array(
    'menu_name' => 'menu-recruiter-menu',
    'link_path' => '<front>',
    'router_path' => '',
    'link_title' => 'For Applicants',
    'options' => array(
      'attributes' => array(
        'title' => '',
      ),
    ),
    'module' => 'menu',
    'hidden' => '0',
    'external' => '1',
    'has_children' => '0',
    'expanded' => '0',
    'weight' => '50',
  );
  // Exported menu link: menu-recruiter-menu:manage/job
  $menu_links['menu-recruiter-menu:manage/job'] = array(
    'menu_name' => 'menu-recruiter-menu',
    'link_path' => 'manage/job',
    'router_path' => 'manage/job',
    'link_title' => 'Manage jobs',
    'options' => array(
      'attributes' => array(
        'title' => '',
      ),
    ),
    'module' => 'menu',
    'hidden' => '0',
    'external' => '0',
    'has_children' => '0',
    'expanded' => '1',
    'weight' => '-49',
  );
  // Exported menu link: menu-recruiter-menu:search/resume
  $menu_links['menu-recruiter-menu:search/resume'] = array(
    'menu_name' => 'menu-recruiter-menu',
    'link_path' => 'search/resume',
    'router_path' => 'search/resume',
    'link_title' => 'Resume search',
    'options' => array(
      'attributes' => array(
        'title' => '',
      ),
    ),
    'module' => 'menu',
    'hidden' => '0',
    'external' => '0',
    'has_children' => '1',
    'expanded' => '0',
    'weight' => '-46',
  );
  // Exported menu link: user-menu:user
  $menu_links['user-menu:user'] = array(
    'menu_name' => 'user-menu',
    'link_path' => 'user',
    'router_path' => 'user',
    'link_title' => 'User account',
    'options' => array(
      'alter' => TRUE,
    ),
    'module' => 'system',
    'hidden' => '0',
    'external' => '0',
    'has_children' => '0',
    'expanded' => '0',
    'weight' => '-10',
  );
  // Exported menu link: user-menu:user/logout
  $menu_links['user-menu:user/logout'] = array(
    'menu_name' => 'user-menu',
    'link_path' => 'user/logout',
    'router_path' => 'user/logout',
    'link_title' => 'Log out',
    'options' => array(),
    'module' => 'system',
    'hidden' => '0',
    'external' => '0',
    'has_children' => '0',
    'expanded' => '0',
    'weight' => '10',
  );
  // Translatables
  // Included for use with string extractors like potx.
  t('For Applicants');
  t('For Recruiters');
  t('Job search');
  t('Log out');
  t('Login / Register');
  t('Manage jobs');
  t('My resume');
  t('Resume search');
  t('User account');


  return $menu_links;
}
