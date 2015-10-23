### Introduction ###

View Permission Per Node (VPPN) is a very simple access control module with
relation to **viewing** content on a per-node basis.

**Note:** This module only deals with viewing nodes, it does not affect other
op's (eg. create/update/view).

There are a lot of access control modules, many of them are compared
[here](https://www.drupal.org/node/270000).

This module only uses [hook_node_access](https://api.drupal.org/api/drupal/modules!node!node.api.php/function/hook_node_access/7),
so it should play *fairly* well with other access control modules.

### Installation ###

- [Enable](https://drupal.org/documentation/install/modules-themes/modules-7) as
  usual,
- Set permissions for users that can use and administer VPPN.

### Configuration ###

- Navigate to `admin/config/content/vppn` and select the content types,
- Create or edit a node with one of the types selected above,
- There will be a new vertical tab named *View Access*,
- Select the roles that will have view access to this node.

#### Notes ####

- Roles with the *bypass node access* permission will not be listed,
- Selecting no roles will skip using this module for access control,
- Selecting even one role will enable this module for access control, and deny
  access to any users without one of the selected roles.
