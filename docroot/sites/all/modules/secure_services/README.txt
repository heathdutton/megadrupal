Secure Serivces 
=============================

This module adds HTTP basic authentication to the Services module.
It allow access to only those users that have permission set for this module.


Installation
------------

Unpack the module and place the secure_services folder in your site's
module directory (e.g. sites/all/modules).
Set permission for the role that you want to access services.


CGI/FastCGI compatibility
-------------------------

If you are using the CGI/FastCGI server API, you must apply a patch to your
.htaccess file for basic authentication to work.

Add the following rewrite rule to your .htaccess file manually before :

    RewriteRule .* - [E=HTTP_AUTHORIZATION:%{HTTP:Authorization}]

**********************************************************************
  Header append Vary Accept-Encoding
</FilesMatch>
</IfModule>
RewriteRule .* - [E=HTTP_AUTHORIZATION:%{HTTP:Authorization}]
</IfModule>
***********************************************************************
