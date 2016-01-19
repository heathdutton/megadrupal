<?php

/**
 * Template for when the Preview Sync is not currently running.
 */

?>

<table class="table">
  <tr class="ok">
    <th width="25%"><?php print t('Preview Sync status'); ?></th>
    <td><?php print t('Idle'); ?></td>
  </tr>
  <tr class="<?php print check_plain($preview_row_class); ?>">
    <th><?php print t('Preview environment'); ?></th>
    <td>
      <ul>
        <li><?php print t('Bootstrap: <strong>@bootstrap</strong>', array('@bootstrap' => $preview_bootstrap)); ?></li>
        <li><?php print t('Database: <strong>@db_status</strong>', array('@db_status' => $preview_db_status)); ?></li>
        <li><?php print t('URI: <strong>@uri</strong>', array('@uri' => $preview_uri)); ?></li>
      </ul>
    </td>
  </tr>
  <tr class="<?php print $dump_file_row_class; ?>">
    <th><?php print t('Database dump file'); ?></th>
    <td>
      <strong><?php print check_plain($dump_file); ?></strong> (<?php print $dump_file_exists ? t('exists') : t('does not exist'); ?>)
    </td>
  </tr>
  <tr>
    <th><?php print t('Last Preview Sync'); ?></th>
    <td><?php print format_interval(REQUEST_TIME - $last_run) . t(' ago by @name', array('@name' => format_username($last_run_user))); ?></td>
  </tr>
  <tr>
    <th rowspan="2"><?php print t('Content currently "in review" on this site'); ?></th>
    <td>
      <ul>
      <?php foreach ($review_nodes as $type => $node) : ?>
        <li>
          <?php print t('<strong>@total</strong> @type', array('@total' => $node->count, '@type' => node_type_get_name($type))); ?>
        </li>
      <?php endforeach; ?>
      </ul>
    </td>
  </tr>
  <tr>
    <td><?php print t('<strong>@total</strong> total', array('@total' => $review_total)); ?></td>
  </tr>
</table>
