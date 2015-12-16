<?php

/**
 * @file
 * User Stats non-themed user picture.
 */

/**
 * Bare user picture handler.
 */
class views_handler_field_user_picture_bare extends views_handler_field_user_picture {
  function render($values) {
    if (module_exists('image')) {
      $output = '';

      // When an image style is not defined, use the image style from the account settings.
      $style_name = $this->options['image_style'];
      if (empty($style_name)) {
        $style_name = variable_get('user_picture_style', '');
      }

      // Load the picture file and get the uri.
      if ($user_picture_fid = $this->get_value($values)) {
        $user_picture = file_load($user_picture_fid);
        $user_picture_filepath = $user_picture->uri;
      } else {
        $user_picture_filepath = variable_get('user_picture_default', '');
      }

      // Return empty string when either style_name or picture are unavailable.
      if (empty($style_name) || empty($user_picture_filepath)) {
        return $output;
      }

      // Use the user name for alt attribute.
      $user_name = $values->{$this->aliases['name']} ? $values->{$this->aliases['name']} : variable_get('anonymous', t('Anonymous'));
      $alt = t("@user's picture", array('@user' => $user_name));

      // Output the picture with image_style.
      if (file_valid_uri($user_picture_filepath)) {
        $output = theme('image_style', array('style_name' => $style_name,'path' => $user_picture_filepath, 'alt' => $alt));
      }

      // Wrap the picture in a link to the user picture.
      if ($this->options['link_photo_to_profile'] && user_access('access user profiles')) {
        $uid = $this->get_value($values, 'uid');
        $output = l($output, "user/$uid", array('html' => TRUE));
      }

      return $output;
    }
  }
}
