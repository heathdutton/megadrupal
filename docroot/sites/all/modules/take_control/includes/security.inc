<?php

function take_control_security_error($msg = 'You are not authorized to perform this action.') {
  die($msg);
}

function take_control_user_has_role($role, $user = NULL) {
  if ($user == NULL) {
    global $user;
  }

  if (is_array($user->roles) && in_array($role, array_values($user->roles))) {
    return TRUE;
  }

  return FALSE;
}