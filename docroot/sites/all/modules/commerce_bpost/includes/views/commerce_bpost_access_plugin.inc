<?php

/**
 * @file
 * Access plugin file.
 */


/**
 * Access plugin that provides property based access control.
 */
class commerce_bpost_access_plugin extends views_plugin_access {

  function summary_title() {
    return t('Commerce bpost access plugin');
  }
  /**
   * Determine if we give access to bpost order administration.
   */
  function access($account) {
    return commerce_bpost_access_order();
  }

  function get_access_callback() {
    return array('commerce_bpost_access_order', array());
  }
}