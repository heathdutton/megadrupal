<?php

/**
 * @file
 * Contains themed output of the ad form.
 *
 * Available variables
 * -------------------
 * $form The form to be themed.
 */

  $keywords = drupal_render($form['keywords_title']) . drupal_render($form['keywords']);
  $keywords_order = drupal_render($form['enabled_keywords']);
  $tiers = drupal_render($form['tiers_title']) . drupal_render($form['tiers']);
  $tiers_order = drupal_render($form['enabled_tiers']);
  $buttons = drupal_render($form['buttons']);
?>
<?php print drupal_render_children($form); ?>
<div id="google-dfp-tabs">
  <ul>
    <li><a href="#tiers-tab"><?php print t('Tiers');?></a></li>
    <li><a href="#keywords-tab"><?php print t('Keywords');?></a></li>
  </ul>
  <div id="tiers-tab">
    <?php print $tiers_order . $tiers;?>
  </div>
  <div id="keywords-tab">
    <?php print $keywords_order . $keywords;?>
  </div>
</div>
<?php print $buttons;?>
