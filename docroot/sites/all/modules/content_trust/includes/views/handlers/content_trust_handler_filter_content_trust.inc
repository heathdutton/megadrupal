<?php

/**
 * Filter the view by limiting the content types to those selected in the Content Trust configuration.
 *
 * @ingroup views_filter_handlers
 */
class content_trust_handler_filter_content_trust extends views_handler_filter {
  function query() {
    $setting = ($this->view->base_table === 'comments')
      ? 'content_trust_moderate_comment_types'
      : 'content_trust_moderate_content_types';

  	$types = variable_get($setting, array());
  	$types = array_filter($types);

    // Show all content if we're not configured to limit to specific types
    if (empty($types)) {
      $types = array_keys(node_type_get_names());
    }

    $this->ensure_my_table();
    $field = "{$this->table_alias}.{$this->real_field}";
    $this->query->add_where($this->options['group'], $field, $types, 'IN');
  }

  function admin_summary() {}
}
