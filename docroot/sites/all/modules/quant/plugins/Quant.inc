<?php

/**
 * Quant class
 */
class Quant {
  // Attributes (see quant.api.php)
  var $id = NULL;
  var $label = NULL;
  var $labelsum = TRUE;
  var $table = NULL;
  var $field = NULL;
  var $group = NULL;
  var $query = NULL;
  var $queryCustom = NULL;
  var $dataType = NULL;
  var $chartType = NULL;
  // Execution variables
  var $period = NULL;
  var $days = NULL;
  // Classes
  var $data = NULL;
  var $chart = NULL;
  
  /**
   * Set the time period/range
   * 
   * @see execute()
   */
  function set_period($period) {
    $this->period = $period;
    // Check if period is a string
    if (is_string($this->period)) {
      $this->period = strtotime($this->period);
    }
    // Check if period is an array
    else if (is_array($this->period)) {
      // Check if the values are strings
      foreach ($this->period as $i => $date) {
        if (is_string($date)) {
          $this->period[$i] = strtotime($date); 
        }
      }
    }
  }
  
  /**
   * Determine the amount of days in the period
   */
  function set_days() {
    if (is_numeric($this->period)) {
      $this->days = ceil((time() - $this->period) / 86400);
    }
    else if (is_array($this->period)) {
      $this->days = ceil(($this->period[1] - $this->period[0]) / 86400);
    }
  }
  
  /**
   * Execute the quant for a given period
   * 
   * @param $period
   *   The timeframe for fetch data for.
   *   Options:
   *     1) A UNIX timestamp. Data will be fetched from the current time
   *        back until the provided timestamp.
   *     2) A string compatable with strtotime(), ie, '-2 weeks'. Data will
   *        be fetched from the current time until the provided time.
   *     3) An array containing two of the previous options, where the first
   *        value will be the start time, and the last will be the end time.
   */
  function execute($period) {
    // Include the needed classes
    quant_include('QuantData', 'plugins');
    quant_include('QuantChart', 'plugins');
    
    // Set the time period
    $this->set_period($period);
    
    // Determine the amount of days in the period
    $this->set_days();
    
    // Build the data
    $this->data = new QuantData($this, $this->execute_query());
    
    // Determine the chart plugin to use
    $plugin = quant_get_chart_plugin();
    
    // Include the chart plugin
    quant_include($plugin->plugin['file'], $plugin->plugin['path'], NULL);
    
    // Create and build a chart
    $this->chart = new $plugin->plugin['class']($this, $plugin);
    $this->chart->build();
  }
  
  /**
   * Execute the quant query
   * 
   * @return
   *   A database resource result
   */
  function execute_query() {
    // Use a pre-defined query, if there is one
    if (isset($this->query)) {
      if (is_array($this->period)) {
        return db_query($this->queryCustom, array(':period0' => $this->period[0], ':period1' => $this->period[1]));
      }
      else {
        return db_query($this->query, array(':period' => $this->period));
      }
    }
    
    // Generate the query for this quant
    $query = db_select($this->table, $this->table);
    if ($this->dataType == 'count') {
      $query->fields($this->table, array($this->count));
      $query->addExpression("COUNT({$this->table}.{$this->count})", 'count');
      $query->groupBy("{$this->table}.{$this->count}");
    }
    else {       
      $query->fields($this->table, array($this->field));
      if ($this->dataType == 'multiple') {
        $query->fields($this->table, array($this->group));
      }
      $query->orderBy("{$this->table}.{$this->field}", 'DESC');
    }
    if (is_array($this->period)) {
      $query->condition("{$this->table}.{$this->field}", $this->period[0], '>=');
      $query->condition("{$this->table}.{$this->field}", $this->period[1], '<=');
    }
    else {
      $query->condition("{$this->table}.{$this->field}", $this->period, '>=');
    }
    // Add the generated query to the object
    $this->query = $query;

    return $this->query->execute();
  }
  
  /**
   * Render the quant so it can be printed on the screen
   * 
   * @return
   *   HTML output
   */
  function render() {
    return $this->chart->render();
  }
}
