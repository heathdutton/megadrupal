<?php

function crush_command_sql($parsed_command) {
  $query_str = implode(' ', $parsed_command['args']);
  if (preg_match('/^\s*SELECT/i', $query_str)) {
    // Select query.
    try {
      $result = db_query($query_str)->fetchAll();
    }
    catch (Exception $e) {
      return array($e->getMessage());
    }
    $messages = array();
    foreach ($result as $row) {
      $messages[] = print_r($row, TRUE);
    }
    return $messages;
  }
  else {
    try {
      db_query($query_str)->execute();
    }
    catch (Exception $e) {
      return array($e->getMessage());
    }
    // If we get here, nothing crashed and burned.
    return array(t('Query executed successfully.'));
  }
}

