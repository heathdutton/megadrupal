
CONTENTS OF THIS FILE
---------------------

 * Overview
 * Use Cases
 * Installation

OVERVIEW
--------
Fixed Field is a module that displays the same content for all entities the
field is attached to. This provides the user with a powerful way to attach
content to entities.

Using this module in combination with other features such as tokens and filters
(e.g. Insert View to insert a view as a field within an entity or Insert Block
to insert blocks as fields within nodes) as well as HTML markup or PHP scripting
makes static and dynamic, entity-wide attachments possible.

This module provides similar functionality to other modules, such as Insert
View, Insert Block, Media, EVA: Entity Views Attachment and plain old blocks.
However while some of these modules provide methods of attachment of content
to individual nodes and blocks provide a very flexible method to attatch content
to pages, they have several drawbacks.

Insert View, Insert Block and Media attached content cannot be reused on several
entities. The content has to be attached to each entity individually.
While blocks can be displayed or hidden on different pages, they are limited to
theme-defined areas.
Drupal's Field module, however provides an extremely powerful way of arranging
content within entities. Fixed Field aims to bridge the gap between the
limitations of other methods of attaching content and the benefits of Drupal's
fieldable entities.

USE CASES
---------

 * embedding ads in nodes
 * put some fixed content on a node like "This week's special is widgets!" on
   all your products and be able to change them all at once via an admin ui
 * Along with Insert View provide a similar functionality as EVA: Entity Views
   Attachment, however with more flexibility. For example the position of the
   view within the node can be set using the fields ui., and views can be
   wrapped in html or php. For example, tables of views can be created.
 * "Blocks" within nodes
 * Displaying debug information attached to entities. This works especially well
   with the Token API. This way debugging information can also easily be hidden,
   simply by hiding the field.

INSTALLATION
------------

see http://drupal.org/documentation/install/modules-themes/modules-7

