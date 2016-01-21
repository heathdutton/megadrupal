
CONTENTS OF THIS FILE
---------------------

 * Introduction
 * Installation
 * Versioning System
 * Project Plans


INTRODUCTION
------------

Current Maintainer: Kevin Day <thekevinday@gmail.com>

The need for Web Accessibility is ever increasing. Drupal needs to be capabable
of providing an accessible interface as well as accessible content.  The
purpose of this module is to provide the ability to make CCK fields accessible
without breaking existing designs.

This project will attempt to provide what is needed to assist a drupal site
developer in following the WCAG 2.0 and the upcoming ARIA 1.0 Web Accessibility
Standards.  This in itself does not make a website accessible, this module
only provides the means to do so.


INSTALLATION
------------

This module does not directly modify the database and relies on CCK to perform
database interactions. Simply add the module to /sites/all/modules/.


VERSIONING SYSTEM
-----------------

I, Kevin Day, use version numbers to communicate the status of a given project.
Communicating to the end-user whether a project version is stable, unstable, a
bugfix release, or a feature release should take no more effort than looking at
the number itself.  Described here is a short explanation of the versioning
system I use for all of the projects I own (modified with consideration to
Drupal's versioning system).

A version number is broken up into 3 parts: Major.Micro-Dev:

Major Version Number:
  A Major version number represent features and redesigns of a project and API
  breakage is assumed. Backwords compatibility is not expected, but nor is it
  denied.

Micro Version Number:
  The Micro version number represent bug fixes and security fixes. Micro
  bugfixes should _never_ break API with the exception being serious security
  issues.

-Dev tag:
  The -dev tag will be appended to any version that is under development. Drupal
  uses the scheme Major.x-dev, with Major being the only actual number.  This
  practice will be followed such that version 1.0 development will be: 1.x-dev
  and a version 1.0 bugfix might be: 1.1.


PROJECT PLANS
-------------

The original project plans consist of three major versions.  Each of the major
versions will represent a milestone of features added.

The first phase, version 1.0, will consist of the bare minimal set of features
for accessibility support.  These are the HTML DOM attributes that are not
currently provided in drupal 6.x but are needed as failsafes for when
proper attributes (such as aria-label) are not available.

An additional reason behind keeping the first release simple and small will
give me, Kevin Day, time to get familiar with how drupal project management
works.

The first phase will not have any features such as aria-label added and will
be provided and maintained as a bugfix/security fix release until version 3.0.

The second phase, version 2.0, will consist of the core features of the ARIA.
These will be the most commonly used and most easily implemented set of ARIA
attributes.

The third phase, version 3.0, will consist of all of the remaining features
of the ARIA.  By this release it is hoped that the ARIA 1.0 will not longer
be a draft but instead an official release.  Expect API changes between 2.0
and 3.0 to reflect changes between the ARIA 1.0 draft and the ARIA 1.0 release.

Aside from the official milestone objectives, I expect a few additional
problems/issues to occur.  These are left open to be handled with and planned
for when the time comes to deal with them.

Of the potential problems that can happen I am expecting the following:
1) API changes due to evolution in drupal (such as Drupal 6.x to 7.x).
2) API changes to ignorance on my part in different aspects of the project.
3) API changes due to CCK API Changes.
4) API changes due to WCAG and ARIA changes.
5) At some point, and probably with version 3.0, the number of available
   options will overwhelm the administration of CCK fields.  It is expected
   that once I, Kevin Day, learn more advanced Drupal programming I will have
   to create a separate accessibility administration page to cope with all of
   the modifications needed to provide CCK fields with the ability to be made
   accessible.
6) At this time, this projects goal is only to focus on CCK Form Fields. CCK is
   not just about the form fields, but also about the presentation of the form
   field data.  It is very easy to predict that users will eventually want this
   module to also provide the ability to make the presented CCK fields also
   capable of providing accessible data.  At this time I, Kevin Day, do not
   know if this will be allowed to be supported in this specific project or if
   another project will have to be created.  Doing this will also make the
   issue mentioned in #5 more likely to happen.
