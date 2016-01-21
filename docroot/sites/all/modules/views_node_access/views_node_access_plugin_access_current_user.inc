<?php

/**
 * @file
 * Views access plugin that provides node-type-based access control.
 */

class views_node_access_plugin_access_current_user extends views_plugin_access {

  function get_access_callback() {
    return array('views_node_access_current_user', array());
  }

  function summary_title() {
    return check_plain(t('Current user'));
  }

}
