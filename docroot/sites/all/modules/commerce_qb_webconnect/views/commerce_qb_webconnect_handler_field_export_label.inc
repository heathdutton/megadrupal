<?php

class commerce_qb_webconnect_handler_field_export_label extends views_handler_field {
  function query(){
    $this->add_additional_fields(array('data', 'item_type'));
  }

  function render($values) {
    $info = commerce_qb_webconnect_export_info();
    $type = $values->item_type;
    $data = drupal_json_decode($values->data);

    $label_callback = isset($info[$type]['label_callback']) ? $info[$type]['label_callback'] : NULL;

    if (function_exists($label_callback)) {
      $uri_callback = isset($info[$type]['uri_callback']) ? $info[$type]['uri_callback'] : NULL;
    
      if (isset($this->options['link_to_reference']) && $this->options['link_to_reference'] && function_exists($uri_callback)) {
        $uri = $uri_callback($data);
        if ($uri) {
          return l($label_callback($data), $uri);
        } else {
          return $label_callback($data) . ' (Invalid)';
        }
      }
    
      return $label_callback($data);
    }
  }

  function options_form(&$form, &$form_state) {
    parent::options_form($form, $form_state);
  
    $form['link_to_reference'] = array(
      '#type' => 'checkbox',
      '#title' => t('Link to reference'),
      '#description' => t('Link label to reference URI if possible'),
      '#default_value' => $this->options['link_to_reference'],
    );
  }

  function option_definition() {
    $options = parent::option_definition();  
    $options['link_to_reference'] = array('default' => FALSE);
  
    return $options;
  }
}
