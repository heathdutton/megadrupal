<?php

/**
 * Simple caching based on arguments and display_id
 * 
 * Makes it easier to only purge Views caches for specific argument(s)
 * 
 * This cache can only be purged from the code.
 * 
 * @see
 *   views_arg_cache_flush()
 */
class views_arg_cache_plugin_cache extends views_plugin_cache {
  /**
   * What table to store data in.
   */
  var $table = VIEWS_ARG_CACHE_TABLE;
  
  /**
   * Override of parent::get_results_key()
   */
  function get_results_key() {
    global $user;

    // Taken from parent::get_results_key()
    if (!isset($this->_results_key)) {
      $build_info = $this->view->build_info;

      $query_plugin = $this->view->display_handler->get_plugin('query');

      foreach (array('query','count_query') as $index) {
        // If the default query back-end is used generate SQL query strings from
        // the query objects.
        if ($build_info[$index] instanceof SelectQueryInterface) {
          $query = clone $build_info[$index];
          $query->preExecute();
          $build_info[$index] = (string)$query;
        }
      }
      $key_data = array(
        'build_info' => $build_info,
        'roles' => array_keys($user->roles),
        'super-user' => $user->uid == 1, // special caching for super user.
        'language' => $GLOBALS['language']->language,
      );
      foreach (array('exposed_info', 'page', 'sort', 'order') as $key) {
        if (isset($_GET[$key])) {
          $key_data[$key] = $_GET[$key];
        }
      }
      
      $args = $this->truncate_args($this->view->args);
      $this->_results_key = _views_arg_cache_get_key($this->view->name, $this->display->id, $args) . ':results:' . md5(serialize($key_data));
    }
    
    return $this->_results_key;
  }

  /**
   * Override of parent::get_output_key()
   */
  function get_output_key() {
    global $user;
    // Taken from parent::get_output_key()
    if (!isset($this->_output_key)) {
      $key_data = array(
        'result' => $this->view->result,
        'roles' => array_keys($user->roles),
        'super-user' => $user->uid == 1, // special caching for super user.
        'theme' => $GLOBALS['theme'],
        'language' => $GLOBALS['language']->language,
      );

      $args = $this->truncate_args($this->view->args);
      $this->_output_key = _views_arg_cache_get_key($this->view->name, $this->display->id, $args) . ':output:' . md5(serialize($key_data));
    }
    
    return $this->_output_key;
  }
  
  /**
   * Clear out cached data for a view.
   */
  function cache_flush() {
    views_arg_cache_flush($this->view->name, $this->display->id, $this->view->args);
  }
  
  function truncate_args($args = array()) {
    // As per the plugin options, we may want to only use some
    // of the arguments when making the key
    if ($this->options['arg_count']) {
      return array_slice($args, 0, $this->options['arg_count']);
    }
    return $args;
  }
  
  /**
   * Return a short title for this cache plugin
   */
  function summary_title() {
    return t('Argument cache');
  }
  
  function option_definition() {
    $options = parent::option_definition();
    $options['arg_count'] = array('default' => 0);
    return $options;
  }

  function option_defaults(&$options) {
    $options['arg_count'] = 0;
  }

  function options_form(&$form, &$form_state) {
    $options = array(0 => t('All'));
    for ($i = 1; $i <= 10; $i++) {
      $options[$i] = $i;
    }

    $form['arg_count'] = array(
      '#type' => 'select',
      '#title' => t('Amount of arguments to use'),
      '#description' => t('Select the amount of arguments to use to generate the key. The arguments will be used in order of which they appear. This is useful if not all arguments are significant in caching.'),
      '#options' => $options,
      '#default_value' => $this->options['arg_count'],
    );
  }
}
