<?php

/**
 * @file
 * Contains a Views field handler to take care of displaying links to entities
 * as fields.
 */

class avatar_handler_link_field extends views_handler_field {
  function construct() {
    parent::construct();

    $this->additional_fields['aid'] = 'aid';
    $this->additional_fields['type'] = 'type';
  }

  function option_definition() {
    $options = parent::option_definition();

    $options['text'] = array('default' => '', 'translatable' => TRUE);
    $options['popup'] = array('default' => 1);

    return $options;
  }

  function options_form(&$form, &$form_state) {
    parent::options_form($form, $form_state);

    $form['text'] = array(
      '#type' => 'textfield',
      '#title' => t('Text to display'),
      '#default_value' => $this->options['text'],
    );
    $form['popup'] = array(
          '#type' => 'checkbox',
          '#title' => t('Popup details when user hovers over link?'),
          '#default_value' => $this->options['popup'],
    );
  }

  function query() {
    $this->ensure_my_table();
    $this->add_additional_fields();
  }
  

  function render($values) {
    $aid = $values->{$this->aliases['aid']};
    $avatar = avatar_load($aid);
    
    $text = !empty($this->options['text']) ? $this->options['text'] : t("View");
    
    $avatar_type = avatar_type_load($avatar->type);
    
    $attributes = array();
    $usepopup = (is_array($avatar_type->data) && array_key_exists('use_popup', $avatar_type->data))?$avatar_type->data['use_popup']:1;
    
    if ($this->options['popup'] && $usepopup) {
      $attributes['rel'] = $aid;
      $attributes['class'] = array('avatar-popup');
    }
    return l($text, 'avatar/' . $aid, $attributes);
  }
}
