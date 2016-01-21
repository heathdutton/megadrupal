// $Id$

/**
 * @file
 * README file for Domain Taxonomy.
 */

Domain Taxonomy
Implements taxonomy domain access logic (like domain access for nodes).

CONTENTS
--------

1. Installation

----
1  Installation

   1) activate module (admin/build/modules)
   2) customize module settings on special control panel (admin/build/domain/taxonomy)
      Some options of this module repeat similar options on main Domain Access setup
      form (admin/build/domain). But in this place similar options works for domain
      access terms managment.

      some new options:

      - "Vocabulares without domain access rules"
      	Select checkboxes for vocabulares with terms which don`t use domain taxonomy access mechanism
      	and can be accessible anywhere by default.

      - "Share this vocabulares to all affiliates by default"
		select vocabulares which would be like node types in "Domain node types" domain access setting.
		Terms in this vocabulares will be published to all affiliates when created. If publishing options
		won't be inherited.

      - "Parent vocabulares for node types"
		Inherit access rules for nodes from terms. When editing or creating nodes you can use "inherit
		from parent term" options to inherit domain access rules for this node from parent term. But
		node can have several associated vocabulares and this option specify which vocabulare use for
		inherit by current content type.
		Example: choose vocabulary "Forum" for node type "Forum Topic" if you want nodes in forums to
		inherit parents forums domain access rules.

	  - "Term link patterns"
		This like Node link patterns. Uses for term links when SEO is activated for domain taxonomy
		link output.
		IMPORTANT: For rewrite urls for terms like for nodes you must patch Domain
				   Access 'settings_custom_url.inc' file. In this file at begin of
				   function 'custom_url_rewrite_outbound' insert code below:
				   --------------
				   if (module_exists('domain_taxonomy')) {
				 	  domain_taxonomy_url_rewrite_outbound($path, $options, $original_path);
				   }
				   --------------

    3) Declare terms access rules on term edit forms on submit. Term forms extended by elements
       identical to node edit forms + some specific term options:

      - "Load domain access options for term from parent term"
        This boolean organize inheriting domain access rules from parent taxonomy term. If true then
        domain access form settings overrides by parent term.

      - "Override all child terms"
        This boolean organize inheriting domain access rules of current term to all his child terms.

      - "Override all child nodes"
        This boolean organize inheriting domain access rules of current term to all his child nodes.

 	4) EDIT/CREATE node in vocabulares which uses domain access rules

	   Node form extends by new boolean option "Load domain access options for node from parent term",
	   that works on submit.

       When editing node with 'set domain access' permission you can edit this option.
       Otherwise this option hides for user and get true value when creating node and false value when
       node edit.

       It works this way: registered user or anonymous create new node (may be Forum topic).
       For example: User without permissions to choose domains for publishing nodes, selects
       taxonomy(Forum) only for this node. And after submiting, this node will gain domain access rules
       from parent Forum term. After this, admin can edit this node. With 'set domain access' rights he
       will see an option "Load domain access options for node from parent term" in domain access
       options block. This options will be false for saved node. Admin can assign individual domain
       access rules for this node if inherit options is not set.

!!
Note, that terms by domain filtering is disabled on urls like admin/* for administration access to all terms

