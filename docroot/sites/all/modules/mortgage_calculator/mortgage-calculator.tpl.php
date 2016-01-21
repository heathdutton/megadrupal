<?php

/**
 * @file
 * Default implementation of a mortgage calculator result template.
 *
 * Available variables:
 * - $header: A header array as used by theme_table(),
 * - $rows: A rows array as used by theme_table(), containing calculation result per month/year
 * - $row['number_of_payments']: number of payments
 * - $row['payment']: annual payment
 *
 *
 * @see template_preprocess()
 * @see template_process()
 */
?>
<div class="mortgage-calculator">
  <?php print theme('table', array('header' => $header, 'rows' => $rows, 'attributes' => array('class' => array('mortgage-calculator-table')))); ?>
</div>
