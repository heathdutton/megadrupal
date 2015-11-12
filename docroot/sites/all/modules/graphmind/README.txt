
Graphmind

Troubleshooting:
If you have any kind of problems with the module (especially with loading the 
  map) check this list first:
- have the latest module (Graphmind)
- have the latest stable Services module (currently is 6.x-2.2)
- have the latest Amfphp module (currently is 6.x-1.0-beta2)
- have the Amfphp 3rd party library (http://goo.gl/txBp)
- have the following service submodules enabled: node_service, system_service, 
  user_service, file_service, views_service, services_keyauth (they should be 
  enabled on install by default)
- On Services > Settings > General: in the selectlist choose Key authentication, then: ONLY check [Use sessid]
- amfphp gateway is ok: /?q=services/amfphp gives: "amfphp and this gateway are installed correctly..."
- you are the admin user (uid = 1)
- if you are not the admin than you have the following permissions: get own user data, load node data, access content
- clean url is does NOT matter
- have flash player (> 10.0)
- the mindmap node in Drupal is published and you have edit/view access