= Warning Drupal module
Dag Wieers <dag@wieers.com>

This is a simple module to add warnings to certain forms to discourage
humans from adding spam and commercial or unrelated URLs on the premise
that the website is moderated anyway.


== Background
When I started using Drupal for my personal blog it only took some people only
a few weeks to abuse the site's comment system to post commercial URLs. But
worse, those posts included what seemed reasonable (although sometimes
useless) content. For moderators this simply adds work to verify whether the
posted URLs are related to the post or comment and whether the comment is
relevant.

Often the comment is related to the subject (giving a personal opinion,
agreeing) something a spammer deemed helpful to not get anyone suspicious and
remove the comment, but they either include a URL in the body (sometimes
disguised) or in the contact information.

I was also unlucky to get listed in (what is called) do-follow lists,
apparently some people are looking for ways to get their own sites higher up
in Google and by linking their site on other sites they succeed in that. Now,
because of a bug in Drupal6 contributed links did not by default get a
no-follow tag and that triggered me to get listed on various do-follow lists,
despite my site being moderated. We fixed that in Drupal6.

I got about 10 to 15 spams a day, with peaks of 30 spams that I had to
moderate. All other means to block spam failed for these spam. Mollom even
indicates that I get an average of 200 spams that they successfully block. And
I do send all the spam I manually tag to Mollom.

== Idea
It was clear that most of the spam that got through the filters were being
added manually and therefor where very custom and appeared on-topic. I was
confident that these people would not have commented if they had known my
personal blog was moderated.

Even people coming from one of the do-follow list may not have bothered to
spend the effort if they had known it was in vain from the start. So a simple
warning message with a very strict policy (we delete everything that links
back to unrelated sites) would be sufficient to get most people to hit the
road.


== Implementation
Since I am pretty new to Drupal development I implemented the module with the
limited knowledge I have. I encourage you to send me feedback about the
implementation so that the module can improve and be more versatile.

The implementation now 'abuses' the forms and adds a fieldgroup with name
Warning to the form it is attached to. The fieldgroup only contains a
description, being the warning message.

Currently only 2 warnings have been added:

 - Comment warning
 - New user warning

but other warnings can be added with limited skill. If you have an idea or
need in that area, pleae let me know.


== Permissions
I wanted the warning messages to be configurable by Role, so that the
administrator can decide who can see those messages. The problem here is that
Drupal's permission systems seems to expect that permissions granted to an
administrator were also granted automatically to anyone else.

In the strict sense that seems logical, but in my case I may wish to have only
warnings for anonymous users, and not for eg. authenticated users or
administrators, however if you add a warning for authenticated users, the
message certainly should apply to anonymous users.

This is quite the opposite than what the permissions systems implemented. So
this is the main reason why the permissions are reversed.

 - administer warnings
 - disable comment warning
 - disable register warning


== Links

 - http://svn.rpmforge.net/svn/trunk/tools/warning/

// vim: set syntax=asciidoc:
