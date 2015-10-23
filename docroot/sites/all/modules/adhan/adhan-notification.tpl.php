<?php
  /**
   * @file
   * Default theme implementation for the Adhan notification
   *
   * Available variables:
   * - $prayer: The number of the prayer for which it is time now
   * - $label: The name of the current prayer
   * - $adhan: Rendered audio element which will play the actual adhan
   *
   * @ingroup themeable
   */
?>

<?php print t("It is now time for the @prayer prayer.", array('@prayer' => $label)); ?>

<?php if (variable_get('adhan_notify', 0) == 2): ?>
  <?php print $adhan; ?>
<?php endif; ?>