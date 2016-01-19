<?php
/**
 * @file
 * This template handles the layout of the Weathercomau widget block.
 *
 * Variables available:
 * - $wrapper_classes: (string) Classes to be used on the wrapper element.
 * - $title_classes: (string) Classes to be used on the title element.
 * - $title: (string) Actual title.
 * - $content_classes: (string) Classes to be used on the forecasts container element.
 * - $forecasts_classes: (array) Classes to be used on each individual forecast element.
 * - $forecasts: (array) Each forecast is a rendered forecast.
 *
 * @ingroup weathercomau_templates
 */
?>
<div class="<?php print $wrapper_classes; ?>">

  <h3 class="<?php print $title_classes; ?>"><?php print $title; ?></h3>

  <div class="<?php print $content_classes; ?>">
    <?php foreach ($forecasts as $id => $forecast): ?>
      <div class="<?php print $forecasts_classes[$id]; ?>">
        <?php print $forecast; ?>
      </div>
    <?php endforeach; ?>
  </div>

</div>
