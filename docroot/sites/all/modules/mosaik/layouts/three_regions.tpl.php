<?php
/**
 * @file
 * three_regions.tpl.php
 *
 * Three regions Mosaik layout.
 * It provides the following region variables:
 * - $top
 * - $content
 * - $right
 */
?>
<div id="mosaik-layout-three-regions">
  <div id="top-region" class="mosaik-region">
    <div class="mosaik-content-wrapper">
      <?php print $top ?>
    </div>
  </div>
  <div id="wrapper">
    <div id="content-region" class="mosaik-region">
      <div class="mosaik-content-wrapper">
        <?php print $content ?>
      </div>
    </div>
    <div id="right-region" class="mosaik-region">
      <div class="mosaik-content-wrapper">
        <?php print $right ?>
      </div>
    </div>
    <div class="clearfix"></div>
  </div>
</div>