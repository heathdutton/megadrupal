Add revision support to users and fields like there is for nodes.

For the moment all field from {users} have revisions, except for password, it's possible that I still add password. 

What's working:
* Create/edit users
* Fields on users
* Show revisions
* Revert revision
* Delete revision
* Permissions: view, revert and delete
* Views integration: basic stuff but needs some more attention 

To do:
* Views integration
* Diff integration
* ...

WARNING: this is still alpha code, most of it will work but isn't properly tested, so make sure you have backups!

BTW: I'll appreciate all help I can get, so feel free to:
* Test
* Code review
* Patch
* ....


