Readme
================================================================================
The Warden module connects a Drupal website with the Warden website monitoring
tool.

Warden aims to pull in status and security information from multiple sites into
a single centralised dashboard. Simply add this module to the sites you want to
include and configure them to send information to the accompanying Warden server
which provides the dashboard application.

The Warden server software can be downloaded from GitHub[1]

Why do I need this?
================================================================================
If you manage multiple websites it helps to have a single centralised dashboard
where the status of all your sites can be seen and viewed.

Configuration
================================================================================
By strict design and principle, this module doesn't have any UI exposed settings
or configuration forms. The reason behind this philosophy is that - as a pure -
utility module only site administrators should be able to change anything and if
they do, things should be traceable in settings.php.

The following configuration options are available:

|      $conf setting       | Default          |               Description                 |
|:-------------------------|:-----------------|:------------------------------------------|
| warden_server_host_path  | ''               | Required to specify the location of your  |
|                          |                  | Warden server. No trailing slash.         |
|                          |                  | e.g. 'https://warden.company.com'         |
|                          |                  |                                           |
| warden_allow_requests    | FALSE            | Set this to TRUE to allow external        |
|                          |                  | callbacks to the site. When set to FALSE  |
|                          |                  | pressing refresh site data in Warden will |
|                          |                  | not work                                  |
|                          |                  |                                           |
|                          |                  |                                           |
|                          |                  |                                           |
| warden_public_allow_ips  | '127.0.0.1,::1'  | List the IP addresses which can make      |
|                          |                  | callback requests. This is usually be the |
|                          |                  | IP address of your Warden server          |
|                          |                  |                                           |
| warden_http_username     | ''               | If your Warden server is protected with   |
|                          |                  | basic HTTP authorization, you can provide |
|                          |                  | the username here                         |
|                          |                  |                                           |
| warden_http_password     | ''               | The HTTP basic authorization password.    |
|                          |                  |                                           |
| warden_token             | random string    | This is generated automatically and is    |
|                          |                  | used as a shared secret between the site  |
|                          |                  | and warden so Warden can trust messages   |
|                          |                  | sent from this site.                      |
|                          |                  |                                           |
| warden_match_custom      | TRUE             | Include custom modules in the data that   |
|                          |                  | is sent to Warden.                        |
|                          |                  |                                           |
| warden_preg_match_custom | '{^sites\/([A-z, | Provide a regex pattern for the module to |
|                          | \.,\-]+)\/module | match the module filename to. A module    |
|                          | s\/custom\/*}'   | file path matching this regex means the   |
|                          |                  | module is a custom module.                |
|                          |                  |                                           |
| warden_match_contrib     | TRUE             | Include contrib modules in the data sent  |
|                          |                  | to Warden.                                |
|                          |                  |                                           |
| warden_preg_match_contrib| '{^sites\/([A-z, | Provide a regex pattern for matching a    |
|                          | \.,\-]+)\/module | modules file as being a contrib module    |
|                          | s\/contrib\/*}'  |                                           |
|                          |                  |                                           |
| warden_match_core        | TRUE             | Include core module versions in the data  |
|                          |                  | which goes to Warden                      |
|                          |                  |                                           |
| warden_context_options   | array()          | Provide stream wrapper context options[2] |
|                          |                  |                                           |

The warden_context_options parameter allows you to pass information into the stream wrappr.
So, for example, if your warden server uses a self signed SSL certificate then you may
want to ensure it is properly verified during any requests by adding the certificate to
you sites codebase locally and using the following configuration in your settings.php

$conf['warden_context_options'] = array(
  'ssl' => array(
    'verify_peer'   => TRUE,
    'cafile'        => 'sites/all/conf/self-signed.crt',
  ),
);

For more information about what can be placed into the context then read this[2]

[1]:  https://github.com/teamdeeson/warden
[2]:  http://phpsecurity.readthedocs.org/en/latest/Transport-Layer-Security-(HTTPS-SSL-and-TLS).html