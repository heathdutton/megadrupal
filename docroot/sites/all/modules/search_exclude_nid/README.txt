Description
-----------
This module only hides single nodes identified by their node IDs from the
search result.

If you have other or further requirements (like hiding nodes by node type) 
check out the custom search module: 
- http://drupal.org/project/custom_search

Prerequisites
-------------

The Search module has to be enabled.


Installation & Configuration
----------------------------

1. Enable the module.
2. Add the permission "Administer search exclude nid" to the necessary roles 
3. To add nodes to the exclusion list, simply visit the modules configuration
   page at Administration > Configuration > Search and metadata > Hide nodes
   from search.
4. Use the node title lookup to search for the nodes you want to hide. After
   adding the node IDs, save the form.
