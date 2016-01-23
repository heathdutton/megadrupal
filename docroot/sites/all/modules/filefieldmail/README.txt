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

New in 7.1
----------------
If the protected file is configured as private, it will be checked if the
user really receive the link in mail or just guessed the download URL correctly.
For this reason a hash at the end of the download URL is verified.