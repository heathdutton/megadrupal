<?php

/**
 * Template for when the Preview Sync is currently running.
 */

?>

<table class="table table-tasks">
  <tr class="warning">
    <th width="25%"><?php print t('Current Preview Sync status'); ?></th>
    <td><?php print t('Active'); ?></td>
  </tr>
  <tr>
    <td colspan="2" class="tasks">
      <?php $index = 0; ?>
      <?php foreach ($tasks as $id => $task) : ?>
        <?php $index++; ?>
        <div class="task" data-item-id="<?php print $id; ?>">
          <div class="box <?php if ($task['status'] == 1) {print 'success';} else if ($task['status'] == 2) {print 'in-progress';} else if ($task['status'] == 0) {print 'failure';} else {print 'not-started';} ?>">
            <span><?php print $task['duration_friendly']; ?></span>
          </div>
          <span class="index"><?php print $index; ?></span>
          <span class="title"><?php print check_plain($task['data']['title']); ?></span>
        </div>
      <?php endforeach; ?>
    </td>
  </tr>
  <tr>
    <th><?php print t('Total elapsed time'); ?></th>
    <td class="elapsed-time"><?php print format_interval(REQUEST_TIME - $last_run); ?></td>
  </tr>
  <tr>
    <th><?php print t('Estimated time remaining for this task'); ?></th>
    <td class="remaining-time"><?php print t('Unknown'); ?></td>
  </tr>
  <tr>
    <th><?php print t('Initiated by'); ?></th>
    <td><?php print format_username($last_run_user); ?></td>
  </tr>
</table>

