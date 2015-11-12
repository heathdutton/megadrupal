
Inline Registration allows anonymous users to register via the node/add page,
thus removing a step/barrier from the user actually publishing content. If
you're going to use this module, or allow anonymous users to post content to
your site at all for that matter, you should really use CAPTCHA to help keep
the spam-bots from trashing your site.

Features:
- Adds user_register() form to node/add pages if the user is not logged in
- Associates the new piece of content with the new user
- Can log the user in after node creation depending on user registration
  settings
- Compatible with modules: email_registration, logintoboggan,
  registration_toboggan and other


INSTALLATION:

1) Put the module in your Drupal modules directory and enable it in
   admin/modules.
2) Go to admin/structure/types and select an appropriated content type. Then,
   in the Inline Registration vertical tab, enable Inline Registration.
