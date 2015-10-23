<?php

/**
 * @file
 * OpenAM module page callbacks.
 */

/**
 * Page callback for the user page.
 *
 * Anonymous users get redirected to OpenAM, authenticated receive standard
 * user page.
 */
function openam_user_page() {
  global $user;
  if ($user->uid) {
    menu_set_active_item('user/' . $user->uid);
    return menu_execute_active_handler(NULL, FALSE);
  }
  else {
    openam_redirect_user();
  }
}
