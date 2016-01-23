<?php

/**
 * @file
 * maestro-task-interactive-function-edit.tpl.php
 */

?>

<table>
  <tr>
    <td><?php print t('Unique Form ID:'); ?></td>
    <td><input type="text" name="content_type" value="<?php print $td_rec->task_data['content_type']; ?>"></td>
  </tr>
  <tr>
    <td colspan="2"><?php print t('Form field definitions - in the Drupal Form API array format:'); ?></td>
  </tr>
  <tr>
    <td colspan="2"><textarea name="form_api_code" rows="8" style="width: 100%;"><?php print $td_rec->task_data['form_api_code']; ?></textarea></td>
  </tr>
  <tr>
    <td colspan="2" style="font-style: italic; font-size: 0.8em;"><?php print t('Create your form fields in an array variable named $form. Leave out any default values, the system will add them automatically.'); ?></td>
  </tr>
</table>
