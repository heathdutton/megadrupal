<?php

/**
 * @file
 * Default theme implementation for displaying affiliate store product page.
 *
 * This template can be overridden by templates below in increasing specificity:
 * - affiliate-store--{vocabulary name}.tpl.php
 * - affiliate-store--{vocabulary name}--{term name}.tpl.php
 * - affiliate-store--{term ID}.tpl.php
 * where:
 * - vocabulary name: One of network, merchant, category, subcategory, or
 *   subsubcategory.
 * - term name: Same as $term_name below but in lowercase.
 * - term ID: Same as $tid below.
 *
 * Available variables:
 * - $message: Storefront message.
 * - $browse_title: Browse fieldset title.
 * - $browse_items: Browse fieldset category links.
 * - $browse: Fieldset for browsing products based on list of category links
 *   output from theme_item_list().
 * - $products: Product nodes from node_view_multiple() that match the current
 *   filter criteria. Pager will be included automatically.
 * - $term: Full term object, empty when at storefront. Contains data that may
 *   not be safe.
 * - $term_name: Name of term, or its synonym if there is one. Empty when at
 *   storefront.
 * - $tid: Product term ID, 0 when at storefront.
 * - $vid: Product vocabulary ID, 0 when at storefront.
 *
 * Other variables:
 * - $classes_array: Array of HTML class attribute values. It is flattened
 *   into a string within the variable $classes.
 * - $user: Full user object. Contains data that may not be safe.
 *
 * Page status variables:
 * - $is_front: Flags true when presented in the front page.
 * - $logged_in: Flags true when the current user is a logged-in member.
 * - $is_admin: Flags true when the current user is an administrator.
 *
 * @see template_preprocess_affiliate_store()
 */
?>
<div id="affiliate-store">
  <?php if ($message): ?>
    <div class="affiliate-store-message">
      <?php print $message; ?>
    </div>
  <?php endif; ?>

  <div id="affiliate-store-term-<?php print $tid; ?>" class="affiliate-store-vocab-<?php print $vid; ?>">
    <?php if ($browse): ?>
      <div class="affiliate-store-browse">
        <?php print $browse; ?>
      </div>
    <?php endif; ?>

    <div class="affiliate-store-content">
      <?php print $products; ?>
    </div>
  </div>
</div>
