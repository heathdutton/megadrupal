<?php
/**
 * @file
 *  Shell client
 */

/**
 * Encapsulates the logic around running shell commands.
 */
class Shell {

  protected $cwd;
  protected $env;
  protected $stdout;
  protected $stderr;
  protected $exit;
  protected static $initial_env;

  public function __construct(array $options = array()) {
    $this->cwd = !empty($options['cwd']) ? $options['cwd'] : getcwd();
    $this->stdout = '';
    $this->stderr = '';
    $this->exit = 0;
    $this->getInitialEnv();
    $this->env = !empty($options['env']) ? $options['env'] : array();
  }

  /**
   * Provides the initial env
   * (the set of env variables that php itself was run with)
   *
   * @return array
   */
  public function getInitialEnv() {
    if (is_null(self::$initial_env)) {
      $envstr = $this->run('env');
      self::$initial_env = array();
      if (preg_match_all('|(.+)=(.+)|', $envstr, $matches)) {
        $keys = array_map('trim', $matches[1]);
        $values = array_map('trim', $matches[2]);
        self::$initial_env = array_combine($keys, $values);
      }
      self::$initial_env = array_merge(self::$initial_env, variable_get('shell_client_initial_env', array()));
      drupal_alter('shell_client_initial_env', self::$initial_env);
    }
    return self::$initial_env;
  }

  /**
   * Provides the current environment merged with the initial env,
   * or NULL if the initial env has not yet been initialized.
   *
   * @return array
   */
  public function getEnv() {
    if (!is_array(self::$initial_env)) {
      // getInitialEnv() calls run(), and run() calls getEnv()
      // for proc_open() in run() to return the current environment, the env param must be NULL (NOT an empty array)
      // therefore, getEnv() needs to return NULL if initial_env is not yet populated.
      return NULL;
    }
    return array_merge(self::$initial_env, $this->env);
  }

  /**
   * Execute a shell command
   *
   * This method will start a new process on the current machine and
   * run shell commands. Once the command has been run, the method will
   * return the command line output.
   *
   * @param  string $command Shell command to be run
   * @param  array $options
   * @param  array $args
   * @return string|bool Returns the command output or FALSE on failure
   */
  public function run($command, $options = array(), $args = array()) {
    $descriptors = array(
      0 => array("pipe", "r"),
      1 => array("pipe", "w"),
      2 => array("pipe", "w")
    );
    $prepared_command = $this->prepareCommand($command, $options, $args);
    $process = proc_open($prepared_command, $descriptors, $pipes, $this->cwd, $this->getEnv());

    if (!is_resource($process)) {
      throw new \RuntimeException('Unable to execute command: ' . $prepared_command);
    }

    $this->stderr = stream_get_contents($pipes[2]);
    $this->stdout = stream_get_contents($pipes[1]);

    fclose($pipes[2]);
    fclose($pipes[1]);
    fclose($pipes[0]);
    $this->exit = proc_close($process);

    if (!empty($this->stderr)) {
      $error_type = WATCHDOG_ERROR;
      if (0 === $this->exit) {
        $error_type = WATCHDOG_NOTICE;
      }
      watchdog('shell_client', 'Error when running command `' . $prepared_command . '`: ' . trim($this->stderr), NULL, $error_type);
    }

    return 0 === $this->exit ? $this->stdout : FALSE;
  }

  /**
   * Provides the stdout content from the last process executed.
   *
   * @return string
   */
  public function getStdout() {
    return $this->stdout;
  }

  /**
   * Provides the stderr content from the last process executed.
   *
   * @return string
   */
  public function getStderr() {
    return $this->stderr;
  }

  public function getError() {
    return $this->getStdout() . "\n" . $this->getStderr();
  }

  /**
   * Provides the exit code from the last process executed.
   *
   * @return int
   */
  public function getExit() {
    return $this->exit;
  }

  /**
   * Prepares a command for execution.
   *
   * @return string
   *  The prepared command.
   */
  public function prepareCommand($command, array $options = array(), array $args = array()) {
    $command_parts = array($command);

    $variables = array(
      'command' => $command,
      'options' => $options,
      'args'    => $args,
    );
    drupal_alter('shell_client_command', $variables);
    extract($variables);

    if (count($options) > 0) {
      $options_items = array();
      foreach ($options as $name => $value) {
        $options_item = $name;
        if (!is_null($value)) {
          $options_item .= ' ' . escapeshellarg($value);
        }
        $options_items[] = $options_item;
      }
      $command_parts[] = implode(' ', $options_items);
    }

    if (count($args) > 0) {
      $command_parts[] = implode(' ', array_map('escapeshellarg', $args));
    }

    return implode(' ', $command_parts);
  }
}
