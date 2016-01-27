Site Disclaimer Module
======================

This lightweight but highly configurable module adds a Site Disclaimer text and an "[x] I agree" checkbox to
the registration page.

Site Disclaimer can contain items like:
 * Terms of Use/Terms of Service
 * Legal Terms & Conditions
 * Acceptable Use Policy
 * Privacy Policy
 * any other legal statements

Visitors are required to accept the terms to register for an account on the website. Administrator can later revise any of the terms and Site Disclaimer module will require all registered users to accept the revisions upon their login.

Detailed Features
=================

 * Provide Site Disclaimer during registration and require visitors to accept the conditions by checking the [x] checkbox.
 * Fully Configurable, with multi-lingual support.
 * Module configuration page shows the preview of how the registration page will look like.
 * Can use nodes: One node can be selected to be shown in a compact scrolled box, or one or more nodes can be linked from the checkbox label text.
 * Using nodes allows formatting and styling flexibility of Drupal:
   - Site Disclaimer nodes can be treated as any other content, including WYSIWYG editor, popups, printable views etc.
   - Nodes can link to other nodes, for allowing a compact Site Disclaimer text with separate sections linked from it (e.g. privacy, terms of service, disclaimers, etc.).
   - Supports node translations
 * Site Disclaimer can be updated at any time by the site Administrator and version of Site Disclaimer can be increased. All registered users will be required to accept the new terms upon login before they can use the website.
 * Themeable ('theme_site_disclaimer' is responsible for the Site Disclaimer section on the registration page and 'site_disclaimer_checkbox_label' is responsible for styling of the "I agree" checkbox)


Installation And Configuration
==============================

1. Move or copy or extract the 'site_disclaimer' folder to sites/all/modules.

2. Enable the module 'Site Disclaimer' on the page admin/modules.

3. Create a Site Disclaimer page at node/add/page (other content type can be used if desired). Do not promote the node to the front page.

4. Go to admin/config/people/site_disclaimer and type the title of your Site Disclaimer page
in the autocomplete text field "Node Title" in "Node to include with Site Disclaimer" section.

5. Save your configuration.

6. Clear your Drupal cache at admin/config/development/performance by clicking
'Clear cached data'.

7. Log out and access the registeration page at user/register.

It will now be required for anyone wishing to sign up to check the 'I agree with these terms' checkbox.


Simpletest
==========
The module has simpletest coverage of main functionality.


Credits
=======

Site Disclaimer module was forked from "Terms of Use" module for Drupal 6 by chill35 (Caroline Schnapp) in 2010.

The code was heavily refactored and improved by iva2k, new features added, and then converted to Drupal 7.


FAQS
====

QUESTION 1:
I have a very long text for terms of use of my site, so I don't like to
show all this text in the registration page. Is it possible to have this solution:
Put only a link to the Site Disclaimer page in the registration page containing the
checkbox, like this:

    Terms of Use of this page are available here (<- this a link to the terms
    of use page)
    [x] I certify that I read and I agree with these terms

ANSWER:
Yes, this module has a very easy way to create just what you need:
 * Create any number of pages with detailed conditions, such as "terms of use", "privacy policy", etc.
 * Create a short page that contains only the desired statement for the registration page and includes all the links to the pages with detailed conditions. 

You can use WYSIWYG editor for these pages and style the content as desired if necessary WYSIWYG modules are installed. One neat possibility is to use lightbox or highslide module to make the detailed pages popup when links are clicked.

Last, on the Site Disclaimer module configuration page enter the short page title or node id, enter the text for the checkbox label, save configuration and preview your results.


QUESTION 2:
I installed the module, but I am wondering, how to ask already
registered users to check again Site Disclaimer "I agree" checkbox, after an update of the Terms of Use page?

ANSWER:
Site Disclaimer module supports that. Go to the Site Disclaimer module configuration page and click the "add new version" radio button under "Site Disclaimer Version" section. Optonally you can enter details of the changes (something like "we revised Privacy Policy to guard your data better"). 

Then save configuration and users with an existing account will be asked to accept the new version when they log in, and they will not be able to use the website until they have accepted.


QUESTION 3:
It doesn't look nice when the terms page is long. Why not use a popup
instead? or make the Terms scrollable, or collapsible?

ANSWER:
You have many options at your disposal to make it exactly as you need:
* Use a short node with links to detailed pages (see QUESTION 1).
* You can limit the height of the Site Disclaimer section on module configuration page. This will show the Site Disclaimer page in a scrolled pane (if the page is longer).
* You can link to the Terms instead of showing them in the registration form (see the available tokens for the [x] checkbox label on the module configuration page).

