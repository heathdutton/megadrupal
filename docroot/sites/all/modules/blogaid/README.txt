DESCRIPTION
===========

Helper module for bloggers. This module contains some usability enhances for content creation and blog managemant. It reduces the mouse clicks. This 1.x version designed for one user blog sites, not multi blog sites!

"Blog Aid adminblock"
contains some link
- create content for all enabled node type
- approve new comments (if Comment module is enabled)
- approve new trackbacks (if Trackback module is installed and enabled)
- file manager (if IMCE module is installed and enabled)
- My account
- Administration
- Logout

"Draft blog entries"
This block contains links to un-published story edit.

Add new buttons to node edit forms:
If you create a new story (or any selected node type) you see new "save to draft" button. If you push this button the node will be set "un-published" before being saved.
If you edit an existing un-published story (or any selected node type) you see new "save to public" button. If you push this button the node will be set "published" and the "Authored on" field will be the current date.
If you edit an existing story (or any selected node type) you see new "save and edit" button. If you push this button the node is being saved and you will see the edit form again.

Add Set/Unset sticky link

INSTALLATION
============
1. Place the "blogaid" folder in your "modules" directory (i.e. sites/all/modules/blogaid).
2. Enable the Blog Aid module under Administer >> Site building >> Modules.
3. Turn on the Blog Aid adminblock and Draft blog entries. Turn off Navigation block. (don't worry you will have a link to admin pages or you can use the "Two Magic" ?q=user and ?q=admin)
4. Set the Blog Aid module under Administer >> Site configuration >> Blog Aid settings
5. If you don't edit your blog with administrator user(uid=1) you must set permissions to the apropriate user role. (admin/user/permissions#module-blogaid)

AUTHOR
======
István Palócz (istvan at palocz.hu)