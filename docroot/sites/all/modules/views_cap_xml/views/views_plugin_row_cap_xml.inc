<?php

class views_plugin_row_cap_xml extends views_plugin_row {

  var $row_index;
  var $element_tree;

  function option_definition() {
    return array(
      'edxlde:contentDescription' => array('default' => ''),
      'cap:alert' => array(

        'cap:identifier' => array('default' => ''),
        'cap:sender' => array('default' => ''),
        'cap:sent' => array('default' => ''),
        'cap:status' => array('default' => ''),
        'cap:msgType' => array('default' => ''),
        'cap:scope' => array('default' => ''),
        'cap:code' => array('default' => ''),
        'cap:incidents' => array('default' => ''),
        'cap:info' => array(
          'cap:language' =>  array('default' => ''),
          'cap:category' =>  array('default' => ''),
          'cap:event' => array('default' => ''),
          'cap:responseType' => array('default' => ''),
          'cap:urgency' => array('default' => ''),
          'cap:severity' => array('default' => ''),
          'cap:certainty' => array('default' => ''),
          'cap:eventCode' => array(
            'cap:valueName' => array('default' => ''),
            'cap:value' => array('default' => ''),
          ),
          'cap:effective' => array('default' => ''),
          'cap:expires' => array('default' => ''),
          'cap:senderName' => array('default' => ''),
          'cap:headline' => array('default' => ''),
          'cap:description' => array('default' => ''),
          'cap:instruction' => array('default' => ''),
          'cap:web' => array('default' => ''),
          'cap:contact' => array('default' => ''),
          'cap:resource' => array(
            'cap:resourceDesc' => array('default' => 'map'),
            'cap:mimeType' => array('default' => 'text/html'),
            'cap:uri' => array('default' => ''),
          ),
          'cap:area' => array(
            'cap:areaDesc' => array('default' => ''),
            'cap:polygon' => array('default' => ''),
            'cap:circle' => array('default' => ''),
            'cap:geocode' => array(
              'cap:valueName' => array('default' => ''),
              'cap:value' => array('default' => ''),
            ),
          ),
        ),
      ),
    );
  }

  function build_option_definition_tree(&$options, $xml_el_name, $detail, $option_key = '') {

    if(isset($detail['sub'])) {
      foreach($detail['sub'] as $sub_xml_el_name => $sub_detail) {
        $this->build_option_definition_tree($options, $sub_xml_el_name,$sub_detail,$option_key.'_'.$sub_xml_el_name);
      }
      unset($options[$xml_el_name]);
    }
    else {
      $options[$option_key] = array('default' => '');
    }
  }

  function build_options_form_tree($xml_el_name, $detail, $options) {

    $initial_labels = array('' => t('- None -'));
    if (!empty($detail['default'])) {
      $initial_labels['_default'] = '"'.$detail['default'].'"';
    }

    $view_fields_labels = $this->display->handler->get_field_labels();
    $view_fields_labels = array_merge($initial_labels, $view_fields_labels);

    $el = array();

    $el[$xml_el_name] = array(
      '#type' => 'select',
      '#title' => $xml_el_name,
      '#options' => $view_fields_labels,
      '#default_value' => isset($options[$xml_el_name])?$options[$xml_el_name]:'',
      '#required' => FALSE,
    );

    if(is_array($detail) && !isset($detail['default'])) {

      $el[$xml_el_name] = array(
        '#type' => 'fieldset',
        '#title' => $xml_el_name,
        '#collapsible' => FALSE,
        '#collapsed' => FALSE,
      );

      foreach($detail as $sub_xml_el_name => $sub_detail) {
        $el[$xml_el_name] += $this->build_options_form_tree($sub_xml_el_name, $sub_detail, $options[$xml_el_name]);
      }

    }

    return $el;
  }

  function options_form(&$form, &$form_state) {
    parent::options_form($form, $form_state);
    foreach($this->option_definition() as $xml_el_name => $detail) {
      $form += $this->build_options_form_tree($xml_el_name, $detail, $this->options);
    }
  }

  function validate() {
    $errors = parent::validate();
    return $errors;
  }



  function build_element_tree($xml_el_name, $detail, $options) {

    if(is_string($options[$xml_el_name]) && !empty($options[$xml_el_name])) {

      if($options[$xml_el_name] == '_default') {
        $value = $detail['default'];
      } else {
        $value = $this->get_field($this->row_index, $options[$xml_el_name]);
      }
    }

    $el = array();
    if(!empty($value)) {
      $el = array(
        'key' => $xml_el_name,
        'value' => $value,
      );
    }

    if(is_array($detail) && !isset($detail['default'])) {

      $values = array();
      foreach($detail as $sub_xml_el_name => $sub_detail) {
        $sub_value = $this->build_element_tree($sub_xml_el_name, $sub_detail, $options[$xml_el_name]);
        if(!empty($sub_value)) {
          $values[] = $sub_value;
        }
      }

      if(!empty($values)) {
        $el = array('key'=> $xml_el_name, 'value' => $values);
      }
    }

    return $el;

  }


  function render($row) {
    static $row_index;
    if (!isset($row_index)) {
      $row_index = 0;
    }
    $this->row_index = $row_index;

    // Create the RSS item object.
    $contentObject = new stdClass();

    $option_defn = $this->option_definition();
    $cap_elements = array();
    foreach($option_defn['cap:alert'] as $xml_el_name => $detail) {
      $el = $this->build_element_tree($xml_el_name, $detail, $this->options['cap:alert']);
      if(!empty($el)) {
        $cap_elements[] = $el;
      }
    }

    $contentObject->elements[] = array(
      'key' => 'edxlde:contentObject',
      'value' => array(

        array('key'=>'edxlde:contentDescription', 'value' => $this->get_field($this->row_index, $this->options['edxlde:contentDescription'])),

        array(
          'key'=> 'edxlde:xmlContent',
          'value' => array(
            array(
              'key' => 'cap:alert',
              'value' => $cap_elements,
              'attributes' => array('xmlns:cap' => 'urn:oasis:names:tc:emergency:cap:1.2')),
          ),
        ),
      )
    );

    $row_index++;

    return theme($this->theme_functions(),
      array(
        'view' => $this->view,
        'options' => $this->options,
        'contentObject' => $contentObject,
        'field_alias' => isset($this->field_alias) ? $this->field_alias : '',
      )
    );
  }

  /**
   * Retrieves a views field value from the style plugin.
   *
   * @param $index
   *   The index count of the row as expected by views_plugin_style::get_field().
   * @param $field_id
   *   The ID assigned to the required field in the display.
   */
  function get_field($index, $field_id) {

    if (empty($this->view->style_plugin) || !is_object($this->view->style_plugin) || empty($field_id)) {
      return '';
    }
    return $this->view->style_plugin->get_field($index, $field_id);
  }
}