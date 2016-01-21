<?php

/**
 * Cache a view based on uc_multiprice data
 */
class uc_multiprice_plugin_cache extends views_plugin_cache_time {
  /**
   * Generate uc_multiprice-specific output cache key
   *
   * {@inheritdoc}
   */
  function get_output_key() {
    return $this->get_custom_cache_key('get_output_key');
  }

  /**
   * Generate uc_multiprice-specific results cache key
   *
   * {@inheritdoc}
   */
  function get_results_key() {
    return $this->get_custom_cache_key('get_results_key');
  }

  /**
   * Generate uc_multiprice customized cache key
   *
   * @param $method string Base method name to retrieve a key
   * @return string
   */
  private function get_custom_cache_key($method) {
    global $user;

    $base_key = parent::$method();

    $uc_multiprice_data = uc_multiprice_country_id();
    $cache_key_suffix = ':' . $uc_multiprice_data['country_code'] . ':' . md5(serialize($user->roles));

    return $base_key . $cache_key_suffix;
  }
}
