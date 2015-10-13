<?php
/**
 * @file
 * Template for a custom layout: 3 columns and 9 columns in a 12 column susy framework
 */
?>
<div class="dv-custom dv-pane">
<div class="dv-custom-wrapper">
    <div class="logo-desc">
      <?php print $content['logo']; ?>
      <?php print $content['description']; ?>
    </div>
    <div class="map">
      <?php print $content['map']; ?>
      <div class="data-tags">
        <div class="data">
          <?php print $content['data']; ?>
        </div>
        <div class="tags">
          <?php print $content['tags']; ?>
        </div>
      </div>
    </div>
  </div>
  <div class="footer">
    <?php print $content['footer']; ?>
  </div>
</div>