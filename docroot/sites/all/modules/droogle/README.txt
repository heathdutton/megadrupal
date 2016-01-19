README.txt
------------
Droogle has been updated to use the latest version of google's API.  Droogle
now uses a client id and client secret which is described a bit more in the 
Droogle config and setup section.
The Droogle Module integrates Google Docs with Drupal. It provides a list of a 
configured google user's documents.  The module can also be configured with 
organic groups to show a list of documents for each group (each group 
needs to be associated with a Google user account).

The module's creation is sponsored by Babson College and is to be used to
better manage documents for groups, classes, etc. Integrating documents
with google means better access to documents on ipads, iphones, etc, since
there are many apps for these devices to sync your google docs account(s). 

Droogle is not to be confused with BBoogle, which is Blackboards
integration with Google Docs -- but Droogle works much the same way
and serves the same purpose. 

We are looking for ideas on how to improve the module to make it as
useful as possible in eduction and to the general community. We are
open to having co-maintainers, preferably with another college or university. 

INSTALL 
------------ 

Droogle installs like all
Drupal Modules, just put it in the modules directory and enable
the module

***** You do need to grab google's latest api code from github at the url
https://github.com/google/google-api-php-client
and put the google-api-php-client directory in your sites/all/libraries directory.  Then
rename the google-api-php-client to google-api-php-client-git-version
We use the old library as well so I've renamed this folder to not conflict with the old
version of the API (which google still keeps in subversion).  Until we migrate all our code to this
new api in github, renaming this directory will be necessary.

DROOGLE CONFIG AND SETUP 
------------------------ 
Go to admin/settings/droogle and put in a client id and client secret 
for the google account you would like to use.  With this latest version
of the google drive api, which uses more secure OAUTH authentication, you'll
need to generate a client id and client secret for your google user account
at https://cloud.google.com/console.

1.)  Create a project
2.)  In the APIs tab (a link in the left sidebar) enable both the Drive API and the Drive SDK.
3.)  Generate a Client Id and Client Secret on the Credentials tab (its not really a tab its a link in the left sidebar).

You can optionally setup Organic Groups to look for a group cck field to read the google
client id and client secret for each group. 
 

The module is presented as is under the opensource license that
is within the module for you to read, we make no guarantees. That
being said we're looking to make this module great and feel free
to submit issues and suggestions to our issue que. 

GOOGLE API Exposed for Drupal Developers 
----------------------------------------- 
This version of Drupal has a function called droogle_file_upload 
and works great using the latest gdrive api as of 1/30/2014 to
create and share a gdrive document.

Other api functions previously in droogle, I'm hoping to port over
to this version (this version which uses the most up to date api).
