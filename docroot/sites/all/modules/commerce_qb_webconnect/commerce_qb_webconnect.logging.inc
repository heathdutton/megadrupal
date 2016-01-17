<?php
/**
 * Logging functions
 *
 * Useful for watching Quickbooks WebConnect updates running in realtime on the server.
 * Location of log file is defined in the UI.
 */

/**
 * Write a line to the qbclog
 *
 * @param string $msg
 *  The message to log
 *
 * @param type $variable
 *  A variable to print
 *
 * @param type $indent
 *  Whether or not to indent
 */
function cqbc_log_write($msg, $variable = null, $indent = true) {
  if (!variable_get('commerce_qb_webconnect_enable_logging', 0)) {
    return;
  }

  $handle = _cqbc_log_open();
  $margin = $indent ? '  * ' : '';

  // double quotes are needed
  $string = $margin . $msg . "\n";

  if ($variable) {
    $dump = '';

    if (is_array($variable) || is_object($variable)) {
      cqbc_dump_to_string($variable, $dump);
    } else {
      $dump = $variable;
    }

    $string .= $dump . "\n";
  }

  fwrite($handle, $string);
  fclose($handle);
}

/**
 * Open a handler for the qbclog file
 *
 * @param string $mode
 *  Mode argument for fopen
 *
 * @return file resource
 */
function _cqbc_log_open($mode = 'a') {
  $path = variable_get('commerce_qb_webconnect_log_filepath', '');
  $logpath = $path . '/qbclog.txt';

  if (!is_file($logpath) && !touch($logpath)){  
    watchdog('QB Webconnect', t('Could not create Commerce QB Connect log file.'));
    return;
  }

  $handle = fopen($logpath, $mode);
  return $handle;
}

/*
 * Print a block opening for an update run
 */
function cqbc_log_block_start() {
  if (variable_get('commerce_qb_webconnect_truncate_log', 0)) {

    // truncate log
    $handle = _cqbc_log_open('w');
    fclose($handle);
  }

  cqbc_log_write('-- Start update: ' . date('Y-m-d G:i:s') . ' --', null, false);
}

/*
 * Print a block closure for an update run
 */
function cqbc_log_block_end() {
  cqbc_log_write("-- End update: " . date('Y-m-d G:i:s') . " --\n", null, false);
}

/**
 * Given a variable, render it recursively in readable plaintext.
 * Via http://php.net/manual/en/function.var-dump.php :: flyfy1
 *
 * @param mixed $var
 *  Variable to dump
 *
 * @param $output
 *  Referenced container variable
 *
 * @return string
 */
function cqbc_dump_to_string($var,&$output,$prefix="") {
  foreach($var as $key=>$value) {
    if(is_array($value)) {
        $output.= '  ' . $prefix.$key.": \n";
        cqbc_dump_to_string($value,$output,"  ".$prefix);
    } else{
        $output.= '  ' . $prefix.$key.": ".$value."\n";
    }
  }
}

/**
 * Load a string of XML into a SimpleXMLElement
 *
 * @param string $qbxml_string
 *  Valid XML
 *
 * @return SimpleXMLElement
 */
function cqbc_qbxml_parse($qbxml_string) {
  if (class_exists('SimpleXMLElement') && $qbxml_string) {
    $parsed = new SimpleXMLElement($qbxml_string);

    return $parsed;
  }
}

/**
 * Extract an array of error messages, or a single error message string from a
 * SimpleXMLElement made from a Quickbooks response XML string
 *
 * @param SimpleXMLElement $qbxml_obj
 *  Must be of the kind sent via receiveResponseXML
 *
 * @return array
 *  Returns integer indexed array of error info (statusCode, statusMessage).
 */
function cqbc_qbxml_read_error($qbxml_obj){
  $errors = array();
  $response_elements = $qbxml_obj->QBXMLMsgsRs->children();

  if ($qbxml_obj instanceof SimpleXMLElement) {
    $response_elements = $qbxml_obj->QBXMLMsgsRs->children();

    foreach ($response_elements as $name => $element) {
      $attributes = $element->attributes();
      if (isset($attributes->statusSeverity) && isset($attributes->statusMessage) && $attributes->statusSeverity == 'Error') {
        $errors[] = array("statusCode"=>(string)$attributes->statusCode, "statusMessage"=>(string)$attributes->statusMessage);
      }
    }
    return $errors;
  }
}

/**
 * Given raw XML, return an array of error messages or a single error message
 * @see cqbc_qbxml_parse()
 *
 * @param type $qbxml_string
 *  XML string, must be of the kind sent via receiveResponseXML in the response
 *  property.
 *
 * @return mixed
 *  @see cqbc_qbxml_read_error()
 */
function cqbc_qbxml_parse_error($request) {
  
  $qbxml = cqbc_qbxml_parse($request->response);
  if ($qbxml) {
    return cqbc_qbxml_read_error($qbxml);
  }
  else {
    $errors = array();
    $errors[] = array("statusCode"=>$request->hresult, "statusMessage"=>$request->message);
    return $errors;
  }
}


