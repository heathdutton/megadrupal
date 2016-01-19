# Commerce Checkout Information

Adds the ability to add extra information to the checkout panes

## Installation

Place the module in your project, most likely it should go into sites/all/modules or sites/<domain>/modules.

Alternatively you have add it to your make file using the following (just replace <revision> with revision you choose to use).

projects[commerce_checkout_information][type] = module
projects[commerce_checkout_information][download][type] = git
projects[commerce_checkout_information][download][url] = git.drupal.org:sandbox/lslinnet/2186901.git
projects[commerce_checkout_information][download][revision] = <revision>

This module requires the commerce checkout module as it is those panes that it modifies.

## Usage

When you go to the "Store settings > checkout settings" (admin/commerce/config/checkout) and edit one of the checkout panes there is now a checkbox in the settings fieldset which when click reveals a text box which allows you to enter the information text that helps your user through the checkout flow.

### Theming

It is possible to overwrite the theming of the text by implementing "YOURTHEME_commerce_checkout_information_text" function.

It has 2 important variables called "text" and "attributes"

## Future plans

I have plans to implement a trigger and render selection functionality, the purpose of the trigger is to select when the information text is visible (hover, click or always shown).
Further it should be possible to how it is rendered when shown (inline, popup, overlay, tool tip etc), rendering options should of reflex which trigger is selected - e.g. "always shown" does only allow for inline, where as hover and click should allow for all 4 options.

