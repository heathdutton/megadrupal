<?php
/**
 * @file
 * Contains Drupal\GitClone\GitClone.
 */

namespace Drupal\GitClone;

use Gitonomy\Git\Admin;
use Gitonomy\Git\Repository;

/**
 * Class GitClone.
 *
 * @package Drupal\GitClone
 *
 * @todo Most (if not all) the public properties should really be protected.
 * The way Entity API uses them (creation/saving) seems to be causing issues
 * with privatizing them (even with magic getter/setter methods). Research
 * more on this subject and change them later, if possible.
 */
class GitClone extends \Entity {

  /**************************************************************************
   * Public properties.
   **************************************************************************/

  /**
   * The name of the providing module if the entity has been defined in code.
   *
   * @var string
   */
  public $module;

  /**
   * The repository machine name identifier.
   *
   * @var string
   */
  public $name;

  /**
   * The set reference of the git clone.
   *
   * @var array
   */
  public $ref;

  /**
   * The set reference type of the git clone.
   *
   * @var string
   */
  public $refType;

  /**
   * All supported references for the repository.
   *
   * @var array
   */
  public $refs;

  /**
   * The repository object.
   *
   * @var \Gitonomy\Git\Repository
   */
  public $repository;

  /**
   * Settings for the entity.
   *
   * @var array
   */
  public $settings;

  /**
   * The exportable status of the entity.
   *
   * @var int
   */
  public $status;

  /**
   * The display title of this entity.
   *
   * @var string
   */
  public $title;

  /**
   * The public remote repository URL this git entity will use.
   *
   * @var string
   */
  public $url;

  /**************************************************************************
   * Protected properties.
   **************************************************************************/

  /**
   * Allowed reference types.
   *
   * @var array
   */
  protected static $allowedRefs = array('branch', 'tag');

  /**
   * Determines whether or not the entity has been initialized.
   *
   * @see GitClone::init()
   *
   * @var bool
   */
  protected $initialized;

  /**
   * Stores the output from commands that have ran.
   *
   * @var array
   */
  protected $output;

  /**************************************************************************
   * Sub-classed methods.
   **************************************************************************/

  /**
   * {@inheritdoc}
   */
  public function __sleep() {
    $vars = parent::__sleep();
    unset(
      $vars['initialized'],
      $vars['output'],
      $vars['rdf_mapping'],
      $vars['refs'],
      $vars['repository']
    );
    return $vars;
  }

  /**
   * {@inheritdoc}
   */
  public function save($queue = TRUE) {
    $this->init(TRUE);
    if ($queue) {
      $this->queue();
    }
    /** @var EntityController $controller */
    $controller = entity_get_controller($this->entityType);
    return $controller->save($this);
  }

  /**
   * {@inheritdoc}
   */
  public function setUp() {
    parent::setUp();
    $this->init();
  }

  /**************************************************************************
   * Public methods.
   **************************************************************************/

  /**
   * Processes a git clone entity from the queue.
   *
   * Do not use this method directly, use the GitClone::queue() method instead.
   *
   * @see GitClone::queue()
   * @see git_clone_cron_queue_info()
   * @see _git_clone_dequeue_callback()
   *
   * @throws \Exception
   *   Thrown when a git command has failed.
   */
  public function dequeue() {
    $tokens = array(
      '@id' => $this->identifier(),
    );

    // Check if this method was invoked as a result of using GitClone::queue().
    // This ensures that this method cannot be invoked directly. Do not throw
    // an exception as that will cause cron to re-queue something that wasn't
    // properly queued to begin with. Even if by some chance that this was a
    // valid queue item, not throwing an exception will allow it to be deleted
    // from the existing queue and then re-queued on the next cron run.
    if (!$this->isQueued()) {
      watchdog('git_clone', '@id: entity does not exist in queue, aborting dequeue. Use the \Drupal\GitClone\GitClone::queue() method to queue an entity.', $tokens);
      return;
    }

    $error = FALSE;
    if (!$this->reset()) {
      $error = '@id: Unable to reset the working tree.';
    }
    elseif (!$this->fetch()) {
      $error = '@id: Unable to fetch remote.';
    }
    elseif (!$this->checkout()) {
      $error = '@id: Unable to checkout reference.';
    }
    elseif (!$this->merge()) {
      $error = '@id: Unable to merge origin into local working tree.';
    }

    // Retrieve the output of all the commands that ran.
    $output = $this->getOutput();

    // Handle any errors and throw an exception so it can re-queue.
    if ($error) {
      watchdog('git_clone', $error . $output, $tokens, WATCHDOG_ERROR);
      throw new \Exception(format_string($error . ' See logs for more details.', $tokens));
    }

    // Update the last fetched time using time() instead of REQUEST_TIME since
    // this process can take much longer than when the initial request started.
    $this->lastFetched(time());

    // Log a successful dequeue.
    watchdog('git_clone', '@id: Successfully fetched and dequeued' . $output, $tokens);
  }

  /**
   * Retrieves the output from commands that have ran.
   *
   * @param bool $reset
   *   Toggle determining whether or not to remove the stored output returned.
   *
   * @return string
   *   The output of ran commands.
   */
  public function getOutput($reset = TRUE) {
    $output = '';
    if (isset($this->output)) {
      $output = "\n<br/><br/><pre><code>" . implode("\n", $this->output) . "</code></pre>";
      if ($reset) {
        unset($this->output);
      }
    }
    return $output;
  }

  /**
   * Returns the git clone file system directory path.
   *
   * @param bool $create
   *   Toggle determining whether or not to create the directory if it does not
   *   exist.
   * @param bool $absolute
   *   Toggle determining whether or not to return the entire system path. If
   *   FALSE, it will be prefixed with the gitclone:// stream wrapper.
   *
   * @return string|FALSE
   *   The git clone path or FALSE on error.
   */
  public function getPath($create = TRUE, $absolute = TRUE) {
    if (empty($this->refType) || empty($this->ref) || empty($this->name)) {
      return FALSE;
    }
    $path = "gitclone://$this->refType/$this->name";
    if ($create) {
      if (!is_dir($path) && !drupal_mkdir($path, NULL, TRUE)) {
        drupal_set_message(t('The directory %directory does not exist and could not be created.', array('%directory' => $path)), 'error');
        return FALSE;
      }
      if (is_dir($path) && !is_writable($path) && !drupal_chmod($path)) {
        drupal_set_message(t('The directory %directory exists but is not writable and could not be made writable.', array('%directory' => $path)), 'error');
        return FALSE;
      }
    }
    else {
      if (!is_dir($path)) {
        return FALSE;
      }
      if (!drupal_is_cli() && is_dir($path) && !is_writable($path) && !drupal_chmod($path)) {
        drupal_set_message(t('The directory %directory exists but is not writable and could not be made writable.', array('%directory' => $path)), 'error');
      }
    }
    if ($absolute) {
      return drupal_realpath($path);
    }
    return $path;
  }

  /**
   * Retrieves a repository's remote references, parsing them if necessary.
   *
   * @param bool $force
   *   Toggle determining whether or not to force a remote fetch.
   * @param bool $reset
   *   Toggle determining whether to reset exist cached data.
   *
   * @return bool
   *   TRUE if there are remote references, FALSE on failure.
   */
  public function getRefs($force = FALSE, $reset = FALSE) {
    // Immediately return if already set.
    if (!isset($this->is_new) && !$force && !empty($this->refs)) {
      return TRUE;
    }

    // Set defaults so they can be used elsewhere in code.
    $this->refs = array(
      'branch' => array(),
      'tag' => array(),
    );

    // Immediately return if URL is not yet set.
    if (!$this->url) {
      return FALSE;
    }

    // Load cached references for this URL.
    $cid = 'gitclone:refs:' . drupal_hash_base64($this->url);
    if (!$reset && ($cache = cache_get($cid)) && isset($cache->data)) {
      $this->refs = $cache->data;
      return TRUE;
    }

    // No cache found, attempt to retrieve the remote references.
    $output = $this->run('ls-remote', array($this->url), TRUE);

    // Command failed execution, (e.g. invalid URL).
    if ($output === FALSE) {
      return FALSE;
    }

    // Extract the references from the output.
    $default = NULL;
    foreach (array_filter(explode("\n", $output)) as $lines) {
      list($hash, $ref) = explode("\t", $lines);
      if ($ref === 'HEAD') {
        $default = $hash;
        continue;
      }
      $parts = explode('/', $ref);
      if (isset($parts[1]) && isset($parts[2])) {
        $type = $parts[1];
        if ($type === 'heads') {
          $type = 'branch';
        }
        elseif ($type === 'tags') {
          $type = 'tag';
        }
        $name = $parts[2];
        if ($default === $hash && is_array($this->settings)) {
          $this->settings['default'] = $name;
        }
        $this->refs[$type][$name] = array(
          'name' => $name,
          'sha' => $hash,
          'type' => $type,
        );
      }
    }

    // Cache the references.
    cache_set($cid, $this->refs, 'cache');

    return TRUE;
  }

  /**
   * Retrieves the git clone settings.
   *
   * @return array|string
   *   The settings array or serialized string (depending on the original state
   *   it was in).
   */
  public function getSettings() {
    $settings = array();
    if (!isset($this->settings)) {
      $this->settings = array();
    }

    // Since the settings property can be both an array or a serialized string
    // (depending on the current point in the Entity API process), the
    // serialized string must be checked for, converted to and array, merged
    // with defaults and converted back to a serialized string.
    $serialized = FALSE;
    if (is_string($this->settings)) {
      if ($settings = unserialize($this->settings)) {
        $serialized = TRUE;
      }
      else {
        $settings = array();
      }
    }
    elseif (is_array($this->settings)) {
      $settings = $this->settings;
    }

    // Merge in default settings.
    $defaults = array(
      'default' => NULL,
      'fetch_threshold' => 3600,
    );
    $settings = drupal_array_merge_deep($defaults, $settings);

    // Return the correct format.
    return $this->settings = ($serialized ? serialize($settings) : $settings);
  }

  /**
   * Fully initializes the git clone entity after it has been created or loaded.
   *
   * @param bool $force
   *   Toggle determining whether or not to force a reinitialization.
   *
   * @return GitClone
   *   The current GitClone entity instance.
   *
   * @see GitClone::save()
   * @see EntityController::load()
   *
   * @chainable
   */
  public function init($force = FALSE) {
    if (!$force && isset($this->initialized)) {
      return $this;
    }
    $this->initialized = TRUE;

    // Ensure a Gitonomy repository is instantiated.
    if (!$force && !isset($this->repository)) {
      $options = _git_clone_gitonomy_options();
      if (($path = $this->getPath(FALSE)) && !empty($this->url)) {
        $git_exists = file_exists("$path/.git");
        if (!$git_exists && in_array($this->refType, self::$allowedRefs)) {
          try {
            $this->repository = Admin::init($path, FALSE, $options);
            $this->run('remote', array('add', 'origin', $this->url));
          }
          catch (\Exception $e) {
            drupal_set_message($e->getMessage(), 'error');
          }
        }
        elseif ($git_exists && in_array($this->refType, self::$allowedRefs)) {
          $this->repository = new Repository($path, $options);
        }
      }
      else {
        $temp_dir = 'temporary://git_clone-' . drupal_hash_base64(REQUEST_TIME);
        if (file_prepare_directory($temp_dir, FILE_CREATE_DIRECTORY | FILE_MODIFY_PERMISSIONS)) {
          $temp_dir = drupal_realpath($temp_dir);
          drupal_register_shutdown_function(function () use ($temp_dir) {
            file_unmanaged_delete_recursive($temp_dir);
          });
          try {
            $this->repository = Admin::init($temp_dir, FALSE, $options);
          }
          catch (\Exception $e) {
            watchdog('git_clone', $e->getMessage(), WATCHDOG_ERROR);
          }
        }
        if (!$this->repository) {
          drupal_set_message(t('Unable to create temporary git clone repository: @directory. Please verify your system temporary directory is writable.', array(
            '@directory' => $temp_dir,
          )), 'error');
        }
      }
    }

    // Initialize the settings.
    $this->getSettings();

    // Initialize the refs.
    $this->getRefs($force);

    return $this;
  }

  /**
   * Determines whether or not the git clone has expired.
   *
   * This is based on the fetch threshold setting.
   *
   * @return bool
   *   TRUE or FALSE
   */
  public function isExpired() {
    if ($last_fetch = $this->lastFetched()) {
      // Use time() instead of REQUEST_TIME since clones can take longer.
      return (time() - $last_fetch) >= $this->settings['fetch_threshold'];
    }
    return TRUE;
  }

  /**
   * Determines whether or not a git clone is currently queued.
   *
   * @return bool
   *   TRUE or FALSE
   *
   * @see GitClone::queue()
   */
  public function isQueued() {
    $queued = array();
    /** @var \SelectQuery $query */
    $query = db_select('queue', 'q')->fields('q')->condition('name', 'git_clone');
    foreach ($query->execute()->fetchAllAssoc('item_id') as $item) {
      /** @var GitClone $clone */
      if ($clone = unserialize($item->data)) {
        $queued[] = $clone->identifier();
      }
    }
    return in_array($this->identifier(), $queued);
  }

  /**
   * Retrieves or sets the last time a git clone was fetched.
   *
   * @param int $timestamp
   *   The timestamp to set. If set to TRUE, the current time() will be used.
   *
   * @return int
   *   The timestamp of the last fetch or 0 if no timestamp has been recorded.
   */
  public function lastFetched($timestamp = NULL) {
    $cid = 'gitclone:last_fetched';
    $last_fetched = array();

    // Retrieve cached last fetched data.
    if (($cache = cache_get($cid)) && isset($cache->data)) {
      $last_fetched = $cache->data;
    }

    // Set the timestamp.
    if ($timestamp) {
      $last_fetched[$this->name] = $timestamp;
      cache_set($cid, $last_fetched);
    }
    elseif (!isset($last_fetched[$this->name])) {
      $last_fetched[$this->name] = 0;
    }

    return $last_fetched[$this->name];
  }

  /**
   * Queues a git clone entity.
   *
   * @return GitClone
   *   The current GitClone entity instance.
   *
   * @see git_clone_cron_queue_info()
   * @see _git_clone_dequeue_callback()
   * @see GitClone::dequeue()
   *
   * @chainable
   */
  public function queue() {
    // Only continue if not already queued and fetch threshold has expired.
    if (!$this->isQueued() && $this->isExpired()) {
      /** @var \DrupalQueueInterface $queue */
      $queue = \DrupalQueue::get(GIT_CLONE_QUEUE, TRUE);
      $queue->createQueue();
      $queue->createItem($this);
      drupal_set_message(t('Queued the "@label" git clone to fetch on next cron run.', array(
        '@label' => $this->label(),
      )));
    }
    return $this;
  }

  /**************************************************************************
   * Protected methods.
   **************************************************************************/

  /**
   * Checks out the reference currently assigned to the GitClone entity.
   *
   * @return GitClone
   *   The current GitClone entity instance.
   *
   * @chainable
   */
  protected function checkout() {
    $args = array('-B', $this->ref);
    if ($this->refType === 'branch') {
      $args[] = '--track';
      $args[] = "origin/$this->ref";
    }
    elseif ($this->refType === 'tag') {
      $args[] = "tags/$this->ref";
    }
    return $this->run('checkout', $args);
  }

  /**
   * Cleans the repository of any un-tracked files.
   *
   * Forcibly removes all un-tracked empty directories and ignored files.
   *
   * @return GitClone
   *   The current GitClone entity instance.
   *
   * @chainable
   */
  protected function clean() {
    return $this->run('clean', array('-dfx'));
  }

  /**
   * Fetches all remote references.
   *
   * @return GitClone
   *   The current GitClone entity instance.
   *
   * @chainable
   */
  protected function fetch() {
    $args = array('--all', '--force', '--prune');
    if ($this->refType === 'tag') {
      $args[] = '--tags';
    }
    return $this->run('fetch', $args);
  }


  /**
   * Logs the output of a command that has ran.
   *
   * @param string $command
   *   Git command that was ran (checkout, branch, tag).
   * @param array $args
   *   Arguments of git command.
   * @param mixed $output
   *   The output from the command.
   */
  protected function log($command, array $args = array(), $output = NULL) {
    static $binary;
    if (!isset($binary)) {
      $binary = _git_clone_binary_path();
    }
    if (!isset($this->output)) {
      $this->output = array();
    }
    $this->output[] = format_string("@binary @command @args\n!output", array(
      '@binary' => $binary,
      '@command' => $command,
      '@args' => implode(' ', $args),
      '!output' => $output,
    ));
  }

  /**
   * Merges the remote origin reference into the current working tree.
   *
   * @return GitClone
   *   The current GitClone entity instance.
   *
   * @chainable
   */
  protected function merge() {
    // Only pull down references that can change (e.g. branches).
    if ($this->refType === 'branch') {
      return $this->run('merge', array("origin/$this->ref"));
    }
    return $this;
  }

  /**
   * Resets the current working tree.
   *
   * @param bool $hard
   *   Toggle determining whether or not to pass the --hard option to git. This
   *   will reset HEAD, index and working tree.
   * @param bool $clean
   *   Toggle determining whether or not to also clean the repository of any
   *   un-tracked files.
   *
   * @return GitClone
   *   The current GitClone entity instance.
   *
   * @chainable
   */
  protected function reset($hard = TRUE, $clean = TRUE) {
    $this->run('reset', array('--hard'));
    if ($clean) {
      $this->clean();
    }
    return $this;
  }

  /**
   * Runs git commands on the repository.
   *
   * This is a wrapper around \Gitonomy\Git\Repository::run(). The GitClone
   * entity must catch whether or not the repository has actually been
   * initialized and any errors produced from the command executed.
   *
   * @param string $command
   *   Git command to run (checkout, branch, tag).
   * @param array $args
   *   Arguments of git command.
   * @param bool $return_output
   *   Toggle determining whether or not to return the output from $command.
   *
   * @return FALSE|string|GitClone
   *   Anytime there is an error, the return value will always be FALSE.
   *   If $return_output is set, the output of $command is returned.
   *   Otherwise, the current GitClone instance is returned for chainability.
   *
   * @see \Gitonomy\Git\Repository::run()
   *
   * @chainable
   */
  protected function run($command, array $args = array(), $return_output = FALSE) {
    $return = $return_output ? '' : $this;
    $command_hook = _git_clone_hook_name($command);
    $context = array('output' => $return_output);
    drupal_alter('git_clone_pre_' . $command_hook, $args, $this, $context);
    try {
      $output = $this->repository->run($command, $args);
      $this->log($command, $args, $output);
      drupal_alter('git_clone_post_' . $command_hook, $output, $this, $context);
      if ($return_output) {
        $return = $output;
      }
    }
    catch (\Exception $e) {
      watchdog('git_clone', $e->getMessage(), array(), WATCHDOG_ERROR);
      $return = FALSE;
    }
    return $return;
  }

}
