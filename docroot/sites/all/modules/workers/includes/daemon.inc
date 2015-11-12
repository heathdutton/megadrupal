<?php
/**
 * @file
 */

function workers_daemon_start($name, $config) {
  if (!function_exists($config['callback'])) {
    throw new Exception('Worker callback not found');
  }

  if (isset($config['log_callback'])) {
    _workers_daemon_logger($config['log_callback']);
  }

  $pid_file = $config['pid_file'];

  if (file_exists($pid_file)) {
    $pid = intval(file_get_contents($pid_file));

    if (posix_kill($pid, SIG_DFL)) {
      _workers_daemon_log($name, 'Daemon already run');
      exit;
    }
    else {
      // Pid file exists, but there is no process.
      if (!unlink($pid_file)) {
        throw new Exception('Can not delete pidfile');
      }
    }
  }

  $pid = pcntl_fork();

  if ($pid == -1) {
    throw new Exception('Can not demonize');
  }
  elseif ($pid > 0) {
    // Killing the root process.
    exit;
  }

  _workers_daemon_log($name, 'Process demonized');

  // Write pid file.
  file_put_contents($pid_file, getmypid());

  // Make the current process a session leader.
  posix_setsid();

  declare(ticks=1);
  pcntl_signal(SIGTERM, '_workers_daemon_signal_handler');
  pcntl_signal(SIGQUIT, '_workers_daemon_signal_handler');

  $iteration = 0;
  while (!_workers_daemon_stop()) {
    sleep($config['sleep']);

    $iteration++;

    // Log each 5 iteration.
    if ($iteration == 1 || $iteration % 10 === 0) {
      _workers_daemon_log(
        $name,
        sprintf('Iteration #%d. Memory usage: %01.2f Mb', $iteration, memory_get_usage() / 1024 / 1024)
      );
    }

    $config['callback']($name);

    if (memory_get_usage() > $config['memory_limit']) {
      _workers_daemon_log($name, 'Memory limit reached');
    }
  }

  _workers_daemon_log($name, 'Daemon stopped!');

  // Delete pid file.
  unlink($pid_file);
}

function _workers_daemon_signal_handler($signal) {
  if (in_array($signal, array(SIGQUIT, SIGTERM))) {
    _workers_daemon_stop(TRUE);
  }
}

function _workers_daemon_stop($stop = FALSE) {
  static $stop_daemon = FALSE;

  if ($stop) {
    $stop_daemon = TRUE;
  }

  return $stop_daemon;
}

function _workers_daemon_logger($callback = NULL) {
  static $logger;

  if ($callback) {
    if (!function_exists($callback)) {
      throw new Exception('Log callback not found');
    }
    $logger = $callback;
  }

  return $logger;
}

function _workers_daemon_log($name, $message) {
  if ($logger = _workers_daemon_logger()) {
    /* @var string $logger */
    $logger($name, $message);
  }
}
