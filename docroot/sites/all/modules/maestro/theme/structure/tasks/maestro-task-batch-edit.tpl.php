<?php

/**
 * @file
 * maestro-task-batch-edit.tpl.php
 */

?>

<table>
  <tr>
   <td colspan="2">
      <?php print t('Handler base path:'); ?>
      <?php print $td_rec->task_data['handler_location'];?>
      <br><em><?php print t('Handler is a PHP script which sets a variable $success to true to complete this task')?></em>
    </td>
  </tr>
  <tr>
    <td><?php print t('Handler:'); ?></td>
    <td><input type="text" name="handler" value="<?php print $td_rec->task_data['handler']; ?>"></td>
  </tr>
</table>
