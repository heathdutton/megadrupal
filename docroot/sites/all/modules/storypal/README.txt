--------------------------
Storypal module for Drupal
--------------------------

Introduction
------------
Drupal & Storify are two amazing products and two very popular technologies.
No module were done to link the two technologies, until now :-)
This module has been done to let you incorporate stories from Storify.com
into your nodes in a really easy way.

How Storypal works
------------------
Storypal is a module who provides two things:
 - a "Storify field"
 - an input filter.

The field:
When the user will fill that field, Drupal will transform the url into HTML
when a user visit the page, yes sir, as simple as that.

The filter
Storypal allows you now to enable a 'Storify' filter for each available
text formats. It means that when you write a text in a node, you can add
Storify tokens like: [storify:StoryUrl].
Those tokens will be replaced with the story as you would see it on
Storify.com.

To work properly, Storypal is using a standard Storify PHP class which
can be used not only in Drupal, but in any PHP project.
I'm planning to improve it a lot and release it as a single project later.
Storypal is not a copycat of the Wordpress module, example, It's using
the Drupal's native cache for each request to lighten the load on
Storify's servers.

The Storypal class extends the Storify class and provides the Drupal's caching
system.
So, when you use the module, you better use Storypal instead of Storify.

Requirements
------------
 - Libraries module
 - Storify PHP library: https://github.com/Polzme/storify

What's next ?
-------------
I have some good ideas for this module before putting it live.

 - Enable contextual links on Story so we can inline edit the Story,
   using the Storify interface in a overlay.
 - ... your idea ?
