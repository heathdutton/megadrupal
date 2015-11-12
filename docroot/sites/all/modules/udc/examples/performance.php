<?php
/**
 * @file
 *
 * Simple request/process time performance testing
 *
 */


// Init
require_once('enable_examples.inc');

// Mean value
function array_mean($a) {
  return !empty($a) ? (array_sum($a) / count($a)) : 0;
}

// Standard deviation
function array_std($a) {
  if (empty($a)) return 0;
  $m=array_mean($a); $s2=0;
  foreach ($a as $v) $s2=($v-$m)*($v-$m);
  return sqrt($s2);
}

// Result formatting
function format_results($caption) {
  global $_n; global $_t; global $_ping_time;
  $t_all  = round(toc()/$_n);
  $t_svr  = round(array_mean($_t)*1000);
  $t_std  = round(array_std($_t)*1000);
  $ratio  = round(array_mean($_t)/$_ping_time, 1);
  $t_diff = $t_all - $t_svr;
  return "
    <tr>
      <td class=\"type\">$caption</td>
      <td class=\"value\">$t_all</td>
      <td class=\"value\">$t_svr</td>
      <td class=\"value\">$t_std</td>
      <td class=\"value\">$ratio</td>
      <td class=\"value\">$t_diff</td>
    </tr>";
}

// Num loops
if (!isset($_GET['n']) || intval($_GET['n']) < 1) {
  $_n = 1;
}
else {
  $_n = $_GET['n'];
}

// Profile fields
if (isset($_GET['pf'])) {
  $pf = explode(',', $_GET['pf']);
  foreach ($pf as $k => $v) $pf[$k] = trim(strtolower($v));
  $pf = array_filter($pf);
}
else {
  $pf = array();
}

// Common fields
if (isset($_GET['cf'])) {
  $cf = isset($_GET['cf']) ? explode(',', $_GET['cf']) : array();
  foreach ($cf as $k => $v) $cf[$k] = trim(strtolower($v));
  $cf = array_filter($cf);
}
else {
  $cf = array();
}

set_time_limit(600);

?>
<html><head>
    <style type="text/css"><!--
        table       { border-collapse: collapse; }
        td, th      { border: solid 1px black; padding: 5px; font-size: 12px; font-family: monospace; }
        th          { background-color: black; color: white; }
        .caption    { text-align: left; font-weight: bold; }
        .value      { text-align: center; }
    --></style>
</head><body>
    <form method="GET" action="<?= $_SERVER['PHP_SELF'] ?>">
        <table>
            <tr><td>Number of loops</td><td><input type="text" name="n" value="<? print $_n; ?>"></td></tr>
            <tr><td>Included profile fields</td><td><input type="text" name="pf" value="<? print implode(',', $pf); ?>"></td></tr>
            <tr><td>Included common fields</td><td><input type="text" name="cf" value="<? print implode(',', $cf); ?>"></td></tr>
            <tr><td></td><td><input type="submit" name="go" value="start"></td></tr>
        </table>
    </form>
    <table>
        <tr>
            <th class="type">Request type</th>
            <th class="value">Client time<br/>ms</th>
            <th class="value">Server time<br/>ms</th>
            <th class="value">Server time stddev<br/>ms</th>
            <th class="value">Ratio to ping<br/>1</th>
            <th class="value">Network time<br/>ms</th>
        </tr>
<?

try {

  if($_n > 0) {

    // Loop ping
    tic(); $_t = array();
    for ($i=0; $i<$_n; $i++) {
      $dpu = new DrupalUserInfo();
      $dpu->ping();
      $_t[]= $dpu->getServerProcessTime();
      $_ping_time = array_mean($_t);
    }
    print format_results('Ping');

    // Loop List without roles
    tic(); $_t = array();
    for ($i=0; $i<$_n; $i++) {
      $dpu = new DrupalUserList();
      $dpu->request(false, false);
      $_t[]= $dpu->getServerProcessTime();
    }
    print format_results('List without roles');

    // Loop List with roles
    tic(); $_t = array();
    for ($i=0; $i<$_n; $i++) {
      $dpu = new DrupalUserList();
      $dpu->request(false, true);
      $_t[]= $dpu->getServerProcessTime();
    }
    print format_results('List with roles');

    // Loop auth with no fields
    tic(); $_t = array();
    for ($i=0; $i<$_n; $i++) {
      $dpu = new DrupalUserAuth();
      $dpu->request('test', 'test', null, true, false);
      $_t[]= $dpu->getServerProcessTime();
    }
    print format_results('Auth without fields');

    if (!empty($pf)) {
      // Loop auth with profile field
      tic(); $_t = array();
      for ($i=0; $i<$_n; $i++) {
        $dpu = new DrupalUserAuth();
        $dpu->request('test', 'test', null, true, $pf);
        $_t[]= $dpu->getServerProcessTime();
      }
      print format_results('Auth with ' . count($cf) . ' profile fields');
    }

    if (!empty($cf)) {
      // Loop auth with common field
      tic(); $_t = array();
      for ($i=0; $i<$_n; $i++) {
        $dpu = new DrupalUserAuth();
        $dpu->request('test', 'test', null, true, $cf);
        $_t[]= $dpu->getServerProcessTime();
      }
      print format_results('Auth with ' . count($cf) . ' common fields');
    }

    // Loop info no fields
    tic(); $_t = array();
    for ($i=0; $i<$_n; $i++) {
      $dpu = new DrupalUserInfo();
      $dpu->request('test', null, false, false);
      $_t[]= $dpu->getServerProcessTime();
    }
    print format_results('Info without fields');

    if (!empty($pf)) {
      // Loop info with a profile field
      tic(); $_t = array();
      for ($i=0; $i<$_n; $i++) {
        $dpu = new DrupalUserInfo();
        $dpu->request('test', NULL, false, $pf);
        $_t[]= $dpu->getServerProcessTime();
      }
      print format_results('Info with ' . count($pf) . ' profile fields');
    }

    if (!empty($cf)) {
      // Loop info with a common field
      tic(); $_t = array();
      for ($i=0; $i<$_n; $i++) {
        $dpu = new DrupalUserInfo();
        $dpu->request('test', NULL, false, $cf);
        $_t[]= $dpu->getServerProcessTime();
      }
      print format_results('Info with ' . count($cf) . ' common fields');
    }
  }

} catch (Exception $e) {
    die("Exception: " . $e->getMessage());
}

?>
    </table>
</body></html>
