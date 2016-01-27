Area Print is a simple module that let's you add print button or link that sends any given element(with id) to printer. 
All drupal-css is applied and you can append your own on top of that.

In it's current form, the module provides only API-like functionality, no UI at all, though this may change in the future.

USAGE:

Where ever you can add php code(php-field, text-area with php input format, custom module, template, etc.), 
you can call the button like this:
print area_print_form($options);
where $options is an associative array containing:

 - target_id: Id of the element you want to print (optional, defaults to 'content'),
 - button_id: Id that will be given to the print link or button (optional, defaults to 'area_print_button'),
 - value: The text for the link/button (optional, defaults to t('print')),
 - type:  either 'link' or 'button' (optional, defaults to 'button'),
 - custom_css: path to a css file that will get added to the page before printing (optional).

Example:

In your custom template, or php-node, or somewhere:

1. First define your printable area like this:

  <div id="my_printable_div">
    I want to print this!
  </div>


2. Then print the button:

<?php
$options = array(
  'target_id' => 'my_printable_div',
  'button_id' => 'my_print_button',
  'value' => t('Print'),
  'type' => 'link',
  'custom_css' => drupal_get_path('module','my_module').'/css/print.css',
  );

print area_print_form($options);
?>

KNOWN PROBLEMS:
As always, Internet Explorer acts unlike all the other browsers, and this module is no exception.
There is a problem where IE cannot "catch-up" with the changes made to the page with javascript 
before printing and closing the page. As a temporary solution, when you print with IE, you will 
see alert box saying "Press OK to print". This gives the browser enought time to complete it's tasks. 
I will try to come up with better solution, but in the meanwhile, this works(and patches are welcome).
