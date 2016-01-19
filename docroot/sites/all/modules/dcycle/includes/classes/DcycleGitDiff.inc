<?php

class DcycleGitDiff extends DcycleCheck {
  function _Preflight_() {
    $errors = array();
    if (!DcycleEnvironment::ExecAvailable()) {
      $errors[] = t('The exec() function is not available, but required to fetch the version.');
    }
    else {
      try {
        $is_git_repo = DcycleGit::IsGitRepo();
      }
      catch(Exception $e) {
        $errors[] = t('Could not determine if current repo is a git repo (%reason)', array('%reason' => $e->GetMessage()));
      }
      if (!$is_git_repo) {
        $errors[] = t('Drupal root is not a git repo; version is unavailable.');
      }
    }
    if (count($errors)) {
      $this->AddError($errors);
      return FALSE;
    }
    return TRUE;
  }

  function FindPreviousCommit() {
    $return = file('public://ci/artifacts/DcycleGitDiff/DcycleGitDiff');
    return $return;
  }

  /**
   * The actual code to run this check.
   */
  function _Run_() {
    $this->previous_commit = $this->FindPreviousCommit();
    $lines = array(DcycleGit::LatestVersion());
    if (!$this->previous_commit) {
      $lines[] = 'This is the first commit being tracked';
    }
    else {
      $last_commit = $this->previous_commit[0];
      $extra_lines = array();
      $command = 'diff ' . $this->previous_commit[0] . ' ' . DcycleGit::LatestVersion();
      DcycleEnvironment::RunCommand('git', $command, $extra_lines);
      $lines += $extra_lines;
    }
    $this->AddArtifact(self::ParseArtifacts($lines));
    $this->AddMetric($this->ParseMetrics($lines));
    return $lines;
  }

  static function ParseArtifacts($lines) {
    return implode(PHP_EOL, $lines);
  }

  function ParseMetrics($lines) {
    if ($lines[1] == 'This is the first commit being tracked') {
      return array('size' => 0);
    }
    else {
      return array('size' => count($lines) - 1);
    }
  }
}
