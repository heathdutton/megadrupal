Commerce Views Pane is packaged for download at
http://drupal.org/project/commerce_views_pane.


Installing:
[1] Create a new View
[2] Add a new display and select "Commerce checkout pane"
[3] [Optional] add a contextual filters as you need.
[4] Build the view - fields, relationships, etc
[5] Save the view
[6] Visit "admin/commerce/config/checkout" and enable the new view.
    Example: "View: Test Checkout Pane", where "Test Checkout Pane" is
    the human-readable name of the view
[7] Configure argument values in pane configuration. You can use replacement
    patterns from Commerce Order entity.
    Example: Mapping to an Order state views argument use token
    "[commerce-order:state]"

