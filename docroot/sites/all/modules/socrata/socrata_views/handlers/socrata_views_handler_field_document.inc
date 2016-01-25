<?php

/**
 * @file
 * Definition of socrata_views_handler_field_document.
 */

/**
 * Field handler to provide simple renderer that turns a document field into a clickable link.
 *
 * @ingroup views_field_handlers
 */
class socrata_views_handler_field_document extends views_handler_field {
  /**
   * Collect options for field display.
   */
  function option_definition() {
    $options = parent::option_definition();

    $options['display_as_link'] = array('default' => TRUE, 'bool' => TRUE);
    $options['link_text_option'] = array('default' => 'filename');
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
      '#default_value' => $this->options['display_as_link'],
    );
    $form['link_text_option'] = array(
      '#type' => 'radios',
      '#title' => t('Select what to use as the link text'),
      '#options' => array(
        'filename' => t('Filename from Socrata, otherwise provided text'),
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
   * Generate output of document.
   */
  function render($values) {
    $text = '';
    $value = $this->get_value($values);
    if (!empty($value)) {
      if (!empty($value['filename'])) {
        $text = $this->sanitize_value($value['filename'], 'xss');
        if ($this->options['display_as_link'] && !empty($value['file_id'])) {
          if ('link_text' == $this->options['link_text_option'] && !empty($this->options['link_text'])) {
            $text = $this->sanitize_value($this->options['link_text'], 'xss');
          }

          $this->options['alter']['make_link'] = TRUE;
          $this->options['alter']['path'] = "https://opendata.socrata.com/views/{$this->definition['dataset']}/files/{$value['file_id']}?filename={$value['filename']}";
        }
      }
    }

    return $text;
  }
}
