Comment Score
=============

Allows administrators to configure arbitrary rules in which to score the
"badness" or "goodness" of comments on the site.

The more positive the number, the more negativity points it has. It is typically
easier to find negative modifiers than it is positive.

Integration with other modules
==============================

* Views - there is a views handler for the comment score, so you are able to
  create moderation screens for instance that list comments waiting to be
  published, sorted by score ascending (so best to worst).
* Rules - there are events triggered whenever a comment is created that is
  either above or below the threshold for the user. This allows you to perform
  arbitrary actions like email sending, or granting a "trusted commenter" role.
* Mollom - you can optionally send the comment text to Mollom, where the
  response for whether the comment is likely SPAM is used to add negativity
  points.


Included score modifiers
========================

* Blacklisted words (with regex matching)
* Capital letter to lowercase letter ratios
* URLs in the comment
* Length of comment
* Mollom SPAM score (optional)

API
===

There is an alter hook allowing you to inject your own custom modifiers into the
score for a comment, the context passed in is:

* full comment text
* authors user object (or the anonymous user)

You can then write your own logic, e.g. if the comment is submitted between the
hours of 2am to 6am (your local time) add 50 negativity points.

Module uses
===========

This module lends itself to helping to moderate inbound comments, from either
anonymous or authenticated users. It can help your moderation team address the
"cream" comments first, leaving the "cruft" for later when you have time.
