
Node Access Relation README

CONTENTS OF THIS FILE
----------------------

  * Introduction
  * Requirements
  * Installation
  * Usage
  
 
INTRODUCTION
------------
Gives content access permissions to users if they are referenced as a source
bundle in a directional relation. Checks view, update, and delete grant 
operations, and can pass those on to the target bundle relation content, or 
trigger a different grant configuration according to settings.

NOTE: This is a sandbox project and under very active development. 

Project page: http://drupal.org/sandbox/andyhawks/1594688


REQUIREMENTS
------------
http://drupal.org/project/relation


INSTALLATION
------------
Install and enable the relation, relation_ui and node_access_relation modules.

For detailed instructions on installing contributed modules see:
http://drupal.org/documentation/install/modules-themes/modules-7


USAGE
-----
  1. Enable the relation, relation_ui, and node_access_relation modules.
  2. Create a new Relation Type.
  3. Check 'Directional Relation' to set access controls.
  4. Set the Source Bundle as a user and the Target Bundle as a node type.
  5. Set the node access permissions for the target node bundle as desired 
     and save the relation type.
  6. Source users or users with access to the source node type should now 
     have access to target node bundles based on the node access permissions.
     Additional linked relations where the target node bundle used above is 
     the source can now be created with permissions inherited through 
     node_access_relation.
     
For detailed instructions on using Relation see:
https://drupal.org/node/1274796
