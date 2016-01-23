CONTENTS OF THIS FILE
---------------------

 * Introduction
 * Installation
 * Why fields?
 * Credits


INTRODUCTION
------------

Field Paywall allows developers to replace fields on entities with a message
depending on user permissions. It's useful for giving visitors teasers to
content before advising them to sign up to see more.

INSTALLATION
------------
1. After enabling the module, visit the fieldable entity you wish to add a
   paywall to and add a new field with type 'paywall'.
2. In the field configuration screen select which fields should be hidden and 
   what the message replacing them will be.
3. Visit the user permissions page and choose which users can bypass the paywall
   you've created.

WHY FIELDS?
-----------
Paywall configuration is stored in fields because it allows developers to very
easily retrofit paywalls to existing entities and export field configuration 
using tools such as Features. Paywalls can also be re-used across multiple
entity types and bundles. Furthermore the position of the paywall can be
adjusted using the Field UI display configuration.

It may seem a bit strange at first using a field to hide other fields, but it's
proven a very flexible, easy to configure, easy to export and highly
maintainable approach. 

CREDITS
-------
Developed by Marton Bodonyi (@codesidekick) whilst working on the Brightday 
project for News Corp. Australia.
