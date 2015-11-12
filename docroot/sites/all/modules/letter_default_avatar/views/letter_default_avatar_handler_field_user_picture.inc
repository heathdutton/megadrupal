<?php

/**
 * @file
 * Definition of letter_default_avatar_handler_field_user_picture.
 */

/**
 * Field handler to provide simple renderer that allows using a themed user link.
 *
 * @ingroup views_field_handlers
 */
class letter_default_avatar_handler_field_user_picture extends views_handler_field_user_picture {
  function construct() {
    parent::construct();
    $this->additional_fields['default_avatar'] = 'default_avatar';
  }

  function render($values) {
    // Construct the user entity.
    $account = new stdClass();
    $account->uid = $this->get_value($values, 'uid');
    $account->name = $this->get_value($values, 'name');
    $account->mail = $this->get_value($values, 'mail');
    $account->default_avatar = $this->get_value($values, 'default_avatar');
    $account->picture = $this->get_value($values);

    // Setup the variables to pass to the theming function.
    $theme_variables = array(
      'account' => $account,
      //'wrapper_element' => $this->options['element_type'],
      'link_to_profile' => $this->options['link_photo_to_profile'],
    );

    if ($this->options['image_style'] && module_exists('image')) {
      $theme_variables['image_style'] = $this->options['image_style'];
    }

    return theme('user_picture', $theme_variables);
  }
}
