<?php
/**
 * @file
 * Git backend implementation.
 */

/**
 * Git backend.
 */
class PHRepositoryGitBackend extends PHRepositoryBackend {
  protected $delegate;

  /**
   * Returns the Git repository URL.
   */
  public function getURL() {
    return isset($this->metadata->ph_repository_git_url) ? $this->metadata->ph_repository_git_url->value() : FALSE;
  }

  /**
   * Returns Git repository URL as summary.
   */
  public function getSummary() {
    if ($url = $this->getURL()) {
      return t('Repository URL: @url', array('@url' => $url));
    }
    else {
      return parent::getSummary();
    }
  }

  /**
   * Returns an instance of a Git delegate handler.
   */
  public function getDelegate() {
    if (!isset($this->delegate)) {
      $this->delegate = new PHRepositoryGitDelegate(drupal_realpath($this->repository->getWorkingDirectory()), $this->getURL());
    }
    return $this->delegate;
  }

  /**
   * Synchronizes the repository.
   *
   * @throws PHRepositoryBackendException
   *   If an error occurred in the backend.
   */
  public function synchronize() {
    $this->getDelegate()->update();
  }

  /**
   * Lists recognized versions in the repository, suitable for exporting.
   *
   * @return array
   *   An array of version arrays.
   *
   * @throws PHRepositoryBackendException
   *   If an error occurred in the backend.
   */
  public function listVersions() {
    $refs = $this->getDelegate()->listRefs();
    $versions = array();
    foreach ($refs as $ref) {
      if (ph_version_valid_format($ref, 'tag')) {
        $versions[] = ph_version_extract_info($ref);
      }
    }
    return $versions;
  }

  /**
   * Exports a version of a repository into the given directory.
   *
   * @param $path
   *   Path of the directory to export module contents into, i.e. not under a
   *   further subdirectory.
   * @param array $version
   *   Version to export.
   * @param array $versionSince
   *   Version to compare with when compiling details for a development version.
   * @return array
   *   Detailed version (i.e. with commit increment information) of the export.
   *
   * @throws PHRepositoryBackendException
   *   If an error occurred in the backend.
   */
  public function export($path, $version, $versionSince = NULL) {
    return $this->getDelegate()->export($path, $version, $versionSince);
  }
}

/**
 * Default Git delegate.
 */
class PHRepositoryGitDelegate {
  /**
   * Git repository directory path.
   * @var string
   */
  protected $directory;

  /**
   * Remote repository URL.
   * @var string
   */
  protected $remoteUrl;

  /**
   * Git binary path.
   * @var string
   */
  protected $gitBin;

  /**
   * Current working directory.
   * @var string
   */
  private $cwd;

  /**
   * Constructs a Git delegate.
   */
  public function __construct($directory, $remoteUrl) {
    $this->directory = $directory;
    $this->remoteUrl = $remoteUrl;
    $this->setUp();
  }

  /**
   * Sets up the internal Git handler.
   */
  protected function setUp() {
    $this->gitBin = ph_repository_git_find_executable();
    if (empty($this->gitBin)) {
      throw new RuntimeException('Path to Git binary is not executable.');
    }
    if (!is_dir($this->directory) || !is_writable($this->directory)) {
      throw new RuntimeException('Given path is not a writable directory.');
    }
    $this->init();
  }

  /**
   * Executes a Git command and returns an array of result lines.
   */
  protected function gitExec($command) {
    $args = func_get_args();
    $commandString = $this->gitBin . ' ' . implode(' ', $args);
    exec(escapeshellcmd($commandString), $resultArray, $exitStatus);
    if ($exitStatus) {
      throw new PHRepositoryGitDelegateException($commandString, implode(' ', $resultArray), $exitStatus);
    }
    else {
      return $resultArray;
    }
  }

  /**
   * Changes the working directory to the repository directory.
   */
  protected function changeDirectory() {
    if (isset($this->cwd)) {
      return;
    }
    $this->cwd = getcwd();
    chdir($this->directory);
  }

  /**
   * Restores the working directory to the previous directory.
   */
  protected function restoreDirectory() {
    if (isset($this->cwd)) {
      chdir($this->cwd);
      $this->cwd = NULL;
    }
  }

  /**
   * Initializes a Git repository directory.
   */
  protected function init() {
    $this->changeDirectory();

    if (count(scandir($this->directory)) > 2) {
      // Ensure non-empty directory is controlled by Git.
      $this->gitExec('status');
    }
    else {
      // Initialize empty directory.
      $this->gitExec('init');
    }

    // Set up Git repository URL.
    try {
      $result = $this->gitExec('config', '--get remote.origin.url');
      if (reset($result) != $this->remoteUrl) {
        $this->gitExec('remote', 'set-url', 'origin', escapeshellarg($this->remoteUrl));
      }
    }
    catch (PHRepositoryGitDelegateException $e) {
      $this->gitExec('remote', 'add', 'origin', escapeshellarg($this->remoteUrl));
    }

    $this->restoreDirectory();
  }

  /**
   * Fetches the repository and checks out in working directory.
   */
  public function update() {
    $this->changeDirectory();
    $this->gitExec('fetch', '--force', '--all', '--prune');
    $this->gitExec('fetch', '--force', '--tags');
    $this->restoreDirectory();
  }

  /**
   * Lists exportable references.
   */
  public function listRefs() {
    $this->changeDirectory();
    $refs = $this->gitExec('show-ref');
    $results = array();
    foreach ($refs as $ref) {
      @list(, $name) = explode(' ', $ref, 2);
      if (preg_match('`^refs/(?:tags|remotes/origin)/(.*)$`', $name, $match)) {
        $results[] = $match[1];
      }
    }
    $this->restoreDirectory();
    return $results;
  }

  /**
   * Exports a version into a directory.
   */
  public function export($path, $version, $since = NULL) {
    $version = ph_version_standardize($version);
    $tag = ph_version_format($version, 'tag');
    $this->changeDirectory();

    // Export.
    if (ph_version_is_dev($version)) {
      // Sync remote branch.
      $this->gitExec('branch', '-f', $tag, 'origin/' . $tag);
    }
    $this->gitExec('read-tree', $tag);
    $this->gitExec('checkout-index', '-a', '-f', '--prefix=' . $path . '/');

    // Refine branch version. If version since is empty, it means no detail is
    // available about existing releases and the version should be unaltered.
    if (ph_version_is_dev($version) && !empty($since) && is_array($since) && !ph_version_is_dev($since)) {
      $tagSince = ph_version_format($since, 'tag');
      if ($tagSince) {
        // Check branch head against the tag to refine version.
        $refHead = $this->gitExec('show-ref', '-s', '--heads', $tag);
        $refTag = $this->gitExec('show-ref', '-s', '--tags', $tagSince);
        $version = $since;
        // Match refs for zero increment.
        if ($refHead == $refTag) {
          $version['increment'] = 0;
        }
        // Count commits since tag for increment.
        else {
          $refsSince = $this->gitExec('rev-list', $tag, '^' . $tagSince);
          $version['increment'] = count($refsSince);
        }
      }
    }

    $this->restoreDirectory();

    return $version;
  }
}

/**
 * Git delegate exception.
 */
class PHRepositoryGitDelegateException extends PHRepositoryBackendException {
  protected $command;
  protected $output;

  public function __construct($command, $output, $code = 0) {
    parent::__construct('Command: ' . $command . "\nOutput: " . $output, $code);
  }
}
