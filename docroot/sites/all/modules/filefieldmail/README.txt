Overview
--------
Allows filefield downloads to be restricted until the requester confirms 
the email address. This email address is stored in the database and 
reported back to privileged users. The requester receives a mail with a 
link to the requested file.

Features
--------
Captures file downloads of files which are provided through a filefield 
and configured as tracked. Anonymous users are redirected to a form 
where they can enter their email address, instead of immediately 
starting the download (which would be standard behavior). After 
submitting the email address, they receive an email witch contains the 
download link as well as configurable subject and text areas. 

For logged in users the download starts immediately as usual.

Download statistics of tracked files by anonymous and logged in users 
are provided under Reports / Tracked downloads. 

Requirements
------------
The File module (from Core) must be enabled.
A content type must contain a filefield.
A configured mail server

Configuration
-------------
Install as usual. Click the "manage display" tab of the content type 
which contains the filefield which you want to be tracked. Set the 
format of the filefield to "Generic file (with tracking)". Save the 
display settings. 

Go to configuration / content authoring / Filefield Mail Configuration. 
Insert the subject of the mail, the text of the body field before (e.g. 
Thank you for your interest..) and behind (e.g. Sincerely, Your Fubar 
Team) the download link. Save the configuration. 

Make sure that Public file system path under Configuration / File Systems
is set correctly.

Recommended Modules
-------------------
It is strongly recommended to use Captcha or a module with similar 
functionality. FileField Mail sends a email to an address provided by an 
anonymous user. Therefore it can be misused by malicious software to 
fill someones mailbox. Prevent this by using a module which ensures that 
the user is human. 

Similar Projects
----------------
See http://drupal.org/node/1613392 for a comparison of download tracking 
modules. 

Filefield track and Webform Protected Downloads are similar modules, 
because they have D7 versions and collect an e-mail address from the 
user. 

Filefield track captures the download and redirects the user to a form. 
The user can enter an email address. Filefield track does not send an 
email, but starts the download immediately after submission. Thus, the 
email address is not verified and any formally correct address will be 
accepted. 

Webform Protected Downloads tracks file downloads only from webforms. It 
does not track downloads from content types. It replaces the download 
link in a webform with a field to insert the email address. The 
replacement of the link makes the page less readable for the user, 
especially if the page contains a whole list of links. Webform Protected 
Downloads sends an email to the address to verify it. The text of the 
mail with the download link can not be configured. 

Filefield Mail combines features of both modules and extends them. It 
tracks filefields in content types. The links are not replaced, so that 
they still lock the way they were configured. The mail address is 
verified by sending an email. The text of the email can be configured.

Some of the code is taken from those two modules. Thank you guys :-)

Why a new module (and not a patch for an existing module)?
----------------------------------------------------------
The functionalities of Webform Protected Downloads and FileField Mail can not be merged, since one focuses exclusively on webforms and the other can be used for all content types.

FileField Track has some functionality, which does not make sense for FileField Mail, e.g. the "URL to file (with tracking)" format and setting cookies.

The FileField Track module uses cookies to relieve the unregistered user from having to type the e-mail address repeatedly for every download. This may be useful if the download starts immediately after insertion of the address and the visitors computer is used only by one person.

But it prevents the visitor from having different downloads sent to different email accounts. And it becomes a problem, if more than one visitors share the same computer without different user accounts. Therefore, FileField Mail omits using cookies.

This different necessities are mutually exclusive and can not sensibly be built into the same module.
