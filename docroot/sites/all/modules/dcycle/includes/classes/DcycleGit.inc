<?php

class DcycleGit {
  /**
   * Same as DcycleGit::LatestVersionStatus() but does not throw an Exception.
   *
   * @return
   *   The version of the site in a human-readable form or a message stating
   *   why the version cannot be retrieved.
   */
  static function DisplayVersion() {
    if (!DcycleEnvironment::ExecAvailable()) {
      return t('The exec() function is not available, but required to fetch the version.');
    }
    else {
      try {
        $is_git_repo = self::IsGitRepo();
      }
      catch(Exception $e) {
        return t('Could not determine if current repo is a git repo (%reason)', array('%reason' => $e->GetMessage()));
      }
      if (!$is_git_repo) {
        return t('Drupal root is not a git repo; version is unavailable.');
      }
      else {
        return self::LatestVersionStatus();
      }
    }
  }

  /**
   * @throws
   *   Exception
   */
  static function IsGitRepo() {
    return DcycleEnvironment::Exec('git status');
  }

  /**
   * @throws
   *   Exception
   */
  static function LatestVersion() {
    return self::LatestVersionInfo('%h');
  }

  /**
   * @throws
   *   Exception
   */
  static function LatestVersionStatus() {
    return t('Version') . ' ' . self::LatestVersion() . ' (' . self::LatestVersionDate() . ') ' . (self::IsDirty() ? t('with uncommitted changes') : ('clean'));
  }

  /**
   * @throws
   *   Exception
   */
  static function IsDirty() {
    return DcycleEnvironment::Exec('git diff-index HEAD');
  }

  /**
   * @throws
   *   Exception
   */
  static function LatestVersionDate() {
    return self::LatestVersionInfo('%ai');
  }

  /**
   * @param $placeholder
   *   See http://opensource.apple.com/source/Git/Git-19/src/git-htmldocs/pretty-formats.txt
   *   Please include the % sign, for example '%h', '%ai'...
   *   No validation is done
   *
   * @throws
   *   Exception
   */
  static function LatestVersionInfo($placeholder) {
    return DcycleEnvironment::Exec("git log --pretty='format:'$placeholder -n1");
  }
}
