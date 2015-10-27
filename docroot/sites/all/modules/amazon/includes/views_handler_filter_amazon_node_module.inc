<?php

/**
 Filter node-linked amazom products by source module
 */
class views_handler_filter_amazon_node_module extends views_handler_filter_in_operator {
  function construct() {
    parent::construct();
    $this->value_title = t('Source module');
    $options = array();
    $result = db_query("SELECT DISTINCT ain.module FROM {amazon_item_node} ain");
    while ($module = db_fetch_array($result)) {
      $options[$module['module']] = $module['module'];
    }
    $this->value_options = $options;
  }

  function options(&$options) {
    parent::options($options);
    $result = db_query("SELECT DISTINCT ain.module FROM {amazon_item_node} ain");
    $module = db_fetch_array($result);
    $options['value'] = array($module['module']);
  }
}

