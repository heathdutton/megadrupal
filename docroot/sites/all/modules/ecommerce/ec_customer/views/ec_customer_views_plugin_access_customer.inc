<?php

/**
 * @file
 * Implement customer access plugins.
 */

class ec_customer_views_plugin_access_customer extends views_plugin_access {
  function access($account) {
    return ec_customer_check_access('user', $account->uid);
  }

  function get_access_callback() {
    return array('ec_customer_check_access', array('user'));
  }

  function summary_title() {
    return t('Customer');
  }
}

