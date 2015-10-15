<?php
/**
 * @file milli_piyango-block-3.tpl.php
 * This template handles the layout of the On Numara Block.
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

<?php if (!empty($settings['items']['w_ten']) && !empty($items['winners']['ten']->item(0)->nodeValue)): ?>
  <div class="score">
    <strong><?php print t('Score 10:') ?></strong> <?php print check_plain($items['winners']['ten']->item(0)->nodeValue); ?>
  </div>
<?php endif; ?>

<?php if (!empty($settings['items']['w_nine']) && !empty($items['winners']['nine']->item(0)->nodeValue)): ?>
  <div class="score">
    <strong><?php print t('Score 9:') ?></strong> <?php print check_plain($items['winners']['nine']->item(0)->nodeValue); ?>
  </div>
<?php endif; ?>

<?php if (!empty($settings['items']['w_eight']) && !empty($items['winners']['eight']->item(0)->nodeValue)): ?>
  <div class="score">
    <strong><?php print t('Score 8:') ?></strong> <?php print check_plain($items['winners']['eight']->item(0)->nodeValue); ?>
  </div>
<?php endif; ?>

<?php if (!empty($settings['items']['w_seven']) && !empty($items['winners']['seven']->item(0)->nodeValue)): ?>
  <div class="score">
    <strong><?php print t('Score 7:') ?></strong> <?php print check_plain($items['winners']['seven']->item(0)->nodeValue); ?>
  </div>
<?php endif; ?>

<?php if (!empty($settings['items']['w_six']) && !empty($items['winners']['six']->item(0)->nodeValue)): ?>
  <div class="score">
    <strong><?php print t('Score 6:') ?></strong> <?php print check_plain($items['winners']['six']->item(0)->nodeValue); ?>
  </div>
<?php endif; ?>

<?php if (!empty($settings['items']['w_none']) && !empty($items['winners']['none']->item(0)->nodeValue)): ?>
  <div class="score">
    <strong><?php print t('Score 0:') ?></strong> <?php print check_plain($items['winners']['none']->item(0)->nodeValue); ?>
  </div>
<?php endif; ?>

<?php if (!empty($settings['items']['b_ten']) && !empty($items['bonus']['ten']->item(0)->nodeValue)): ?>
  <div class="bonus">
    <strong><?php print t('10 Bonus:') ?></strong> <?php print check_plain($items['bonus']['ten']->item(0)->nodeValue); ?>
  </div>
<?php endif; ?>

<?php if (!empty($settings['items']['b_nine']) && !empty($items['bonus']['nine']->item(0)->nodeValue)): ?>
  <div class="bonus">
    <strong><?php print t('9 Bonus:') ?></strong> <?php print check_plain($items['bonus']['nine']->item(0)->nodeValue); ?>
  </div>
<?php endif; ?>

<?php if (!empty($settings['items']['b_eight']) && !empty($items['bonus']['eight']->item(0)->nodeValue)): ?>
  <div class="bonus">
    <strong><?php print t('8 Bonus:') ?></strong> <?php print check_plain($items['bonus']['eight']->item(0)->nodeValue); ?>
  </div>
<?php endif; ?>

<?php if (!empty($settings['items']['b_seven']) && !empty($items['bonus']['seven']->item(0)->nodeValue)): ?>
  <div class="bonus">
    <strong><?php print t('7 Bonus:') ?></strong> <?php print check_plain($items['bonus']['seven']->item(0)->nodeValue); ?>
  </div>
<?php endif; ?>

<?php if (!empty($settings['items']['b_six']) && !empty($items['bonus']['six']->item(0)->nodeValue)): ?>
  <div class="bonus">
    <strong><?php print t('6 Bonus:') ?></strong> <?php print check_plain($items['bonus']['six']->item(0)->nodeValue); ?>
  </div>
<?php endif; ?>

<?php if (!empty($settings['items']['b_none']) && !empty($items['bonus']['none']->item(0)->nodeValue)): ?>
  <div class="bonus">
    <strong><?php print t('0 Bonus:') ?></strong> <?php print check_plain($items['bonus']['none']->item(0)->nodeValue); ?>
  </div>
<?php endif; ?>