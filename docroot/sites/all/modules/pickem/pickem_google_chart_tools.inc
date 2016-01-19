<?php
/**
 * @file
 *
 * Provides the object definitions for the google chart tools helper class.
 */

/**
 * Definition of the google chart tools helper class.
 */
class pickem_google_chart_tools_helper {

  // Properties
  private $props = array();

  // Magic overload method.
  public function __get($name) {
    if (array_key_exists($name, $this->props)) {
      return $this->props[$name];
    }
    trigger_error(check_plain('Undefined property via __get(): ' . $name), E_USER_NOTICE);
    return NULL;
  }

  // Constructor
  public function __construct() {
    $this->props = array(
      'library_loaded' => FALSE,
    );
  }

  /**
   * Return a string with the appropriate javascript to render a chart.
   *
   * @param $chart
   *   A stdClass chart object.
   * @return
   *   A string consisting of JavaScript to embed into a web page.
   */
  function render($chart) {
    if (!isset($chart)) {
      $chart = new stdClass;
      $chart->visualization = 'google.visualization.ColumnChart';
      $chart->packages = array('corechart');
      $chart->container_id = 'chart_div';
      $chart->cols = array(
        array('string', 'Year'),
        array('number', 'Sales'),
        array('number', 'Expenses'),
      );
      $chart->rows = array(
        array('2004', 1000, 400),
        array('2005', 1170, 460),
        array('2006', 660, 1120),
        array('2007', 1030, 540),
      );
      $chart->options = array(
        'width' => 400,
        'height' => 240,
        'title' => 'Company Performance',
        'hAxis' => array(
          'title' => 'Year',
          'titleTextStyle' => array(
            'color' => 'red'
          )
        )
      );
    }

    $packages = json_encode($chart->packages);
    $chart_data_columns = '';
    foreach ($chart->cols as $col) {
      // Cols look like an arg list, not a json array.
      $list = json_encode($col);
      $list = rtrim($list, ']');
      $list = ltrim($list, '[');
      $chart_data_columns .= "data.addColumn(" . $list . ");\n";
    }
    $chart_data_rows = json_encode($chart->rows);
    $chart_options = 'var options = ' . json_encode($chart->options);
    $chart_output = '';
    if (!$this->props['library_loaded']) {
      $chart_output .= '<script type="text/javascript" src="https://www.google.com/jsapi"></script>';
      $this->props['library_loaded'] = TRUE;
    }
    $chart_output .= <<<EOD
  <script type="text/javascript">
    google.load("visualization", "1", {packages:$packages});
    google.setOnLoadCallback(drawChart_$chart->container_id);

    function drawChart_$chart->container_id() {
      var data = new google.visualization.DataTable();
      $chart_data_columns
      data.addRows($chart_data_rows);

      $chart_options

      var chart = new $chart->visualization(document.getElementById('$chart->container_id'));
      chart.draw(data, options);
    }
    </script>
EOD;



    return $chart_output;
  }
}
