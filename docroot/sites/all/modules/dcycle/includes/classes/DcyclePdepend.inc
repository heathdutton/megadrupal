<?php

class DcyclePdepend extends DcycleCheck {
  function _Preflight_() {
    $errors = array();
    if (!DcycleEnvironment::ExecAvailable()) {
      $errors[] = t('The exec() function is not available');
    }
    $error = NULL;
    DcycleEnvironment::CommandAvailable('pdepend', $error);
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
    // see https://www.drupal.org/node/2316653
    DcycleEnvironment::RunCommand('pdepend', '--summary-xml=sites/default/files/summary.xml --suffix=module,test,inc,install sites/all/modules/custom/
', $lines);
    $this->AddArtifact(self::ParseArtifacts($lines));
    $this->AddMetric(self::ParseMetrics($lines));
    return $lines;
  }

  static function ParseArtifacts($lines) {
    return implode(PHP_EOL, $lines);
  }

  static function ParseMetrics($lines) {
    $metrics = array();
    try {
      $summary = self::loadXML('public://summary.xml');
      $attributes = $summary->attributes();
      $selected = array(
        'ccn' => 'Cyclomatic complexity',
        'cloc' => 'Comment lines to code',
        'clsa' => 'Number of abstract classes',
        'clsc' => 'Number of concrete classes',
        'lloc' => 'Lines of code',
        'noc' => 'Number of classes',
        'nof' => 'Number of functions',
        'nom' => 'Number of methods',
      );
      foreach ($selected as $abbreviation => $description) {
        $metrics[$description . ' (' . $abbreviation . ')'] = $attributes[$abbreviation];
      }
    }
    catch (Exception $e) {
      $this->AddError($e->getMessage());
    }
    return $metrics;
  }
}
