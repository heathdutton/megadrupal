<?php

function paas_metric_serverinfo_call($value) {
  switch ($value) {
    case 'os':
      echo strstr(__FILE__, ':/') ? 'win' : 'unix';
      break;
    case 'server':
      echo $_SERVER['SERVER_SOFTWARE'];
      break;
    case 'mods':
      $commands = array();
      if (stristr($_SERVER['SERVER_SOFTWARE'], 'apache')) {
        $commands[] = 'apache2ctl -t -D DUMP_MODULES';
        $commands[] = 'apachectl -t -D DUMP_MODULES';
      }
      if (stristr($_SERVER['SERVER_SOFTWARE'], 'nginx')) {
        $commands[] = 'nginx -V';
      }
      $paths = array('/usr/sbin/', '/usr/bin/', '/usr/local/sbin/', '/usr/local/bin/', '/sbin/', '/bin/', '');
      $mods = '';
      foreach ($commands as $command) {
        foreach ($paths as $path) {
          $mods = `$path$command`;
          if ($mods) {
            echo $mods;
            return;
          }
        }
      }
      break;
    case 'mem':
      if (!strstr(__FILE__, ':/')) {
        readfile('/proc/meminfo');
      }
      else {
        // Todo: check on Win via `systeminfo`
      }
      break;
    case 'cpu':
      if (!strstr(__FILE__, ':/')) {
        $info = `/sbin/sysctl -a`;
        if (preg_match_all('/sched_domain\\.cpu([0-9]{1,2})\\./', $info, $matches)) {
          echo count(array_unique($matches[1]));
        }
      }
      else {
        // Todo: check on Win via `systeminfo`
      }
      break;
  }
}
