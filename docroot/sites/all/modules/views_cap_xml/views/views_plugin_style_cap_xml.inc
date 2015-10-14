<?php
class views_plugin_style_cap_xml extends views_plugin_style {

  function option_definition() {
    return array(
      'edxlde:distributionID_prefix' => array('default' => 'UniqueID'),
      'edxlde:senderID' => array('default' => 'sender@example.com'),
      'edxlde:distributionStatus' => array('default' => 'Actual'),
      'edxlde:distributionType' => array('default' => 'Report'),
      'edxlde:combinedConfidentiality' => array('default' => 'UNCLASSIFIED AND NOT SENSITIVE'),
      'edxlde:language' => array('default' => 'en-AU'),
    );
  }
  
  function options_form(&$form, &$form_state) {
    foreach($this->option_definition() as $key => $detail) {
      $form[$key] = array(
        '#type' => 'textfield',
        '#title' => $key,
        '#default_value' => $this->options[$key],
      );      
    }    
  }
  
  function render() {
    if (empty($this->row_plugin)) {
      vpr('views_plugin_style_default: Missing row plugin');
      return;
    }
    $rows = '';

    // Fetch any additional elements for the channel and merge in their
    // namespaces.
    foreach ($this->view->result as $row_index => $row) {
      $this->view->row_index = $row_index;
      $rows .= $this->row_plugin->render($row);
    }
    
    $theme_vars = array(
      'view' => $this->view,
      'options' => $this->options,
      'rows' => $rows,
    );
        
    $output = theme($this->theme_functions(),$theme_vars);
    
    //This just pretty-prints things if it can
    if(class_exists('DOMDocument')) {
      try {
        $dom = new DOMDocument("1.0");
        $dom->preserveWhiteSpace = false;
        $dom->formatOutput = true;
        @$dom->loadXML($output);
        $output = $dom->saveXML($dom->documentElement);
      }
      catch(Exception $ex) {
        
      }
    }
    
    $output = '<?xml version="1.0" encoding="utf-8" ?>'."\n".$output;

    
    unset($this->view->row_index);
    return $output;
  }

  /**
   * Get a rendered field.
   *
   * @param $index
   *   The index count of the row.
   * @param $field
   *    The id of the field.
   */
  function get_field($index, $field) {
    if (!isset($this->rendered_fields)) {
      $this->render_fields($this->view->result);
    }

    if(empty($this->rendered_fields)) {
      return;
    }

    if(!is_string($field)) {
      return;
    }

    if($fields = $this->rendered_fields) {

      if( isset($fields[$index]) && isset($fields[$index][$field]) ) {
        return $fields[$index][$field];
      }
    }
  }

}