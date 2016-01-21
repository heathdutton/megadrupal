<?php

/**
 * @file
 * Definition of asset_views_plugin_access_asset_insert_in_wysiwyg.
 */

/**
 * Access plugin that provides access control by custom access callback.
 */
class asset_views_plugin_access_asset_insert_in_wysiwyg extends views_plugin_access {
  function access($account) {
    return asset_insert_asset_views_access($account);
  }

  function get_access_callback() {
    return array('asset_insert_asset_views_access', array());
  }

  function summary_title() {
    return t('Access to search & insert assets');
  }
}
