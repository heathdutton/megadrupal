Contains
A configurable action set "Domain VBO: modify user domains" that can be used in
Views Bulk Operations on all entities of type 'user'.
- choose the action to perform: replace with, add, remove.
- choose the domains to replace/add/remove for the user(s).

A configurable action set "Domain VBO: modify node domains" that can be used in
Views Bulk Operations on all entities of type 'node'.
- choose the action to perform: replace with, add, remove.
- choose the domains to replace/add/remove for the node(s).

Setup
Download and enable this module.
Create a View with Views Bulk Operations. (see Administration Views for a good
example).
Add 'Domain VBO: modify node domains' or 'Domain VBO: modify user domains' as an
action and save the View.
For convenience add the field 'Domain Access: Domain ID' to the View to you can
see what domains are assigned.

Notes and similar modules
If you add the field 'Domain Access: Domain ID' to your View you can see which
domains are assigned to the node or user. Note that these domains are not
aggregated by default, therefore each domain gets its own row in your View
result. This doesn't have any influence on the VBO operation or Domain
functionality. It is just a visible thing.
If you use Domain Access Entity module for entities other than 'node' or 'user'
you can simply use the 'modify entity values' action that is part of entity api.
The Domain Rules module contains similar actions but not a full
replace/add/remove action for users and nodes.
The Domain Actions module is also similar, but for d6 only and unsupported