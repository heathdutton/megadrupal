User Force Term
================================================================================

Summary
--------------------------------------------------------------------------------

The User Force Term module lets you force all content submitted by a user to be
associated with a specific taxonomy term.


Requirements
--------------------------------------------------------------------------------

The User Force Term module requires the Taxonomy (core) module.


Installation
--------------------------------------------------------------------------------

1. Copy the user_force_term folder to sites/all/modules or to a site-specific
   modules folder.
2. Go to Structure > Modules and enable the User Force Term module.


Configuration
--------------------------------------------------------------------------------

1. Create the vocabulary you wish to use with user_force_term. Only node
   types associated with this vocabulary are affected by the module.
2. Add some terms to the vocabulary.
3. Go to Users -> Force taxonomy term and choose the vocabulary you just created
   and populated.
4. Go to Users and edit the users whose contributions you wish to force to be
   associated with a term from the selected vocabulary.
   From the Term association dropdown in the Account information fieldset
   choose the term you want the user's contributions to be associated with or
   <none> if you don't want a forced term association.


Support
--------------------------------------------------------------------------------

Please post bug reports and feature requests in the issue queue:

  http://drupal.org/project/user_force_term


Credits
--------------------------------------------------------------------------------

Author: Morten Wulff <wulff@ratatosk.net>


