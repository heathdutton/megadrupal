<?php
/**
 * @file
 * Theme file to display calculator.
 */
?>
<noscript><p>Calc requires javascript to be enabled to work.</p></noscript>
<div id="calc-wrapper" style="display:none;">
<div id="calcFormResult"><?php print drupal_render($form['calc_result']); ?></div>

<div class="clear_both"></div>
<div class="calcFormItem"><?php print drupal_render($form['calc_b7']); ?></div>
<div class="calcFormItem"><?php print drupal_render($form['calc_b8']); ?></div>
<div class="calcFormItem"><?php print drupal_render($form['calc_b9']); ?></div>
<div class="calcFormItem"><?php print drupal_render($form['calc_div']); ?></div>
<div class="calcFormItem"><?php print drupal_render($form['calc_clr']); ?></div>
<div class="clear_both"></div>

<div class="calcFormItem"><?php print drupal_render($form['calc_b4']); ?></div>
<div class="calcFormItem"><?php print drupal_render($form['calc_b5']); ?></div>
<div class="calcFormItem"><?php print drupal_render($form['calc_b6']); ?></div>
<div class="calcFormItem"><?php print drupal_render($form['calc_mul']); ?></div>
<div class="calcFormItem"><?php print drupal_render($form['calc_prc']); ?></div>
<div class="clear_both"></div>

<div class="calcFormItem"><?php print drupal_render($form['calc_b1']); ?></div>
<div class="calcFormItem"><?php print drupal_render($form['calc_b2']); ?></div>
<div class="calcFormItem"><?php print drupal_render($form['calc_b3']); ?></div>
<div class="calcFormItem"><?php print drupal_render($form['calc_min']); ?></div>
<div class="calcFormItem"><?php print drupal_render($form['calc_rep']); ?></div>
<div class="clear_both"></div>

<div class="calcFormItem"><?php print drupal_render($form['calc_b0']); ?></div>
<div class="calcFormItem"><?php print drupal_render($form['calc_sgn']); ?></div>
<div class="calcFormItem"><?php print drupal_render($form['calc_pnt']); ?></div>
<div class="calcFormItem"><?php print drupal_render($form['calc_pls']); ?></div>
<div class="calcFormItem"><?php print drupal_render($form['calc_eql']); ?></div>

<?php print drupal_render_children($form); ?>
</div>
