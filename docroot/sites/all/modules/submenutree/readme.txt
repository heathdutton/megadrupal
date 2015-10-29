CONTENTS OF THIS FILE
---------------------

 * Introduction
 * Requirements
 * Installation
 * Configuration
 * Usage
 * Sponsors

INTRODUCTION
------------

Current Maintainers:

 * Beng Tan <http://drupal.org/user/132729>
 * Devin Carlson <http://drupal.org/user/290182>

Submenu Tree provides a method for structuring content hierarchically. For
content which has a menu link, Submenu Tree display a list of content which is
at the same level or below the content in the menu.

Content that at the same level as a selected piece of content is referred to
as "sibling" content while content that is below a selected piece of content
is referred to as "sub" content.

Lists of sub or sibling content can be displayed either in a block or directly
inside of the body of a piece of content. Lists can be formatted in six
different ways:

 * Menu
 * Titles only
 * Teasers
 * Teasers with links
 * Full text
 * Full text with links

Submenu Tree also provides a block which displays "extended" links. Extended
links are made up of all menu links in an administrator-selected menu that are
two or more levels deep.

REQUIREMENTS
------------

The Block and Menu modules included with Drupal are required in order to
install Submenu Tree.

INSTALLATION
------------

Submenu Tree can be installed via the standard Drupal installation process
(http://drupal.org/node/895232).

CONFIGURATION
-----

Sub and sibling content default settings can be configured per content type on
the content types page [admin/structure/types].

Submenu Tree configuration options can be configured on the Submenu Tree
configuration page [admin/config/content/submenu-tree].

Configuration options include:

 * Block title generation
 * Extended menu

USAGE
-----

As an Administrator or a user with either the Administer submenu content or
Administer Sibling content permissions, you will see a vertical tab titled
"Sub and Sibling content" while editing content.

Selecting the tab will display options for enabling sub and/or sibling
content, depending on user permissions.

Selecting either option will enable the display of sub/sibling content for the
current content and expand the number of options presented. The additional
options include a title textfield, a display select box and a weight select
box. Note that both sub and sibling content can have unique titles, display
settings and weights.

Each option has an impact on the display of sub and/or sibling content:

 * Title
    Optionally specify a title display above the sub/sibling content list or
    as the sub/sibling content block title.
 * Display
    Selects where and how the sub/sibling content will be displayed. When
    displaying sub/sibling content in a block, ensure that the respective
    block(s) are enabled.
 * Weight
    Selects where in the page the sub/sibling content will appear when one of
    the "content" display types has been selected.

Sponsors
--------

Development of Submenu Tree is sponsored by ThinkLeft (http://thinkleft.com)
and the Ontario Ministry of Northern Development and Mines
(http://www.mndm.gov.on.ca).
