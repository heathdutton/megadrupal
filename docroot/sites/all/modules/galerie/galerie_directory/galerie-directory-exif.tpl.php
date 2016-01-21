<table class='galerie-exif'>
<?php foreach ($exif as $label => $value): if ($label != 'data'): ?>
  <tr>
    <th><?php echo $label ?></th>
    <td><?php echo $value ?></td>
  </tr>
<?php endif; endforeach; ?>
</table>
