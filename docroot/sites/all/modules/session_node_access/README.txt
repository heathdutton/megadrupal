INTRODUCTION
------------
This module grants per-session permissions for users to access nodes
they created.

Yet another simple node access module, this time catering to users whose nodes
have to be published for them to have access.
It grants temporary (session based) node access permissions to users with set
roles after they create that node.

Consider the following use case:
Content created by anonymous users is set to be unpublished until a mod reviews
it. With this module the anonymous user gets to view/edit/delete their freshly
created content without it to be accessible to anyone else. This access lasts
as long as the user's session lasts, which means, as soon as the session
expires, or the user changes the browser, they loose access to their content
until it gets published.
Obviously this is better than seeing 'Access denied' right after creating a
node.

In case of non-anonymous roles it is possible to set 'view own unpublished
content' in Drupal 7 so the use of this module is not necessary.
However in marginal cases one may still want for the users to have access to
the node only after submission, or wouldn't like to grant 'view own
unpublished' for all content types. In this scenario this module may be helpful
as well.
  
INSTALLATION
------------
* Install as usual, see
https://drupal.org/documentation/install/modules-themes/modules-7 for furtheri
nformation.
  
CONFIGURATION
-------------
* ATTN: This module will not work if another content access module (or drupal's
native permission tab) doesn't grant set users to create nodes of a given
type. The module will check if the users have this permission and will
warn if that's not the case.

* ATTN 2: As most drupal access modules, this module does not deny access to
anything, it only grants access. Please make sure to check the settings of
other access control modules and the permissions tab if something does is not
working as expected.

* In order to have access to the module settings you need to grant it the
appropriate permimssions in the permissions overview.
* To configure the module, click on 'configure' next to its entry on the module
overview page or visit the link admin/config/session_node_access.

* Restrict by user roles: Only users with these roles will be given access to
their content right after creation.
* Restrict by content type: here you can alter the modules functionality by
restricting it to certain content types.
* Grant these permissions: The permissions to be granted by the module can be
set here.
* Take efffect only on published nodes: Set whether the access permissions
granting is to be perfomed on published nodes only or on published and
unpublished ones.
