<?php
/**
 * @file
 * Repository entity implementation.
 */

/**
 * Repository entity.
 */
class PHRepository extends Entity {
  /**
   * Repository type information.
   * @var array
   */
  protected $typeInfo;

  /**
   * Repository backend.
   * @var PHRepositoryBackendInterface
   */
  protected $backend;

  /**
   * Repository ID.
   * @var int
   */
  public $rid;

  /**
   * Repository type.
   * @var string
   */
  public $type;

  /**
   * Path to local working directory for the repository backend.
   * @var string
   */
  protected $workingDirectory;

  /**
   * Sets up a repository.
   */
  protected function setUp() {
    parent::setUp();

    // Set up repository type information.
    $this->typeInfo = ph_repository_get_type($this->type);
  }

  /**
   * Assembles a path to the working directory.
   */
  public function getWorkingDirectory() {
    if (!$this->rid) {
      throw new PHRepositoryException('New repository has not been saved.');
    }
    elseif (!isset($this->workingDirectory)) {
      $directory = ph_repository_working_directory() . '/' . $this->type . '/' . $this->rid;
      if (file_prepare_directory($directory, FILE_CREATE_DIRECTORY)) {
        $this->workingDirectory = $directory;
      }
      else {
        throw new PHRepositoryException('Working directory cannot be properly prepared.');
      }
    }
    return $this->workingDirectory;
  }

  /**
   * Returns the repository backend.
   */
  public function backend() {
    if (!isset($this->backend)) {
      // Set up backend.
      $entityInfo = $this->entityInfo();
      $bundleInfo = $entityInfo['bundles'][$this->bundle()];
      if (!empty($bundleInfo['backend class'])) {
        $this->backend = new $bundleInfo['backend class']($this);
      }
    }
    return $this->backend;
  }

  /**
   * Removes transient properties from serialization.
   */
  public function __sleep() {
    $vars = parent::__sleep();

    // Do not serialize runtime information.
    unset($vars['typeInfo'], $vars['backend'], $vars['workingDirectory']);

    return $vars;
  }
}

/**
 * Interface of a backend.
 */
interface PHRepositoryBackendInterface {
  /**
   * Returns a textual summary of the repository given this backend.
   */
  public function getSummary();

  /**
   * Synchronizes the repository.
   *
   * @throws PHRepositoryBackendException
   *   If an error occurred in the backend.
   */
  public function synchronize();

  /**
   * Lists recognized versions in the repository, suitable for exporting.
   *
   * @return array
   *   An array of version arrays.
   *
   * @throws PHRepositoryBackendException
   *   If an error occurred in the backend.
   */
  public function listVersions();

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
   *   This argument is ignored if $version is not development.
   * @return array
   *   Detailed version (i.e. with commit increment information) of the export.
   *
   * @throws PHRepositoryBackendException
   *   If an error occurred in the backend.
   */
  public function export($path, $version, $versionSince = NULL);
}

/**
 * Base class for a repository backend.
 */
abstract class PHRepositoryBackend implements PHRepositoryBackendInterface {
  /**
   * Associated repository entity.
   * @var PHRepository
   */
  protected $repository;

  /**
   * Repository metadata wrapper.
   * @var EntityDrupalWrapper
   */
  protected $metadata;

  /**
   * Constructs a backend.
   */
  public function __construct($repository) {
    $this->repository = $repository;
    $this->setUp();
  }

  /**
   * Sets up the controller.
   */
  protected function setUp() {
    $this->metadata = entity_metadata_wrapper('ph_repository', $this->repository);
  }

  /**
   * Returns a textual summary of the repository given this backend.
   */
  public function getSummary() {
    return t('ID: @id', array('@id' => $this->repository->rid));
  }

  /**
   * Synchronizes the repository.
   *
   * @throws PHRepositoryBackendException
   *   If an error occurred in the backend.
   */
  public function synchronize() {
    // Do nothing.
  }
}

/**
 * Exception thrown by a repository.
 */
class PHRepositoryException extends Exception {}

/**
 * Exception thrown by a repository backend handler.
 */
class PHRepositoryBackendException extends PHRepositoryException {}
