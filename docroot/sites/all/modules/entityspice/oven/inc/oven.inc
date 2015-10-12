<?php

define ('OVEN_OPENING_TOKEN', '{{');
define ('OVEN_CLOSING_TOKEN', '}}');

function _oven_explode_pop($delimiter, $string) {
  $data = explode($delimiter, $string);
  array_pop($data);
  return implode($delimiter, $data);
}

function _oven_get_parent_dir() {
  return _oven_explode_pop('/', dirname(__FILE__));
}

function _oven_get_camel_case($name) {
  $camel = "";
  $flag = FALSE;
  for($i = 0 ; $i < strlen($name); $i++) {
    $n = $name[$i];
    if ($n == '_') {
      $flag = TRUE;
      continue;
    }

    $camel .= $flag ? strtoupper($n) : $n;
    $flag = FALSE;
  }

  $camel[0] = strtoupper($camel[0]);
  return $camel;
}

function _oven_is_drush() {
  return defined('DRUSH_VERSION');
}

function _oven_copy_dir_recursive($source, $dest, $name=NULL) {
  if(_oven_is_drush()) {
    $dest = $name ? $dest . '/' . $name : $dest;
    if(!drush_copy_dir($source, $dest , FILE_EXISTS_ABORT)) {
      return [FALSE, 'directory exists'];
    }
  }
  else {
    if (is_dir($dest)) {
      return [FALSE, 'directory exists'];
    }
    if(!__oven_copy_recursive($source, $dest)) {
      return [FALSE, 'failed to copy directory'];
    }
  }

  return [TRUE, ''];
}

function __oven_copy_recursive($source, $dest) {
  if (is_dir($source)) {
    if (!file_prepare_directory($dest, FILE_CREATE_DIRECTORY)) {
      return FALSE;
    }

    $directory = dir($source);
    while (FALSE !== ($read = $directory->read())) {
      if($read != '.' && $read != '..') {
        continue;
      }
      if (!__oven_copy_recursive($source . '/' . $read, $dest . '/' . $read)) {
        return FALSE;
      }
    }

    $directory->close();
  }
  else {
    file_unmanaged_copy($source, $dest);
  }
}

function __oven_error_msg($error_no) {
  switch ($error_no) {
    case __OvenValidator::E_NO_ERROR:
      $msg = 'No error';
      break;

    case __OvenValidator::E_NONE_MACHINE:
      $msg = 'Machine name should only contain ';
      $msg .= 'lowercase letters, numbers, underscore and begin with a letter, ';
      $msg .= "it's length should be less than 63 characters.";
      break;

    case __OvenValidator::E_NONE_ALPHANUM:
      $msg = 'Argument must contain only letters, numbers and underscores.';
      break;

    case __OvenValidator::E_EMPTY:
      $msg = 'Argument can not be empty.';
      break;

    case __OvenValidator::E_TOO_LONG:
      $msg = 'Length of argument must be less than 63 characters.';
      break;

    case __OvenValidator::E_NONE_LABEL:
      $msg = 'Argument must contain only Letters, numbers,  underscore and whitespaces.';
      break;

    case __OvenValidator::E_BAD_URI:
      $msg = 'URI must not begin or end with a slash.';
      break;

    case __OvenValidator::E_INVALID:
      $msg = 'Invalid argument';
      break;

    default:
      $msg = 'Unknown error.';
      break;
  }

  return $msg;
}

function _oven_files_rename_recursive($new_path, $replacement, $with) {
  $open = opendir($new_path);
  while ($read = readdir($open)) {
//    drush_op('chdir', $newpath);
    chdir($new_path);
    if ($read == '.' || $read == '..') {
      continue;
    }

    if(is_dir($read)) {
      _oven_files_rename_recursive($new_path . '/' . $read, $replacement, $with);
      continue;
    }

    // Remove .txt from name
    $new_name = _oven_explode_pop('.', $read);
    rename($read, str_replace($replacement, $with, $new_name));
  }

  closedir($open);
}

function _oven_files_replace_recursive($new_path, array $tokens) {
  $open = opendir($new_path);
  while ($read = readdir($open)) {
    drush_op('chdir', $new_path);
    if ($read == '.' || $read == '..') {
      continue;
    }
    if(is_dir($read)) {
      _oven_files_replace_recursive($new_path . '/' . $read, $tokens);
      continue;
    }

    $file = new __OvenFileHandlerDrush($read);
    $read = $file->read();
    $tokenizer = new __OvenTokenizer($read, $tokens);
    $replaced = $tokenizer->getText();
    $file->replace($replaced);
  }

  closedir($open);
}

function _oven_ekran($path, $tokens, $machine) {
  $source = _oven_get_parent_dir() .  '/placeholders';
  $newpath = $path . '/' . $machine;
  list($ok, $msg) = _oven_copy_dir_recursive($source, $newpath);

  _oven_files_rename_recursive($newpath, 'PLACEHOLDER', $machine);
  _oven_files_replace_recursive($newpath, $tokens);

  return [$ok, $msg, $newpath];
}

function _oven_default_arguments($machine_name) {
  $tokens = [];

  foreach (__OvenTokenInfoName::getTokensObject(FALSE) as $token) {
    $token->set($token->defaultValue());
    $token->processValue();
    $tokens[$token->get('name')] = $token;
  };
  $tokens['machine'] = new __OvenTokenInfoName('machine');
  $tokens['machine']->set($machine_name);

  return $tokens;
}
