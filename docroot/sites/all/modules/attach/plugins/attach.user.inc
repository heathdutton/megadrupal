<?php
/**
 * Renders attachd user account.
 *
 * It reuses author_pane block if this module is enable. If not, a minimal info
 * is displayed.
 */
function attach_user_process($params) {
  if ($account = user_load((int) $params['uid'])) {
    if ($params['simple']) {
      $output = theme('username', array('account' => $account));
    }
    else {
      if (module_exists('author_pane')) {
        $output = theme('author_pane', array('account' => $account, 'caller' => 'attach'));
      }
      else {
        $output = theme('username', array('account' => $account));
      }
    }
    return $output;
  }
}
