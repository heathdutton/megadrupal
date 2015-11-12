<?php

// Date formats
define('QUANT_DATE_DAY_FORMAT', 'M j'); // Month Day (Feb 12)
define('QUANT_DATE_MONTH_FORMAT', 'M y'); // Month Year (Jun 09) 

/**
 * QuantData class used to process and format the data returned by
 * the Quant query
 */
class QuantData {
  var $quant = NULL;
  var $result = NULL;
  var $data = array();
  
  function __construct($quant, $result) {
    $this->quant = $quant;
    $this->result = $result;
    $this->generate_data();
  }
  
  /**
   * Generate the data
   */
  function generate_data() {
    switch ($this->quant->dataType) {
      case 'single':
        $this->generate_data_single();
        break;
      case 'multiple':
        $this->generate_data_multiple();
        break;
      case 'count':
        $this->generate_data_count();
        break;
    }
  }
  
  /**
   * Generate chart data for a singular data point across
   * a time period
   * 
   * @see quant_build_data()
   */
  function generate_data_single() {
    // Extract the days
    $days = $this->quant->days;
    // Extract the period
    $period = $this->quant->period;
    // Determine when the starting time is
    $start = is_array($period) ? $period[1] : time();
    // The date() format to use. We compare by month if there are more than 183 days.
    $format = ($days > 183) ? QUANT_DATE_MONTH_FORMAT : QUANT_DATE_DAY_FORMAT;
    // Whether or not to jump by day or month
    $interval = ($days > 183) ? 2629743 : 86400;
    // Possibly convert days to months
    $steps = ($days > 183) ? ($days / 30) : $days;
    // Extract the database field
    $field = $this->quant->field;
  
    // Create a new array that's preformatted with a key for
    // every single time period
    $dates = $this->date_array($start, $steps, $interval, $format); 
   
    // Calculate the amount of occurrences per time period
    foreach ($this->result as $item) {
      if (isset($dates[date($format, $item->$field)])) {
        $dates[date($format, $item->$field)]++;
      }
    }
    
    // Set in ascending order
    $dates = array_reverse($dates);
    
    $this->data = $dates;
  }
  
  /**
   * Generate chart data for a multiple data point over
   * a time period
   */
  function generate_data_multiple() {
    $data = array();
    $dates = array();
    // Extract the days
    $days = $this->quant->days;
    // Extract the period
    $period = $this->quant->period;
    // Determine when the starting time is
    $start = is_array($period) ? $period[1] : time();
    // The date() format to use. We compare by month if there are more than 96 days.
    $format = ($days > 75) ? QUANT_DATE_MONTH_FORMAT : QUANT_DATE_DAY_FORMAT;
    // Whether or not to jump by day or month
    $interval = ($days > 75) ? 2629743 : 86400;
    // Possibly convert days to months
    $steps = ($days > 75) ? ($days / 30) : $days;
    // Extract the field
    $field = $this->quant->field;
    // Extract the group field
    $group = $this->quant->group;
    
    // Place the items into an array for initial grouping by $group
    // For example: $data['page'] = array('May 10', 'May 18');
    foreach ($this->result as $item) {
      $data[$item->$group][] = date($format, $item->$field);
    }
    
    // Iterate through each group of items and calculate the amount of
    // items for each time period
    foreach ($data as $type => $values) {
      // Create a new array that's preformatted with a key for
      // every single time period
      $dates[$type] = $this->date_array($start, $steps, $interval, $format);
  
      // Increment for each time period
      foreach ($values as $value) {
        if (isset($dates[$type][$value])) {
          $dates[$type][$value]++;
        }
      }
      
      // Set in ascending order
      $dates[$type] = array_reverse($dates[$type]);
    }
    
    $this->data = $dates;
  }
  
  /**
   * Generate chart data for an aggregate (count) data point 
   * across a time period
   */
  function generate_data_count() {
    foreach ($this->result as $item) {
      $this->data[$item->{$this->quant->count}] = $item->count; 
    }
  }
  
  /**
   * Build the date array for charts to fill plotted data on
   * 
   * @param $start
   *   A timestamp representing the day to begin
   * @param $steps
   *   The number of steps (days/months) to go back
   * @param $interval
   *   A numeric value representing the step size in seconds
   * @param $format
   *   The date format to be used in date()
   * @return
   *   An array keyed with the date formatted by format. All values are 
   *   initially set to 0.
   */
  function date_array($start, $steps, $interval, $format) {
    $dates = array();
    for ($i = 0; $i < $steps; $i++) {
      $dates[date($format, $start)] = 0;
      $start -= $interval; // Backtrack a period
    }
    return $dates;
  }
}
