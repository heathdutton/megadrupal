/* $Id$ */

-- SUMMARY --

The predelete module hooks into the deletion process of nodes. By default it is
not possible to react on a deletion attempt before the deletion of a node. This
is cured by providing the hook_predelete_node(). Other modules may implement the
hook and add custom checks on the node that is about to be deleted.

The module ships with an API documentation and an example module that provides
a single checkbox field. Nodes that contain the field could only be deleted if
the checkbox is checked.

To Do:
A screencast and a blogpost about this module are coming soon.


For a full description of the module, visit the project page:
  http://drupal.org/project/predelete

To submit bug reports and feature suggestions, or to track changes:
  http://drupal.org/project/issues/predelete


-- REQUIREMENTS --

None.

-- INSTALLATION --

* Install as usual, see http://drupal.org/documentation/install/modules-themes/modules-7 for further information.

* Implement the hook_predelete_node() as described in this README or enable the
  predelete_field module.


-- CONFIGURATION --

* Configure user permissions in Administration » People » Permissions »
  predelete module:

  - bypass predeletion check

    Users in roles with this permission are allowed to delete nodes directly
    without the function of this module. By default the deletion attempt od all
    users are validated by this module (even those of uid==1).

* Implement hook_predelete_node as described in predelete.api.php

-- FAQ --

Q Why such a module?
A Because drupal core does not provide the functionality and I thought it was useful.

Q But node hooks are called hook_node_blablabla
A Yes. And becaue this module is no part of the node module, the hook naming
  convention causes the hook to be called hook_{modulenam}_{hook} and thats
  hook_predelete_node().


-- CONTACT --

Current maintainers:
* Mirko Haaser (McGo) - http://drupal.org/user/87891
