<?php

/**
 * @file
 * Contains a Views field handler to take care of displaying links to entities
 * as fields.
 */

class avatar_handler_name_field extends views_handler_field {
  function construct() {
    parent::construct();

    $this->additional_fields['aid'] = 'aid';
    $this->additional_fields['type'] = 'type';
  }

  function option_definition() {
    $options = parent::option_definition();

    $options['link'] = array('default' => 1);
    $options['popup'] = array('default' => 1);

    return $options;
  }

  function options_form(&$form, &$form_state) {
    parent::options_form($form, $form_state);

    $form['link'] = array(
          '#type' => 'checkbox',
          '#title' => t('Add link to avatar?'),
          '#default_value' => $this->options['link'],
    );
    $form['popup'] = array(
          '#type' => 'checkbox',
          '#title' => t('Popup details when user hovers over link?'),
          '#default_value' => $this->options['popup'],
          '#description' => t("This does nothing if link is not checked.")
    );
  }

  function render($values) {
    $aid = $values->{$this->aliases['aid']};
    $avatar = avatar_load($aid);
    
    $text = $avatar->name;
    
    if ($this->options['link']) {
      $avatar_type = avatar_type_load($avatar->type);
      
      $attributes = array();
      $usepopup = (is_array($avatar_type->data) && array_key_exists('use_popup', $avatar_type->data))?$avatar_type->data['use_popup']:1;
      if ($this->options['popup'] && $usepopup) {
        $attributes['rel'] = $aid;
        $attributes['class'] = array('avatar-popup');
      }
      return l($text, 'avatar/' . $aid, array('attributes' => $attributes));
    }
    else {
      return $this->sanitize_value($text);
    }
  }
}
