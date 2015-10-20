<?php

/**
 * @file
 * Provides traits for sanitizing and working with data.
 *
 * Useful during development, very useful :|
 * I Hate PHP
 */

class EntitySpiceException extends Exception {
}

class EntitySpiceExceptionBadData extends Exception{
}

class EntitySpiceExceptionBadConfiguration extends Exception{
}

trait EntitySpiceDataHandler {
  protected function exceptionBadData($name, $type, $message = '') {
    $msg = 'Bad data/name. Name: ' . $name . ' - type: ' . $type;
    if (!empty($message)) {
      $msg .= ' - additional info: ' . $message;
    }

    ddebug_backtrace();
    throw new EntitySpiceExceptionBadData($msg);
  }

  protected function exceptionEmptyData($name, $type, $msg = '') {
    ddebug_backtrace();
    throw new EntitySpiceExceptionBadData('Empty data for Name: ' . $name . ' - type: ' . $type, $msg);
  }

  protected function exceptionBadConfiguration($message) {
    ddebug_backtrace();
    throw new EntitySpiceExceptionBadConfiguration($message);
  }

  protected function sanitizeNotEmpty($data, $name, $type, $shoud_not_be_empty = TRUE) {
    if ($shoud_not_be_empty && empty($data)) {
      $this->exceptionEmptyData($name, $type);
    }
  }

  protected function sanitizeString($data, $name = '', $not_empty = TRUE, $throw_exception = TRUE) {
    $msg = '';
    $is_empty = empty($data);
    if (!$is_empty && !is_string($data)) {
      $msg = 'Invalid string';
    }
    elseif ($is_empty && $not_empty) {
      $msg = 'Empty string';
    }

    if ($msg && $throw_exception) {
      ddebug_backtrace();
      throw new EntitySpiceExceptionBadData("$msg for: $name");
    }

    return empty($msg);
  }

  protected function sanitizeInteger($data, $name = '', $not_empty = TRUE, $throw_exception = TRUE) {
    if ($data === '0' || $data === 0) {
      return TRUE;
    }

    $msg = '';
    $is_empty = empty($data);
    if(!$is_empty && !is_int($data)) {
      if (!is_string($data)) {
        $msg = 'Invalid data type';
      }
      elseif (preg_match('#[^0-9]+#', $data)) {
        $msg = 'Invalid integer';
      }
    }
    elseif ($is_empty && $not_empty) {
      $msg = 'Empty integer data';
    }

    if($msg && $throw_exception) {
      ddebug_backtrace();
      throw new EntitySpiceExceptionBadData("$msg for: $name");
    }

    return empty($msg);
  }

  protected function sanitizeStringRegex($data, $name, $regex, $should_match = FALSE) {
    if (preg_match($data, $regex) XOR $should_match) {
      $this->exceptionBadData($name, 'string', 'Malformed string');
    }
  }

  protected function sanitizeData($type) {
    $args = func_get_args();
    array_shift($args);
    return call_user_func_array(array($this, 'sanitize' . ucfirst($type)), $args);
  }

  protected function setData($name, $type, $data, $not_empty = TRUE, $throw_exception = TRUE) {
    $this->sanitizeString($name, 'Property name');

    switch ($type) {
      case 'string':
        $this->sanitizeString($data, $name, $not_empty, $throw_exception);
        break;

      case 'integer':
        $this->sanitizeInteger($data, $name, $not_empty, $throw_exception);
        break;

      default:
        $this->exceptionBadConfiguration('Unknown data type: ' . $type . ' for: ' . $name);
    }

    $this->{$name} = $data;
  }

  /**
   * Set an entity property on object.
   */
  protected function setEntity($name, $entity, $entity_type = NULL, $load = TRUE, $not_empty = TRUE) {
    $this->sanitizeString($name, 'property name');
    $this->sanitizeNotEmpty($entity, $name, 'entity', $not_empty);

    if ($load) {
      $entity = entity_metadata_wrapper($entity_type, $entity);
    }
    elseif(!is_int($entity) && !is_object($entity)) {
      $this->exceptionBadData('entity', 'integer/entity');
    }

    $this->{$name} = $entity;
  }

  protected function setTextFieldData($value = '') {
    return array('value' => '');
  }

  protected function getFieldSetData($type, $data) {
    switch ($type) {
      case 'text':
        return $data;
        break;

      default:
        $this->exceptionBadConfiguration('Unsupported field type: ' . $type);
    }
  }

  /**
   * Checks to see if data is set or is null.
   */
  protected function dataIsSet($type, $data) {
    if ($type == 'integer') {
      return ($data === 0 || $data === '0') || !empty($data);
    }

    return !empty($data);
  }
}


/**
 * AN UNSAFE, VERY UNSAFE DANGEROUS replacement for dpm.
 *
 * Very much like dpm but only for when dpm can't be used and you are getting
 * WSOD.
 *
 *
 * ATTENTION ATTENTION: NO ACCESS CHECK!!!!!!!
 * This function does NOT CHECK for user's access to see if user has access to
 * dpm data. THIS IS CONSIDERED A GREAT SECURITY RISK IF USED IMPROPERLY.
 */
function _wsod_dpm($what, $name='UNNAMED') {
  if(!ENTITYSPICE_DEBUG) {
    return;
  }

  $export = var_export($what, TRUE);
  $export = str_replace(' ', '&nbsp', $export);
  $export = nl2br($export);
  echo '<h1>', $name, '</h1>', $export;
  echo "<br><br><h3>___________________________end of ", $name, '_______________________</h3><br><br>';
}
function _dpm_wsod($what, $name='UNNAMED') {
  _wsod_dpm($what, $name);
}

function _wsod_backtrace() {
  $btr = debug_backtrace();
  $new_btr = array();
  foreach ($btr as $index) {
    $add0 = $index['function'] . ' :: ' . $index['file'] . '::' . $index['line'] .  "\n\n";
    $add1 = print_r($index['args'], TRUE);

    $new_btr[] = $add0 . "<br>" . $add1 . "\n<h2>__________________________</h2>\n";
  }

  $new_btr = implode("\n", $new_btr);
  _dpm_wsod($new_btr, 'btr');
}