<?php

/**
 * @file
 * Default theme implementation to present user list.
 */
?>

<?php if (isset($users)):?>
  <ul>
    <?php foreach ($users as $val):?>
      <?php $full_name = (isset($val->real_name) && !empty($val->real_name)) ? $val->real_name : $val->name;?>
      <?php $chat_name = (strlen($full_name) > 16) ? str_pad(drupal_substr($full_name, 0, 16), 18, '..') : $full_name;?>
      <li>
        <img id="<?php print $val->uid;?>_indicator" class="googlechatstatus googlechatstatus_<?php print $val->status_indicator;?>" src="<?php echo base_path() . drupal_get_path('module', 'googlechat');?>/images/cleardot.gif" alt="Online">
        <a id="googlechatbuddy_<?php echo str_replace(array('.', ' ', '@'), array('_', '_', '_'), $val->name); ?>" href="javascript:void(0)" onclick="javascript:chatWith('<?php print $val->name; ?>')" title="Chat with <?php print $full_name; ?>" name="<?php echo $chat_name; ?>"><?php echo $chat_name; ?></a>
      </li>
    <?php endforeach;?>
  </ul>
<?php endif;?>
