<?php
/**
 * @file
 *
 * Views cache plugin file. Provides prefetch caching.
 *
 */

/**
 * Caching plugin that provides static caching for sake of conistency.
 */
class cache_graceful_views_plugin_cache_graceful extends views_plugin_cache {
  function option_defaults(&$options) {
    $options['results_expire'] = '';
    $options['results_prefetch'] = '';
  }

  function cache_start() {
    /* do nothing */
  }

  function options_form(&$form, &$form_state) {
    $form['results_expire'] = array(
      '#type' => 'textfield',
      '#title' => t('Query results expiration'),
      '#description' => t(
        'The length of time in seconds raw query results should be cached. (blank = ' .
        variable_get('cache_graceful_expire', CACHE_GRACEFUL_EXPIRE) .
        ', 0 = disabled)'
      ),
      '#default_value' => $this->options['results_expire'],
    );
    $form['results_prefetch'] = array(
      '#type' => 'textfield',
      '#title' => t('Query results prefetch'),
      '#description' => t('The length of time in seconds before prefetch should be performed. (blank = ' .
        variable_get('cache_graceful_prefetch', CACHE_GRACEFUL_PREFETCH) .
        ', 0 = disabled)'
      ),
      '#default_value' => $this->options['results_prefetch'],
    );
  }

  function summary_title() {
    return format_interval($this->options['results_expire'], 1) . '/' . format_interval($this->options['results_prefetch'], 1);
  }

  function cache_get($type) {
    if ($type != 'results') return FALSE;

    $key = $this->get_results_key();

    $args = array('_cache_graceful_views_execute', serialize($this->view));

    // Load graceful cache
    $data = cache_graceful($key, $args, 'cache', $this->options['results_expire'], $this->options['results_prefetch'], array('ensure_result' => TRUE));

    // Put results back in local scope
    $this->view->result = $data['result'];
    $this->view->total_rows = $data['total_rows'];
    $this->view->set_current_page($data['current_page']);
    $this->view->execute_time = 0;

    return TRUE;
  }

  function cache_set($type) {
    if ($type != 'results') return FALSE;
    return TRUE;
  }
}
