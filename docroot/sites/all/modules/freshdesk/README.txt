

        FRESDESK MODULE - README
______________________________________________________________________________

NAME:       Freshdes
AUTHORS:    Brady Owens <info@fastglass.net>
______________________________________________________________________________


DESCRIPTION

This module allows some level of integration between your Drupal website and
the Freshdesk support service.


INSTALLATION

Step 1) Download latest release from https://www.drupal.org/project/freshdesk

Step 2)
  Extract the package into your 'modules' directory.

Step 3)
  Enable the forward module.

  Go to "/admin/modules" and put a checkmark in the 'Enabled' column next to
  'Forward'.

Step 4)
  Go to "/admin/config/services/freshdesk" to configure the module.
  
Step 5)
  Enable permissions appropriate to your site.

  Go to "/admin/people/permissions" to configure permissions.

EMBEDDED FORM

The embedded form (iframe) works via a token so it can be put anywhere. You are
free to choose whatever method that you would like to display the token.

If you want a quick and easy way to put it into content. You can download the
module Token Filter https://www.drupal.org/project/token_filter

SSO URL

This module provides an SSO URL to allow customers to authenticate with your
Drupal user database to access Freshdesk without creating an account. Currently
the URL is fixed and cannot be changed. Path for the SSO link is 
freshdesk/support/portal. If your website was sample.com the address would be
http://sample.com/freshdesk/support/portal
It is highly recommended this be run of SSL.

To tell Freshdesk that you have SSO capability and to get your Shared Secret, go
to your admin page in your account. Under general settings click "Security".
Obtain the secret and setup the remote login URL and the remote logout URL. The
login URL will be as above with the path freshdesk/support/portal.
____________________________________________________________________________
http://www.fastglass.net
