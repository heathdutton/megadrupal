<?php
/**
 * Simple caching of query results for Views displays, but per-user instead of
 * global for all users.
 */
class views_plugin_cache_time_per_user extends views_plugin_cache_time {

  function get_results_key() {
    global $user;

    if (!isset($this->_results_key)) {
      // Add a 'user' key so this result is unique per user.
      $key_data = array(
        'build_info' => $this->view->build_info,
        'roles' => array_keys($user->roles),
        'user' => $user->uid,
        'language' => $GLOBALS['language'],
      );
      foreach (array('exposed_info', 'page', 'sort', 'order') as $key) {
        if (isset($_GET[$key])) {
          $key_data[$key] = $_GET[$key];
        }
      }

      $this->_results_key = $this->view->name . ':' . $this->display->id . ':results:' . md5(serialize($key_data));
    }

    return $this->_results_key;
  }

  function get_output_key() {
    global $user;
    if (!isset($this->_output_key)) {
      // Add a 'user' key so this result is unique per user.
      $key_data = array(
        'result' => $this->view->result,
        'roles' => array_keys($user->roles),
        'user' => $user->uid,
        'theme' => $GLOBALS['theme'],
        'language' => $GLOBALS['language'],
      );

      $this->_output_key = $this->view->name . ':' . $this->display->id . ':output:' . md5(serialize($key_data));
    }

    return $this->_output_key;
  }
}

