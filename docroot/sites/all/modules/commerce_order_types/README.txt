This module provides a UI for site administrators to define new Commerce Order
types. Basically, what Commerce Custom Line Items is for Commerce line items.
Every order type defined by this module can have different fields, display
settings, permissions, etc. Every order type has its own "Create order" form.

How do I use this module?
The simplest way is via Rules. If you create a Rule that acts on the event
"Before saving a commerce order", you can add an action to "Set a data value"
and use the "commerce order: type" selector. Use whatever conditions you'd like
to specify what type the new order should be saved as; by default, it will still
get saved as a plain ol' commerce order. This works even if the user uses the
add to cart form.

The other way is through code; either by implementing
hook_commerce_order_presave() or custom code that calls commerce_order_new().

But why? Wouldn't it be easier to just _____?
Maybe (probably). Separating order types might make your life easier if your
store sells different kinds of products. For instance, you might want donation
products and physical goods to have different order workflows, have separate
views for reporting, etc. Or maybe you run a marketplace type setup, and each
of your merchants wants to collect different information on their orders.
Not that any of these things are not possible with one order type, but using
order types might make your rules/code/views a bit simpler down the road.
