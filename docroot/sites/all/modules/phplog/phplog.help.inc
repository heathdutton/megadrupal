<?php
/**
 * @file
 * Contains admin help documentation.
 */

/**
 * Implements hook_help().
 */
function phplog_help($path, $arg) {
  if ($path == 'admin/help#phplog') {
    drupal_add_css('http://alexgorbatchev.com/pub/sh/current/styles/shThemeEclipse.css',
      array('type' => 'external'));
    drupal_add_js('http://alexgorbatchev.com/pub/sh/current/scripts/shCore.js',
      array('type' => 'external'));
    drupal_add_js('http://alexgorbatchev.com/pub/sh/current/scripts/shAutoloader.js',
      array('type' => 'external'));

    drupal_add_css(drupal_get_path('module', 'phplog') . '/shStyle.css');
    drupal_add_js(drupal_get_path('module', 'phplog') . '/shSetup.js');

    // Translated text.
    $t_about = t('About');
    $t_setup = t('Setup');
    $t_usage = t('Usage');

    $t_about_1 = t('This module lets you send watchdog alerts to your php error_log. Handy for when you don\'t want to fill up your database with logs (c\'mon Drupal! not a good idea) and if due to server/network setup the Drupal Core alternate "syslog" module is not usable.');

    $t_setup_1 = t('Turn this module on.');
    $t_setup_2 = t('(Optionally) Configure the threshold for PHP error_logs in your logging settings.');

    $t_usage_1 = t('Simply making calls to watchdog() will now also transparently output a bunch of information to your php error_log.');
    $t_usage_2 = t('For those interested in grokking watchdog alerts in your error_log, the format of watchdog alerts is:');
    $t_usage_3 = t('Where the first %s is: SEVERITY');
    $t_usage_4 = t('the second %s is: TYPE');
    $t_usage_5 = t('and the third %s is: MESSAGE');
    $t_usage_5b = t('Also, "base_url" is drupal\' $base_url (useful for multi-site configurations)');
    $t_usage_6 = t('So on a linux-system, if you just want to extract all the ERROR messages from your php error_log, something like this could work:');
    $t_usage_7 = t('Or if you were just interested in seeing explicitly phplog-sourced watchdog messages:');

    $base_help = <<<EOT
<ul>
  <li><a href="#about">$t_about</a></li>
  <li><a href="#setup">$t_setup</a></li>
  <li><a href="#usage">$t_usage</a></li>
</ul>

<h2><a id="about"></a>$t_about</h2>
<p>$t_about_1</p>

<h2><a id="setup"></a>$t_setup</h2>
<ol>
  <li>$t_setup_1</li>
  <li>$t_setup_2</li>
</ol>

<h2><a id="usage"></a>$t_usage</h2>
<p>$t_usage_1</p>
<p>$t_usage_2</p>
<pre>
phplog+watchdog|%s|%s|%s|location=\'%s\'|hostname=\'%s\'|link=\'%s\'|uid=\'%s\'|referer=\'%s\'|timestamp=\'%s\'|base_url='%s'
</pre>
 <ul>
  <li>$t_usage_3</li>
  <li>$t_usage_4</li>
  <li>$t_usage_5</li>
  <li>$t_usage_5b</li>
</ul>
<p>$t_usage_6</p>
<pre class="brush: bash">
$ grep -n ERROR error_log | awk -F\| '{ print $4; }'
</pre>
<p>$t_usage_7</p>
<pre class="brush: bash">
$ grep -n phplog+watchdog error_log
</pre>
EOT;
    return $base_help;
  }
}
