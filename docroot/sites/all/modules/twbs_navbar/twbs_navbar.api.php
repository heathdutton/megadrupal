<?php

/**
 * @file
 * Documents API functions for twbs_navbar.module.
 */

/**
 * Add menu button to twbs_navbar.
 *
 * All elements should place under $build['navbar_nav'] as individual set. In
 * case 2nd level menu is required, you also need to manually add the
 * trigger as 1st level button, too.
 *
 * @param array $build
 *   The structured array describing the data to be rendered.
 */
function hook_twbs_navbar_view_alter(&$build) {
  // This example add a simple 1st level menu link back to front.
  $build['navbar_nav']['simple'] = array(
    '#theme' => 'link',
    '#text' => t('Home'),
    '#path' => '<front>',
    '#options' => array(
      'html' => TRUE,
      'attributes' => array(
        'class' => array(
          'twbs-navbar-icon',
          'twbs-navbar-icon-sample',
        ),
      ),
    ),
  );

  // This example add a complex trigger 1st level menu button, plus a list
  // of 2nd level menu links in popup style.
  $build['navbar_nav']['complex'][] = array(
    '#theme' => 'link',
    '#text' => t('User'),
    '#path' => '#',
    '#options' => array(
      'html' => TRUE,
      'attributes' => array(
        'data-toggle' => 'dropdown',
        'class' => array(
          'dropdown-toggle',
          'twbs-navbar-icon',
          'twbs-navbar-icon-complex',
        ),
      ),
    ),
  );
  $build['navbar_nav']['complex'][] = array(
    '#theme' => 'links',
    '#links' => array(
      array(
        'title' => t('Create new account'),
        'href' => 'user/register',
      ),
      array(
        'title' => t('Log in'),
        'href' => 'user',
      ),
      array(
        'title' => t('Request new password'),
        'href' => 'user/password',
      ),
    ),
    '#attributes' => array(
      'class' => 'dropdown-menu',
    ),
  );
}
