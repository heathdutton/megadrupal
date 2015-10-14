
-- SUMMARY --

The BeautyTips Form Errors is a simple integration with popular
BeautyTips module, that provides you "Tooltip-like" form error
messages.

You can use default tooltip style for error messages as shown
on the image or create your custom style using BeautyTips
module features.

Feel free to give your feedback on any improvements.

For a full description of the module, visit the project page:
  http://drupal.org/project/bt_form_errors

To submit bug reports and feature suggestions, or to track changes:
  http://drupal.org/project/issues/bt_form_errors


-- REQUIREMENTS --

None.


-- INSTALLATION --

* Install as usual module.


-- CONFIGURATION --

* Configure user permissions in Administration » People » Permissions:

  - Access configuration page

    Users in roles with the "Administer BeautyTips Form Errors" permission
    will be able to change module settings.

* Customize beautytips appearance in Administration » Configuration »
  User interface » BeautyTips » BeautyTips Form Errors.


-- CUSTOMIZATION --

* To override the default text-shadow and links appearance/hover behaviour:

  Ovveride css class in your theme:

     .bt-error .bt-content { text-shadow: none } // For text disabling shadow

     .bt-error .bt-content a { /* any css */ } // To alter links


-- CONTACT --

Current maintainers:
* Artem Taranyuk (artem.taranyuk) - http://drupal.org/user/345192

This project has been sponsored by:
* Blink Reaction
  Blink Reaction is a leader in Enterprise Drupal Development, delivering
  robust, high-performance websites for dynamic companies.
  http://blinkreaction.com
