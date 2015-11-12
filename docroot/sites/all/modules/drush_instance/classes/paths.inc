<?php

/**
 * @file
 * Drush engine classes/paths, for handling non-Drupal file paths.
 */

/**
 * Helper class, to handle all paths.
 */
class DrushInstancePaths {

  /**
   * Implements DrushInstancePaths::__construct().
   *
   * Register site alias config, paths and whether it's a rebuild.
   *
   * @param array $alias
   *   alias config.
   * @param array $paths
   *   path config, keyed by path.
   * @param bool $rebuild
   *   whether this is a rebuild (or an initial build.)
   */
  public function __construct(array $alias, array $paths, bool $rebuild) {
    $this->alias = $alias;
    $this->paths = $paths;
    $this->rebuild = $rebuild;
  }

  /**
   * Handle all paths registered.
   */
  public function handleAllPaths() {
    $success = TRUE;
    foreach($this->paths as $path => $config) {
      // Specific handler based on type, or 'rebuild' if that's appropriate
      $type = $this->rebuild ? 'rebuild' : $config['source']['type'];

      // Execute handler.
      $success = $success && $this->handlePath($path, $config, $type);
      if (!$success) {
        drush_log("Could not move into new build: $path", "warning");
        break;
      }
      // Change/set permissions?
      if (array_key_exists("permissions", $config)) {
        $success = $success && _drush_instance_perm_path($this->alias, $path, $config);
      }
      if (!$success) {
        drush_log("Could not set permissions: $path", "warning");
        break;
      }
    }
    return $success;
  }

  /**
   * Handle one path using a subsidiary class.
   */
  public function handlePath(string $path, array $config, $type) {
    $handler_class = "DrushInstancePath$type";
    if (!class_exists($handler_class)) {
      drush_set_error("No handler exists for preserving paths of type=$type");
      return FALSE;
    }
    $handler = new $handler_class($this->alias, $path, $config, $this);
    return $handler->handle();
  }
}

/**
 * Base path handler.
 */
abstract class DrushInstancePathAbstract {

  // Which derived classes require a source e.g. copy, rsync.
  public $requiresFrom = TRUE;
  public $from = NULL;
  public $fromAbsolute = NULL;

  /**
   * Implements DrushInstancePathAbstract::__construct().
   *
   * Register the site alias, and the single path and its configuration.
   * Attempt to make all directories down to this target.
   */
  public function __construct(array $alias, string $path, array $config, DrushInstancePaths $pathsHandler) {
    // Key injected configuration.
    $this->alias = $alias;
    $this->path = $path;
    $this->config = $config;
    $this->pathsHandler = $pathsHandler;

    // Derived configuration begins here.
    // Absolute path to this target file.
    $this->fullPath = $alias['root'] . "/$path";

    // Get our type from our classname.
    $this->type = $this->typeFromClass();

    // Ensure we have a source if we need one.
    if ($this->requiresFrom) {
      if (!isset($this->config['source']['from'])) {
        throw new Exception("Path type={$this->type} needs 'source':'from'.");
      }
      // Set from and from-absolute paths.
      $this->from = $this->config['source']['from'];
      $this->fromAbsolute = $this->from;
      // If absolute path already begins "/", don't add root.
      if (strpos($this->fromAbsolute, "/") !== 0) {
        $this->fromAbsolute = $this->alias['root'] . "/" . $this->from;
      }
    }

    // Ensure all directories above our target exist.
    if(!$this->buildContainingDir($this->fullPath)) {
      throw new Exception("Could not create all subdirectories required for {$this->type}:$path");
    }
  }

  /**
   * Helper method to create directory hierarchy containing new path.
   */
  private function buildContainingDir($full_new_path) {
    // Check all directories on new path exist, not including the last bit
    $check_path = "/";
    foreach(split("/", $full_new_path) as $new_path_part) {
      if(!file_exists($check_path) && !drush_mkdir($check_path)) {
        return FALSE;
      }
      $check_path .= "/$new_path_part";
    }
    return TRUE;
  }

  /**
   * Helper method to derive the type of handler from its class name.
   */
  private function typeFromClass() {
    return strtolower(str_replace("DrushInstancePath", "", get_class($this)));
  }
}

/**
 * Extension path handler interface.
 */
interface DrushInstancePathInterface {
  public function handle();
}

/**
 * Path handler for touch: create empty files.
 */
class DrushInstancePathTouch extends DrushInstancePathAbstract implements DrushInstancePathInterface {

  // Touching a file doesn't require a source.
  public $requiresFrom = FALSE;

  /**
   * Implements DrushInstancePathInterface::handle().
   */
  public function handle() {
    return (@file_put_contents($this->fullPath, '') !== FALSE);
  }
}

/**
 * Path handler for mkdir: create empty directories.
 *
 * Note other handlers will create directory hierarchies if required.
 */
class DrushInstancePathMkdir extends DrushInstancePathAbstract implements DrushInstancePathInterface {

  // Making a directory doesn't require a source.
  public $requiresFrom = FALSE;

  /**
   * Implements DrushInstancePathInterface::handle().
   */
  public function handle() {
    return @drush_mkdir($this->fullPath);
  }
}

/**
 * Path handler for copy: copy a file from elsewhere in e.g. sites/default.
 */
class DrushInstancePathCopy extends DrushInstancePathAbstract implements DrushInstancePathInterface {

  /**
   * Implements DrushInstancePathInterface::handle().
   */
  public function handle() {
    // Copy using copy() or drush's custom command, depending on whether or not
    // the source is a file or directory
    if (is_dir($this->fromAbsolute)) {
      return drush_copy_dir($this->fromAbsolute, $this->fullPath);
    }
    return copy($this->fromAbsolute, $this->fullPath);
  }
}

/**
 * Path handler for rsync: transfer an entire directory hierarchy from external source.
 */
class DrushInstancePathRsync extends DrushInstancePathAbstract implements DrushInstancePathInterface {

  /**
   * Implements DrushInstancePathInterface::handle().
   */
  public function handle() {
    $additional_options = $this->config['source'];
    unset($additional_options['type']);
    unset($additional_options['from']);

    // Run rsync
    drush_command_include('core-rsync');
    include_once(dirname(__FILE__) . "/../forked.rsync.inc");
    // Swap out DRUSH_COMMAND_ARGS context
    // Note that it might also need to swap out other contexts in future for
    // e.g. default arguments
    $context =& drush_get_context();
    $cached_args = $context['DRUSH_COMMAND_ARGS'];
    $context['DRUSH_COMMAND_ARGS'] = array();
    // Ensure the target directory exists
    @drush_mkdir($this->fullPath);
    // Appending "/" to both paths ensures directories get rsynced, not
    // a new subdirectory created locally
    $return_value = _instance_drush_core_rsync($this->from . "/", $this->fullPath . "/", $additional_options);

    // Swap context back in and return
    $context['DRUSH_COMMAND_ARGS'] = $cached_args;
    return $return_value;
  }
}

/**
 * Path handler for symlink: make symbolic link from elsewhere on filesystem.
 */
class DrushInstancePathSymlink extends DrushInstancePathAbstract implements DrushInstancePathInterface {

  /**
   * Implements DrushInstancePathInterface::handle().
   */
  public function handle() {
    return symlink($this->fromAbsolute, $this->fullPath);
  }
}

/**
 * Path handler for rebuild: just preserve if requested.
 */
class DrushInstancePathRebuild extends DrushInstancePathAbstract implements DrushInstancePathInterface {

  /**
   * Implements ::__construct().
   *
   * Override core constructor: rebuild needs a "from" path, but it's always
   * automatically derived from the actual path, using the old directory.
   */
  public function __construct(array $alias, string $path, array $config, DrushInstancePaths $pathsHandler) {
    // Determine "from" based on location of old build.
    $this->from = $this->fromAbsolute = $alias['_old_dir'] . "/$path";

    // We no longer require a "from" as far as our parent constructor cares.
    $this->requiresFrom = FALSE;
    parent::__construct($alias, $path, $config, $pathsHandler);
  }

  /**
   * Implements DrushInstancePathInterface::handle().
   */
  public function handle() {
    // Preservation on rebuild is now a tricky thing.
    // Switch/case statements cast booleans, so we have to itemise them.
    $action = $this->config['preserve_on_rebuild'];
    // FALSE/'discard' - discard file and don't rebuild.
    if ($action === FALSE) {
      drush_log("Boolean FALSE preserve_on_rebuild is deprecated: use 'discard' instead.", "warning");
      return TRUE;
    }
    if ($action === "discard" || $action === NULL) {
      return TRUE;
    }

    // 'source' - re-source this from the original source location.
    if ($action === "source") {
      return $this->pathsHandler->handlePath($this->path, $this->config, $this->config['source']['type']);
    }

    // Anything else, just continue (in future we could throw an exception.
    // But do warn against using booleans.
    if ($action === TRUE) {
      drush_log("Boolean TRUE preserve_on_rebuild is deprecated: use 'keep' instead.", "warning");
    }

    // Again, to avoid replicating large files, we should use rename() -
    // drush_move_dir() actually copies the contents
    $ret = @drush_op('rename', $this->fromAbsolute, $this->fullPath);
    if (!$ret) {
      drush_log("Could not move {$this->path} to new build", "warning");
    }
    else {
      drush_log("Moved {$this->path} to new build", "ok");
    }
    return $ret;
  }
}
