The point of this module is to make it easy for a user to see which node or 
nodes display a particular Commerce product.

This is achieved by altering the default Commerce module 'Products' view 
(admin/structure/views/view/commerce_products/edit), adding a field listing the
IDs of nodes that reference each product listed by the view, and also by adding 
a fieldset listing links to any product display node(s) to the product edit page
(admin/commerce/products/<product_id>).

The module was created to help Commerce product administrators be able to easily
see where/how the products they are administering are displayed on their site,
without having to do separate searches for each listed product display node(s).

This module adds another property to the Commerce product entity,
'display_node_ids', which is a list of which nodes are configured to display the
product. The view alteration is achieved with the help of a custom entity views
field handler.

If required, you can access the property added to the Commerce product entity 
programatically, in the following manner:

  ...
  $product_id = 1;
  $product_wrapper = entity_metadata_wrapper('commerce_product', $product_id);
  $display_node_ids = $product_wrapper->display_node_ids->value();
  foreach ($display_node_ids as $nid) {
    print 'nid: ' . $nid;
  }
  ...
