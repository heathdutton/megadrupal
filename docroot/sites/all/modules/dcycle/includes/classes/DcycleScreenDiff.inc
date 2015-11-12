<?php

class DcycleScreenDiff extends DcycleCheck {
  function _Preflight_() {
    global $base_url;
    if (!$base_url) {
      $errors[] = t('Please set $base_url in your settings.php');
    }
    $errors = array();
    if (!DcycleEnvironment::ExecAvailable()) {
      $errors[] = t('The exec() function is not available');
    }
    $error = NULL;
    DcycleEnvironment::CommandAvailable('phantomjs', $error, ' -v');
    if ($error) {
      $errors[] = $error;
    }
    DcycleEnvironment::CommandAvailable('compare', $error);
    if ($error) {
      $errors[] = $error;
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
    $last = 'public://ci/artifacts/DcycleScreenDiff/last.png';
    $this->RemoveFile($last);
    $this->RemoveFile('public://ci/artifacts/DcycleScreenDiff/diff.png');
    $current = 'public://ci/artifacts/DcycleScreenDiff/current.png';
    if (file_exists($current)) {
      rename($current, $last);
    }
    global $base_url;
    DcycleEnvironment::RunCommand('phantomjs', 'sites/all/modules/contrib/dcycle/includes/classes/DcycleScreenDiff/capture.js ' . $base_url . ' sites/default/files/ci/artifacts/DcycleScreenDiff/current.png', $lines);
    if (!file_exists($last)) {
      copy($current, $last);
    }
    DcycleEnvironment::RunCommand('convert', 'sites/default/files/ci/artifacts/DcycleScreenDiff/last.png -resize "$(identify sites/default/files/ci/artifacts/DcycleScreenDiff/current.png|awk \'{print $3}\')>" -size $(identify sites/default/files/ci/artifacts/DcycleScreenDiff/current.png|awk \'{print $3}\') xc:white +swap -gravity North -composite sites/default/files/ci/artifacts/DcycleScreenDiff/last-resized.png');
    $lines = array();
    DcycleEnvironment::RunCommand('compare', 'sites/default/files/ci/artifacts/DcycleScreenDiff/last-resized.png sites/default/files/ci/artifacts/DcycleScreenDiff/current.png -metric PSNR -verbose -compose src sites/default/files/ci/artifacts/DcycleScreenDiff/diff.png 2>&1', $lines);
    // see https://www.drupal.org/node/2316653
    $this->AddArtifact(self::ParseArtifacts($lines));
    $this->AddMetric(self::ParseMetrics($lines));
    return $lines;
  }

  function RemoveFile($file) {
    if (file_exists($file)) {
      unlink($file);
    }
  }

  static function ParseArtifacts($lines) {
    return implode(PHP_EOL, $lines);
  }

  static function ParseMetrics($lines) {
    $value = 0;
    foreach ($lines as $line) {
      $line = trim($line);
      if (substr($line, 0, strlen('all:')) == 'all:') {
        $value = substr($line, strlen('all:'));
        if ($value == 'inf') {
          $value = 0;
        }
      }
    }
    return array('difference' => $value);
  }
}
