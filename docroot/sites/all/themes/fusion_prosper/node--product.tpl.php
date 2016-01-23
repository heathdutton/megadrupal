<?php
?>

<div id="node-<?php print $node->nid; ?>" class="<?php print $classes; ?> clearfix"<?php print $attributes; ?>>
  <div class="inner">
    <?php print render($title_prefix); ?>
    <?php if (!$page): ?>
      <h2<?php print $title_attributes; ?>><a href="<?php print $node_url; ?>"><?php print $title; ?></a></h2>
    <?php endif; ?>
    <?php print render($title_suffix); ?>

    <?php if ($display_submitted || !empty($content['links']['terms'])): ?>
      <div class="meta">
        <?php if ($display_submitted && isset($submitted) && $submitted): ?>
          <span class="submitted"><?php print $submitted; ?></span>
        <?php endif; ?>

        <?php if (!empty($content['links']['terms'])): ?>
          <div class="terms terms-inline">
            <?php print render($content['links']['terms']); ?>
          </div>
        <?php endif; ?>
      </div>
    <?php endif; ?>

  <?php if (!$teaser): ?>
    <div id="node-top" class="node-top region nested">
      <?php print render($node_top); ?>
    </div>
  <?php endif; ?>

    <div id="product-group" class="product-group">
      <div class="images">
        <?php print $fusion_uc_image; ?>
      </div><!-- /images -->

      <div class="content clearfix">
        <div id="content-body">
          <?php print $fusion_uc_body; ?>
        </div>

        <div id="product-details" class="clear">
          <div id="field-group">
            <?php print $fusion_uc_weight; ?>
            <?php print $fusion_uc_dimensions; ?>
            <?php print $fusion_uc_sku; ?>
            <?php print $fusion_uc_list_price; ?>
            <?php print $fusion_uc_sell_price; ?>
            <?php print $fusion_uc_cost; ?>
          </div>
          
          <div id="price-group">
            <?php print $fusion_uc_display_price; ?>
            <?php print $fusion_uc_add_to_cart; ?>
          </div>

        </div><!-- /product-details -->

        <?php if ($fusion_uc_additional && !$teaser): ?>
          <div id="product-additional" class="product-additional">
            <?php print $fusion_uc_additional; ?>
          </div>
        <?php endif; ?>

        <?php print render($content['links']); ?>

        <?php print render($content['comments']); ?>
  
      </div><!-- /content -->
    </div><!-- /product-group -->
  </div><!-- /inner -->

  <?php if (!$teaser): ?>
    <div id="node-bottom" class="node-bottom region nested">
      <?php print render($node_bottom); ?>
    </div>
  <?php endif; ?>
</div>