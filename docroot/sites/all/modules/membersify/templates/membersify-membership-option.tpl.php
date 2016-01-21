<?php
/**
 * @file
 * Template file for the membership plan option item.
 *
 * Available variables:
 *
 * @var $name: The name of the plan.
 * @var $description: The description of the plan.
 * @var $price: The price of the plan.
 */

?>
<!-- membersify-membership-option template -->
<span class='membersify_plan_name'><?php print $name; ?></span> - <span class='membersify_plan_price'><?php print $price; ?></span>
<div class='membersify_plan_description'><?php print $description; ?></div>
<!-- /membersify-membership-option template -->
