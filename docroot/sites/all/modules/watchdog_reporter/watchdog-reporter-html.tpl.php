<?php
/**
 * @file
 * Watchdog reporter HTML mail template
 *
 */
?>

<html>
  <head>
    <title></title>
  </head>
<body style="margin:0; padding: 0;">
  <table style="width:100%; height:100px;background-color: #48A9E4;background-image: -moz-linear-gradient(center top , #0779BF 0%, #48A9E4 100%);">
    <tr>
      <th>
        <img src="http://ww1.prweb.com/prfiles/2012/07/23/9728121/Watchdog.png" />
      </th>
    </tr>
  </table>
  <?php if ($variables['preset']['#values']['watchdog_reporter_group'] != WATCHDOG_REPORTER_GROUP_NONE) {?>
  <table style="width:980px;margin:0 auto;" cellspacing="0" cellpadding="0">
    <tr>
      <th><?php print t('Message') ?></th>
      <th><?php print t('Count') ?></th>
    </tr>
    <?php foreach ($variables['result'] as $key => $row) { ?>
     <tr style="border: 10px solid black;">
       <td style="padding: 5px 10px; border: 1px solid #999;"><?php echo $key; ?></td>
       <td style="padding: 5px 10px; border: 1px solid #999;"><?php echo $row->count; ?></td>
     </tr>
    <?php } ?>
  </table>
  <?php } else {?>
  <table style="width:980px;margin:0 auto;" cellspacing="0" cellpadding="0">
    <tr>
      <th><?php print t('Message') ?></th>
    </tr>
    <?php foreach ($variables['result'] as $row) { ?>
     <tr>
       <td style="padding: 5px 10px; border: 1px solid #999;"><?php echo $row->message; ?></td>
     </tr>
    <?php } ?>
  </table>
  <?php }?>
</body>
</html>
