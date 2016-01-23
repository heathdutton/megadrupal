What does uc_product_as_attribute_option do?
--------------------------------------------
* Allows for the association attribute options to other product nodes
* Creates a new Attribute Option view mode (under Manage Display)
  with its own independent display settings
* Renders the product node (using the Attribute Option display settings)
  associated to an attribute option underneath it
* Manages stock of the associated product nodes when a customer when its
  attribute option is selected


Sample use cases
----------------
* Your store sells a barebones product with add-ons, and you wish to describe
  the difference between each attribute option to the customer instead of having
  a mere list of labels.
* Your store needs to manage stock for each attribute option independently
  instead of having configurations. For example, a customer ordering a product A
  with options B and C would decrement the stock A, B and C together (this is
  instead of the native Ubercart behavior where A with option B and C gets
  assigned a single SKU).
* You want to expose product bundle builder with features like Product Kits to
  users.


Installation
------------
Simply copy the uc_product_as_attribute_option folder into your
sites/all/modules folder and then browse to admin/build/module to enable it.


Configuration
-------------

  Add an attribute
  ----------------
  Add an attribute to the store as usual. For more information, see:
  https://www.drupal.org/node/1311856


  Configure attribute options
  ---------------------------
  When adding or editing an option, you may elect to search for nodes to
  associate the product with using the "Product association" text entry.
  Associations between objects and nodes are valid for any product in your store
  It could be a regular product, a kit, or any other node belonging to a product
  class. In fact, it is recommended that you setup a product type specifically
  for association with attribute options in order to ensure maximum flexibility.

  Entering text into the "Product association" entry will offer auto-complete
  suggestions for all product nodes by title.

  When a product node is associated to an option, its pricing information will
  be used to control the attribute adjustments. It will also be rendered
  alongside the attribute option on nodes (using the Attribute Option display
  mode).


  Configure the Attribute Options display mode
  --------------------------------------------
  As previously mentioned, associated product nodes are rendered next to its
  option on products containing attributes. To control the way the associated
  node is rendered, adjust the display settings to your taste. All of Drupal's
  fields and field formatters are at your disposal.

  Note that display of associated product nodes can be further customized by
  writing a specific template for the view mode, or by using modules such as
  Display Suite or Panelizer.


  Configure cart and checkout behavior
  ------------------------------------
  The manner in which option associated product nodes appear in the cart and
  checkout is highly customizable.

  Browse to Admin > Store > Configuration > Products and select the "Product as
  attribute option settings" vertical tab. Here select one of the four possible
  configurations:
    * No change:
      * Identical to stock attribute behavior; the single product is added to
        the cart and the attribute configuration is printed in its description.
      * Stock is only managed for the product with attributes in this mode.

    * Stock management only:
      * Same as "No change", preserves stock attribute during cart and checkout.
      * Adds a verification when checkout completes and decrements the stock for
        any product nodes associated to attribute options for products on the
        order.

    * Itemize as a unit:
      * Behaves similar to product kits, wherein the product with attributes and
        the associated option products for selected attribute options are added
        to the cart. They will be itemized both on checkout and on the resulting
        order, however while in the cart the customer ONLY sees the single
        product with its attribute description. The associated products are NOT
        visible nor modifiable in the cart.
      * Stock management is natively supported, as the option associated
        products become actual line items on the order.

    * Itemize as individual products:
      * The product with attributes and each of its associated items are added
        and itemized in the cart, on checkout and on the resulting order.
        Customers can add, remove, or change the quantity of the associated
        products in the cart.
      * It remains up to the developer of the site to handle behavior in the
        event that the customer modifies quantities or removes associated
        products from the order.
      * Stock management is natively supported, as the option associated
        products become actual line items on the order.

  Consider carefully which option is best for you. Note that without itemization
  an option association product will NOT appear on orders, sales reports, etc.

  The same applies for stock management; with "No change", a sale only affects
  the stock of the product with attributes and not the stock of products
  associated to its options.


Caveats and known bugs
----------------------
* Stock management for associated options is only handled on checkout completion
  (via stock level decrements). Due to the fact both the options and the nodes
  associated to them may change at any point in time, there is no guarantee that
  the options will have the same configuration as when the order was made. Thus
  upon order cancellation, stock levels are not incremented.
* Currently, products can only be associated to nodes at a global level. In
  other words, the node association to an option cannot be customized per-node
  or per-class. This is not a technical limitation, but rather this feature has
  not been sponsored yet. Patches are welcome!
* At the moment, only the radio buttons attribute type is supported.

Credits and License:
--------------------
Licensed under GNU General Public License (GNU GPLv2+).

Development of this module was performed by Grindflow Management LLC [1] and was
sponsored by pcmarket.com.au in association with Friction Networks Pty Ltd [2].

[1] http://grindflow.com
[2] http://friction.com.au
