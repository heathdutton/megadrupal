<?php
/**
 * @file
 *   Page generation callbacks for the Commerce bulk product creation module.
 */
/**
 * Page callback: Displays a list of product types that the user can create.
 *
 * Path: admin/commerce/products/add-bulk
 *
 * @see commerce_bpc_menu()
 */
function commerce_bpc_add_page() {
  $item = menu_get_item();
  $content = system_admin_menu_block($item);

  // $content can only be empty if user has the 'administer product types'
  // permission.
  if (!$content && count(commerce_product_types()) > 0) {
      // If there is no $content, but there are products to create
      // that means that there no bulk-able product types.
        return '<p>' . t('None of the product types you have created so far are available for bulk creation. In order to be avalable for bulk creation, a product type must have at least one field that supports it, such as a list field. Go to the <a href="@manage-product-types">manage product types page</a> to add fields to your product types.', array('@manage-product-types' => url('admin/commerce/products/types'))) . '</p>';
  }
  return theme('product_add_list', array('content' => $content));
}
