<?php
/**
 * @file
 * Template file for Entity Documentation PDF module.
 */
?>

<html>
  <head>
    <style type="text/css"><?php print $variables['css'];?></style>
  </head>
  <title>
    <h1><?php print $variables['params']['name'];?></h1>
  </title>
  <body>
    <div class = "description">
      <h3><?php print t('Description'); ?></h3>
      <p><?php print $variables['params']['description']; ?></p>
      <h3><?php print t('Properties'); ?></h3>
      <ul>
        <?php foreach ($variables['params']['properties'] as $property) {?>
          <li><strong><?php print $property['name']; ?></strong> : <?php print $property['value']; ?></li>
          <?php } ?>
      </ul>
    </div>
    <div class = "fields">
      <h3><?php print t('Fields'); ?></h3>
      <table class = "fields-table" border = "1" cellspadding = "5">
        <tr>
          <?php foreach ($variables['field_columns'] as $field_column): ?>
          <th><strong><?php print $field_column; ?></strong></th>
          <?php endforeach; ?>
        </tr>
        <?php
        if (isset($variables['fields'])):
          foreach ($variables['fields'] as $field_key => $field): ?>
          <tr>
            <?php foreach ($field as $field_param_key => $field_param):?>
            <td><?php print $field_param; ?></td>
            <?php endforeach; ?>
          </tr>
          <?php
          endforeach;
        endif;
        ?>
      </table>
    </div>
  </body>
</html>
