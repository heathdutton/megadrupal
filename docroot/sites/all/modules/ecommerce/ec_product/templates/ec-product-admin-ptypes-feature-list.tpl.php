<?php

/**
 * @file
 */
?>
<?php if (!empty($features)): ?>
<table id="feature-table">
  <thead>
    <tr>
      <th>Name</th>
      <th>Description</th>
      <th>Weight</th>
      <th></th>
    </tr>
  </thead>
  <tbody>
  <?php foreach ($features as $feature): ?>
    <tr<?php echo $feature['draggable'] ? ' class="draggable"' : ''; ?>>
      <td><?php echo $feature['name']; ?></td>
      <td><?php echo $feature['description']; ?></td>
      <td><?php echo $feature['weight']; ?></td>
      <td><?php echo $feature['ops']; ?></td>
    </tr>
  <?php endforeach; ?>
  </tbody>
</table>

<?php echo $submit; ?>
<?php endif; ?>