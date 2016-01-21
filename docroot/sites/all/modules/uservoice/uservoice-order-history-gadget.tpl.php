<?php
/**
 * @file
 * Template file for order history gadget. This gadget is designed to displaye
 * order history information from your Drupal site within the UserVoice admin
 * console (fetched by UserVoice via ajax callback). It depends on integration
 * with contributed ecommerce Drupal modules.
 *
 * Available variables:
 * - $gadget_title: The gadget title.
 * - $account: User account object associated with this gadget request.
 * - $orders: An array orders for the given account.
 *
 * @see uservoice_order_history_gadget()
 * @see template_preprocess_uservoice_order_history_gadget()
 * @see https://developer.uservoice.com/docs/gadgets/creating-your-own/
 */
?>
<?php global $base_url; ?>
<!DOCTYPE html>
<html>
  <head>
    <title><?php print $gadget_title; ?></title>
    <link href="https://cdn.uservoice.com/packages/gadget.css" media="all" rel="stylesheet" type="text/css" />
    <link href="<?php print $base_url . '/' . drupal_get_path('module', 'uservoice'); ?>/uservoice.gadget.css" media="all" rel="stylesheet" type="text/css" />
  </head>
  <body>

    <!-- Check that we have something to show in the gadget. -->
    <?php if (!empty($orders)): ?>
      <table class="orders">
        <thead>
          <caption><?php print $gadget_title; ?></caption>
          <tr>
            <th>Date</th>
            <th>Stat</th>
            <th>Gate</th>
            <th>Tot $</th>
          </tr>
        </thead>
        <tbody>
          <?php foreach ($orders as $order): ?>
            <tr>
              <td><?php print date('m/d/y', $order['date']); ?></td>
              <td><?php print l(substr($order['status'], 0, 4), $order['link'], array('absolute' => TRUE)); ?></td>
              <td><?php print substr($order['gateway'], 0, 4); ?></td>
              <td><?php print $order['amount']; ?></td>
            </tr>
          <?php endforeach; ?>
        </tbody>
      </table>

    <!-- Nothing to show: let UserVoice know that we have an empty gadget. -->
    <?php else: ?>
      <script type="text/javascript">
        window.gadgetNoData = true;
      </script>
    <?php endif; ?>
    <script src="https://cdn.uservoice.com/packages/gadget.js" type="text/javascript"></script>
  </body>
</html>
