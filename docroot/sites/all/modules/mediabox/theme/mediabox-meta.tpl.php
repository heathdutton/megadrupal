<table>
  <?php $counter = 0; ?>
  <?php foreach ($metas as $meta): ?>
  <tr class="<?php echo ($counter % 2) ? "even" : "odd" ?>">
    <th>
      <?php print $meta['label']; ?>
    </th>
    <td>
      <?php print $meta['value']; ?>
    </td>
  </tr>
  <?php endforeach; ?>
</table>
