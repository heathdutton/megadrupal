<?php
/*
 * @file hotel_booking_pane_help.tpl.php
 * Provides cart, order and checkout pane with help text
 * @copyright Copyright(c) 2010 Lee Rowlands
 * @license GPL v2 http://www.fsf.org/licensing/licenses/gpl.html
 * @author Lee Rowlands leerowlands at rowlands-bcs dot com
 *
 * Available variables
 * $node the product node
 * $product the product in the cart (see $product->data for hotel_booking details)
 * $cart whether the pane is being shown in the cart, and the order can be edited
 */
?>
<?php if (variable_get('hotel_booking_teaser_in_panes','FALSE')) : ?>
  <div class="hotel-booking-pane-teaser"><?php print $node->teaser;?></div>
<?php endif; ?>
<div class="hotel-booking-pane-help">
  <p>
    <?php print t('The following table breaks down your stay by day, listing price per night of the room'); ?>
    <?php if (isset($product->data['booking_upgrades'])) :?>
        <?php print t(', along with any applicable date based Add-On items.')?>
      </p>
      <p>
        <?php print t('Other Add-On items apply to your entire stay.'); ?>
      </p>
    <?php else: ?>
    </p>
    <?php endif; ?>
    <?php if ($cart && isset($product->data['booking_upgrades']) && is_array($product->data['booking_upgrades'])) : ?>
      <p>
        <?php print l('Edit Add-Ons & Upgrades', 'booking_upgrades/'. $product->cart_item_id) ?>
      </p>
    <?php endif;?>
</div>
