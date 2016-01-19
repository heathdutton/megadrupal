# Commerce Order Counter

## Introduction

**Commerce Order Counter** is a module for
[Drupal Commerce](http://drupalcommerce.org) (or
[Commerce Kickstart](https://drupal.org/project/commerce_kickstart))
that implements a generic framework for having order numbers that are
**independent** of order IDs, which are used for referential integrity
at the schema level. 

The interest of such is for example in invoicing where invoices are
usually obliged to have a sequential numbering scheme. This module
defines a `CommerceOrderCounter` interface that allows you to
define **your own** order numbering system in a clean and simple way.

## Basic Commerce Order Counter

There's a *default* implementation of the the `CommerceOrderCounter`
interface provided by the module. It implements a simple integer
counter. The starting/current value is set at
`/admin/commerce/config/order-counter`. There you can set your the
value to be used now as the base. A common use case where such a thing
would be useful is when you place some orders when developing the site
and now you want to start a new series at 100 for example.

At `/admin/commerce/config/order-counter` you set the counter base to
**99**.

Now when placing an order and completing the checkout the order will
have the number **100**. So when invoicing the client you'll start
your invoice numbering at 100.

You can skip the numbers at your whim and/or business needs.

Note that at the DB level, the `order_id` column is sequential since
is an auto-increment field. This module realizes the possibility that
[Drupal Commerce](http://drupal.org/project/commerce) makes available
of **separating** order numbers from order IDs. By default they're the
same.

## Implementing your own order numbering scheme

You can implement an arbitrary numbering scheme using the available
interface. 

Besides that you need to add to your `settings.php`:

    // Make the module use my order numbering scheme.
    $conf['CommerceOrderCounterClass'] = 'YourCommerceOrderCounter';
   
where `YourCommerceOrderCounter` is the name of your class implementing
the `CommerceOrderCounter` interface.

Note that the configurable counter base value setable at
`admin/commerce/config/order-counter` should be in **accordance** with
the scheme you implemented. For example if your scheme is
`year-number`, starting at 2013, the initial value should be `2013-1`
(first order of 2013). 

## Rules 

The module installs a new rule that is invoked whenever the
`Completing the checkout process` event is fired. So when completing
the checkout at all stages up to the payment phase the rule will be
invoked and the order number incremented. At
`admin/commerce/config/order-counter` the value shown it will be the
new value (after updating).

Note also that the rules action that increments the order number is
configurable and you can use tokens. The configuration is acessible
through the following page trail:

    Home > Administration > Configuration > Workflow  > Rules  >
    Editing reaction rule "Increment the order counter upon checkout
    completion"  > Editing action "Update the order number"

By default it uses the token `[site:order-counter]` that references
the basic order counter.

## TODO

 1. Add a rule for incrementing the order number when adding an order
    directly through the interface at `admin/commerce/orders/add`.
    
 2. Implement as an example the `year-number` order numbering scheme.
 
## Acknowledgements 

The current work is sponsored by [CommerceGuys](http://commerceguys.com).
