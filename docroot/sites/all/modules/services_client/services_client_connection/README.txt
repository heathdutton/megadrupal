Services Client Connection
==========================

Provides extensible API for talking to remote Drupal site via services and UI
for configuring connections to remote sites. Module allows to define multiple
connections with different attributes to one or multiple sties. Connections
can be exported either via hook or using features.

Dependencies

* ctools
* PHP cURL extension

Architecture
============

This module aims to create general API for connecting to remote site. To achieve
that it uses ctools plugin system. Each connection consists of class with three
plugins. API user shouldn't take care of any aspect of creating requests and
parsing responses. Each plugin can have own configuration (similar to Feeds module).

* Authentication

Ensures that requests to remote site will be authenticated. Currently there are two
available methods:
 
 * Session authentication
 * OAuth (requires OAuth api module)

* Server plugin

Server plugin formats request for remote services server i.e. REST or XML-RPC. By
default module comes with two plugins. Server also parses response from remote Drupal
site and translate data from text form to PHP structures. Supported remote servers:

 * XML-RPC server
 * REST server (allow to communicate via JSON for example)

* Request plugin

Request handles HTTP connection to remote server, sending data and getting HTTP response.
Server is responsible for interpreting low-level HTTP status codes and to generate
exception if connection failed for some reason. Supported is cURL 

Usage
=====

Connection can be defined via administration UI and referenced by name or manually
by passing configuration array in PHP.

1. Configure new connection
   admin/structure/services_client/connection

   - Administration title
   - Description
   - Remote services version
   - Endpoint - remote site URL 
   - Plugins - Authentication, Server, Request

2. Configure each plugin
   admin/structure/services_client/connection/list/[machine_name]/auth
   admin/structure/services_client/connection/list/[machine_name]/server
   admin/structure/services_client/connection/list/[machine_name]/request

Each connection can be exported to code either manually using
  
   hook_services_client_connection_default_connections();

or with Features module.

After connection is defined either in DB or in hook can be initialized with
services_client_connection_get.

   $api = services_client_connection_get('machine_name');
   $node = $api->get('node', 14);
   $node['title'] = 'New title';
   $api->update('node', 14, $node);

Since connection to remote site can be unsuccessful because of many different problem
like network, authentication or site issues Services Client Connection raises
ServicesClientConnectionResponseException . Exception contains all request and response
data. Example:

   try {
     $api = services_client_connection_get('machine_name');
     $node = $api->get('node', 14);
     $node['title'] = 'New title';
     $api->update('node', 14, $node);
   }
   catch (ServicesClientConnectionResponseException $e) {
     $e->log();
   }

In example log method is used which write request and response to watchdog as
'scc' type and error severity. If custom handling is required exception provides
public attributes $e->request and $e->response.

ServicesClientConnection class provides methods that should cover every request
to remote drupal site running services 3.x.

   * index
   * create
   * update
   * get (retrieve)
   * delete
   * targetAction
   * relationships
   
Developers
==========

Hooks documentation is available in services_client_connection.api.php. Examples how 
to write plugins are in services_client_connection.module.

