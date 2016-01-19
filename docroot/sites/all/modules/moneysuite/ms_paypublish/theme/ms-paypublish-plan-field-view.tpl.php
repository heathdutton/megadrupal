<?php
/**
 * @file
 * Default theme implementation for the Pay to Publish field formatter.
 *
 * Available variables:
 * - $node: The node record.
 * - $plan: The plan record.
 * - $pp_node: The pay to publish node record.
 * - $is_sticky: Whether or not the author has paid sticky status.
 * - $sticky_expiration: The timestamp of the sticky expiration.
 * - $is_promoted: Whether or not the author has paid promoted status.
 * - $promoted_expiration: The timestamp of the promoted expiration.
 * - $is_published: Whether or not the author has paid for publishing.
 * - $published_expiration: The timestamp of the published expiration.
 * - $status: The status of the pay to publish node record.
 * - $order: The order associated with the record, or FALSE.
 * - $actions: An array of actions that can be performed on this node.
 *
 * @see ms_paypublish_field_formatter_view()
 * @ingroup themeable
 */

?>

<style type='text/css'>
  .ms_paypublish_plan_label {
    font-weight: bold;
  }
</style>

<div class='ms_paypublish_plan_field'>
  <div class='ms_paypublish_plan_status'>
    <?php print t("This content is currently: @status", array('@status' => $status)); ?>
  </div>

  <?php if (!empty($actions)) { ?>
    <div class='ms_paypublish_plan_actions'>
      <span class='ms_paypublish_plan_label'><?php print t("Actions"); ?>:</span>
      <?php print implode(' | ', $actions); ?>
    </div>
  <?php } ?>
</div>
