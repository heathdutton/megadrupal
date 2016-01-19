
AUL(Access User Lists)

--------------------------------------------------------------------------------
AUL(Access User Lists) is very similar to the ACL(Access Control Lists). The 
difference that AUL creates access per user and adds nodes to it(ACL works vice 
versa. It creates grand per node and adds users).

AUL module can be useful when logic in content access in your site is very hard
and you don't want to create lots of grants per each user because it slows page 
load and causes long queries.

With AUL you can create for example just 3 realms for each user (one for view, 
one for edit, one for delete) and than add nodes to your AUL.
Using AUL we move our big list of access "locks" in node_access table. But 
hook_node_access_records will work quickly and we won't have long queries with 
access conditions because we don't have many access "keys".
