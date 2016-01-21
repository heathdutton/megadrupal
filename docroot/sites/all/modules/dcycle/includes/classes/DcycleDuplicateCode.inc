<?php

class DcycleDuplicateCode extends DcycleCheck {
  function _Preflight_() {
    #https://github.com/sebastianbergmann/phpcpd
    $errors = array();
    if (!DcycleEnvironment::ExecAvailable()) {
      $errors[] = t('The exec() function is not available, but required to fetch the version.');
    }
    elseif (!DcycleEnvironment::CommandAvailable('phpcpd')) {
      $errors[] = 'Please install phpcpd, available at https://github.com/sebastianbergmann/phpcpd';
    }
    return TRUE;
  }

  /**
   * The actual code to run this check.
   */
  function _Run_() {
    $lines = array();
    DcycleEnvironment::RunCommand('phpcpd', 'sites/all/modules/custom/ --names="*.inc,*.module,*.test"', $lines);

    $this->AddArtifact(self::ParseArtifacts($lines));
    $this->AddMetric($this->ParseMetrics($lines));
    return $lines;
  }

  static function ParseArtifacts($lines) {
    return implode(PHP_EOL, $lines);
  }

  function ParseMetrics($lines) {
    $metrics = array();
    foreach ($lines as $line) {
      $matches = array();
      preg_match('/([0-9\.]*) duplica/', $line, $matches);
      if (isset($matches[1])) {
        $metrics['duplication'] = $matches[1];
      }
      $matches = array();
      preg_match('/([0-9]*) total/', $line, $matches);
      if (isset($matches[1])) {
        $metrics['checked'] = $matches[1];
      }
    }
    return $metrics;
  }
}
