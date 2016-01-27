UC Shipment-Payment Link
http://drupal.org/project/uc_shipment_payment_link
-----

UC Shipment-Payment Link allows you specify which shipment and payment methods
should be used together. When a customer selects a shipment method during
checkout, only the payment methods you selected will remain visible, those you
specified as inappropriate will be removed automatically.

After the usual installation of the module, you will find its settings form in
Administer > Store administration > Configuration > Shipment-Payment Link
settings. Just follow the help instructions at the top of the form and put the
checkboxes in the desired locations.

Note that, due to an inherent limitation of how checkout panes appear in
UberCart, you cannot "Use collapsing checkout panes with next buttons during
checkout" in Store Administration > Configuration > Checkout settings. The
reason is that if you do so, the checkout pane appearing later (usually the
payment method pane) is not yet created and ready when the user choses among
shipping quotes. You need to have both panes already open once before the module
can cross-link them and make modifications to the second pane based on the user
selection in the first one.
