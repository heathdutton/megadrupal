<?php
/**
 * @file
 * Template for the PageRank Widget module.
 */

$url = variable_get('pr_widget_testurl', 0) ? parse_url(variable_get('pr_widget_testurl', 0), PHP_URL_HOST) : parse_url($GLOBALS['base_url'], PHP_URL_HOST);
?>
<div class="pr_widget-wrap">
<?php if (variable_get('pr_widget_pagerank_enabled', 1)): ?>
  <?php $link = variable_get('pr_widget_link', 'http://domaintyper.com/PageRankCheck/'); ?>
  <?php if (!empty($link)): ?>
    <a href="<?php print $link . $url; ?>" title="<?php print t('Check on') . ' ' . parse_url($link, PHP_URL_HOST); ?>" rel="nofollow" target="_blank">
  <?php endif; ?>
  <table class="pr_widget-table">
    <tbody class="pr_widget-body">
      <tr>
        <td class="pr_widget-first">
          <?php print check_plain(variable_get('pr_widget_string', t('PageRank'))); ?>
        </td>
        <td class="pr_widget-last">
          <?php
            print check_plain(variable_get('pr_widget_ratio', t('[No data yet]'))) . check_plain(variable_get('pr_widget_suffix', ''));
          ?>
        </td>
      </tr>
    </tbody>
  </table>
  <?php if (!empty($link)): ?>
    </a>
  <?php endif; ?>
<?php endif; ?>
<?php if (variable_get('pr_widget_notice_enabled', 0)): ?>
<table class="pr_widget-table notice" style="width: auto;">
<tbody class="pr_widget-body">
  <tr>
    <td class="pr_widget-notice">
      <?php
        $host = variable_get('pr_widget_url_name', $url);
        $year = variable_get('pr_widget_year', '');
        $notice = ' ' . variable_get('pr_widget_prepend', '') . ' © ' . (($year != date('Y') && !empty($year)) ? $year . '-' . date('Y') : date('Y'));
        print $notice . ' ' . $host;
      ?>
    </td>
  </tr>
</tbody>
</table>
<?php endif; ?>
</div>
