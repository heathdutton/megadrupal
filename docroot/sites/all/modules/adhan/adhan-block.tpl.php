<?php
  /**
   * @file
   * Default theme implementation for rendering Adhan block
   *
   * Available variables:
   * - $info_table: A rendered table containing general information
   * - $error: If location wasn't detected, an error message, otherwise FALSE
   * - $time_table: A rendered table containing the prayer times
   * - $compass: The rendered compass image
   *
   * @ingroup themeable
   */
?>

<?php print $info_table; ?>

<?php if ($error): ?>
  <p class="adhan-error">
    <?php print $error; ?>
  </p>
<?php else: ?>
  &nbsp;
<?php endif; ?>

<?php print $time_table; ?>

<?php print $compass; ?>

<div id="adhan-dialog"></div>