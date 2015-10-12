
AUL Relations - Synchronizes grants of related nodes.

--------------------------------------------------------------------------------

Important notes:
- Current module works only if you grant access with next AUL API functions:
  - aul_add_aul()
  - aul_remove_aul()
  or if you use UI of aul_vbo or aul_roles module.
- Current module works only with node access
- Currently module supports entity_reference field
- Module can work with field_collection hierarchy. (On your content type you can
  create field_collection field and put entity_reference field inside 
  field_collection)

How to use module:
1) For your content type(node) create entityreference field that references
   another content type(node).
2) Check "Sync grants with referenced node" at the very bottom of 
   entityreference field setings ("edit") page
3) That's it. From now all the grants of your related content types will be 
   synced.
