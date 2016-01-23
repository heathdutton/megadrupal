INTRODUCTION

What is osCaddie?
osCaddie is an initiative at Appnovation to enable Enterprise customers to
leverage Open Source technologies in an integrated and scalable fashion. Two
key components of such a system are Drupal, which is used for front end
interactions, and Alfresco, which is used for back end storage of digital
assets.

Many organizations require WCM functionality such as collaboration (such
as forums), favorites, comments, and ratings; and Enterprise Content
Management (ECM) functionality such as digital content storage and
versioning, workflows, and highly customizable metadata models. ECMs and WCMs
are typically delivered by separate software, each with different strengths.

To best fulfill these requirements, the recommended technology stack includes
both Drupal and an open source ECM solution such as Alfresco. This approach
combines the best aspects of each and is more flexible and robust than either
one system alone.

Features
osCaddie Drupal Alfresco is an integrated solution providing seamless
communication between Drupal and Alfresco. Contents are synced with Alfresco
whenever there is an update to content in Drupal, Alfresco will be updated.
New content is created on Alfresco as soon as it is created on Drupal. osCaddie
Drupal Alfresco allows content-type mapping with Drupal and Alfresco, meaning
a non-standard content-type (such as additional textfields, radio buttons, or
checkboxes) can be used on Alfresco. An administrative interface provides site
admins to easily setup the connection between the two systems, and manages the
content-types.

 1. A custom-made REST JSON API from scratch that facilitates the communication
    between Drupal and Alfresco. CMIS protocol could not be used because it
    focuses on file transfer and document browsing and not the protocol for
    content-type mapping.
 2. The Drupal-side API is flexible enough to accept any content-type structure
    along with all associated custom fields. The parser and extractor used can
    convert a content-type into JSON and back accurately.
 3. osCaddie Drupal Alfresco has an implicit feature which allows contents and
    content-types to be backed-up and restored. Alfresco behaves like content
    repository that can be used to restore all contents of a particular site.
    We need to ensure that content and content-types are restored 100%.

The solution is built using Drupal as a set of modules that are standalone
and can be deployed to any new or existing Drupal instance. osCaddie Alfresco
portion of the solution is compatible with Alfresco 3 and 4 community and
enterprise.


INSTALLATION

Drupal:
Installing the Drupal portion of osCaddie requires an instance running the latest
copy of Drupal 7.x. osCaddie core module must be installed since it provides the
foundational API to communicate with Alfresco, and at minimum the osCaddie Model.
Install osCaddie Alfresco Node if you want to send nodes to Alfresco, given that osCaddie Alfresco
Model is properly configured. osCaddie currently supports core fields.

Alfresco:
For installation of the Alfresco portion of osCaddie, please see the README.txt
file within the Alfresco module.

Note: Both osCaddie Drupal module(s) and osCaddie Alfresco Webapp must be installed
in order for the system to operate properly.


CONTACT

Richard Mo (https://drupal.org/user/2295526)
