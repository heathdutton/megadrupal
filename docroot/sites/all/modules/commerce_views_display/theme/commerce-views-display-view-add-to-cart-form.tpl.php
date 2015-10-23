<?php
/**
 * @file commerce-views-display-add-to-cart.tpl.php
 *
 * @ingroup views_templates
 */
?>
<?php if (!empty($product_display_field_elements_rendered) || !empty($add_to_cart_form_rendered)): ?>
<div class="<?php print $classes; ?>">
  <?php if (!empty($product_display_field_elements_rendered)): ?>
    <div class="commerce-views-display-fields clearfix">
      <?php print $product_display_field_elements_rendered; ?>
    </div>
  <?php endif; ?>

  <?php if (!empty($add_to_cart_form_rendered)): ?>
    <div class="commerce-views-display-add-to-cart-form clearfix">
      <?php print $add_to_cart_form_rendered; ?>
    </div>
  <?php endif; ?>
</div>
<?php endif; ?>
