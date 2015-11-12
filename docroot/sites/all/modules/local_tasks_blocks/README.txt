CONTENTS OF THIS FILE
---------------------

  * Introduction
  * Installation
  * Upgrading from 1.x to 2.x
  * Maintainers


INTRODUCTION
------------
Local tasks blocks turns the standard MENU_LOCAL_TASKS into blocks that can be
repositioned or interacted with at the theme layer.  It provides 2 variations,
each representing a different presentation of the tasks on a page:

1) Local tasks - This is the standard Drupal system output for the
tasks, usually rendered as tabs on the page.

2) Local tasks menu - This block renders through theme_menu_links and
theme_menu_tree, and provides a nice tree of links similar to Drupal core's
menu system output.  It also allows MENU_LOCAL_TASKS to be integrated with
other administration modules such as the Admin module
(http://drupal.org/project/admin).

For each presentational variation, 2 additional blocks are exposed:
"Primary tasks only" and "Secondary tasks only," to allow the site designer to
choose where primary and secondary tasks show up on a page.

Each block exposes several configuration options, including the ability to turn
into an accordion, hiding the task links until a user expands the block.


INSTALLATION
------------
Activate the module and assign at least one of these provided blocks to a theme
region:

1) Local tasks
2) Local tasks: Primary
3) Local tasks: Secondary
4) Local tasks menu
5) Local tasks menu: Primary
6) Local tasks menu: Secondary


UPGRADING FROM 1.X TO 2.X
-------------------------
The block names have changed, along with the addition of the 2 different display
variations (Plain and Menu styles).  When upgrading from the 1.x blocks, the
upgrade script will attempt to automate translating old block placement to new
blocks.  For clarity, these are assumed to be equivalent blocks in 2.x:
1.x Block 	              2.x Block
---------                 ---------
Local tasks: all          Local tasks
Local tasks: combo 	      Local tasks menu
Local tasks: primary 	    Local tasks menu: Primary
Local tasks: secondary 	  Local tasks menu: Secondary


MAINTAINERS
-----------
- jay.dansand (Jay Dansand)
- manarth
- lotyrin
