<?php

class views_table_highlighter_plugin_style extends views_plugin_style_table {
  function options_form(&$form, &$form_values) {
    parent::options_form($form, $form_values);
    $vth_options = $this->options['views_table_highlighter'];

    // prepare some sample values for our form, by executing the current view.
    $example_view = $this->view->copy();
    if ($this->view->current_display)
      $example_view->set_display($this->view->current_display);
    else
      $example_view->init_display();
    $example_view->pre_execute();
    $example_view->execute();
    $example_view_results = array();
    foreach ($example_view->result as $k=>$result) {
      $example_view_results[$k] = _views_table_highlighter_cook_fields((array)$result);
    }

    $fieldtext = '';
    $fields = (array)$example_view->result[0];
    unset($fields['_field_data']);
    ksort($fields);
    foreach ($fields as $field => $value) {
      // find first non-null value
      foreach ($example_view_results as $result) {
        $value = $result[$field];
        if ($value)
          break;
      }
      $fieldtext .= '<li>$'.$field.' == "'.htmlentities(addslashes($value)).'"</li>';
      $fields[$field] = $value;
    }
    reset($fields);

    $form['views_table_highlighter'] = array(
      '#type' => 'fieldset',
      '#title' => t('Table Highlighter'),
      '#collapsible' => TRUE,
      '#collapsed' => FALSE,
      'code' => array(
        '#type' => 'textarea',
        '#rows' => 2,
        '#title' => t('Highlighter PHP code'),
        '#default_value' => $vth_options['code'] ? $vth_options['code'] : 'if ($'.key($fields).'=="'.htmlentities(current($fields)).'") return \'red\';',
        '#description' => "<p>".t('Enter PHP code that returns the color with which this row should be highlighted.  Return without a value to leave this row unlit.').'</p>'
          .'<table><tr valign="top"><td>'
            .t('Available fields:')
            .'<ul>'.$fieldtext.'</ul>'
            .t('(Above are sample values from the first few rows of this view.)')
          .'</td><td>'
            .t('Available colors:')
            ."<ul><li>'red'</li><li>'yellow'</li><li>'green'</li><li>'cyan'</li><li>'blue'</li><li>'magenta'</li></ul>"
          .'</td></tr></table>'
      ),
    );
  }
}

