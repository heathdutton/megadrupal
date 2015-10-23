Overwiew

This module provides a tiny API to make SIMPLE and EASY the usage of jQuery
Dialogs with Drupal pages and forms.

This module hasn't as many features as ctools or DialogAPI as it is aimed to
be used as a page callback function on hook_menu, not a full featured API.

It provides two functions: one per page dialogs and other one for form dialogs
as drupal_get_form replacement. These functions are intended to be used as
'page callback' argument on hook_menu item arrays without additional coding
or configutarion.

With these functions on your page callback, you only need to call the path with
ajaxed links (with nojs path argument and use-ajax class on it). As simple as
that, jQuery Dialog for lazy developers!

The module has a third function to use as form callback with some advanced
features but with the same SIMPLE and EASY usage basis.


_____________

Usage example

Add a menu callback for your form on your module hook_menu function

<?php
  // JS menu path for example dialog form.
  // It is similar to example dialog page but with
  // monodialog_get_dialog_form page callback.
  $items['monodialog_example/form/ajax'] = array(
    'type' => MENU_CALLBACK,
    'title' => 'MonoDialog Form Example',
    'description' => 'Shows a dialog window with a form in it.',
    // Calls the MonoDialog form function.
    'page callback' => 'monodialog_get_dialog_form',
    // Passes the function that will generate the form
    // array to be displayed on the dialog
    // as firts parameter, jQuery dialog options array as second one
    // and any argument that form function needed as extra params.
    'page arguments' => array('monodialog_example_form', array(
      'title' => t('MonoDialog Form Example'))),
    // Only users with the propper rights should see the page.
    'access callback' => 'user_access',
    'access arguments' => array('monodialog example view'),
    // Lets drupal executes our generateds Ajax Framework commands.
    'delivery callback' => 'ajax_deliver',
  );
?>

Adds an ajaxed link to it (it must have /nojs on path and
use-ajax class) on a page, block...

<?php
  l(t('View Dialog Form'), 'monodialog_example/form/nojs', array(
    'attributes' => array('class' => array('button', 'use-ajax'))));
?>

Voilá! Your form will be opened on a modal dialog without any other
additional config needed.

You can find an example module inside with this example and more.
