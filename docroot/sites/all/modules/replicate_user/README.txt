CONTENTS OF THIS FILE
---------------------

 * Introduction
 * Description
 * Requirements
 * Recommended modules

INTRODUCTION
------------
Replicate User extends Replicate module to manage the cloning of user entities.

Configure user fields replication rules on admin/config/people/replicate-user.
See Replicate module to run replication.

DESCRIPTION
-----------
When you clone a user entity, the username needs to be updated to be unique.
As the decision on how to update the username can be different per website,
the replication of users is not managed by the Replicate module
which is only an API (not taking any functional decision).

Replicate User provides a simple UI to let administrator to configure
how the user entity is replicated. Username, password and email (mail & init)
fields replication can be configured.

This module doesn't provide any UI to replicate users.
Please have a look to Replicate UI module to get an administrative
interface to replicate entities.

REQUIREMENTS
------------
This module requires the following module:
 * Replicate (https://drupal.org/project/replicate)

RECOMMENDED MODULES
-------------------
 * Replicate UI (https://drupal.org/project/replicate_ui)
   When enabled, provides an UI to perform replication.
