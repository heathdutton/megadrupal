
Private Taxonomy
==========================

Enables private vocabularies where users can create and delete their own terms.
Default terms for a vocabulary can be used to populate the terms for new users
and there is a mechanism for cloning the default terms for existing users.

Original module development sponsored by ewave.co.il

shai dot yallin at gmail.com

D6 port by dodazzi

INSTRUCTIONS

There are two permissions that control most of the behavior of this module.
'Administer own taxonomy' allows a user to create and modify their private
terms.  Without this permission a user can have their own terms but they are
created and modified by some other means (e.g. administrator).  'View private
taxonomies' controls whether a user can see terms belonging to other users.
The use case for this permission is if a user wants to have their own set of
terms to tag content they create but other users' should be able to see these
tags when viewing the content.

For the first permission it may be necessary to automatically create terms
for existing users or new users. There are settings that enable this at
Structure -> Taxonomy -> User taxonomy.

When granting 'View private taxonomies' there may be different requirements
when creating content.  Sometimes when creating or editing content, just the
user's terms should be accessed even though the user has permission to see all
users' terms.  In other cases all terms should be available when creating or
editing content. The Private Term Reference field is provided to allow better
control over which terms are available during content creation and editing.
The field settings can be set to the private vocabulary to be used and whether
just the user's terms are used or all user's terms can be used.  The associated
widgets give first preference to the user's terms.  So if the term 'Apple'
exists both for the current user and another user, only the user's term is
available.  Otherwise, if the same term appears twice but neither belong to the
current user then just one term is available but which one is unspecified.
For free tagging, if only the current user's terms are available then if the
user enters another user's term it is not used and the same term is created for
the user.  The field settings are available once the Private Term Reference
field type is added to a content type.

Originally when the 'View private taxonomies' permission was added, the core
fields (select and checkboxes) were modified to only show the current user's
terms.  This approach has been deprecated because it is too limited to work
in all cases.  There is a setting at Structure -> Taxonomy -> User taxonomy
that can disable the modifications to the core fields.

