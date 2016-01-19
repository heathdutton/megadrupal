This module allows to force using HTTPS at some pages, and HTTP at the others.

The module is very close to Secure Login. It is also intended for sites that
want to offer anonymous sessions via HTTP or HTTPS and authenticated sessions
only via HTTPS, and anonymous insecure sessions are migrated to authenticated
secure sessions upon login.

The difference is that instead defining for what forms HTTPS must be used
(like it is for Secure Login), the module allows to define paths for which HTTPS
must be used.

As opposed to Ubercart SSL, this module is standalone, and allows settings paths
from Drupal UI without using hooks.

Installation and usage

    It is supposed that SSL certificate is properly installed, and the site is
    accessible through both HTTP and HTTPS

    Download and install module as any other usual drupal module

    If you are logged in at HTTP version of the site, right after installing the
    module you will be redirected to HTTPS, and hence to re-login.

    Visit admin/config/people/custom-ssl-redirect

    You will see two fields for 'secured' and 'ignored' paths respectively. It
    is not recommended remove any path set by default from there unless you
    understand what you are doing.

    Add regular expressions for your custom paths that must use HTTPS to secured
    paths list

    Add regular expressions for your custom paths that must use protocol
    currently used at the page to ignored pages list (this way, these paths will
    be used both with HTTP and HTTPS depending on whether you are at HTTP or
    HTTPS page at the moment).

    Save the configuration
