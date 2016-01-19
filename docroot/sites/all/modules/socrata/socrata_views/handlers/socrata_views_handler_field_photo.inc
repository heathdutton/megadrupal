<?php

/**
 * @file
 * Definition of socrata_views_handler_field_photo.
 */

/**
 * Field handler to provide simple renderer that turns a photo field
 * into a clickable link.
 *
 * @ingroup views_field_handlers
 */
class socrata_views_handler_field_photo extends views_handler_field {
  /**
   * Collect options for field display.
   */
  function option_definition() {
    $options = parent::option_definition();

    $options['display_as_link'] = array('default' => TRUE, 'bool' => TRUE);
    $options['link_text'] = array('default' => 'Photo', 'translatable' => TRUE);

    return $options;
  }

  /**
   * Option form.
   */
  function options_form(&$form, &$form_state) {
    $form['display_as_link'] = array(
      '#type' => 'checkbox',
      '#title' => t('Display as link'),
      '#default_value' => $this->options['display_as_link'],
    );
    $form['link_text'] = array(
      '#type' => 'textfield',
      '#title' => t('Link text'),
      '#description' => t('Text to use for link'),
      '#default_value' => $this->options['link_text'],
    );

    parent::options_form($form, $form_state);
  }

  /**
   * Generate output of image.
   */
  function render($values) {
    $text = '';
    $value = $this->get_value($values);
    if (!empty($value)) {
      $text = !empty($this->options['link_text']) ? $this->sanitize_value($this->options['link_text'], 'xss') : $this->sanitize_value($value, 'xss');
      $img_url = "https://opendata.socrata.com/views/{$this->definition['dataset']}/files/{$value}";
      if ($this->options['display_as_link']) {
        $this->options['alter']['make_link'] = TRUE;
        $this->options['alter']['path'] = $img_url;
      }
      else {
        $text = "<img src=\"{$img_url}\" />";
      }
    }

    return $text;
  }
}
