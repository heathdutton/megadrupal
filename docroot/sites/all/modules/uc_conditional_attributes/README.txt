Conditional Attributes (uc_conditional_attributes)

What does uc_conditional_attributes do?
---------------------------------------
uc_conditional_attributes can be used to define dependency relationships between
options and attributes for each product class. Once established, these
dependencies will be applied to all products of that class. Dependent attributes
will only be shown (for "enable"-type dependencies) or hidden (for "disable"
type dependencies) once the option on the parent attribute is selected.

For example, say you run a pizza online store and you offer a free drink when
the user orders a large pizza. Your pizza product class could have a 'Size'
attribute with options Small/Medium/Large and a 'Drink' attribute with options
for various popular soda brands. uc_conditional_attributes could be used to
define 'Size' as a parent attribute with option 'Large' enabling 'Drink'. The
drink selection would then remain hidden until the time when the user selects
'Large' as their choice for 'Size'.


Features of uc_conditional_attributes
-------------------------------------
* Supports two dependency modes: Disable (always show until dependency matches)
  and Enable (always hide until dependency matches). Examples, given attributes
  "Parent" and "Child" with options "Yes" and "No" for both:
    i)  If the attribute Child has a dependency on attribute Parent with option
        Yes and type Disable, then ONLY when Yes is chosen for Parent, Child
        will be hidden and disabled. If Yes is the default option, this will
        even occur upon page load.
    ii) If the attribute Child has a dependency on attribute Parent with option
        Yes and type Enable, then ONLY when Yes is chosen for Parent, Child will
        be shown and enabled. At all other times, it will be hidden and
        disabled. If Yes is the default option, this will even occur upon page
        load.

* Supports nested dependencies. For example, given attributes "A", "B" and "C"
  with each options "Yes" and "No" and these rules:
    i)  A: Yes -> {Disables} -> B
    ii) B: Yes -> {Disables} -> C
  In this setup, when A is set to Yes it will disable B which will in turn
  disable C, regardless of B's state. The same is true for Enable dependencies.

* Designed in a way so that even "Required" attributes can be disabled and
  hidden if necessary. A null value will be assigned to these attributes when
  customers put the product in shopping cart so that Ubercart will not complain.

* Establishes the dependencies between Attributes and Options in on the
  product-class level. No definition is needed on product level.


Considerations and limitations
------------------------------
* Designed for select-type attributes only. Only select drop-down menu attribute
  types can be set as parents. You can, however, use a select attribute to
  disable or enable any other attribute type such as checkboxes or radio
  buttons.

* It is not possible to customize dependencies at a per-product level.


Default assumptions and behaviour explained
-------------------------------------------
uc_conditional_attributes makes these assumptions:
  * for undefined attributes / options, display as normal (do not show or hide)
  * if a dependency is defined as "Enable", then only when the depending option
    is selected will the dependent attribute be shown; otherwise it will be
    disabled.
  * if a dependency is defined as "Disable", then only when the depending option
    is selected will the dependent attribute be hidden; otherwise it will be
    shown as normal.

For example, given "Color" with options "Red" and "Green" and an attribute
"Delivery Time" (specific options are irrelevant for this example):
  i)  Color: Green -> {Enable} -> Delivery time
  ii) Color: Red -> {Disable} -> Delivery time
Definition (ii) is unnecessary, as the assumptions made for Enable relationships
will ensure that "Delivery Time" is hidden at all times /unless/ Green is
selected.


Credits and License:
--------------------
Licensed under GNU General Public License (GNU GPLv2+).

Current development of this module is sponsored by Grindflow Management LLC.

This module is based on a fork of uc_cano [1], originally developed by Vizteck
Solutions and sponsored by PromDressExpress.com.

[1] http://www.ubercart.org/project/uc_cano
