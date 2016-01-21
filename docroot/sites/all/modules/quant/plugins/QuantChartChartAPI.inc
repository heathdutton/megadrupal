<?php

// Number allowed colors
define('QUANT_PALETTE_AMOUNT', 25);

/**
 * QuantChart plugin class to generate charts using
 * chart.module (Chart API)
 */
class QuantChartChartAPI extends QuantChart {
  var $output = NULL;
  var $chart = NULL;
  
  /**
   * Implements parent::variables().
   */
  function variables() {
    return array(
      'quant_chartapi_palette',
    );
  }
  
  /**
   * Implements parent:adminSettings().
   */
  function adminSettings() {
    $form = array();
    $palette = $this->palette();

    // Color settings
    $form['chartapi_color'] = array(
      '#type' => 'fieldset',
      '#title' => t('Color settings'),
      '#collapsible' => TRUE,
      '#collapsed' => TRUE,
      '#description' => t('Specify the colors that the charts will be rendered in.'),
    );
    for ($i = 0; $i < QUANT_PALETTE_AMOUNT; $i++) {
      $form['chartapi_color']['quant_chartapi_palette_color_' . $i] = array(
        '#type' => 'textfield',
        '#title' => t('Color') . ' #' . ($i + 1),
        '#default_value' => $palette[$i],
        '#field_prefix' => '#',
        '#size' => 10,
        '#maxlength' => 6,
      );
    }
    
    return $form;
  }
  
  /**
   * Implements parent::adminSettingsValidate().
   */
  function adminSettingsValidate(&$form, &$form_state) {
    // Iterate through colors
    $colors = array(); // Store all colors in a single array
    for ($i = 0; $i < QUANT_PALETTE_AMOUNT; $i++) {
      $color = $form_state['values']['quant_chartapi_palette_color_' . $i];
      $colors[] = strtoupper($color);
      // Remove form value to avoid multiple color variables
      unset($form_state['values']['quant_chartapi_palette_color_' . $i]);
    }
    
    // Make sure we at least have one color
    if (empty($colors)) {
      form_set_error('color', t('You need to enter at least one color.')); 
    }
    else {
      // Save colors in a single variable
      $form_state['values']['quant_chartapi_palette'] = $colors;
    }
  }
  
  /**
   * Implements parent::build().
   */
  function build() {
    // Initialize the chart
    switch ($this->quant->chartType) {
      case 'line':
        $this->line_chart();
        break;
      case 'bar':
        $this->bar_chart();
        break;
      case 'pie':
        $this->pie_chart();
        break;
    }
    
    // Fill the chart with data
    switch ($this->quant->dataType) {
      case 'single':
        $this->chart_single();
        break;
      case 'multiple':
        $this->chart_multiple();
        break;
      case 'count':
        $this->chart_count();
        break;
    }
    
    // Add color
    $this->color();
    // Add size
    $this->size();
  }
  
  /**
   * Implements parent::render().
   */
  function render() {
    // Generate the chart
    $this->output = theme('chart', array('chart' => $this->chart));
    // Return the output
    return $this->output;
  }
  
  /**
   * Initialize the creation of a line chart
   */
  function line_chart() {
    $this->chart = array(
      '#chart_id' => $this->quant->id,
      '#title' => $this->title(),
      '#type' => CHART_TYPE_LINE,
      '#adjust_resolution' => TRUE,
      '#chart_fill' => chart_fill('c', 'fff'),          
      '#grid_lines' => chart_grid_lines(20, 20, 1, 5), 
    );
  }
  
  /**
   * Initialize the creation of a bar chart
   */
  function bar_chart() {
    $this->chart = array(
      '#chart_id' => $this->quant->id,
      '#title' => $this->title(),
      '#type' => CHART_TYPE_BAR_V,
      '#adjust_resolution' => TRUE,
      '#grid_lines' => chart_grid_lines(30, 20),
      '#bar_size' => chart_bar_size(45, 15), 
    );  
  }
  
  /**
   * Initialize the creation of a bar chart
   */
  function pie_chart() {
    $this->chart = array(
      '#chart_id' => $this->quant->id,
      '#title' => $this->title(),
      '#type' => CHART_TYPE_PIE,
      '#adjust_resolution' => TRUE,
    );
  }
  
  /**
   * Helper function to generate the title of the chart
   */
  function title() {
    $title = $this->quant->label;
 
    // If the quant wants the sum of item amounts in the title
    if ($this->quant->labelsum) {
      $sum = 0;
      foreach ($this->quant->data->data as $value) {
        if ($this->quant->dataType == 'multiple') {
          foreach ($value as $amount) {
            $sum = $sum + $amount;
          }
        }
        else {
          $sum = $sum + $value;
        }
      }
    
      $title .= ' (' . t('Total: !sum', array('!sum' => $sum)) . ')';
    }
    
    return chart_title($title);
  }
  
  /**
   * Add color to the chart
   */
  function color() {
    // Load the color palette
    $palette = $this->palette(TRUE); 
    
    // If quant is singular, add a random color
    if ($this->quant->dataType == 'single') {
      $this->chart['#data_colors'][] = $palette[rand(0, count($palette))];
    }
    else {
      for ($i = 0; $i < count($palette); $i++) {
        $this->chart['#data_colors'][] = $palette[$i];
      }
    }
  }
  
  /**
   * Add size to the chart
   */
  function size() {
    $this->chart['#size'] = chart_size(
      variable_get('quant_width', 500), 
      variable_get('quant_height', 200)
    ); 
  }
  
   /**
    * Add an x-axis label to the chart
    * 
    * @param $label
    *   The label for the x-axis
    */
  function x_label($label) {
    $this->chart['#mixed_axis_labels'][CHART_AXIS_X_BOTTOM][0][] = chart_mixed_axis_label($label);
  }
  
  /**
   * Add an y-axis range to the chart
   * 
   * @param $min
   *   The minimum value for the y-axis
   * @param $max
   *   The maximum value for the y-axis
   */
  function y_range($min, $max) {
    $max = max($max, 1); // Prevent a max that's zero
    $this->chart['#mixed_axis_labels'][CHART_AXIS_Y_LEFT][0][] = chart_mixed_axis_range_label($min, $max);
  }
  
  /**
   * Return the available color palette
   */
  function palette($random = FALSE) {
    $palette = variable_get('quant_chartapi_palette', array());
  
    if (!$palette) {
      $default_palette = chart_color_schemes();
      $palette = $default_palette['default'];
    }
  
    return $random ? shuffle($palette) : $palette;
  }
  
  /**
   * Fill a count-type chart with data
   */
  function chart_count() {
    $max = 0; // Determine the highest available value on y-axis
    
    foreach ($this->quant->data->data as $key => $value) {
      $this->chart['#data'][] = $value;
      
      // If pie chart, let's add the numeric value to the label
      if ($this->quant->chartType == 'pie') {
        $this->x_label($key . ' (' . $value . ')');
      }
      else {
        $this->x_label($key);
      }
      
      $max = max($max, $value);
    }
    
    $this->y_range(0, $max);
  }
  
  /**
   * Fill a single datapoint chart with data
   */
  function chart_single() {
    $max = 0; // Determine the highest available value on y-axis
    $interval = 0; // Counter to help break the x-axis label
    $period = ceil(count($this->quant->data->data) / 10); // Period when to break x-axis
  
    foreach ($this->quant->data->data as $date => $value) {
      // Only show the X label every calculated period
      if (!$interval) {
        $this->x_label($date);
        $interval = $period;
      }
      $this->chart['#data'][] = $value;
      $max = max($max, $value);
      $interval--;
    }
    
    $this->y_range(0, $max);
  }
  
  /**
   * Fill a multiple datapoint chart with data
   */
  function chart_multiple() {
    $max = 0; // Determine the highest available value on y-axis
    $interval = 0; // Counter to help break the x-axis label
    $x = FALSE; // Only register the x-axis labels once
    
    foreach ($this->quant->data->data as $type => $values) {
      // Set type as a legend
      $this->chart['#legends'][] = $type; 
      // Period when to break x-axis
      $period = ceil(count($this->quant->data->data[$type]) / 10);
      
      foreach ($values as $date => $value) {
        $this->chart['#data'][$type][] = $value;
        $max = max($max, $value);
        
        if (!$x) { // Only set x-axis labels once
          if(!$interval) {
            $this->x_label($date);
            $interval = $period;
          }
          $interval--;
        }
      }
      
      $x = TRUE; // x-axis labels have been set
    }
    
    $this->y_range(0, $max);
  }
}
