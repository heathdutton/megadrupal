<?php
/**
 * @file
 * Default theme implementation for thermometer displays.
 *
 * @see template_preprocess_redhen_campaign_thermometer()
 */
?>
<style type="text/css">
  .redhen-campaign-thermo-percent-<?php print $id; ?> { width: <?php print $percent; ?>%; }
</style>

<div class="redhen-campaign-thermo-percent-bar-wrapper">
  <div class="redhen-campaign-thermo-percent-bar redhen-campaign-thermo-percent-<?php print $id; ?>"></div>
</div>
