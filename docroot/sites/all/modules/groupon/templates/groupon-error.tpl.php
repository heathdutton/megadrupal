<?php
/**
 * @file
 * Default theme implementation to display error messages.
 *
 * Available variables:
 * - $default_error_message: This message will come from the groupon API.
 * - $error - This is a custom message and administer acn set this message.
 */
?>
<div id='groupon-wrapper'>
  <div class='groupon-row'>
    <div class='groupon-says-title'>
      <?php if ($default_error_message) : ?>
        <h2>Error: </h2>
        <?php echo check_url($default_error);?>
        <hr/>
      <?php endif; ?>
      <div><?php echo filter_xss_admin($error);?></div> 
    </div>
  </div>
</div>
