Remote import - Hostmaster
==========================

This Drupal module provides a UI for fetching sites from remote Aegir servers.

Installation
------------

Install this module like any other, and enable in the usual way.

You also need to install the backend portion of this extension for this to work
correctly: https://drupal.org/project/remote_import

Usage
-----

You'll need to add the remote Aegir server as any other server in the frontend,
selecting ONLY the 'hostmaster' remote service as you do so.

This means, of course, that you'll need to add your ssh key to this server and
set it up like other remote servers. Note that you don't need to install
anything other than the SSH key on this server.

A general guide to setting up SSH on remote servers can be found here:
http://community.aegirproject.org/node/30#SSH_keys

Once you've set up your server in the frontend you should get a new menu item
called: 'Import remote sites' when viewing the server node. Click on that link
and follow the instructions there.
