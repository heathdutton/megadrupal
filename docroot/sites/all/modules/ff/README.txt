Fast Fetch provides an easy way for your custom modules to grab information about Drupal content without having to call the standard Drupal API, which be overkill in some instances.

Dependencies 
____________ 
none

Install 
_______

1. Copy the ff folder to the modules folder in your installation.

2. Enable the module using Administer -> Site building -> Modules (/admin/build/modules).


Functions
_________

ff_title ($nid, $type = "", $published = "")
   Fetches the value of the title field for the provided Node ID ($nid).  
   Optionally checks for a provided node type and published status.
   
   $nid         - Node ID to look up
   $type        - (Optional) Machine name of content type to look up
   $published   - (Optional) Published status (1 = published, 0 = not published)
   
   Returns node title based on Node ID or FALSE.
   
   
ff_term ($tid, $vid = "")
   Fetches the term name for the provided Term ID ($tid). Optionally checks for 
   a provided Vocabulary ID ($vid).
   
   $tid     - Term ID to look up
   $vid     - (Optional) Vocabulary ID
   
   Returns term name based on Term ID or FALSE.
   
   
ff_type ($nid)
   Fetches the content type of the node for the provided Node ID ($nid).
   
   $nid     - Node ID to look up
   
   Returns value of 'type' field within node table or FALSE.
