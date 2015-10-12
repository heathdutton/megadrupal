Contextual Entity List is a Ctools  plugin consisting context and content type.
After enabling the module when we create a variant of a page manager
in a context area we get an additional option in drop down
is called "Entity Type".

This option basically helps to choose existing entity
and corresponding bundle of that entity.
After adding that context for a particular entity
& it's bundle(Currently for node & taxonomy)
a content type plugin will be available under "widget"
is called "Contextual Entity".
There we can specify the page number to enable pagination.

On the basis of the above configuration this page will be rendered with
available nodes (of that bundle & entity) along with comments
and comment form section.
So advantage is that we can change the content list dynamically of a page
by changing only the context of that panel/page.
So this is easy to identify in which context the page is displaying the data
and the context is assigned from Entity lebel.

--------------------------------------------
1.Enable modules from Modules->Contextual Suite.
2.Permission->access bundle call -> to specific role(Admin/authenticate user).
3.In a panel varient of a page in context->choose Entity Type from drop down.
4.From configuration choose Entity type & corresponding bundle.
5.From Content -> widget-> Contextual Entity available.
--------------------------------------------------------
Module list on which contextual Entity depends,
1.entity
2.panel
3.ctools
4.page_manager
----------------------
Use case instruction
--------------------
