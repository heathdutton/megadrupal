<?php
/**
 * @file
 * Theming for the 2gis company teaser view mode.
 */
?>
<?php if(!empty($choose_link)): ?>
  <div class="widget-2gis-title-action">
    <?php print $choose_link; ?>
  </div>
<?php endif; ?>

<?php if(!empty($back_link)): ?>
  <div class="widget-2gis-title-action">
    <?php print $back_link; ?>
  </div>
<?php endif; ?>

<?php if(!empty($data['name'])): ?>
  <div class="widget-2gis-title"><?php print $data['name']; ?></div>
<?php endif; ?>

<?php if(!empty($data['city_name']) && !empty($data['address'])): ?>
  <div class="widget-2gis-line">
    <i class="line-icon line-icon-address"></i>
    <?php print $data['city_name']; ?>, <?php print $data['address']; ?>
  </div>
<?php endif; ?>

<?php if(!empty($contacts)): ?>
  <?php foreach ($contacts as $name => $group): ?>
    <?php foreach ($group as $type => $items): ?>
      <?php foreach ($items as $item): ?>
        <div class="widget-2gis-line">
          <i class="line-icon line-icon-<?php print $type; ?>"></i>
          <?php print $item; ?>
        </div>
      <?php endforeach; ?>
    <?php endforeach; ?>
  <?php endforeach; ?>
<?php endif; ?>

<?php if(!empty($data['rubrics'])): ?>
  <div class="widget-2gis-line widget-2gis-line-direction">
    <?php print implode(', ', $data['rubrics']); ?>
  </div>
<?php endif; ?>
