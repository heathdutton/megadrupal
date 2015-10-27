
Commerce Extra Panes
====================

This module allows you to place one or more nodes as checkout panes for
Drupal Commerce.
Commerce Extra panes provide checkout panes for nodes selected and also a
checkout_pane view mode for displaying the nodes.

Install instructions
====================
- Install Drupal Commerce and make sure that checkout module and CTools
  is enabled.
- Enable Commerce Extra panes.
- Access the Manage Display for those node types whose nodes are going to be
  exposed as checkout panes, click on "Checkout pane" view mode option and
  select the fields that are going to be displayed in the panes.
- Go to Store > Configuration > Checkout Settings and there click on
  Checkout Extra Panes tab.
- Select one of the existing nodes by typing its title in the autocomplete form
  and add it.
- Click on Checkout Form to select in which checkout phase the nodes should be
  displayed.

The module provides two tpl files, one for the checkout form phase and the
other for the review phase.

Token Support
=============
Tokens are now supported for node text replacement, there's no UI exposed but
you can use Commerce Order and Node replacement tokens.
Token module is recommended for field support.

Here's an example:

"Thank you for your purchase! Please wire [commerce-order:commerce_order_total]
to 555-COMMERCE-LOVE and use [commerce-order:order-number] as reference."