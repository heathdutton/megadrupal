
CONTENTS OF THIS FILE
---------------------

 * Introduction
 * Requirements
 * Known limitations


INTRODUCTION
------------

Current Maintainer: Tauno Hogue <tauno@brainprod.com>

This module provides a Bean type plugin that allows admins to create blocks
that display a single entity as a block using a particular view mode. The
entity being display is either specified when the bean is created, or
determined dynamically based on the entity currently being viewed.


Requirements
------------

* Entity API (https://drupal.org/project/entity)
* Bean (https://drupal.org/project/bean)


KNOWN LIMITATIONS
-----------------

* Block caching is currently disabled for entity view beans.