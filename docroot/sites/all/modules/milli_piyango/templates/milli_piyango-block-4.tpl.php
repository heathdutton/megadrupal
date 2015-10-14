<?php
/**
 * @file milli_piyango-block-4.tpl.php
 * This template handles the layout of the Super Loto Block.
 *
 * Variables available:
 * - $items: An array of block values.
 * - $settings: An array of block settings.
 *
 */
?>

<?php if (!empty($settings['items']['val'])): ?>
  <div class="values">
    <?php foreach ($items['val'] as $val): ?>
      <?php print check_plain($val->item(0)->nodeValue); ?>&nbsp;
    <?php endforeach; ?>
  </div>
<?php endif; ?>

<?php if (!empty($settings['items']['no']) && !empty($items['info']['no']->item(0)->nodeValue)): ?>
  <div class="drawing-no">
    <strong><?php print t('Drawing no:') ?></strong> <?php print check_plain($items['info']['no']->item(0)->nodeValue); ?>
  </div>
<?php endif; ?>

<?php if (!empty($settings['items']['date']) && !empty($items['info']['date']->item(0)->nodeValue)): ?>
  <div class="drawing-date">
    <strong><?php print t('Drawing date:') ?></strong> <?php print check_plain(_milli_piyango_date($items['info']['date']->item(0)->nodeValue)); ?>
  </div>
<?php endif; ?>

<?php if (!empty($settings['items']['col']) && !empty($items['info']['col']->item(0)->nodeValue)): ?>
  <div class="column-count">
    <strong><?php print t('Column Count:') ?></strong> <?php print check_plain($items['info']['col']->item(0)->nodeValue); ?>
  </div>
<?php endif; ?>

<?php if (!empty($settings['items']['rev']) && !empty($items['info']['rev']->item(0)->nodeValue)): ?>
  <div class="revenue">
    <strong><?php print t('Revenue:') ?></strong> <?php print check_plain($items['info']['rev']->item(0)->nodeValue); ?>
  </div>
<?php endif; ?>

<?php if (!empty($settings['items']['places']) && !empty($items['info']['places']->item(0)->nodeValue)): ?>
  <div class="winning-places">
    <strong><?php print t('Winning Places:') ?></strong> <?php print check_plain($items['info']['places']->item(0)->nodeValue); ?>
  </div>
<?php endif; ?>

<?php if (!empty($settings['items']['w_six']) && !empty($items['winners']['six']->item(0)->nodeValue)): ?>
  <div class="score">
    <strong><?php print t('Score 6:') ?></strong> <?php print check_plain($items['winners']['six']->item(0)->nodeValue); ?>
  </div>
<?php endif; ?>

<?php if (!empty($settings['items']['w_five']) && !empty($items['winners']['five']->item(0)->nodeValue)): ?>
  <div class="score">
    <strong><?php print t('Score 5:') ?></strong> <?php print check_plain($items['winners']['five']->item(0)->nodeValue); ?>
  </div>
<?php endif; ?>

<?php if (!empty($settings['items']['w_four']) && !empty($items['winners']['four']->item(0)->nodeValue)): ?>
  <div class="score">
    <strong><?php print t('Score 4:') ?></strong> <?php print check_plain($items['winners']['four']->item(0)->nodeValue); ?>
  </div>
<?php endif; ?>

<?php if (!empty($settings['items']['w_three']) && !empty($items['winners']['three']->item(0)->nodeValue)): ?>
  <div class="score">
    <strong><?php print t('Score 3:') ?></strong> <?php print check_plain($items['winners']['three']->item(0)->nodeValue); ?>
  </div>
<?php endif; ?>

<?php if (!empty($settings['items']['b_six']) && !empty($items['bonus']['six']->item(0)->nodeValue)): ?>
  <div class="bonus">
    <strong><?php print t('6 Bonus:') ?></strong> <?php print check_plain($items['bonus']['six']->item(0)->nodeValue); ?>
  </div>
<?php endif; ?>

<?php if (!empty($settings['items']['b_five']) && !empty($items['bonus']['five']->item(0)->nodeValue)): ?>
  <div class="bonus">
    <strong><?php print t('5 Bonus:') ?></strong> <?php print check_plain($items['bonus']['five']->item(0)->nodeValue); ?>
  </div>
<?php endif; ?>

<?php if (!empty($settings['items']['b_four']) && !empty($items['bonus']['four']->item(0)->nodeValue)): ?>
  <div class="bonus">
    <strong><?php print t('4 Bonus:') ?></strong> <?php print check_plain($items['bonus']['four']->item(0)->nodeValue); ?>
  </div>
<?php endif; ?>

<?php if (!empty($settings['items']['b_three']) && !empty($items['bonus']['three']->item(0)->nodeValue)): ?>
  <div class="bonus">
    <strong><?php print t('3 Bonus:') ?></strong> <?php print check_plain($items['bonus']['three']->item(0)->nodeValue); ?>
  </div>
<?php endif; ?>