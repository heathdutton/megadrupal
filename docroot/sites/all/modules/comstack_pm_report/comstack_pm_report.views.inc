<?php

/**
 * @file
 * Provide additional Views fields for entity content.
 */

/**
 * Implements hook_views_data_alter().
 *
 * We need to use this hook instead of hook_views_data() as
 * entity API already provides data for our custom entity type.
 */
 function comstack_pm_report_views_data_alter(&$data) {
  // Quick view post content link.
  $data['comstack_pm_report']['view_conversation'] = array(
    'field' => array(
      'title' => t('Link to, or AJAX view the conversation'),
      'help' => t('Provide a link to view the conversation in a modal window if JavaScript is enabled.'),
      'handler' => 'comstack_pm_report_handler_field_view_conversation',
    ),
  );

  $data['comstack_pm_report']['dismiss_report'] = array(
    'field' => array(
      'title' => t('Link to dismiss or AJAX dismiss a report.'),
      'help' => t('Marks a report as dealt with.'),
      'handler' => 'comstack_pm_report_handler_field_dismiss_report',
    ),
  );
}
