CONTENTS OF THIS FILE
---------------------

 * Introduction
 * Requirements
 * Configuration
 * FAQ
 * Maintainers

INTRODUCTION
------------
 This module shows author's name of current commit as notice message.
 It will help QA to identify the person who has pushed invalid code.
 This module works only with Git. Svn support will be added in future.

REQUIREMENTS
------------
 * Git
 * Requires the server to have permission to run exec() statements.

CONFIGURATION
-------------

 * Configure module here admin/config/development/commit-author

FAQ
---


Q: I can't see the author of commit. What's the problem?


A: Check if Git exists and function 'exec' exists, 
   you can do it here - admin/reports/status.
   Also check if name in Git is set,
   you can do it with this command 'git config --list | grep user.name'.
   If your name is not set you can set it
   with command 'git config --global user.name "Example name"'.


Q: I see the author of commit as 'Not Committed Yet'. What does it mean?


A: You haven't committed yet. Possibly, the author of commit is you.
   You can disable it here - admin/config/development/commit-author.


Q: Where can I disable author showing in notices
   from drupal core/contrib modules/custom modules/themes?


A: You can configure it here - admin/config/development/commit-author.


MAINTAINERS
-----------
Current maintainers:
 * Sergey Cherebedov (cherebedov.s) - https://www.drupal.org/user/3133861
