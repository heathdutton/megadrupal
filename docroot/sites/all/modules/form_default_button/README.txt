INTRODUCTION
------------
This module adds the option to override the default behavior when a form is
submitted by pressing Enter from a textfield. This is done on per-form basis.

By default, the browser will pick the first applicable submit button, and
press that. You can prevent a form from being automatically submitted at all
with this module. Or optionally you can configure the form to use a button of
your choosing. For example: you can make it so node and comment forms will
trigger the preview button by default.

IMPLEMENTATION
--------------
If a form is configured to override default browser action, a hidden button
will be inserted in the form. The button will be above all other buttons,
and so it will take precedence. Javascript will capture the pressing of the
hidden button and will either prevent the action, or will trigger click of
the button that is configured to be the new default.

The obvious alternative for the implementation is to bind the Enter keypress
event on text inputs. The problem with event binding is that other javascript
libraries are also binding and preventing events, and this can easily lead to
conflicts. These conflict can become difficult to trace and debug, and the
solution usually involves adjusting the order of binding, or binding to parent
DOM elements, and relying on event propagation order. A catch-22 situation is
also possible. One example for a likely conflict is the misc/autocomplete.js
in Drupal core. On the other hand, the fake button implementation is safe in
comparison to event binding.

REQUIREMENTS
------------
No additional modules are required.
This module relies on javascript in order to work. If javascript is disabled,
the forms will behave as normal.

CONFIGURATION
-------------
Navigate to "admin/config/user-interface/form-default-button" and configure
each form separately. There are two options for overriding the default form
behavior:
- "Do nothing". This will make it so the form won't auto-submit when focused
on a textfield and pressing Enter.
- "Click element". This requires to also provide a jQuery selector for the
button that needs to be made "default", and thus clicked when a form is
auto-submitted. The selector will be used in the context of the same form.
