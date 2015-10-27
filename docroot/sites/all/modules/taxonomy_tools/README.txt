
CONTENTS OF THIS FILE
---------------------

 * Introduction
 * Taxonomy Publisher
 * Taxonomy Publisher Filter
 * Taxonomy Role Access
 * Taxonomy Redirect


INTRODUCTION
------------

A package containing several taxonomy management tools.
The package includes:
 * Taxonomy Publisher
 * Taxonomy Publisher Filter
 * Taxonomy Role Access
 * Taxonomy Redirect
 * Taxonomy Copier

Taxonomy Tools provides an overview page which provides a summary for
vocabularies that use any of the tools included in this module.
Taxonomy Tools overview page can be accessed by going to
admin/structure/taxonomy/%vocabulary_machine_name/overview or by clicking
the "Overview" tab in vocabulary administration page or by clicking "Overview"
tab in term page.

By itself the Taxonomy Tools module provides user with the functionality
of changing taxonomy term order in active branch and deletion of multiple
taxonomy terms at once.

TAXONOMY PUBLISHER
------------------

Taxonomy Publisher adds status "Published" to taxonomy terms and also
provides scheduled publishing/unpublishing of taxonomy terms.
Taxonomy terms' state depending on publishing/unpublishing dates is changed
during cron.

Configuration and usage:
1. Go to admin/config/taxonomy-tools/publisher;
2. Select for which vocabularies you want to use Taxonomy Publisher;
3. Now when adding/editing terms of those vocabularies "Publishing settings"
fieldset will be visible.

TAXONOMY PUBLISHER FILTER v2.0
-------------------------

This module allows you to limit the taxonomy terms listed in a term reference form element or in a views exposed filter.
The current version supports select, checkbox/radio and autocomplete widgets for term reference fields.

Configuration and usage:
1. enable the module from the modules page: 'admin/build/modules'
2. enable taxonomy publisher on the vocabularies you wish to limit
3. enable/disable the terms in the above vocabularies
4. configure roles that show see the limited list only under admin/people/permission

TAXONOMY ROLE ACCESS
--------------------

Taxonomy Role Access allows to control access to taxonomy terms.
Works for each user role and is configurable
for each taxonomy term separately.

Configuration and usage:
1. Go to admin/config/taxonomy-tools/role-access;
2. Select for which vocabularies you want to use Taxonomy Role Access;
3. Select for which user roles you want to use Taxonomy Role Access;
4. Go to overview page of selected vocabulary
admin/structure/taxonomy/*/overview;
You should see columns for selected user roles;
5. Change access settings by clicking the image (green check or red cross);
6. Green check means the taxonomy term is accessible for the specified
user role, red cross means - not accessible.
(Child taxonomy terms inherit the accessibility settings from parent
taxonomy term. By default taxonomy term is accessible.).

TAXONOMY REDIRECT
-----------------

Taxonomy Redirect allows to set a redirection URL for each separate
taxonomy term. If redirection link is set, an icon will be shown next to the
taxonomy terms' name in the Taxonomy Tools overview page.

Configuration and usage:
1. Go to admin/config/taxonomy-tools/redirect;
2. Select for which vocabularies you want to use Taxonomy Redirect;
3. Now when adding/editing terms of those vocabularies "Taxonomy term
redirect link" field will be visible;
4. Enter the URL you want the taxonomy term to redirect to;
5. User roles which have the permission "bypass taxonomy redirect" will see a
message "This taxonomy term is set to redirect to: ---" instead of being
redirected.

TAXONOMY COPIER
---------------

Taxonomy Copier allows to make copies of separate taxonomy terms and
taxonomy term hierarchy branches.

Configuration and usage:
1. Go to http://d7.local/admin/config/taxonomy-tools/copier;
2. Select the default copying options for terms and term associated nodes;
3. Go to overview page of selected vocabulary
admin/structure/taxonomy/*/overview and click copy link next to a
selected taxonomy term;
OR
3. Click "Copy" tab in term page;
4. Select the taxonomy terms you want to copy;
5. Select what to do with taxonomy term associated nodes.
