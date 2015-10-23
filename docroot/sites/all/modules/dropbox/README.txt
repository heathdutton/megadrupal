Dropbox Integration
===================
Project Page
: <http://drupal.org/project/dropbox>

Bug reports and support
: <http://drupal.org/project/issues/dropbox>

Author
: Andrew Berry (deviantintegral)
: <andrew@abdevelopment.ca>
: <http://www.abdevelopment.ca/>

Installation and Configuration
------------------------------
To install the Dropbox Integration module, extract the archive to your modules
folder. This is usually sites/all/modules. As well, the DropBoxUploader library
must be downloaded to the Dropbox module directory. The library can be obtained
at:

http://jaka.kubje.org/software/dropboxuploader/

After enabling the module on the Modules page, there are two permissions
available:

1. **Configure Dropbox account**. This gives users access to enable Dropbox
integration in their user account. It doesn't mean anything for anonymous
users.

2. **Send files to users via Dropbox**. This allows users to send files to
other users via Dropbox. This does not instantly give them access to send
files; each user must also choose which roles they wish to send files to them.
If this permission is disabled after being enabled, it will instantly block
that role from sending files, regardless of individual user settings.

Security, SSL, and Host ID's
----------------------------
This module also **requires that SSL be enabled** to allow users to configure
their Dropbox form. This is because this module requires access to a user's
Dropbox account and password. Dropbox Integration attempts to determine if SSL
is available and will ask a user to go to the secure version if it looks like
it is. It is not required to use SSL to send files, or to modify other fields
on the Edit page; just to view or change a user's account information. If you
**know that you don't need SSL**, such as using the module on a local
development site or on an Intranet, and can't enable SSL, there is a variable
to disable SSL checking. I won't support this as I don't want to be responsible
for any accidental disclosures of account credentials, so if you really want to
disable the SSL check, read the source code to find out.

User Settings
-------------
If a user has permission to enable Dropbox on their account, an additional set
of options is displayed on their user Edit tab. The settings will only display
**if the user is connecting over SSL**. Users can choose the roles allowed to
send them files. User ID 1 will always be able to send a file, so if users
simply want site administrators to send them files they can enter just their
account information. Users can specify a maximum limit of files sent per hour
per anonymous user.

Feedback and Support
--------------------
Feel free to contact me at the above addresses for paid customization of this
module. For general support, bug reports, and patches, please see the project
issue queue.
