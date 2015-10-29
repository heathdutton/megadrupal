<?php

/**
 * @file
 * @author Bob Hutchinson http://drupal.org/user/52366
 * @copyright GNU GPL
 *
 * avail_roles field handler.
 */

class imagepicker_views_handler_field_avail_roles extends views_handler_field {

  function render($values) {
    $ret = t('All roles may view');
    if (imagepicker_variable_get('imagepicker_publicroles_enabled', 0)) {
      $avail_roles = $values->{$this->field_alias};
      $avail_roles_arr = explode(":", $avail_roles);
      if (count($avail_roles_arr) && $avail_roles_arr[0] != 'all') {
        $avail_roles_str = implode(", ", $avail_roles_arr);
        $ret = t('!r roles may view', array('!r' => $avail_roles_str));
      }
    }
    return $ret;
  }

}
