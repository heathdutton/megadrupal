<?php
/**
 * @file availability_key.tpl.php
 * @copyright Lee Rowlands 2010
 * @license http://www.gnu.org/licenses/gpl.html
 * @author Lee Rowlands <leerowlands at rowlands-bcs dot com>
 * Provides theme representation of template key
*/ 

?>
<div class="clear"></div>
<div class="hotel-booking-key">
  <div class="hotel-booking-key-state hotel-booking-state-available"><?php print t('Available Dates') ?></div>
  <div class="hotel-booking-key-state hotel-booking-state-unavailable"><?php print t('Unavailable Dates') ?></div>
  <div class="hotel-booking-key-state hotel-booking-state-restricted"><?php print t('Available with Restrictions') ?>
    <div class="description"> <?php print t('Place mouse over to view restrictions.');?> </div>
  </div>
  <div class="clear"></div>
</div>
