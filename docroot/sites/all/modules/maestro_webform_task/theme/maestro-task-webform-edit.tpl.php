<?php

/**
 * @file
 * maestro-task-interactive-function-edit.tpl.php
 */

?>
<table>
  <tr>
    <td>
      <?php print t('Select the Webform to use for this task:'); ?>
    </td>
    <td>
      <select name="webform">
        <?php foreach($webforms as $nid => $name) {?>
        <option value="<?php print $nid; ?>" <?php if($nid == $td_rec->task_data['webform_nid']) print "selected"; ?>>
          <?php print $name; ?>
        </option>
        <?php } ?>
      </select>
    </td>
  </tr>
  <tr>
    <td><?php print t('Webform instance name'); ?>:</td>
    <td style="vertical-align:top"><input type="text" name="instance_name" value="<?php print $td_rec->task_data['instance_name']; ?>"></td>
  </tr>
  <tr>
    <td colspan="2"><i><?php print t('The webform instance name is a unique machine-readable name for tracking the webform instance results. It must only contain lowercase letters, numbers, and underscores. If this task is to edit or review a previous submitted webform, then use that instance name.'); ?></i></td>
  </tr>
</table>
