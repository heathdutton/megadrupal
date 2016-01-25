<?php
// $Id $

/**
 * Display database report.
 */
function dba_report_page($drush = FALSE) {
  $output = array();
  $data = new stdClass();

  $data->drush = $drush;

  $data->version = dba_invoke_driver_specific('version', $data, 'report');
  $data->status = dba_invoke_driver_specific('status', $data, 'report');
  $data->variables = dba_invoke_driver_specific('variables', $data, 'report');
  $data->uptime = dba_invoke_driver_specific('uptime', $data, 'report');
  $data->name = dba_invoke_driver_specific('name', $data, 'report');
  $data->driver = dba_invoke_driver_specific('driver', $data, 'report');

  $output = dba_report_header($data);

  $return = dba_invoke_driver_specific('output', $data, 'report');
  if (!empty($return)) {
    $output = array_merge($output, $return);
  }

  $display = '' ;
  foreach ($output as $line) {
    if (substr($line, 0, 1) == '_') {
      $display .= "\n" ;
    }
    $display .= $line . "\n" ;
  }
  if ($drush) {
    return $display;
  }
  else {
    return "<pre>$display</pre>";
  }
}

/**
 * Format section titles like standard mysqlreport sections.
 * Title is already passed through t().
 */
function dba_report_section($title) {
  return "__ $title " . str_repeat('_', 72 - 4 - strlen($title));
}

function dba_report_spaces($count) {
  $output = '';
  while ($count-- > 0) {
    $output .= ' ';
  }
  return $output;
}

/**
 * Format number.
 */
function dba_report_short($number, $kb = FALSE, $d = 2) {
  $n = 0;

  $format= array();

  if ($kb) {
    $format = array('b','Kb','Mb','Gb','Tb');
    while ($number > 1023) {
      $number /= 1024;
      $n++;
    }
  }
  else {
    $format = array('','K','M','G','T') ;
    while ($number > 999) {
      $number /= 1000;
      $n++;
    }
  }

  $short = sprintf("%.${d}f%s", $number, $format[$n]) ;

  if (preg_match('/^(.+)\.00$/', $short, $matches)) {
    return $matches[1] ; // Convert 12.00 to 12, but not 12.00kb to 12kb
  }

  return $short;
}

/**
 * Return value as percentage.
 */
function dba_report_to_percentage($fraction, $integer = FALSE) {
  if ($integer) {
    return (int)$fraction * 100 ;
  }
  else {
    return floatval($fraction * 100) ;
  }
}

/**
 * Display report header.
 */
function dba_report_header($data) {
  $output = array();

  $date = date('d-M-Y H:m:s');
  $version = sprintf('%d.%d.%d', $data->version->major, $data->version->minor, $data->version->patch);
  $output[] = t('!database !version uptime !uptime !date', array('!database' => $data->name, '!version' => sprintf('%-16s', $version), '!uptime' => sprintf('%-12s', $data->uptime), '!date' => sprintf('%24s', $date)));
  return $output ;
}

/**
 * Helper function to highlight error lines in red.
 * TODO: Use CSS.
 */
function _dba_alert_open($alert, $text, $drush) {
  if ($alert && !$drush) {
    return "<font color=\"red\" title=\"$text\">";
  }
  else {
    return '';
  }
}

/**
 * Helper function to highlight error lines in red.
 * TODO: Use CSS.
 */
function _dba_alert_close($alert, $drush) {
  if ($alert && !$drush) {
    return '</font>';
  }
  if ($alert && $drush) {
    return ' <--';
  }
  else {
    return '';
  }
}

/**
 * Helper function, return TRUE if current values are above thresholds.
 */
function dba_report_alert($actual, $limits, $operator = '>=') {
  foreach ($actual as $key => $value) {
    $comparison = "return $value $operator ". $limits[$key] .';';
    if (eval($comparison)) {
      return TRUE;
    }
  }
  return FALSE;
}

/**
 * Class for comparing version.
 */
class dba_report_version {
  public $major;
  public $minor;
  public $patch;

  public function __construct(
    $major = 0,
    $minor = 0,
    $patch = 0) {
    $this->major = $major;
    $this->minor = $minor;
    $this->patch = $patch;
  }

  public function __toString() {
    return sprintf('%02d%02d%02d', $this->major, $this->minor, $this->patch);
  }
  
  public function compare_version($version, $operation = '==') {
  	$current = $this->__toString();
  	$compare = $version->__toString();
  	
  	$comparison = 'return $current' . $operation . '$compare;';
   	return eval($comparison);
  }
}


