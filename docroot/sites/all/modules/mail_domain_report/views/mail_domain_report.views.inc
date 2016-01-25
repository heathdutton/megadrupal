<?php

/**
 * Implements hook_views_pre_execute().
 */
function mail_domain_report_views_pre_execute(&$view) {
  // Remove normal field from query and add it back as a expression.
  if ($view->name == 'mail_domains') {
    $query = $view->build_info['query'];
    $fields =& $query->getFields();
    $expressions =& $query->getExpressions();

    $expressions['users_mail'] = $fields['users_mail'];
    $expressions['users_mail']['expression'] = 'SUBSTR(mail, INSTR(mail, \'@\') + 1)';
    $expressions['users_mail']['arguments'] = array();
    unset($expressions['users_mail']['field']);
    unset($fields['users_mail']);
  }
}
