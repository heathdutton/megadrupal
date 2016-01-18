<?php

/**
 * Implementation of hook_rules_event_info().
 * @ingroup rules
 */
function vchess_rules_event_info() {
 
  $events['vchess_move_made'] = array (
      'group' => 'vchess',
      'label' => t('Move has been made in a particular game'),
       'variables' => array(
           'opponent' => array('type' => 'user', 'label' => t('Opponent')),
           'gid' => array('type' => 'integer', 'label' => t('Game id')),
           'move' => array('type' => 'text', 'label' => t('Last move')),
       ),
  );
  
  $events['vchess_challenge_accepted'] = array (
      'group' => 'vchess',
      'label' => t('Player has accepted a challenge'),
      'variables' => array(
          'opponent' => array('type' => 'user', 'label' => t('Opponent')),
          'gid' => array('type' => 'integer', 'label' => t('Game id')),
      ),
  );
   
  return $events;
}


// /**
//  * Implementation of hook_rules_action_info().
//  */
// function vchess_rules_action_info() {
//   $actions['vchess_rules_action_inform_user'] = array(
//           'group' => 'vchess',
//           'label' => t('Inform user that the move has happened'),
//           'arguments' => array(
//               'gid' => array(
//                   'type' => 'integer',
//                   'label' => t('Game id')
//               ),
// // //               'white_user' => array(
// // //                   'type' => 'user',
// // //                   'label' => t('User playing white'),
// // //               ),
// // //               'black_user' => array(
// // //                   'type' => 'user',
// // //                   'label' => t('User playing black'),
// // //               ),
// //            ),
// //               'node' => array(
// //                   'type' => 'node',
// //                   'label' => t('MYNODE description'),
// //               ),
//           ),
//           'help' => t("MYHELP"),
// //          'module' => 'vchess',
//   );
  
//   return $actions;
// }

// /**
//  * Action for informing user
//  */
// // function vchess_rules_action_inform_user($white_user, $black_user) {
// function vchess_rules_action_inform_user($gid) {
  
// //   dpm($gid);
  
  
// //   dpm($white_user);
// //   dpm($black_user);
// //   dpm("White user is " . $white_user->name);
// //   dpm("Black user is " . $black_user->name);
// }
?>