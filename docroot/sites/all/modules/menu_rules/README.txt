CONTENTS OF THIS FILE
---------------------

 * Introduction
 * Installation
 * Usage
 * Examples
 
INTRODUCTION
------------

Current Maintainer: Kristofer Tengström <kristofer@storleden.se>

Menu Rules provides Rules that are related to menu items. This project started 
out of the need for a D7 replacement for Automenu. I realized that achieving 
the same results through Rules would be both quicker and more generic than 
writing a module specifically for that use. Menu Rules simply aims to integrate 
the menu system with Rules in a comprehensive way.

INSTALLATION
------------

This module is installed just like any other Drupal module, for example through 
the interface at yoursite.com/admin/modules.

USAGE
-----

Here are the rules as of now.

Events:
- A menu link was inserted
- A menu link was updated
- A menu link was deleted
- A menu link was changed in some way (inserted/updated/deleted) - includes the 
operator

Conditions:
- Node has menu entry - checks if the node has a menu link.

Actions:
- Create a menu item for node
- Update a menu item for node
- Rebuild all menus

EXAMPLES
--------

An example of how to use the rules is located inside the ./rules folder. So far 
there is an automenu replacement there which you can import via the Rules 
interface.
