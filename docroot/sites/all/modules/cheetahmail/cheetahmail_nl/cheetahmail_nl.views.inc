<?php
/**
 * @file
 * Provide views data and handlers for cheetahmail_nl.module
 */

/**
 * Implements hook_views_data().
 */
function cheetahmail_nl_views_data() {
  // Filter out content that has already beed sent in Newsletter.
  $data['node']['cheetahmail_nl_sent'] = array(
    'title' => t('Not sent in Newsleter'),
    'help' => t('Filters out content that has already beed sent in Newsletter.'),
    'filter' => array(
      'field' => 'status',
      'handler' => 'views_handler_filter_cheetahmail_nl_sent',
      'label' => t('Changed since last NL'),
    ),
  );

  return $data;
}

/**
 * Filter by published status
 *
 * @ingroup views_filter_handlers
 */
class views_handler_filter_cheetahmail_nl_sent extends views_handler_filter {
  function admin_summary() { }
  function operator_form(&$form, &$form_state) { }
  function can_expose() { return FALSE; }

  function query() {
    $table = $this->ensure_my_table();
    $this->query->add_where_expression($this->options['group'], "$table.changed > " . intval(variable_get('cheetahmail_nl_sent_lasttime', 0)));
  }
}
