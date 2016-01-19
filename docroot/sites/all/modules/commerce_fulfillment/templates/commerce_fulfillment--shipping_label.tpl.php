<html>
  <head>
    <?php
/**
 * @file
 * Template for the shipping label.
 */
print $page['css']; ?>
  </head>
  <body>
    <div class="shipping-wrapper">
      <div id="shipping-header">
        <div class="shipping-company">
          <img src="<?php print $image; ?>" />
          <p class="shipping-company-info">
            <?php
              print '<strong>' . t('@company', array('@company' => $company)) . '</strong><br>'
                  . t('@address', array('@address' => $address)) . '<br>' . t('Phone: @phone', array('@phone' => $phone));
            ?>
          </p>
        </div>
        <div class="tracking-number">
          <p>
            <strong>Tracking Number: </strong>
            <?php print $tracking_number ?>
          </p>
        </div>
      </div>
      <div class="customer-shipping-info">
        <p>
        <span class="customer-label">
            <Strong>Ship To:</strong>
        </span>
          <br>
          <?php
          if($shipping !== NULL):
            print t('@shipping1 @shipping2', array('@shipping1' => $shipping[0], '@shipping2' => $shipping[1])) . '<br>';
            print t('@ship_add, @ship_city @ship_state', array(
                '@ship_add' => $shipping[2],
                '@ship_city' => $shipping[3],
                '@ship_state' => $shipping[4],
              )) . '<br>';

            print t('@ship_zip, @ship_country', array('@ship_zip' => $shipping[5], '@ship_country' => $shipping[6]));
          endif;
          ?>
        </p>
      </div>
    </div>
  </body>
</html>
