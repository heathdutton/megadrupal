CONTENTS OF THIS FILE
---------------------

* Introduction
* Installation
* Design Decisions

INTRODUCTION
------------

Current maintainer: Karen Ziv <me@karenziv.com>

Taxonomy dupecheck was written when Karen Ziv was entering taxonomy terms for a
personal project and discovered that Drupal allows duplicate terms within the
same vocabulary.

INSTALLATION
------------

There are no special requirements for this module; installation uses the
standard Drupal process. If you're reading this, you've likely already
correctly installed this module.

1. Download the module from drupal.org.
2. Untar the archive into your modules directory.
3. Go to Admin > Modules and enable the Taxonomy dupecheck module.
4. Configure settings at Admin > Modules > Taxonomy dupecheck > Configuration

DESIGN DECISIONS
----------------

This module works by hooking into the taxonomy term and vocabulary admin
forms. Ideally, this would have happened instead in
hook_taxonomy_term_presave and hook_taxonomy_vocabulary_presave, but
neither of these hooks allow you to stop the save process, only run
other tasks based on the term/vocabulary info. This means that if you call
taxonomy_term_save or taxonomy_vocabulary_save programmatically, this
validation won't happen.

Also, note that the default comparison is case-insensitive. That is,
'foo' and 'Foo' will be considered duplicates. Case sensitivity can be
configured in the admin configuration (see INSTALLATION).
