
-- SUMMARY --

This module provides way to display progress towards a fundraising goal for
donations and products sold with Drupal Commerce. When configured for a
specific content type, editors will be able to enter a goal total amount and
select which Drupal Commerce products contribute towards that goal. A field
formatter is provided to display a progress bar of donations towards the
goal.

While this module was developed for use with the Commerce Donate module, it
does not require that products be donations. Any Commerce Product type will
work.


-- REQUIREMENTS --

Drupal Commerce - http://drupal.org/project/commerce


-- INSTALLATION --

* Install as usual. For further information, see:
https://drupal.org/documentation/install/modules-themes/modules-7


-- CONFIGURATION --

1) Install the module as usual.

2) Go to Structure > Content Types, and click Manage Fields next to the content
   type you want to display goals on. This will probably be the same one you are
   using as a Product Display. ("Product" or "Donation", for example)

3) Add a new field of type "Price". This will be used to hold the goal total.

4) If you don't already have a field of Product Reference type on the content
   type, add a field of that type, as well. This will be used to store references
   to the products that contribute towards the goal. (If this content type is
   set up as a Product Display, you should already have a Product Reference type
   field here, so you can skip this step.)

5) Go the Manage Display tab for this content type, make sure that your Goal field
   is not hidden, and then set the Format for it to "Commerce Goal Progress Bar".

6) Click the gear icon on the right to set the Product Reference Field to the field
   from step 4. Click Update, then Save.

7) At this point, you can add a goal amount to nodes of this type, select the products
   in the product reference, and a progress bar will be displayed in the node view
   that shows the total of all orders of the linked products.


-- CUSTOMIZATION --

* To change which order statuses are considered "complete" and should contribute
  towards the goal: Go to Store > Configuration > Goals.

* To change the design of the progress bar:

  1) You can override the CSS in a custom theme, or by using a module like CSS
     Injector. (http://drupal.org/project/css_injector)

  2) To change the HTML output, you can override the template file:
     commerce-goal-formatter-progress-bar.tpl.php


-- CONTACT --

Current maintainers:
* Wayne Eaker (zengenuity) - http://drupal.org/user/326925

This project has been sponsored by:
* Kubik Graphic Design Studio (http://www.kubikdesign.net/)


