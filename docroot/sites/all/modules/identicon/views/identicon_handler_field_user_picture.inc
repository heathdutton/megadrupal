<?php

/**
 * @file
 * Definition of identicon_handler_field_user_picture.
 */

/**
 * Field handler for the user picture field that supports identicons.
 */
class identicon_handler_field_user_picture extends views_handler_field_user_picture {

  /**
   * Overrides the render function in views_handler_field_user_picture.
   */
  function render($values) {

    if (variable_get('identicon_enabled', 0) && $this->options['image_style'] && module_exists('image') && !$this->get_value($values)) {
      // Picture field is empty.

      $account = new stdClass();
      $account->uid = $this->get_value($values, 'uid');
      $account->mail = $this->get_value($values, 'mail');
      $image_path = identicon_identicon_image_path($account);

      if (!$image_path) {
        // Fall back to the default image.
        $output = parent::render($values);
      }
      else {
        $output = theme('image_style', array('style_name' => $this->options['image_style'], 'path' => $image_path));
        if ($this->options['link_photo_to_profile'] && user_access('access user profiles')) {
          $uid = $this->get_value($values, 'uid');
          $output = l($output, "user/$uid", array('html' => TRUE));
        }
      }

    }
    else {
      // The parent class uses theme_user_picture in other case,
      // which displays the identicon.
      $output = parent::render($values);
    }

    return $output;
  }

}
