<?php
/**
 * @file
 * Template file for Entity Documentation Confluence module.
 */
?>
<div>
  <h3><?php print t('Description'); ?></h3>
  <div>
    <?php print $description; ?>
  </div>
  <h3><?php print t('Properties'); ?></h3>
  <ul>
    <?php foreach ($properties as $property):?>
      <li>
        <strong><?php print $property['name']; ?></strong> : <?php print $property['value']; ?>
      </li>
    <?php endforeach; ?>
  </ul>
</div>
<p>&nbsp;</p>
<div>
  <h3><?php print t('Fields'); ?></h3>
  <table>
    <tr>
      <?php foreach ($field_columns as $field_column): ?>
        <th>
          <strong><?php print $field_column; ?></strong>
        </th>
      <?php endforeach; ?>
    </tr>
    <?php if (isset($fields)): ?>
      <?php foreach ($fields as $field_key => $field): ?>
        <tr>
          <?php foreach ($field as $field_param_key => $field_param): ?>
            <td>
              <?php print $field_param; ?>
            </td>
          <?php endforeach; ?>
        </tr>
      <?php endforeach; ?>
    <?php endif; ?>
  </table>
</div>
