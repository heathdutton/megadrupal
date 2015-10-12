Author
-------
Created by Kevin O'Brien
http://www.coderintherye.com
Supported through my work at http://www.kiva.org (Make a loan, change a life)

Site adminstration module which helps site admins send static html versions of their sites
to an external web server. This can be very useful if one wants to use Drupal to manage content
but host it on a different server, especially if that server is not capable of having Drupal
installed on it.

Download
---------
http://drupal.org/project/savetoftp

Installation
------------
Untar or unzip the downloaded module to your module directory (e.g. sites/all/modules)
and then enable on the admin modules page (/admin/modules).

Usage
------
Go to a node's edit page, e.g., node/edit/1
At the bottom there will be a "Publish to FTP" button
Selecting this button will enable a form where you can enter FTP credentials
Selecting submit on this form will send the html version of the node as well
as any needed files to the FTP server entered.

Requirements
------------
PHP Version > 5.1.2
Need to have the QueryPath module installed (http://drupal.org/project/querypath)
