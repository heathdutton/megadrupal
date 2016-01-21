<?php

/**
 * @file
 * Definition of socrata_views_handler_field_phone.
 * Borrows heavily from views_handler_field_url.
 */

/**
 * Field handler to provide simple renderer that turns a phone number
 * into a clickable link.
 *
 * @ingroup views_field_handlers
 */
class socrata_views_handler_field_phone extends views_handler_field {
  /**
   * Collect options for field display.
   */
  function option_definition() {
    $options = parent::option_definition();

    $options['display_as_link'] = array('default' => TRUE, 'bool' => TRUE);
    $options['link_text_option'] = array('default' => 'phone_number');
    $options['link_text'] = array('default' => '', 'translatable' => TRUE);

    return $options;
  }

  /**
   * Option form.
   */
  function options_form(&$form, &$form_state) {
    $form['display_as_link'] = array(
      '#type' => 'checkbox',
      '#title' => t('Display as link'),
      '#default_value' => !empty($this->options['display_as_link']),
    );
    $form['link_text_option'] = array(
      '#type' => 'radios',
      '#title' => t('Select what to use as the link text'),
      '#options' => array(
        'phone_number' => t('Phone number'),
        'phone_type' => t('Phone number type from Socrata, otherwise provided text'),
        'link_text' => t('Provided link text'),
      ),
      '#default_value' => $this->options['link_text_option'],
    );
    $form['link_text'] = array(
      '#type' => 'textfield',
      '#title' => t('Link text'),
      '#description' => t('Text to use for link based on selection above'),
      '#default_value' => $this->options['link_text'],
    );

    parent::options_form($form, $form_state);
  }

  /**
   * Generate output of phone.
   */
  function render($values) {
    $text = '';
    $value = $this->get_value($values);
    if (!empty($value)) {
      if ($this->options['display_as_link'] && !empty($value['phone_number'])) {
        $text = $this->sanitize_value($value['phone_number'], 'xss');
        switch ($this->options['link_text_option']) {
          case 'phone_type':
            if (!empty($value['phone_type'])) {
              $text = $this->sanitize_value($value['phone_type'], 'xss');
            }
            elseif (!empty($value['link_text'])) {
              $text = $this->sanitize_value($this->options['link_text'], 'xss');
            }
            break;

          case 'link_text':
            if (!empty($this->options['link_text'])) {
              $text = $this->sanitize_value($this->options['link_text'], 'xss');
            }
            break;
        }

        $this->options['alter']['make_link'] = TRUE;
        $phone_number = $this->sanitize_value($value['phone_number'], 'xss');
        $this->options['alter']['path'] = "tel://{$phone_number}";
      }
    }

    return $text;
  }
}
