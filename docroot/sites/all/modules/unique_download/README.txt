UNIQUE DOWNLOAD
---------------------------

Install
-------
Steps :
1. Download and extract the module into sites/all/modules directory.
2. Enable the module from admin/modules using admin login.
3. You are ready to use the unique download module.
4. There will be menu item available in drupal navigation "Unique URL settings".



Description
-----------

This module is used to generate the unique / one time links to download for specific file.
For now there is very basic hashing algorithm used in this module to generate the hashkey.



Settings
-------- 

Basic settings for modules are located under :
Drupal 6
http://<yoursite>/admin/settings/unique_download
Drupal 7
http://<yoursite>/admin/config/system/unique

There are following options:

1. Multiple download : If you want users can download this file multiple times you can check this option and users will be able to download it multiple times.
This feature is limited currently.

2.Allowed Content types: 
It allows you to select the content type to which the file is bound. You can select one or more content types from the existing content types of your site.

3. Subject : This is the text field which you can use for custom subject for your email.
For instance : "This is your link for unique download."

To generate the Unique URL:
click on the Generate unique URL tab present under settings it will take you to 
Drupal 6
http://<yoursite>/admin/settings/unique_download/generate
Drupal 7
http://<yoursite>/admin/config/system/unique/generate

Here you need to specify following things to generate unique URL:
1. node id of a node to which file is associated.
2. Email address to whom you need to send an email after generating the URL (currently in dev).
3. Expiry of the generated URL.

To see all the generated unique URLs and their status:
Drupal 6
http://<yoursite>/admin/settings/unique_download/download
Drupal 7
http://<yoursite>/admin/config/system/unique/download

It will show all the data related to the URL with following fields:
1. Unique Id - For Administrators use only.
2. File Id - For Administrators use only.
3. Email - Email ID to which file link should be sent.
4. Downloaded - Number of times file has been downloaded from the unique-url
5. Expiry - Expiry of the link according to date specified when generating link.
6. User - who generated link.
7. Downloadlink - The unique url for download.
8. Send Email - Link for Adminstrators / users to send the link on Email specified in Email column.

