#!/usr/bin/php
<?php

$_SERVER['REMOTE_ADDR'] = '127.0.0.1';
$_SERVER['REQUEST_METHOD'] = 'POST'; // D6 expects this variable
define('DRUPAL_ROOT', getcwd());
require_once DRUPAL_ROOT . '/includes/bootstrap.inc';
drupal_bootstrap(DRUPAL_BOOTSTRAP_FULL);

$iterations = 10000;

timer_start('set');
for ($i = 0; $i < $iterations; $i++) {
  cache_set("test-$i", 1, 'cache', CACHE_PERMANENT, array('user' => array(1, 2, 3, 4), 'node' => array(1, 2, 3, 4)));
}
timer_stop('set');

timer_start('get');
for ($i = 0; $i < $iterations; $i++) {
  $cache = cache_get("test-$i");
  if (!isset($cache->data) || $cache->data != 1) exit("cache_get() failed.");
}
timer_stop('get');

timer_start('expire 1');
cache_invalidate(array('node' => array(3)), 'cache');
timer_stop('expire 1');

timer_start('expire 7');
cache_invalidate(array('user' => array(1, 2, 3, 4), 'node' => array(1, 2, 4)), 'cache');
//cache_clear_all('test', 'cache', TRUE);
timer_stop('expire 7');

for ($i = 0; $i < $iterations; $i++) {
  $cache = cache_get("test-$i");
  if (isset($cache->data)) exit("cache expiration failed.");
}

unset($GLOBALS['timers']['page']);
foreach ($GLOBALS['timers'] as $name => $timer) {
  if ($name == 'get' || $name == 'set') {
    $timer['time'] /= $iterations;
  }
  print sprintf("%s:\t\t%.2fms\n", str_pad($name, 12), $timer['time']);
}
