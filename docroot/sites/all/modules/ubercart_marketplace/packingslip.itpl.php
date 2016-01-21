<?php

/**
 * This file is the default packing slip template for Ubercart marketplace.
 */
?>

<table width="95%" border="0" cellspacing="0" cellpadding="1" align="center" bgcolor="#006699" style="font-family: verdana, arial, helvetica; font-size: small;">
  <tr>
    <td>
      <table width="100%" border="0" cellspacing="0" cellpadding="5" align="center" bgcolor="#FFFFFF" style="font-family: verdana, arial, helvetica; font-size: small;">

        <tr valign="top">
          <td>
            <table width="100%" style="font-family: verdana, arial, helvetica; font-size: small;">
              <tr>
                <td>
                  [site-logo]
                </td>
                <td width="98%">
                  <div style="padding-left: 1em;">
                  <span style="font-size: large;">[store-name]</span><br/>
                  [site-slogan]
                  </div>
                </td>
                <td nowrap="nowrap">
                  [store-address]<br />[store-phone]
                </td>
              </tr>
            </table>
          </td>
        </tr>

        <tr valign="top">
          <td>
            <table cellpadding="4" cellspacing="0" border="0" width="100%" style="font-family: verdana, arial, helvetica; font-size: small;">
              <tr>
                <td colspan="2">

                  <table width="100%" cellspacing="0" cellpadding="0" style="font-family: verdana, arial, helvetica; font-size: small;">
                    <tr>
                      <?php if (uc_order_is_shippable($order)) { ?>
                      <td valign="top" width="50%">
                        <b><?php echo t('Shipping Address:'); ?></b><br />
                        [order-shipping-address]<br />
                        <br />
                      </td>
                      <?php } ?>
                    </tr>
                  </table>

                </td>
              </tr>
              <tr>
                <td colspan="2" bgcolor="#EEEEEE">
                  <font color="#CC6600"><b><?php echo t('Shipment Summary:'); ?></b></font>
                </td>
              </tr>

              <tr>
                <td colspan="2">

                  <table border="0" cellpadding="1" cellspacing="0" width="100%" style="font-family: verdana, arial, helvetica; font-size: small;">
                    <tr>
                      <td nowrap="nowrap">
                        <b><?php echo t('Order #:'); ?></b>
                      </td>
                      <td width="98%">
                        [order-link]
                      </td>
                    </tr>

                    <?php if (uc_order_is_shippable($order)) { ?>
                    <tr>
                      <td nowrap="nowrap">
                        <b><?php echo t('Shipping Method:'); ?></b>
                      </td>
                      <td width="98%">
                        [order-shipping-method]
                      </td>
                    </tr>
                    <?php } ?>

                    <tr>
                      <td colspan="2">
                        <br /><b><?php echo t('Products in shipment:'); ?>&nbsp;</b>

                        <table width="100%" style="font-family: verdana, arial, helvetica; font-size: small;">

                          <?php if (is_array($order->products)) {
                            foreach ($order->products as $product) { ?>
                          <tr>
                            <td valign="top" nowrap="nowrap">
                              <b><?php echo $product->qty; ?> x </b>
                            </td>
                            <td width="98%">
                              <b><?php echo $product->title; ?></b> 
                              <br />
                              <?php echo t('SKU: ') . $product->model; ?><br />
                              <?php if (is_array($product->data['attributes']) && count($product->data['attributes']) > 0) {?>
                              <?php foreach ($product->data['attributes'] as $attribute => $option) {
                                echo '<li>'. t('@attribute: @options', array('@attribute' => $attribute, '@options' => implode(', ', (array)$option))) .'</li>';
                              } ?>
                              <?php } ?>
                              <br />
                            </td>
                          </tr>
                          <?php }
                              }?>

                      </td>
                    </tr>
                  </table>

                </td>
              </tr>

              <tr>
                <td colspan="2">
                  <hr noshade="noshade" size="1" />
                  <p><?php echo t('Thanks again for shopping with us.  If you have questions about your order, please contact us.'); ?></p>
			<p><?php echo t('Note: Please do not be alarmed if your complete order is not in this package or listed on this packing slip.  We may ship multiple packages in multiple shipments to expedite the shipping process.'); ?></p>

                  <?php if ($store_footer) { ?>
                  <p><b>[store-link]</b><br /><b>[site-slogan]</b></p>
                  <?php } ?>
                </td>
              </tr>

            </table>
          </td>
        </tr>
      </table>
    </td>
  </tr>
</table>
</td>
</tr>
</table>
