Description
-----------
The present module allows synchronizing content among multiple websites.
Supported entity types: node and taxomony, menu items.
Many hooks were used during creation of the module. It allows widening
synchronization functional by developing additional modules.
Later, it is planned to implement an opportunity of users synchronization
(Entity type: user) by creation of a submodule with the help of main module
hooks. This way we will create an example of main module functional widening
opportunity.
At the current version we implemented an interface of websites interaction
through XML-RPC, i.e.authorization, settings sending/getting , content
sending/getting (entity fields), getting files from remote website.

All synchronization settings can be indicated on one website and all the other
 remote websites will get them automatically (use checkbox "Set the same field
mapping on the remote site") )