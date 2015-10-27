================================
ABOUT
================================

Nginx Securelink module depends on access handlers for deciding who can access a given file.

This module allows you to control access to your files by configuration files.
Configuration files have the ".ngslaccess" extension.

The idea of this, is inspired by Apache's .htaccess files.
but this .ngslaccess files are different...


================================
START HERE
================================

1 - Go to Admin => Modules (admin/modules)

2 - Enable the "Nginx Secure Link: Access By conf Files" module

3 - Go to Configuration => Media => Nginx securelink => Access handlers => Access By file
  (admin/config/media/nginx-securelink/handlers/byfile)

4 - Enter the path to your conf files.
  If you have installed nginx on the same server as Drupal is running,
  then it will be best to set it to the absolute path to your private files directory.
  You can also set a different path. But it should be an absolute path.

  If nginx is running on different server, you should create a folder for your config files.
  Note that this folder should be on the server where Drupal runs. And should be readable by Drupal.

5 - Click "Save configuration".

6 - Keep reading this document.


================================
INTRODUCTION TO .ngslaccess FILES
================================

.ngslaccess files are the way that this module controls access to files
When a secure link to a file is requested, this module scans the path you configured above
and looks for a few .ngslaccess files.
If it finds a proper .ngslaccess file, it will read it and will decide whether the user can
request the file or not.


================================
WHICH .ngslaccess FILE?
================================

.ngslaccess files will become your best friend.
You will have many of them soon.

.ngslaccess is the extension of your config files.
They can also have a base name.
For example they can be named "vide.mp4.ngslaccess" or only ".ngslaccess".

Here, I will teach you how this modules finds the proper .ngslaccess file for controlling access
to a given file, when requested.

Suppose you have a file named 'video.mp4' which is placed inside a folder named 'videos'.
The path to that file is "videos/video.mp4"

Suppose a user requests a securelink to this file.

I will show you the process of finding a proper .ngslaccess file for that request.

NOTE: finding the proper .ngslaccess file happens in the config folder.
  (The folder you have set in step 4 in "START HERE" section of this document)
  Note that the config folder can be the same folder where you store your private files
  (In case when you run nginx on the same server as Drupal runs)
  Or the config folder can be a different folder.
  However the process of finding a proper .ngslaccess file will be the same.
  For the rest of this document, I will suppose the config folder is the same folder
  as you store your private files.

1 - Looks for a file named "video.mp4.ngslaccess" inside a folder named "videos"
  (inside the the config folder.) (everything happens in the the config folder)
  If it is found, and is readable, then the process is finished.
  And our config file will be "videos/video.mp4.ngslaccess"

2 - If the previous step fails, then it looks for a file named ".ngslaccess" (starts with dot)
  inside the "videos" folder.
  If "videos/.ngslaccess" is found and is readable, it will be used. The process is finished.

3 - If the previous step fails, it will then go up one directory.
  It will look for a file named ".ngslaccess" in the config folder (one folder up)
  If it's found and is readable the process is finished. Otherwise it will go up another level.
  It will go up one level each time, until it reaches the config dir's root.

  If no readable ".ngslaccess" file is found in the path tree, This module will ignore the access check
  and will pass the access control to another access controller module.
  If you haven't installed another access controller, the access will be denied by default.


================================
BEHAVOIR OF THIS MODULE
================================

If a proper .ngslaccess file is found, it will be used.
This module will read it and process it. Otherwise will pass the request to another access controller.
If no other access controler is enabled, the request is denied by default.

When a proper .ngslaccess is found, This module will read it and parse it.
If it has a valid format, it will be used.
But if it has an invalid data in it, It will be ignored.
This module won't look for another .ngslaccess file if the current file has invalid data in it.

For information about the content and the format of .ngslaccess files, read on.


================================
ANATOMY OF A .ngslaccess file
================================

A .ngslaccess file is a plain text file (One which you create using vim or notepad)
.ngslaccess files have the format of ini files.
The content of a valid .ngslaccess file looks like this:

[ngslaccess]
handler = userrole
arguments[] = role_name

or

[ngslaccess]
handler = ucorder
arguments[] = 12

Here I will explain it:

The first line of a .ngslaccess should be

[ngslaccess]

The second line is:

handler = <handler_name>

You should put your handler name here.
I will discuss handlers later.

The third line and subsequent lines are like this:

arguments[] = <value>

You should add as many "arguments[] = <value>" as needed by your chosen handler.
you should replace <value> with your desired value.

NOTE: Since .ngslaccess files have the format of ini files, you can add comments to them
  comments are started with a semicolon (;)

NOTE: If you use an invalid access handler or when you use an access handler that does not exist,
  Then your .ngslaccess is invalid.


================================
ACCESS HANDLERS
================================

This modules comes with two handy access handlers out of the box:

1 - userrole
2 - ucorder


================================
THE userrole ACCESS HANDLER
================================

This handler allows or denies request based on user role.
For example suppose we have a role called "premium"
We want to allow premium users to download video.mp4 but deny other users.

We should create a file called video.mp4.ngslaccess and put this content in it:

[ngslaccess]
handler = userrole
arguments[] = premium


================================
THE ucorder ACCESS HANDLER
================================

This handler allows or denies request based on ubercart orders.

For example suppose we want to sell a file named "video.mp4"
We create an ubercart product for this. Suppose its node id (nid) is 12

We want to allow all users who have purchased this product to download this file, but deny others.
We should create a file called video.mp4.ngslaccess and put this content in it:

[ngslaccess]
handler = ucorder
arguments[] = 12


================================
USE CASES
================================

TODO: write some use cases.


================================
AUTHOR
================================

This module is created and maintained by Ahmad Hejazee
http://www.hejazee.ir/
