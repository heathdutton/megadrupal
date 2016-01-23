<?php

/*
 * @file
 * Views stuff for cd_civicrm.
 */

/**
 * Implements hook_views_data_alter().
 */
function cd_civicrm_views_data_alter(&$data) {
  $data['cd_sunlight_legislators']['state_current'] = array(
    'real field' => 'state',
    'title' => t("Current User's State"),
    'help' => t('Filter the view to the State of the currently logged in user.'),
    'filter' => array(
      'handler' => 'cd_civicrm_handler_filter_state_current_user',
      'type' => 'yes-no',
    ),
  );
  $data['cd_sunlight_legislators']['district_current'] = array(
    'real field' => 'district',
    'title' => t("Current User's District"),
    'help' => t('Filter the view to the District of the currently logged in user.
              Only useful when used with the %current_state filter.',
              array('%current_state' => t("Current User's State"))),
    'filter' => array(
      'handler' => 'cd_civicrm_handler_filter_district_current_user',
      'type' => NULL,
    ),
  );
  $data['cd_sunlight_legislators']['state']['filter']['handler'] = 'cd_civicrm_handler_filter_state';
  $data['cd_sunlight_legislators']['gender']['filter']['handler'] = 'cd_civicrm_handler_filter_gender';
}

/**
 * Implements hook_views_default_views_alter().
 */
function cd_civicrm_views_default_views_alter(&$views) {
  if (isset($views['congress'])) {
    $view = & $views['congress'];
    /* Display: Block - Current user */
    $handler = $view->new_display('block', 'Block - Current user', 'block_3');
    $handler->display->display_options['defaults']['filters'] = FALSE;
    /* Filter: Congress: Current User's District */
    $handler->display->display_options['filters']['district_current']['id'] = 'district_current';
    $handler->display->display_options['filters']['district_current']['table'] = 'cd_sunlight_legislators';
    $handler->display->display_options['filters']['district_current']['field'] = 'district_current';
    $handler->display->display_options['filters']['district_current']['value'] = '1';
    $handler->display->display_options['filters']['district_current']['expose']['operator'] = FALSE;
    /* Filter: Congress: Current User's State */
    $handler->display->display_options['filters']['state_current']['id'] = 'state_current';
    $handler->display->display_options['filters']['state_current']['table'] = 'cd_sunlight_legislators';
    $handler->display->display_options['filters']['state_current']['field'] = 'state_current';
    $handler->display->display_options['filters']['state_current']['value'] = '1';
    $handler->display->display_options['filters']['state_current']['expose']['operator'] = FALSE;

    /* Display: Page - Current user */
    $handler = $view->new_display('page', 'Page - Current user', 'page_2');
    $handler->display->display_options['defaults']['filters'] = FALSE;
    /* Filter: Congress: Current User's District */
    $handler->display->display_options['filters']['district_current']['id'] = 'district_current';
    $handler->display->display_options['filters']['district_current']['table'] = 'cd_sunlight_legislators';
    $handler->display->display_options['filters']['district_current']['field'] = 'district_current';
    $handler->display->display_options['filters']['district_current']['expose']['operator'] = FALSE;
    /* Filter: Congress: Current User's State */
    $handler->display->display_options['filters']['state_current']['id'] = 'state_current';
    $handler->display->display_options['filters']['state_current']['table'] = 'cd_sunlight_legislators';
    $handler->display->display_options['filters']['state_current']['field'] = 'state_current';
    $handler->display->display_options['filters']['state_current']['value'] = '1';
    $handler->display->display_options['filters']['state_current']['expose']['operator'] = FALSE;
    $handler->display->display_options['path'] = 'my-congress';
  }
}

/**
 * Implements hook_views_handlers().
 */
function cd_civicrm_views_handlers() {
  return array(
    'info' => array(
      'path' => drupal_get_path('module', 'cd_civicrm') . '/views',
    ),
    'handlers' => array(
      'cd_civicrm_handler_filter_state_current_user' => array(
        'parent' => 'views_handler_filter_boolean_operator',
        'file' => 'cd_civicrm_handler_filter_current_user.inc',
      ),
      'cd_civicrm_handler_filter_district_current_user' => array(
        'parent' => 'views_handler_filter_boolean_operator',
        'file' => 'cd_civicrm_handler_filter_current_user.inc',
      ),
      'cd_civicrm_handler_filter_state' => array(
        'parent' => 'cd_sunlight_handler_filter_state',
      ),
      'cd_civicrm_handler_filter_gender' => array(
        'parent' => 'views_handler_filter_in_operator',
      ),
    ),
  );
}

/**
 * Implementation of hook_views_query_substitutions().
 */
function cd_sunlight_views_query_substitutions($view) {
  global $user;
  $substitutions = array();
  if ($view->base_table == 'cd_sunlight_legislators') {
    $substitutions['***CURRENT_USERS_STATE***'] = 'NULL';
    $substitutions['***CURRENT_USERS_DISTRICT***'] = 'NULL';
    if ($user->uid) {
      $cd = cd_civicrm_user_get_cd($user->uid);
      if ($cd && $cd != 'not found') {
        $substitutions['***CURRENT_USERS_STATE***'] = cd_sunlight_cd_parse_state($cd);
        $substitutions['***CURRENT_USERS_DISTRICT***'] = intval(cd_sunlight_cd_parse_district($cd));
      }
    }
  }
  return $substitutions;
}
