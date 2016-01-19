<?php

/**
 * @file
 * Rijksoverheid Cookie Opt-in variable functions.
 */

/**
 * Implements hook_variable_info().
 */
function ro_cookie_opt_in_variable_info($options) {
  $variables['ro_cookie_opt_in_enabled'] = array(
    'title' => t('Enable Rijksoverheid Cookie Opt-in'),
    'group' => 'ro_cookie_opt_in',
  );
  $variables['ro_cookie_opt_in_log_enabled'] = array(
    'title' => t('Enable Rijksoverheid Cookie Opt-in log'),
    'group' => 'ro_cookie_opt_in',
  );
  $variables['ro_cookie_opt_in_log_cleanup'] = array(
    'title' => t('Periodically cleanup the Rijksoverheid Cookie Opt-in log'),
    'group' => 'ro_cookie_opt_in',
  );
  $variables['ro_cookie_opt_in_log_last_cleanup'] = array(
    'title' => t('Timestamp of last log cleanup'),
    'group' => 'ro_cookie_opt_in',
  );
  $variables['ro_cookie_opt_in_log_days_to_keep'] = array(
    'title' => t('Number of days to keep cookielog entries'),
    'group' => 'ro_cookie_opt_in',
  );
  $variables['ro_cookie_opt_in_add_css'] = array(
    'title' => t('Add Rijkshuisstijl CSS'),
    'group' => 'ro_cookie_opt_in',
  );
  $variables['ro_cookie_opt_in_add_resp_fix'] = array(
    'title' => t('Add responsive fix CSS'),
    'group' => 'ro_cookie_opt_in',
  );
  $variables['ro_cookie_opt_in_cookie_name'] = array(
    'title' => t('Cookie name'),
    'group' => 'ro_cookie_opt_in',
  );
  $variables['ro_cookie_opt_in_cookie_accept'] = array(
    'title' => t('Cookie accept value'),
    'group' => 'ro_cookie_opt_in',
  );
  $variables['ro_cookie_opt_in_cookie_deny'] = array(
    'title' => t('Cookie deny value'),
    'group' => 'ro_cookie_opt_in',
  );
  $variables['ro_cookie_opt_in_close'] = array(
    'title' => t('Close link text'),
    'group' => 'ro_cookie_opt_in',
    'localize' => TRUE,
  );
  $variables['ro_cookie_opt_in_question_title'] = array(
    'title' => t('Title (question)'),
    'group' => 'ro_cookie_opt_in',
    'localize' => TRUE,
  );
  $variables['ro_cookie_opt_in_question_intro'] = array(
    'title' => t('Intro (question)'),
    'group' => 'ro_cookie_opt_in',
    'localize' => TRUE,
  );
  $variables['ro_cookie_opt_in_question_learn_more'] = array(
    'title' => t('Learn more links (question)'),
    'group' => 'ro_cookie_opt_in',
    'localize' => TRUE,
  );
  $variables['ro_cookie_opt_in_change_title'] = array(
    'title' => t('Title (change)'),
    'group' => 'ro_cookie_opt_in',
    'localize' => TRUE,
  );
  $variables['ro_cookie_opt_in_change_intro'] = array(
    'title' => t('Intro (change)'),
    'group' => 'ro_cookie_opt_in',
    'localize' => TRUE,
  );
  $variables['ro_cookie_opt_in_change_learn_more'] = array(
    'title' => t('Learn more links (change)'),
    'group' => 'ro_cookie_opt_in',
    'localize' => TRUE,
  );
  $variables['ro_cookie_opt_in_accept_button'] = array(
    'title' => t('Button (accept)'),
    'group' => 'ro_cookie_opt_in',
    'localize' => TRUE,
  );
  $variables['ro_cookie_opt_in_accept_extras'] = array(
    'title' => t('Extras (accept)'),
    'group' => 'ro_cookie_opt_in',
    'localize' => TRUE,
  );
  $variables['ro_cookie_opt_in_accept_title'] = array(
    'title' => t('Title (accept)'),
    'group' => 'ro_cookie_opt_in',
    'localize' => TRUE,
  );
  $variables['ro_cookie_opt_in_accept_intro'] = array(
    'title' => t('Intro (accept)'),
    'group' => 'ro_cookie_opt_in',
    'localize' => TRUE,
  );
  $variables['ro_cookie_opt_in_accept_current'] = array(
    'title' => t('Current (accept)'),
    'group' => 'ro_cookie_opt_in',
    'localize' => TRUE,
  );
  $variables['ro_cookie_opt_in_accept_learn_more'] = array(
    'title' => t('Learn more links (accept)'),
    'group' => 'ro_cookie_opt_in',
    'localize' => TRUE,
  );
  $variables['ro_cookie_opt_in_deny_button'] = array(
    'title' => t('Button (deny)'),
    'group' => 'ro_cookie_opt_in',
    'localize' => TRUE,
  );
  $variables['ro_cookie_opt_in_deny_extras'] = array(
    'title' => t('Extras (deny)'),
    'group' => 'ro_cookie_opt_in',
    'localize' => TRUE,
  );
  $variables['ro_cookie_opt_in_deny_title'] = array(
    'title' => t('Title (deny)'),
    'group' => 'ro_cookie_opt_in',
    'localize' => TRUE,
  );
  $variables['ro_cookie_opt_in_deny_intro'] = array(
    'title' => t('Intro (deny)'),
    'group' => 'ro_cookie_opt_in',
    'localize' => TRUE,
  );
  $variables['ro_cookie_opt_in_deny_current'] = array(
    'title' => t('Current (deny)'),
    'group' => 'ro_cookie_opt_in',
    'localize' => TRUE,
  );
  $variables['ro_cookie_opt_in_deny_learn_more'] = array(
    'title' => t('Learn more links (deny)'),
    'group' => 'ro_cookie_opt_in',
    'localize' => TRUE,
  );
  $variables['ro_cookie_opt_in_pre_block_link_text'] = array(
    'title' => t('Cookie preferences link text'),
    'group' => 'ro_cookie_opt_in',
    'localize' => TRUE,
  );

  return $variables;
}

/**
 * Implements hook_variable_group_info().
 */
function ro_cookie_opt_in_variable_group_info() {
  $groups['ro_cookie_opt_in'] = array(
    'title' => t('Rijksoverheid Cookie Opt-in'),
    'description' => t('Rijksoverheid Cookie Opt-in settings'),
    'access' => 'administer ro cookie opt-in settings',
    'path' => array(
      'admin/config/system/ro-cookie-opt-in',
    ),
  );

  return $groups;
}
