<?php

/**
 * @file Provides the degenerative cases for access plugins.
 * i.e. Always true and Always false
 *
 * @ingroup views_access_plugins
 */
class views_plugin_access_true extends views_plugin_access {
  function access($account) {
    return TRUE;
  }

  function get_access_callback() {
    return array('views_plugin_access_return_true', array());
  }

  function summary_title() {
    return t('TRUE');
  }
}

class views_plugin_access_false extends views_plugin_access {
  function access($account) {
    return FALSE;
  }

  function get_access_callback() {
    return array('views_plugin_access_return_false', array());
  }

  function summary_title() {
    return t('FALSE');
  }
}