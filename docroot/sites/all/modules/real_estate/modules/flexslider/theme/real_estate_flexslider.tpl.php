<?php
/**
 * @file
 * Template file for the Real Estate Flexslider module.
 */
?>
<div id="real_estate_flexslider">
  <div <?php print drupal_attributes($settings['attributes']) ?>>
    <?php print theme('flexslider_list', array('items' => $items, 'settings' => $settings)); ?>
  </div>
  <div <?php print drupal_attributes($settings['attributes_carousel']) ?>>
    <?php

    $settings['optionset'] = $settings['optionset_carousel'];
    print theme('flexslider_list', array('items' => $items_carousel, 'settings' => $settings));

    ?>
  </div>
</div>
