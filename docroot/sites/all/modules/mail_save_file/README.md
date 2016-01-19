# MAIL SAVE FILE

This is an intentionally-simple module which transparently saves outgoing email
bodies to a file in a given directory.

## Security

The default configuration on this module saves no emails. This is because you
should take care to white-list only the emails you wish to store, and to ensure
they are saved in a secure location.

Drupal sends many emails, and many of them contain sensitive information. Ensure
you save them to secure location, and never ever in a publicly accessible
web folder.

## Roadmap

 * Allow customisation of the format of the saved file (i.e. not just the email
   body).
