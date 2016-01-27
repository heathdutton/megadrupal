<?php

function paas_metric_code_call($value) {
  header('Content-Type: text/plain');
  $mask = '/\.(module|inc|php)/';
  _paas_metric_code_name($value);
  echo "type,name,function,inmodule\n";
  $sql = 'SELECT filename, type FROM {system} WHERE type IN (\'module\', \'theme\') AND name = :value';
  $res = db_query($sql, array(':value' => $value));
  while ($item = $res->fetchObject()) {
    _paas_metric_code_type($item->type);
    _paas_metric_code_ignore($item->filename, TRUE);
    $dir = dirname($item->filename);
    file_scan_directory($dir, $mask, array(
      'callback' => '_paas_metric_code_scan_file'
    ));
  }
}

function _paas_metric_code_name($set = NULL) {
  static $name;
  if ($set) {
    $name = $set;
  }
  return $name;
}

function _paas_metric_code_type($set = NULL) {
  static $type;
  if ($set) {
    $type = $set;
  }
  return $type;
}

function _paas_metric_code_ignore($filename, $set = NULL) {
  static $dirs = array();
  $dir = dirname($filename);
  if (!is_null($set)) {
    $sql = 'SELECT filename FROM {system} WHERE type IN (\'module\', \'theme\') AND filename LIKE :dir';
    $res = db_query($sql, array(':dir' => "$dir/%"));
    while ($item = $res->fetchObject()) {
      if (dirname($item->filename) != $dir) {
        $dirs[] = dirname($item->filename);
      }
    }
  }
  foreach ($dirs as $ignore_dir) {
    if (substr($dir, 0, strlen($ignore_dir)) == $ignore_dir) {
      return TRUE;
    }
  }
  return FALSE;
}

function _paas_metric_code_scan_file($file) {
  static $start = NULL;
  if (is_null($start)) {
    $start = time();
  }
  if (time() > $start + 25) {
    exit;
  }
  
  if (_paas_metric_code_ignore($file)) {
    return;
  }
  
  $code = file_get_contents($file);
  if (preg_match('/class[\s][a-z_]+[\s]\{/si', $code)) {
    // Classes require token parser to work correctly.
    _paas_metric_code_scan_file_tokens($file, $code);
  }
  else {
    // No classes found, use faster regexp parser.
    _paas_metric_code_scan_file_regexp($file, $code);
  }
}

function _paas_metric_code_scan_file_regexp($file, $code) {
  $ismodule = (int) (substr($file, strrpos($file, '.')) == '.module');
  preg_match_all('/function[\s]+([a-z0-9_]+)[\s]*\\([\s]*(\\$|\\))/si', $code, $matches);
  foreach ($matches[1] as $match) {
    if (preg_match('/^hook_/', $match)) {
      echo "hook,," . $match . ",0\n";
    }
    else {
      echo _paas_metric_code_type() . ',' . _paas_metric_code_name() . ',' . $match . ",$ismodule\n";
    }
  }
  preg_match_all('/implement[a-z]+ (?:of )?(hook_[a-z0-9_]+)\\(\\)\\.?[^\\/]*\\*\\/[\\s]*function[\\s]/si', $code, $matches);
  foreach ($matches[1] as $match) {
    echo "hook,," . $match . ",0\n";
  }
}

function _paas_metric_code_scan_file_tokens($file, $code) {
  $ismodule = (int) (substr($file, strrpos($file, '.')) == '.module');
  $tokens = token_get_all($code);
  $doc = FALSE;
  $function = FALSE;
  $class = FALSE;
  $classname = NULL;
  foreach ($tokens as $token) {
    switch ($token[0]) {
      case T_WHITESPACE:
        break;
      case T_CLASS:
        $class = TRUE;
        break;
      case T_DOC_COMMENT:
        $doc = $token[1];
        break;
      case T_FUNCTION:
        $function = TRUE;
        break;
      case T_STRING:
        if ($function) {
          if ($classname) {
            $token[1] = $classname . '::' . $token[1];
          }
          if (preg_match('/^hook_/', $token[1])) {
            echo "hook,," . $token[1] . ",0\n";
          }
          else {
            echo _paas_metric_code_type() . ',' . _paas_metric_code_name() . ',' . $token[1] . ",$ismodule\n";
            if (preg_match('/^[^a-z]+implement[a-z]+ (?:of )?(hook_[a-z0-9_]+)\\(\\)\\.?/i', $doc, $match)) {
              echo "hook,," . $match[1] . ",0\n";
            }
          }
        }
        elseif ($class) {
          $classname = $token[1];
        }
        break;
      default:
        $doc = FALSE;
        $function = FALSE;
        $class = FALSE;
    }
  }
}
