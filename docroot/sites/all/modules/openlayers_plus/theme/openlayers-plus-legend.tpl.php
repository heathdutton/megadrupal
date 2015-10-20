<?php
/**
 * @file
 * Legend template.
 */

/**
 * @param $layer_id The layer's id
 * @param $layer layer array
 * @param $legend an array of legend items
 */
?>
<div class='legend legend-count-<?php print count($legend) ?> clear-block' id='openlayers-legend-<?php print $layer_id ?>'>
  <?php foreach ($legend as $key => $item): ?>
    <?php if (!empty($item['data']['fillColor']) && !empty($item['data']['strokeColor'])): ?>
    
    <?php

      $fillColor_rgb = _openlayers_plus_hex2rgb(check_plain($item['data']['fillColor']));
      $fillColor_rgba = "rgba(" . $fillColor_rgb['red'] . ", " . $fillColor_rgb['green'] . ", " . $fillColor_rgb['blue'] . ", " . $item['data']['fillOpacity'] . ")";
      $strokeColor_rgb = _openlayers_plus_hex2rgb(check_plain($item['data']['strokeColor']));
      $strokeColor_rgba = "rgba(" . $strokeColor_rgb['red'] . ", " . $strokeColor_rgb['green'] . ", " . $strokeColor_rgb['blue'] . ", " . $item['data']['strokeOpacity'] . ")";

      ?>
      <div class='legend-item clear-block'>
      <span class='swatch' style='border: <?php print $item['data']['strokeWidth']; ?>px solid <?php print $strokeColor_rgba; ?>; background: <?php print $fillColor_rgba; ?>'></span>
      <?php if (!empty($layer['title'])): ?>
       <?php print check_plain($item['title']) ?>
    </div>
      <?php endif; ?>
      
      <?php endif; ?>
      
      <?php if (!empty($item['data']['externalGraphic'])): ?>
    
      <div class='legend-item clear-block'>
      <img class='swatch' src='<?php print $item['data']['externalGraphic']; ?>' style='height: <?php print $item['data']['graphicHeight']; ?>; width: <?php print 'auto'; ?>'/>
      <?php if (!empty($layer['title'])): ?>
      <?php print check_plain($item['title']) ?>
      <?php endif; ?>
      </div>
      
      <?php endif; ?>
  <?php endforeach; ?>
</div>
