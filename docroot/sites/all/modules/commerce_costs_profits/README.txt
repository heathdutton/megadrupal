Commerce Costs Profits
===============

Description
-----------

Commerce Costs Profits module provides product costs and order profits
functionality for Drupal Commerce
(http://drupal.org/project/commerce).

Module adds Cost field to products entities and line items to be able
to expose profits in different reports.
By default all declared product type and line items type are affected by this
module.
To prevent line item type or product type from being processed by this module
and additional fields being attached special flags can be used in entities
declarations:

function YOUR_MODULE_commerce_product_type_info() {
  return array(
    'your_product_type_1' => array(
      'type' => 'your_product_type_1',
      'name' => 'Type name 1',
      'disable costs profits line item editing' => true,
    ),
    'your_product_type_2' => array(
      'type' => 'your_product_type_2',
      'name' => 'Type name 2',
      'disable costs profits' => true,
    ),
  );
}

So costs of line items of the first product type won't be editable while
products of the second type won't have cost field attached at all.
Same flags can be used in line type declarations.

Module replaces default commerce line item manager to let editing
of line item cost and unit price in admin area and see margins and
order total changing in realtime.
Special set of permissions is introduced to fine tune which parts of line item's
data are editable.

Two fields are added to order instances to collect expenses and calculate total
order profit for later exposing in reports.

Two handlers are implemented for views: line item profit handler for view's cell
and area handler for line items profits summary.

Dependencies
------------

Drupal Commerce and all of its dependencies
Entity Reference
Views
Variable


Configuration
-------------

- Commerce Costs Profits permissions

  Home > Administration > People > Permissions
  (admin/people/permissions#module-commerce_costs_profits)

- Commerce Costs Profits configuration

  Home > Administration > Store > Configuration > Costs and profits
  (admin/commerce/config/costs_profits)

  Automatic product price recalculation on cost change (turned on by default)
  and default product margin can be configured.

