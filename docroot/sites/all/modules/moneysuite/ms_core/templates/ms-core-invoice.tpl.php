<?php

/**
 * @file
 * Template for the Invoice view of an order. This is used both for printing and for display on the site
 *
 * Variables:
 * ----------
 *
 * @var $order
 *   The order object, which contains lots of useful info
 * @var $op
 *   The operation being performed. Can be 'print', 'view' or 'pdf'
 * @var $invoice_header
 *   The header of the invoice, which you set in the settings page
 */
?>
<html>
<head>
  <meta http-equiv="Content-Type" content="text/html;charset=utf-8">
  <?php if ($op == 'print' || $op == 'pdf') { ?>
    <style>
      tr.ms_total th {
        text-align: right;
        background: #efefef;
        color: #666;
      }

      tr.ms_subtotal td {
        text-align: right;
        background: #fafafa;
      }

      .ms_price {
        text-align: right;
      }

      .ms_adjustment_value {
        text-align: right;
      }

      .ms_order_items, .ms_order_table {
        border: 2px solid #00aa66;
      }

      .ms_order_items tr.odd, .ms_order_items tr.even, .ms_order_table tr.odd, .ms_order_table tr.even {
        background: #fefefe;
        border: 1px solid #bbb;
      }

      .ms_order_items th, .ms_order_table th {
        padding: 3px 10px;
      }

      .ms_order_items td, .ms_order_table td {
        padding: 3px 10px;
      }

      .ms_order_items tr, .ms_order_table tr {
        border: 1px solid #bbb;
        background: #efefef;
      }

      .ms_order_items td.ms_adjustment_display {
        text-align: right;
      }
    </style>
  <?php } ?>
</head>
<body>
<table width="95%" border="0" cellspacing="0" cellpadding="1" align="center"
       bgcolor="#00aa66"
       style="font-family: verdana, arial, helvetica; font-size: small;">
  <tr>
    <td>
      <table width="100%" border="0" cellspacing="0" cellpadding="5"
             align="center" bgcolor="#FFFFFF"
             style="font-family: verdana, arial, helvetica; font-size: small;">
        <tr valign="top">
          <td>
            <table width="100%"
                   style="font-family: verdana, arial, helvetica; font-size: small;">
              <tr>
                <td>
                  <div style="padding-left: 1em;">
                    <?php echo $invoice_header; ?>
                  </div>
                </td>
              </tr>
            </table>
          </td>
        </tr>

        <tr valign="top">
          <td>

            <table cellpadding="4" cellspacing="0" border="0" width="100%"
                   style="font-family: verdana, arial, helvetica; font-size: small;">
              <tr>
                <td colspan="2" bgcolor="#00aa66" style="color: white;">
                  <b><?php echo t('Purchasing Information:'); ?></b>
                </td>
              </tr>
              <tr>
                <td nowrap="nowrap">
                  <b><?php echo t('Order Grand Total:'); ?></b>
                </td>
                <td width="98%">
                  <b><?php echo ms_core_format_money($order->total, $order->currency); ?></b>
                </td>
              </tr>
              <tr>
                <td nowrap="nowrap">
                  <b><?php echo t('Payment Method:'); ?></b>
                </td>
                <td width="98%">
                  <?php echo ms_core_get_payment_gateway_display_name($order->gateway); ?>
                </td>
              </tr>

              <tr>
                <td colspan="2" bgcolor="#00aa66" style="color: white;">
                  <b><?php echo t('Customer Information:'); ?></b>
                </td>
              </tr>

              <tr>
                <td nowrap="nowrap">
                  <b><?php echo t('Name:'); ?></b>
                </td>
                <td width="98%">
                  <?php echo $order->first_name . ' ' . $order->last_name; ?>
                </td>
              </tr>

              <tr>
                <td nowrap="nowrap">
                  <b><?php echo t('E-mail Address:'); ?></b>
                </td>
                <td width="98%">
                  <?php echo $order->email_address; ?>
                </td>
              </tr>

              <tr>
                <td colspan="2">

                  <table width="100%" cellspacing="0" cellpadding="0"
                         style="font-family: verdana, arial, helvetica; font-size: small;">
                    <tr>
                      <td valign="top" width="50%">
                        <b><?php echo t('Billing Address:'); ?></b><br/>
                        <?php echo $order->billing_address['street']; ?><br/>
                        <?php echo $order->billing_address['city']; ?>
                        <?php echo $order->billing_address['state']; ?>
                        <?php echo $order->billing_address['zip']; ?><br/>
                        <?php echo $order->billing_address['country']; ?><br/>
                        <br/>
                        <b><?php echo t('Billing Phone:'); ?></b><br/>
                        <?php echo $order->billing_address['phone']; ?><br/>
                      </td>
                      <?php if (!empty($order->data['shippable'])) { ?>
                        <td valign="top" width="50%">
                          <b><?php echo t('Shipping Address:'); ?></b><br/>
                          <?php echo $order->shipping_address['first_name']; ?>
                          <?php echo $order->shipping_address['last_name']; ?>
                          <br/>
                          <?php echo $order->shipping_address['street']; ?><br/>
                          <?php echo $order->shipping_address['street2']; ?>
                          <br/>
                          <?php echo $order->shipping_address['city']; ?>
                          <?php echo $order->shipping_address['state']; ?>
                          <?php echo $order->shipping_address['zip']; ?><br/>
                          <?php echo $order->shipping_address['country']; ?>
                          <br/>
                          <br/>
                          <b><?php echo t('Shipping Phone:'); ?></b><br/>
                          <?php echo $order->billing_address['phone']; ?><br/>
                        </td>
                      <?php } ?>
                    </tr>
                  </table>

                </td>
              </tr>

              <tr>
                <td colspan="2" bgcolor="#00aa66" style="color: white;">
                  <b><?php echo t('Order Summary:'); ?></b>
                </td>
              </tr>

              <tr>
                <td colspan="2">

                  <table border="0" cellpadding="1" cellspacing="0" width="100%"
                         style="font-family: verdana, arial, helvetica; font-size: small;">
                    <tr>
                      <td nowrap="nowrap">
                        <b><?php echo t('Order #:'); ?></b>
                      </td>
                      <td width="98%">
                        <?php echo l($order->order_number, 'user/' . $order->uid . '/order-history/view/' . $order->order_key); ?>
                      </td>
                    </tr>

                    <tr>
                      <td nowrap="nowrap">
                        <b><?php echo t('Order Date: '); ?></b>
                      </td>
                      <td width="98%">
                        <?php echo format_date($order->created, 'short'); ?>
                      </td>
                    </tr>

                    <tr>
                      <td colspan="2">
                        <?php echo ms_core_get_order_details_table($order); ?>
                      </td>
                    </tr>
                  </table>

                </td>
              </tr>

              <tr>
                <td colspan="2">
                  <?php
                  $table = ms_core_get_order_payments_table($order);
                  echo drupal_render($table);
                  ?>
                </td>
              </tr>

            </table>
          </td>
        </tr>
      </table>
    </td>
  </tr>
</table>

<?php if ($op == 'print' AND variable_get('ms_core_pdfcrowd_support', FALSE)) { ?>
  <a href="https://pdfcrowd.com/url_to_pdf/">Save as PDF</a>
<?php } ?>
</body>
</html>
