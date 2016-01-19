## Description

This module adds a password field for anonymous users to the "Account information" pane on the
checkout step of Drupal Commerce. When the checkout process completes, the user is automatically logged in.

## A little more detail on the process.

The password is hashed using Drupal core's hashing process when the form is submitted and stored into session.
When the checkout process completes, a user is created using the hashed password and email from the order information.
The user is automatically logged in at the end of checkout completed process.


## Similar to "commerce_checkout_complete_registration"

This module is very similar to "commerce_checkout_complete_registration" however that module creates a pane on
the completed order page with the option to create a user.
This module adds the password field to the first step and is required.
