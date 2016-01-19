<?php foreach ($orders as $order): ?>
<table>
  <tr>
    <th>Fulfilment ID</th>
    <td><?php echo $order->Order_Number; ?></td>
  </tr>
  <tr>
    <th>Created</th>
    <td><?php echo $order->DateCreated; ?></td>
  </tr>
  <tr>
    <th>Status</th>
    <td><?php echo $order->Status; ?></td>
  </tr>

  <?php if (isset($order->Shipments->Shipment)): ?>
  <tr>
    <th>Shipping method</th>
    <td><?php echo $order->Shipments->Shipment->ShipMethod; ?></td>
  </tr>

  <?php if (isset($order->Shipments->Shipment->ShipFee) && $mode === 'administrator'): ?>
  <tr>
    <th>Ship fee</th>
    <td><?php echo $order->Shipments->Shipment->ShipFee; ?></td>
  </tr>
  <?php endif; ?>

  <tr>
    <th>Tracking number</th>
    <td><?php echo $order->Shipments->Shipment->TrackingNumber; ?></td>
  </tr>

  <tr>
    <th>Generated tracking URL</th>
    <td>
      <?php
      $url = commerce_fulfilment_oms_get_tracking_url($order->Shipments->Shipment->ShipMethod, $order->Shipments->Shipment->TrackingNumber);
      echo $url ? l($url, $url) : '(unavailable)';
      ?>
    </td>
  </tr>
  <?php endif; ?>
</table>
<?php endforeach; ?>
