<h3>Product: <?php echo $inventory->Name; ?> (<?php echo $inventory['SKU']; ?>)</h3>
<?php foreach ($inventory->Warehouses->Warehouses as $warehouse): ?>
  <table>
    <tr>
      <th>Warehouse</th>
      <td><?php echo $warehouse['Name']; ?></td>
    </tr>
    <?php foreach (array('Total', 'Available', 'InWarehouse', 'Damaged', 'Lost', 'Found') as $state): ?>
      <tr>
        <th><?php echo $state; ?></th>
        <td><?php echo $warehouse->Bin->{'QTY_'.$state}; ?></td>
      </tr>
    <?php endforeach; ?>
  </table>
<?php endforeach; ?>
