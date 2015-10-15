-- SUMMARY --

The Commerce Australia module provides a number of customisations for
Drupal Commerce (http://drupal.org/project/commerce) that allow it to
be better suited for deployment in Australia.

Most notably, these include basic currency formatting for Australia
and workable GST rules.

While these configurations are relatively trivial to make or code,
these modules aim to simplify the deployment process into common
reusable chunks.

The module consists of a main module (which actually does nothing)
and currently two sub-modules:

 -- commerce_australia_currencydisplay
 The default Australian Currency display is 10.00 AUD. This module
 changes the display so that it is $10.00 (as most Australians expect)

 -- commerce_australia_gst
 This adds a default currency configuration and rule set for GST that
 only applies within Australia

-- INSTALLATION --

Install the submodules via the normal way. As the modules are completely
separate and only depend on Drupal Commerce, you may enable them separately.

A note for those installing the gst module - this module creates a tax
type and a tax rate with a machine name of 'gst'. Please ensure that
you do not have any tax types or rates with these names when you install
the module.

-- CUSTOMISATION --

To change the way that GST is calculated, edit the rules from within the
Rules customisation screen (Configuration->Rules) or the Tax Configuration
(Store -> Configuration -> Taxes). You can revert these rules to default
at any stage if you are unhappy with your changes.

To change the display of the currency, disable the currencydisplay module
and create your own module that implements hook_commerce_currency_info_alter
to adjust the display style as you wish. 

-- UNINSTALL --

When you uninstall the commerce_australia_gst module, it will not remove
the GST settings from the UI, as it is conceivable that a user may wish
the types to remain. If you no longer wish to have these types, please
manually delete them from the UI.