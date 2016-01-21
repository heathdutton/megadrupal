<?php
/**
 * @file
 * Theming for the 2gis company teaser view mode.
 */
?>
<li class="widget-2gis-list-item">
  <?php if(!empty($company_title)): ?>
    <?php print $company_title; ?>
  <?php endif; ?>
  <?php if(!empty($data['city_name']) && !empty($data['address'])): ?>
    <div class="widget-2gis-list-item-address">
      <?php print $data['city_name']; ?>, <?php print $data['address']; ?><br/>
    </div>
  <?php endif; ?>
</li>
