BlockAnonymousLinks is a simple module which blocks comments from anonymous users that contain links.

Goal
----
Block spam at an early stage.

Idea
----
It relies on the fact that most spam messages contain links and also on the fact that (for now) (most) spambots don't register on the sites they want to spam.

Use case
--------
This comes out handy when you want to allow anonymous comments on your site and you want to prevent most of the spam messages from being submitted. If you let users register, they still can post comments with links, IF they are logged in.

If an anonymous user tries to post a comment which contains a link, he/she will get a message explaining why the comment has been blocked.

Install
------
The installation is as simple as uploading the module and enabling it in Drupal.

Customization
-------------
The default warning is: "You have to be logged in to post links. This is an anti-spam measure."
You can customize this warning using the translation features of Drupal.