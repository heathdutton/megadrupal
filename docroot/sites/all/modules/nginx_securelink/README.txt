================================
ABOUT
================================

This module allows you to generate secure download links handled by nginx_securelink.
This module is extremely flexible. You can decide exactly who can download files and who can't.
This is done using flexible access handlers.

Please continue reading to get more information.

NOTE: For installation, configuration and usage of this module, please read INSTALL.txt
NOTE: For configuration of nginx, please read INSTALL.txt file.
NOTE: For installation of nginx server, please read INSTALL.nginx.txt


================================
BACKGROUND
================================

Drupal core comes with a built-in Private Download System.
You can create 'file fields' and set them to use "private file system" and you can
attach those fields to entities.
This way, you can limit access to your files.

But this system has a few drawbacks:

 - There is only one permission to restrict access to private files.
 - It is not flexible. If a user can see a node, the (s)he can download attached files as well.
 - It doesn't have a flexible and fine grained access control
 - It's slow. Downloading a file requires Drupal to bootstrap and continue running while the 
   download is in progress.
 - I can write a few more. but I don't remember right now :)


A few months ago, I wanted to sell files using ubercart.
First I did it using Drupal's private file system.
But after a while, it started to suck. It was very slow and Downloads were interrupted
after a few minutes.

I was searching for a good alternative. And This module was born.


================================
ABOUT NGINX SECURE LINK
================================

From Wikipedia:

Nginx (pronounced "engine x") is a web server with a strong focus on high concurrency,
performance and low memory usage.
It can also act as a reverse proxy server for HTTP, HTTPS, SMTP, POP3, and IMAP protocols,
as well as a load balancer and an HTTP cache.

From nginx.org:

The ngx_http_secure_link_module module (0.7.18) is used to check authenticity of requested links,
protect resources from unauthorized access, and limit link lifetime.
The authenticity of a requested link is verified by comparing the checksum value 
passed in a request with the value computed for the request. 
If a link has a limited lifetime and the time has expired, the link is considered outdated.

---
Nginx securelink module is an extension to the nginx server.
It's included and is built into the nginx core. But you should enable it when compiling nginx.

You can create secure links for downloading files.
You can restrict the lifetime of the link.
You can also restrict the IP addresses that can use the link.


================================
HOW THIS MODULE WORKS?
================================

This module can generate secure links.
You setup nginx on a server. and configure it with a secret code.
Then you install this module and configure it with the same secret code.
So this module can generate secure links when users request a link.

This module has a fine-grained access control.
Users first should request a secure link to a file.
Then this module checks the request and passes it to a few access handlers.

If the access is granted, then a secure link is generated and shown to the user.

It isn't required that nginx be installed on the same server as Drupal is running.
you can run Drupal somewhere and run nginx a totally different server far away from Drupal.
Just the secret code should be configured between nginx and Drupal.


================================
FEATURES
================================

This module is flexible and extensible.
you can extend this module and write your custom logic to decide who can request download links.

Out of the box, this module provides a very flexible access controller called "Access By File"
You should enable the "Nginx Secure Link: Access By conf Files" module which is included with this module.

This grants access to files by using configuration files (files with the .ngslaccess extension)
(like .htaccess files)

.ngslaccess files should be placed in a folder in Drupal installation place.
you can configure their place in this module.

This access controller, provides two great access handlers.
 - Access by user role. You can decide which roles can download a single file.
 - Access by Ubercart order. You can sell files. If a user has purchased a product from your store,
   Then (s)he can download a file.

This access controller can control access to individual files or whole directories.
You can also set access parameters for a whole directory and then override it for individual files.


This module also provides a 'Text Filter'. You can enable it in your Text-Formats.
This filter, can generate 'download request' links.
You can put this in your text: [ngsl=folder/file.ext]
and this will be converted to a "download request link" for the file at: "folder/file.ext"
Note: The path is related to nginx root path. (it can be configured.)


================================
INSTALLATION
================================

Please see INSTALLL.txt and INSTALL.nginx.txt


================================
TROUBLESHOOTING
================================

Empty.
TODO: Write troubleshooting guide.


================================
AUTHOR
================================

This module is created and maintained by Ahmad Hejazee
http://www.hejazee.ir/


================================
COMMERCIAL SERVICES
================================

If you need commercial support and services, please contact me via my blog at:
http://www.hejazee.ir/contact
Or use this contact form (You need to have an account in drupal.org):
https://drupal.org/user/911168/contact


================================
SPONSORSHIP
================================

If you would like to support this project, please donate with PayPal using this link.
https://www.paypal.com/cgi-bin/webscr?cmd=_s-xclick&hosted_button_id=XVNGWYCVHL2TE
