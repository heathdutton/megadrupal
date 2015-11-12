<?php

/**
 * @file
 * Contains the chart display plugin.
 */

/**
 * The plugin that handles the chart style.
 */
class datastore_visualization_plugin_style_chart extends views_plugin_style {
  function option_definition() {
    $options = parent::option_definition();
        
    $options['field_value'] = array('default' => '');
    //$options['field_value_label'] = array('default' => '');
    $options['field_label'] = array('default' => '');
    //$options['field_label_label'] = array('default' => '');
    $options['title'] = array('default' => '');
    $options['type'] = array('default' => 'lc');
    $options['width'] = array('default' => 600);
    $options['height'] = array('default' => 400);

    return $options;
  }

  function options_form(&$form, &$form_state) {
    parent::options_form($form, $form_state);

    $options = array('' => t('- None -'));
    $options += $this->display->handler->get_field_labels();

    $form['field_value'] = array(
      '#type' => 'select',
      '#title' => t('Value field'),
      '#options' => $options,
      '#default_value' => $this->options['field_value'],
      '#description' => t('...'),
    );
    
    /*$form['field_value_label'] = array(
      '#type' => 'textfield',
      '#title' => t('Label'),
      '#description' => t('Label for value part.'),
      '#size' => 40,
      '#required' => FALSE,
      '#default_value' => $this->options['field_value_label'],
    );*/
    
    $form['field_label'] = array(
      '#type' => 'select',
      '#title' => t('Label field'),
      '#options' => $options,
      '#default_value' => $this->options['field_label'],
      '#description' => t('...'),
    );
    
    /*$form['field_label_label'] = array(
      '#type' => 'textfield',
      '#title' => t('Label'),
      '#description' => t('Label for label part.'),
      '#size' => 40,
      '#required' => FALSE,
      '#default_value' => $this->options['field_label_label'],
    );*/
    
    $form['title'] = array(
      '#type' => 'textfield',
      '#title' => t('Title'),
      '#description' => t('Chart title.'),
      '#size' => 10,
      '#required' => FALSE,
      '#default_value' => $this->options['title'],
    );

    $form['type'] = array(
      '#type' => 'select',
      '#title' => t('Chart type'),
      '#description' => t('Chart type.'),
      '#options' => datastore_visualization_chart_types(),
      '#required' => TRUE,
      '#default_value' => $this->options['type'],
    );
    
    $form['width'] = array(
      '#type' => 'textfield',
      '#title' => t('Width'),
      '#description' => t('Chart width in pixels.'),
      '#size' => 8,
      '#required' => TRUE,
      '#default_value' => $this->options['width'],
    );
    
    $form['height'] = array(
      '#type' => 'textfield',
      '#title' => t('Height'),
      '#description' => t('Chart height in pixels.'),
      '#size' => 8,
      '#required' => TRUE,
      '#default_value' => $this->options['height'],
    );
  }

  function render() {
    $sets = $this->render_grouping($this->view->result, $this->options['grouping']);
    
    $chart = array(
      '#theme' => 'chart',
      '#title' => $this->options['title'],
      '#type' => $this->options['type'],
      '#chart_id' => drupal_clean_css_identifier($this->view->name . '-' . $this->display->id),
      '#size' => array(
        '#width' => $this->options['width'],
        '#height' => $this->options['height'],
      ),
      '#data' => array(),
      '#labels' => array(),
      '#data_colors' => array(),
      '#adjust_resolution' => TRUE,
    );
    
    $chart += array('#chart_type' => 'datastore_visualization');
    
    foreach ($sets as $title => $records) {
      foreach ($records as $row_index => $row) {
        foreach ($this->view->field as $key => $field) {
          // let the user choose the value field
          if ($key == $this->options['field_value']) {
            $field_alias = $field->field_alias;
            $chart['#data'][] = $row->$field_alias;
          }
          // let the user choose the label field
          if ($key == $this->options['field_label']) {
            $field_alias = $field->field_alias;
            
            // TODO improve this
            if (get_class($field) == 'views_handler_field_date') {
              $chart['#labels'][] = date('m.d.y', $row->$field_alias); 
            } 
            else {
              $chart['#labels'][] = $row->$field_alias;
            }
          }
        }
      }
    }

    return drupal_render($chart);
  }
}
