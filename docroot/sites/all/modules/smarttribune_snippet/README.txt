Smart Tribune AddSnippet
=============

I. Overview

SmartTribune AddSnippet is a module that allows admins to easily insert
Smart Tribune snippet and control its visibility.
It is used to inject Smart Tribune code snippet just before end body tag 
which is the recommended placement for Smart Tribune snippet.

Smart Tribune is a brand new SaaS software that let you personalize your 
online customer relationship, allowing you to build a feedback community 
for your visitors and customers.

Benefits :

    * Significantly reduce the cost of processing your customer support/request.
    * Engage even more your community and create a real knowledge base
    * Improve your products/services and prioritize your road map in line with 
      your customers ideas and expectations.
    * Improve your ranking in the search engines (SEO) and increase your 
      incoming traffic to acquire more new customers
    * Be innovative in your approach to customer relationship

    To learn more about Smart Tribune, feel free to visit the website : 
    http://www.smart-tribune.com

I. Description

  This module allows you to name your Smart Tribune snippets and 
  organize them by weight. Configuration of a single snippet provides
  visibility settings : white/black list by Drupal path.

  Here is the list of regions where you can inject your Smart Tribune
  snippets:
   * Header
   * Footer

  We strongly recommend you to put them in the footer region.

  You can also use drag & drop functionality on the smart tribune snippet
  overview page to change the relative weights of your snippets to make sure
  they execute in the correct order within a given region.

II. Configuration
  
  * Only roles with the "administer Smart Tribune snippet" permission can access
    the administrative page and configure this module.
  * To configure the Smart Tribune snippet module, navigate to:
    Admin > Structure > Smart Tribune Snippet
    (/admin/structure/smarttribune_snippet)
  * The "List" tab will display all Smart Tribune snippets you've created.
    It provides a drag & drop weight table and links for enabling/disabling a
    snippet, editing a snippet, or deleting a snippet.
  * The "Add Smart Tribune snippet" tab will display a form to add a new
    snippet.
  * Once you've added a snippet, you are presented with further configuration
    options such as changing the active/inactive status, white/black listing
    page visibility, and choosing content types to restrict visiblity to. These
    options were designed to look/feel/behave similar to Drupal's core block
    module.

Reminder:
    You can retrieve your Smart Tribune snippet directly from the "Settings 
    button" in your ST administrative interface
    Do not forget to allow your domain from "Allowed websites" section
