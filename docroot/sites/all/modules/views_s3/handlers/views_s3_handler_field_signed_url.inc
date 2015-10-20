<?php

/**
 * @file views_s3_handler_field.inc
 * Basic S3 field handler.
 */

/**
 * Field handler to provide simple renderer.
 */
class views_s3_handler_field_signed_url extends views_s3_handler_field {

  function option_definition() {
    $options = parent::option_definition();
    $options['link_to_s3'] = array('default' => isset($this->definition['link_to_s3 default']) ? $this->definition['link_to_s3 default'] : FALSE);
    $options['link_text'] = array('default' => isset($this->definition['text default']) ? $this->definition['text default'] : '');
    return $options;
  }

  function options_form(&$form, &$form_state) {
    parent::options_form($form, $form_state);
    $form['link_to_s3'] = array(
      '#type' => 'checkbox',
      '#title' => t('Output URL as a link'),
      '#description' => t('Check this if you want the URL displayed as a link.'),
      '#default_value' => !empty($this->options['link_to_s3']),
    );
    $form['link_text'] = array(
      '#type' => 'textfield',
      '#title' => t('Text for the link'),
      '#description' => t('This text will appear linked to the signed URL. Leave it empty to use the URL as the text for the link.'),
      '#default_value' => isset($this->options['link_text']) ? $this->options['link_text'] : '',
      '#states' => array(
        'visible' => array(   // action to take.
          ':input[name="options[link_to_s3]"]' => array('checked' => TRUE),
        ),
      ),
    );
  }

  /**
   * Render whatever the data is as a link to the node.
   *
   * Data should be made XSS safe prior to calling this function.
   */
  function render_link($data, $values) {
    if (!empty($this->options['link_to_s3'])) {
      $this->options['alter']['make_link'] = TRUE;
      $this->options['alter']['path'] = $data;
      if (!empty($this->options['link_text'])) {
        $data = $this->options['link_text'];
      }
    }
    return $data;
  }
  
  /**
   * Render function that renders the link if necessary
   */
   function render($values) {
     $value = $this->get_value($values);
     return $this->render_link($this->sanitize_value($value), $values);
   }

}
