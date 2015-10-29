SUMMARY
-------
This module interacts with a MailMan mailing list server to provide easy 
subscription or unsubscription for users. It provides the following features

1. Site administrators can add mailing lists (that already exist on the mailman 
   server) that they want users to subscribe/unsubscribe to.
2. Users can restrict which Drupal roles can subscribe/unsubscribe to a list.
3. Alternatively, the module can use the 'sync_members' utility of mailman to 
   syncronize all members of a Drupal role with membership of a mailing list 
   (requires Drush, and that MailMan and Drupal are on the same server or Drupal 
   files are accessible on the mailman server).

This module is not a management utility for Mailman. It simply provides 
mechanisms for allowing users to subscribe or unsubscribe to a mailing list.

REQUIREMENTS
------------
1. An active mailing list.  You do not need to be the list administrator. To
   add a list, you must know the subsribe and unsubscribe emails. These are
   typically of the form [list_name]-subscribe@[domain] and
   [list_name]-unsubscribe@[domain].

2. The Drupal site must be able to send emails.


RECOMMENDED MODULES
-------------------
None

INSTALLATION
------------
Install as any typical Drupal module. There are no other special instructions

CONFIGURATION
-------------
To create a list, use the administrato menu and navigate to 'Configuration' ->
'Mailing Lists' and click 'Add a Mailing list'.  

MAINTAINERS
-----------
Drupal user spficklin
