<?php
/**
 * @file
 * Formats the wishlist section of a node display
 */
?>
<div class='wishlist'>
  <div class='wl_spacer'>&nbsp;</div>
  <div class='main-body'>
    <?php 
      print $wishlist_cost; 
      print $wishlist_url1; 
      print $wishlist_url2; 
      print $wishlist_purchased_items_table;
      print $wishlist_last_updated; 
    ?>
  </div>
  <div class='purchase-info'>
    <?php 
      print $wishlist_reveal_form;
      print $wishlist_priority; 
      print $wishlist_requested;
      print $wishlist_purchased;
      print $wishlist_items_user_purchased;
    ?>
  </div>
  <div class='wl_spacer'>&nbsp;</div>
</div>
