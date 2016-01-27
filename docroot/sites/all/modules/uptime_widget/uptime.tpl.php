<?php
/**
 * @file
 * Template for the Permalink block module.
 *
 * Note: You'll need the ID of the monitor. For that, simply go to 
 * "http://api.uptimerobot.com/getMonitors?apiKey=yourApiKey" and get the ID of 
 * the monitor to be queried.
 * And, this code requires PHP 5+ or PHP 4 with SimpleXML enabled.
 */
?>
<div class="uptime-wrap">
<?php if (variable_get('uptime_enabled', 1)): ?>
  <table class="uptime-table" summary="UpTimeRobot.com">
  <tbody class="uptime-body">
    <tr>
      <td class="uptime-first">
        <?php print t('UpTime'); ?>
      </td>
      <td class="uptime-last">
        <?php print check_plain(variable_get('uptime_ratio', t('[Get API key or run cron] '))); ?> %
      </td>
    </tr>
  </tbody>
  <caption><a href="http://www.uptimerobot.com/about.asp" title="<?php print t('Free Website Uptime Monitoring'); ?>">UpTimeRobot.com</a></caption>
</table>
<?php endif; ?>
<?php if (variable_get('uptime_notice_enabled', 0)): ?>
  <table class="uptime-table notice" style="width: auto;">
  <tbody class="uptime-body">
    <tr>
      <td class="uptime-notice">
        <?php
          $host = variable_get('uptime_url_name', parse_url($GLOBALS['base_url'], PHP_URL_HOST));
          $year = variable_get('uptime_year', '');
          $notice = ' ' . variable_get('uptime_prepend', 'All rights reserved') . ' © ' . (($year != date('Y') && !empty($year)) ? $year . '-' . date('Y') : date('Y'));
          print $notice . ' ' . $host;
        ?>
      </td>
    </tr>
  </tbody>
  </table>
<?php endif; ?>
</div>
