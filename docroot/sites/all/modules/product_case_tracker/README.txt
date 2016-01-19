

REQUIREMENTS
------------

Product case tracker module is dependent on ubercart module's
uc_product - http://drupal.org/project/ubercart



INSTALLATION
------------

  - Download and extract the module files.
  - Enable the module from /admin/modules. Module will only
    get enabled if uc_product is enabled.
  - Access /admin/store/settings/case_tracker to change the settings
    for this module, or to set the initial configuration settings.
  - In the settings page, select the content types of 'product' class
    on which case tracker will work upon. These product classes
    can be created at /admin/store/products/classes.
  - Select specific roles for the product designer and customer.



DESCRIPTION
-----------

The module is a bridge between product designer and the customer.
Product case tracker module is designed and developed keeping a
commissioned product in mind. This module is not,  in any way,
concerned with tracking the product after it has been purchased,
it keeps a track of the product-making and how far this
development has reached, via product status.

If a customer, on any given ecommerce site,
likes a product but is not satisfied with the
product color, design, shape, or look and feel, etc. of that product,
given that the customer has some potential interest in that product,
this product can be customized according to customer's indivisual
needs. So the customer orders for customizations in the product and
the ecommerce store/website can easily handle this commissioned
order by using this module.

The module provides a status bar, a diagrammatic view of the product
statuses, and provides a case history table which keeps a
track of all the history related to this product.
When a product is created, there is a 'View Case Tracker' link
available in /admin/content. Click this link to visit the 
case tracker of this commissioned product.

A demo of product development status that can be considered :
  -   Product enquiry by customer
  -   Contract Drafted by Product Designer
  -   Contract Accepted by Customer
  -   Initial Payment by Customer
  -   Delivery Agreement drafted
  -   Full Payment by Customer
  -   Product Delivered
  -   Balance Paid to Artisan



CASE TRACKER GUIDELINES
-----------------------

 * There must be a 'list(text)' field with 'select' widget in the content
type for ubercart's product class. Only if this field is selected in the 
settings page, case tracker will get started. This field will be editable and
valuable only in the case tracker page and nowhere else.

 * As soon as a product is created the super admin or the role with
the administrator rights (assuming this role is the product designer)
would be asked for timeline dates. The timeline dates are the actual
dates which the product designer promises to comply with.
Any deviation from these dates would be recorded in the timeline 
itself and reflect the product designer's performance.

 * Collaborators can be added or removed  by the product designer, and the
super admin only.

 * Only administrator, product designer, collaborators and customer of the
product can view the case tracker. No other user can access the case tracker
keeping in mind the security, and safeguarding the performance of product
designer from other product designers on the site.

 * Product designer can only be changed by the super admin.

 * Any given user can view his/her workspace at /user/<uid>.

 * There is a functionality to send an email with attachment from within the 
case tracker itself, if the customer or the product designer need to interact
via screenshots or product images.

 * Permissions to specific roles :
   -- For the product designer and customer roles, it is recommended
      that you provide 'View all commissioned orders', and 'Administer 
      timeline' permissions.
   -- You must provide 'Administer autocomplete entries' for product designer
      roles. This permission would only let the product designer access the
      other designers and customers on the site.

-------------------------------------------------------------------------------
-------------------------------------------------------------------------------
| ** TODO **                                                                  |
| Remove or hide the selected status field from node/<nid>/edit page, as it   |
| is currently of no use to the case tracker. Any changes made to this field  |
| in the node/edit page will be of no effect on the case tracker, only changes|
| made in the admin/<case_id>/product_case_tracker page will get reflected.   |
-------------------------------------------------------------------------------
-------------------------------------------------------------------------------


CREDITS
-------

* Developed and maintained by
  gauravjeet singh pahuja <gauravjeet at osscube dot com>

* Support and Guidance by
  bhupendra singh <bhupendra at osscube dot com>

* Product Timeline design assisted by
  abhijeet kalsi <abhijeet at osscube dot com>

* Sponsored by
  OSSCube
