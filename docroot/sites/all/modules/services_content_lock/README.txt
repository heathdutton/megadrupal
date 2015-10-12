Overview
==============================================================================
This module adds Services support to the community module called content_lock 
that will prevent two users from editing the same node concurrently. This 
module exposes the main operations of content_lock through Services as a 
resource, so that content locking can be done over a web service API such as 
REST. The main operations it currently implements are retrieve, create, and 
delete.

Features
==============================================================================
This module exposes the following operations of content_lock as a web service:

retrieve - Find out if a node is locked and get info about a lock e.g. who 
           owns the lock and when it was created.
create   - Create a new content lock on a node. Locks a node preventing other 
           users from editing it.
delete   - Deletes an existing lock on a node. Only the lock owner may delete 
           the lock.

Requirements
=============================================================================
This module depends on the following modules:
Services: http://drupal.org/project/services
content_lock: http://drupal.org/project/content_lock


Install
=============================================================================
1. Install and enable the dependant modules: Services, content_lock.
2. Enable the rest_server module which is a sub-module of Services
3. Install and enable this module.
4. Import the services endpoint.
     1. Navigate to: admin/structure/services
     2. Click the "Import" link
     3. Copy & paste the contents of this file into the box:
        services_content_lock/examples/endpoint_import.txt
5. Click Continue then click Save
6. Use the detailed documentation link below for usage and curl examples.

- or -

1. Watch the step-by-step video tutorial (Best viewed in HD 720p):
   http://youtu.be/snfgg0Pe11c

NOTE: It is a good idea to delete the examples/ directory on production
      environments.

Documentation
=============================================================================
http://drupal.org/node/1933330

Use Case Example
=============================================================================
A large support department updates Drupal provided customer cases through 
Salesforce.com, using the Drupal's Services module REST API. In order to 
prevent concurrent editing of nodes over these services, content_locking 
needs to be exposed as a REST service. This is the exact scenario that this 
module was written to solve.
