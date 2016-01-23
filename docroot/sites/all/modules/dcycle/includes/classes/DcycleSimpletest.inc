<?php

class DcycleSimpletest extends DcycleCheck {
  function _Preflight_() {
    $errors = array();
    if (!module_exists('simpletest')) {
      $errors[] = t('Please enable the simpletest module');
    }
    if (count($errors)) {
      $this->AddError($errors);
      return FALSE;
    }
    return TRUE;
  }

  /**
   * The actual code to run this check.
   */
  function _Run_() {
    $lines = array();
    // see https://www.drupal.org/node/2316653
    DcycleEnvironment::RunCommand('php', 'scripts/run-tests.sh --xml sites/default/files/ci/artifacts/DcycleSimpletest --verbose demo', $lines);
    $this->AddArtifact(self::ParseArtifacts($lines));
    $this->AddMetric($this->ParseMetrics($lines));
    return $lines;
  }

  static function ParseArtifacts($lines) {
    self::MoveAll('public://simpletest/verbose', 'public://ci/artifacts/DcycleSimpletest');
    return implode(PHP_EOL, $lines);
  }

  function ParseMetrics($lines) {
    $files = file_scan_directory('public://ci/artifacts/DcycleSimpletest', '/\.xml/');
    $metrics = array();
    $metrics['test-classes'] = count($files);
    $metrics['test-lines'] = 0;
    $metrics['failures'] = 0;
    foreach ($files as $file) {
      $xml = self::loadXML($file->uri);
      $metrics['test-lines'] += count($xml);
      foreach ($xml as $line) {
        if (isset($line->failure)) {
          $metrics['failures']++;
          $this->AddError($line->failure[0]);
        }
      }
    }
    return $metrics;
  }

  static function MoveAll($source, $dest) {
    // See
    // Get array of all source files
    $files = scandir($source);
    // Identify directories
    $source = $source . '/';
    $destination = $dest . '/';
    // Cycle through all source files
    foreach ($files as $file) {
      if (in_array($file, array(".",".."))) continue;
      // If we copied this successfully, mark it for deletion
      if (copy($source.$file, $destination.$file)) {
        $delete[] = $source.$file;
      }
    }
    // Delete all successfully-copied files
    foreach ($delete as $file) {
      unlink($file);
    }
  }
}
