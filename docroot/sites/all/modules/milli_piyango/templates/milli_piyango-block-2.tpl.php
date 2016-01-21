<?php
/**
 * @file milli_piyango-block-2.tpl.php
 * This template handles the layout of the Sans Topu Block.
 *
 * Variables available:
 * - $items: An array of block values.
 * - $settings: An array of block settings.
 *
 */
?>

<?php if (!empty($settings['items']['val'])): ?>
<?php
  $plusone = $items['val']['plusone']->item(0)->nodeValue;
  array_pop($items['val']);
?>
  <div class="values">
    <?php foreach ($items['val'] as $val): ?>
      <?php print check_plain($val->item(0)->nodeValue); ?>&nbsp;
    <?php endforeach; ?>+ <?php print $plusone;?>
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

<?php if (!empty($settings['items']['w_fiveplusone']) && !empty($items['winners']['fiveplusone']->item(0)->nodeValue)): ?>
  <div class="score">
    <strong><?php print t('Score 5+1:') ?></strong> <?php print check_plain($items['winners']['fiveplusone']->item(0)->nodeValue); ?>
  </div>
<?php endif; ?>

<?php if (!empty($settings['items']['w_five']) && !empty($items['winners']['five']->item(0)->nodeValue)): ?>
  <div class="score">
    <strong><?php print t('Score 5:') ?></strong> <?php print check_plain($items['winners']['five']->item(0)->nodeValue); ?>
  </div>
<?php endif; ?>

<?php if (!empty($settings['items']['w_fourplusone']) && !empty($items['winners']['fourplusone']->item(0)->nodeValue)): ?>
  <div class="score">
    <strong><?php print t('Score 4+1:') ?></strong> <?php print check_plain($items['winners']['fourplusone']->item(0)->nodeValue); ?>
  </div>
<?php endif; ?>

<?php if (!empty($settings['items']['w_four']) && !empty($items['winners']['four']->item(0)->nodeValue)): ?>
  <div class="score">
    <strong><?php print t('Score 4:') ?></strong> <?php print check_plain($items['winners']['four']->item(0)->nodeValue); ?>
  </div>
<?php endif; ?>

<?php if (!empty($settings['items']['w_threeplusone']) && !empty($items['winners']['threeplusone']->item(0)->nodeValue)): ?>
  <div class="score">
    <strong><?php print t('Score 3+1:') ?></strong> <?php print check_plain($items['winners']['threeplusone']->item(0)->nodeValue); ?>
  </div>
<?php endif; ?>

<?php if (!empty($settings['items']['w_three']) && !empty($items['winners']['three']->item(0)->nodeValue)): ?>
  <div class="score">
    <strong><?php print t('Score 3:') ?></strong> <?php print check_plain($items['winners']['three']->item(0)->nodeValue); ?>
  </div>
<?php endif; ?>

<?php if (!empty($settings['items']['w_twoplusone']) && !empty($items['winners']['twoplusone']->item(0)->nodeValue)): ?>
  <div class="score">
    <strong><?php print t('Score 2+1:') ?></strong> <?php print check_plain($items['winners']['twoplusone']->item(0)->nodeValue); ?>
  </div>
<?php endif; ?>

<?php if (!empty($settings['items']['w_oneplusone']) && !empty($items['winners']['oneplusone']->item(0)->nodeValue)): ?>
  <div class="score">
    <strong><?php print t('Score 1+1:') ?></strong> <?php print check_plain($items['winners']['oneplusone']->item(0)->nodeValue); ?>
  </div>
<?php endif; ?>

<?php if (!empty($settings['items']['b_fiveplusone']) && !empty($items['bonus']['fiveplusone']->item(0)->nodeValue)): ?>
  <div class="bonus">
    <strong><?php print t('5+1 Bonus:') ?></strong> <?php print check_plain($items['bonus']['fiveplusone']->item(0)->nodeValue); ?>
  </div>
<?php endif; ?>

<?php if (!empty($settings['items']['b_five']) && !empty($items['bonus']['five']->item(0)->nodeValue)): ?>
  <div class="bonus">
    <strong><?php print t('5 Bonus:') ?></strong> <?php print check_plain($items['bonus']['five']->item(0)->nodeValue); ?>
  </div>
<?php endif; ?>

<?php if (!empty($settings['items']['b_fourplusone']) && !empty($items['bonus']['fourplusone']->item(0)->nodeValue)): ?>
  <div class="bonus">
    <strong><?php print t('4+1 Bonus:') ?></strong> <?php print check_plain($items['bonus']['fourplusone']->item(0)->nodeValue); ?>
  </div>
<?php endif; ?>

<?php if (!empty($settings['items']['b_four']) && !empty($items['bonus']['four']->item(0)->nodeValue)): ?>
  <div class="bonus">
    <strong><?php print t('4 Bonus:') ?></strong> <?php print check_plain($items['bonus']['four']->item(0)->nodeValue); ?>
  </div>
<?php endif; ?>

<?php if (!empty($settings['items']['b_threeplusone']) && !empty($items['bonus']['threeplusone']->item(0)->nodeValue)): ?>
  <div class="bonus">
    <strong><?php print t('3+1 Bonus:') ?></strong> <?php print check_plain($items['bonus']['threeplusone']->item(0)->nodeValue); ?>
  </div>
<?php endif; ?>

<?php if (!empty($settings['items']['b_three']) && !empty($items['bonus']['three']->item(0)->nodeValue)): ?>
  <div class="bonus">
    <strong><?php print t('3 Bonus:') ?></strong> <?php print check_plain($items['bonus']['three']->item(0)->nodeValue); ?>
  </div>
<?php endif; ?>

<?php if (!empty($settings['items']['b_twoplusone']) && !empty($items['bonus']['twoplusone']->item(0)->nodeValue)): ?>
  <div class="bonus">
    <strong><?php print t('2+1 Bonus:') ?></strong> <?php print check_plain($items['bonus']['twoplusone']->item(0)->nodeValue); ?>
  </div>
<?php endif; ?>

<?php if (!empty($settings['items']['b_oneplusone']) && !empty($items['bonus']['oneplusone']->item(0)->nodeValue)): ?>
  <div class="bonus">
    <strong><?php print t('1+1 Bonus:') ?></strong> <?php print check_plain($items['bonus']['oneplusone']->item(0)->nodeValue); ?>
  </div>
<?php endif; ?>