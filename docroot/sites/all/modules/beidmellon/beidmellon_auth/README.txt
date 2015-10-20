Fedict beID Mellon module
=========================

The following variables can be set up:

- beidmellon_auth_activation_mail_login_url
    The login URL to mention in the activation mail.

In order to fake a Mellon response, log in as user 1, set up all of these 3
variables and go to beid/connect:

- beidmellon_auth_fedid_debug
    Fake a Mellon response with the FedID in the variable for debugging
    purposes.

- beidmellon_auth_given_name_debug
    Fake a Mellon response with the given name in the variable for debugging
    purposes.

- beidmellon_auth_surname_debug
    Fake a Mellon response with the surname in the variable for debugging
    purposes.
