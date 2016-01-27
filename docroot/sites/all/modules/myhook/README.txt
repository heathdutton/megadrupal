// $Id: README.txt,v 1.2 2010/08/23 05:01:59 ufku Exp $

MyHook
http://drupal.org/project/myhook
====================================
Allows site administrators define custom hooks.


CONFIGURING AND USING
---------------------
1. Go to admin/config/development/myhook and define your custom hooks as if your module's name is "myhook".
Ex:

/**
 * Implements hook_node_insert().
 */
function myhook_node_insert($node) {
   drupal_mail_system('myhook', 'mail')->mail(array(
    'to' => 'admin@example.com',
    'subject' => 'New content',
    'body' => 'New content entered at ' . url('node/' . $node->nid, array('absolute' => TRUE)),
    'headers' => array('From' => variable_get('site_mail', ini_get('sendmail_from'))),
  ));
}


More hooks at http://api.drupal.org/api/group/hooks/7
