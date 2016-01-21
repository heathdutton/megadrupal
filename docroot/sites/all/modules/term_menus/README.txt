
========================== Summary =====================================

Term_menus is designed for the common request in drupal community that user
wants to create a menu based on taxonomy field, i.e., a menu automatically
connecting terms and nodes. Like:

--term1
  --node/1
  --node/23
  --node/16
--term2
--term3
  --term4
    --node/19
    --node/22
    --term6
      --node/3
      --node/5
    --term7
--term8
--term9

========================= Install ======================================

Install this module as usual way for most Drupal modules.

========================= Design =======================================

Because terms and nodes are connected via taxonomy field, this module is based
on taxonomy field. Every taxonomy field could be enabled to have a term menu.

A term menu includes all terms link inside the 'Allowed vocabulary' -- setting
of taxonomy field. For this level, it's what taxonomy_menu at drupal.org has
done. Term menu takes step into the next level, including nodes link associated
with terms via this taxonomy field.

======================== Manage ========================================

A term menu could be enabled at taxonomy feld editing page, e.g.,
'admin/structure/types/manage/article/fields/field_test_term'.

You may check all term menus at 'admin/structure/term_menus'.

Once term menu is enabled for a taxonomy field, there will be Four
ways/UIs that you may manipulate it.

-- You may config/delete this term menu from taxonomy field editing page.

-- You may view/edit/add/delete links for this term menu at Drupal default
menu UI, e.g., 'admin/structure/menu/manage/menu-field-test-term'.

-- You may change node's taxonomy field value, e.g., change its value from
term8 to term19, to update the term menu links automatically.

-- You may update terms to automatically update its term menus links.

NOTE: if you have enabled the sync configs for term menu, all your mannual
changes from default menu UI will be reset when a term/node is
created/updated/deleted!

======================= Configuaration =================================

There're some configuration for term menu at taxonomy field editing page.

-- Enable custom term page for this term menu.

This is used to replace Drupal's default term page for term_menus links. If
you don't enable custom term page here, you will get default term page url
inside term_menus links.

This module creates a new custom term page
'term_menus/%term_menus_menu/%term_menus_tid' for our case. For instance,
a path 'term_menus/menu-field-test-term/4' indicates it's for a term(tid is 4)
which resides in term_menu: menu-field-test-term.

Don't mix it with default term page, e.g., 'taxonomy/term/$tid'. We are not
overriding default term page. Instead, we create our own "fake term page" for
our own usage.

There're some merits resulting from our own "fake term page":
  1). While Drupal default term page will include all nodes , our "fake term
      page" only includes nodes associated with this term via this term menu's
      taxonomy field.

      For instance, you have a vocabulary 'Location', and many terms inside,
      e.g., USA, Germany, France, etc. Two content types -- 'Shoes' and
      'Clothes' have used this vocabulary via two different taxonomy fields
      and you have enabled term menus for both taxonomy fields.

      For Shoes's term menus, term Germany's "fake term page" will only
      include Shoes nodes located in Germany, and for Clothes's term menus,
      term Germany's "fake term page" will only include Clothes nodes located
      in Germany too. And we all know, Drupal default term Germany's page will
      include all nodes, no matter it's Shoes or Clothes! This feature could
      make more senses for your business logic!

  2). It could be enabled to include child term's nodes. Default term page
      doesn't provide this feature.

  3). It could have its own page for each term menu. In this way, each
      term menu's term link will be independent from other term menu's. Admin
      could gain great flexibility to control each term menu.

  4). It could make term_menus module don't interrupt Drupal default term
      pages. Your other modules could do whatever they want to dafault term
      page without worrying about this module term_menus.
      In other words, any changes made to deafult taxonomy term page won't
      have effect on this module. This module could be independent from
      default term page.

  5). It could make use of pathauto integration to construct specific page
      url alias for each term menu, without alias cross with other terms,
      or other menus.

-- Add child nodes for custom term page.

If this term menu uses our own term page, it could be enabled to have its
child term's nodes too. In Drupal default term page, it doesn't include child
term's node.

-- Enable url alias/pathauto integration with this term menu.

If this term menu uses our own term page, it could be enabled to make use
of pathauto integration. There will be many available tokens for you to
construct custom term page's url.

-- Synchronize term menu when a node is updated.

When a node is created/updated/deleted, do you want to synchronize the term
menu as well.
If it's not enabled here, this module does what taxonomy_menu at drupal.org
module has done. But of course, you can create multiple menus based on the
same vocabulary, while taxonomy_menu only creates one menu for a vocabulary.

-- Synchronize term menu when a term is updated.

When a term is created/updated/deleted, do you want to synchronize the term
menu as well.

Note: After the first time term menu is enabled for a taxonomy field, ONLY
      '-- Add child nodes for custom term page.' could be updated again.
      Other configs above will be frozen and can't be updated anymore, once
      they are set.
      This design is to maximize consistency for all links once term menu
      is created. You can play with differnt configs by enable/disable
      term menus multiple times locally.
      Ideally, they should be enabled all to take best advantage of this
      module.

-- REBUILD THIS TERM MENU
  -- Rebuild Button
    If you want to disable/hide those terms link which don't have child nodes,
    you can click this button. It checks the child terms' nodes too.
  -- Reset Button
    This button will enable all term's links, no matter it has child nodes
    or not.

======================= Pathauto integration ===========================

If you are taking advantage of this module's custom term page, you are not
only going to get the child nodes from child terms, you will get pathauto
integration too! So you will get your own term path with term menus's
token support.

For instance, you have a vocabulary called 'Location', and it has many
locations e.g., USA, France, Germany, etc. You have a taxonomy field
enabled term menus with this vocabulary in content type: Products. For
term "France", you could get a path alias "products/location/france",
with token pattern:
"[term_menus:node-type-name]/[term_menus:vocabulary-name]/
[term_menus:term-name]" enabled at "admin/config/search/path/patterns".

======================= Notes / Shortlist ==============================

-- This module will NOT automatically enable/disable term links when
node or term is created/updated/deleted. You have to manually click 'Rebuild'
button to do this. In fact, it's difficult to detect whether disable/enable
"some term link" when "some link" is updated.

-- This module ONLY counts the first term parent. Admin could add multiple
term parents inside vocabulary, but this module will ignore other parents
except the first one existing in db.

====================== Todolist ========================================

-- Add test for this module.

======================= Credits ========================================

This module takes some great idea from https://drupal.org/project/taxonomy_menu
and Drupal core taxonomy module.
