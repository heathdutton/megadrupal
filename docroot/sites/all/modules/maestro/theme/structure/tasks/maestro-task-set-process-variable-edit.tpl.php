<?php

/**
 * @file
 * maestro-task-set-process-variable-edit.tpl.php
 */

?>

<div>
  <span style="float:left;width:49%;padding-left:5px;"><label><?php print t('Variable to Set:'); ?></span></label>
  <span style="float:right;width:49.5%">
      <select name="var_to_set">
<?php
        foreach ($pvars as $value=>$label) {
          if ($value == $td_rec->task_data['var_to_set']) {
?>
            <option value="<?php print $value;?>" selected="selected"><?php print $label;?></option>
<?php
          }
          else {
?>
            <option value="<?php print $value;?>"><?php print $label;?></option>
<?php
          }
        }
?>
      </select>
    </span>
</div>
<div style="clear:both;padding:5px;"></div>
<fieldset style="padding:0px;"><legend><?php print t('Select method to set variable'); ?></legend>
<?php
  foreach ($set_methods as $key => $method) {
?>
<div style="padding-top:5px;">
  <span style="float:left;width:49%;padding-left:5px;">
      <label for="set_type_opt_<?php print $key; ?>"><input type="radio" id="set_type_opt_<?php print $key; ?>" name="set_type" value="<?php print $key; ?>" onchange="toggle_set_type('<?php print $key; ?>');" <?php print ($td_rec->task_data['set_type'] == $key) ? 'checked="checked"':''; ?>>
      <?php print $method['title']; ?></label>
  </span>
  <span style="float:right;width:44%;padding-right:4.9%;vertical-align:top;">
      <input class="set_method" id="set_type_<?php print $key; ?>" type="text" name="<?php print $key; ?>_value" value="<?php print $td_rec->task_data[$key . '_value']; ?>">
  </span>
  <div class="description" style="clear:both;padding-top:0px;padding-left:5px;"><?php print $method['description']; ?></div>
</div>
<div style="clear:both"></div>
<?php
  }
?>
</fieldset>

<script type="text/javascript">
  setTimeout(tick, 500);

  function tick() {
    toggle_set_type('<?php print $td_rec->task_data['set_type']; ?>');
  }


  function toggle_set_type(type) {
    (function($) {
      $('.set_method').hide();
      $('#set_type_' + type).show();
    })(jQuery);
  }
</script>
