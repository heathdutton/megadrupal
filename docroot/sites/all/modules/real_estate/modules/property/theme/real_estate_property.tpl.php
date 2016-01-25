<?php

/**
 * @file
 * A basic template for real_estate_property entities
 */
?>
<div id="node-<?php print $node->nid; ?>" class="<?php print $classes; ?> clearfix dre-property"<?php print $attributes; ?>>
  <?php if (!$page): ?>
    <h2<?php print $title_attributes; ?>>
      <a href="<?php print $node_url; ?>"><?php print $title; ?></a>
    </h2>
  <?php endif; ?>

  <div id="real-estate-media">
    <?php
      print render($content['property_photo']);
    ?>
  </div>
  <div id="real-estate-values">
    <div class="prop-price-wrapper">
      <?php
        print render($content['property_purpose']);
        print render($content['property_price']);
      ?>
    </div>
    <?php
      print render($content['property_bathrooms']);
      print render($content['property_bedrooms']);
      print render($content['property_lot_size']);
      print render($content['property_sq_footage']);
      print render($content['property_yearbuilt']);
    ?>
  </div>
  <div id="real-estate-description">
    <h3 class="real-estate-description-h"><?php print t('Description'); ?></h3>
    <div class="real-estate-description-b">
      <?php
        print render($content['body']);
      ?>
    </div>
  </div>
  <div class="content featured "<?php print $content_attributes; ?>>
    <?php
      hide($content['property_photo']);
      hide($content['property_facilities']);
      hide($content['property_suitables']);
      hide($content['comments']);
      hide($content['links']);
      print render($content);
    ?>
  </div>
  <div class="dre-property-more">
    <?php
      print render($content['property_facilities']);
      print render($content['property_suitables']);
    ?>
  </div>

  <?php print render($content['links']); ?>

  <?php print render($content['comments']); ?>

</div>
