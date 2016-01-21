<?php

/**
 * QuantChart plugin to provide simple HTML table charts
 */
class QuantChartTable extends QuantChart {
  var $output = NULL;
  var $rows = NULL;
  var $header = NULL;
  var $table = NULL;
  
  /**
   * Implements parent::build().
   */
  function build() {
    switch ($this->quant->dataType) {
      case 'single':
        $this->build_single();
        break;
      case 'multiple':
        $this->build_multiple();
        break;
      case 'count':
        $this->build_count();
        break;
    }
  }
  
  /**
   * Implements parent::render().
   */
  function render() {
    // Generate the table
    $this->table = theme(
      'table', 
      array(
        'header' => $this->header, 
        'rows' => $this->rows
      )
    );
    // Generate the output
    $this->output = theme(
      'quant_table',
      array(
        'table' => $this->table, 
        'title' => $this->quant->label, 
        'width' => variable_get('quant_width', 500)
      )
    );
    // Return the output
    return $this->output;
  }
  
  /**
   * Build a table chart for single datapoints
   */
  function build_single() {
    $this->header = array(t('Date'), t('Count'));
    $this->rows = array();
    foreach ($this->quant->data->data as $date => $count) {
      $this->rows[] = array($date, $count);
    }
  }
  
  /**
   * Build a table chart for multiple datapoints
   */
  function build_multiple() {
    $this->header = array(t('Label'), t('Data'));
    $this->rows = array();
    foreach ($this->quant->data->data as $item => $data) {
      $irows = array();
      foreach ($data as $date => $count) {
        $irows[] = array($date, $count);
      }
      $this->rows[] = array($item, theme('table', array('header' => array(t('Date'), t('Count')), 'rows' =>  $irows)));
    }
  }
  
  /**
   * Build a table chart for count data
   */
  function build_count() {
    $this->header = array(t('Label'), t('Count'));
    $this->rows = array();
    foreach ($this->quant->data->data as $title => $count) {
      $this->rows[] = array($title, $count);
    }
  }
}
