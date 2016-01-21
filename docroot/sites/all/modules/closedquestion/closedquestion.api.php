<?php

/**
 * Overrides the loading of answers. The answer can be altered by this function.
 *
 * @param integer $nid
 *   The node id
 * #param integer $uid
 *   The user id
 * @param array &$answer
 *   The answer as it is the Closed Question db table. Keys:
 *     'answer': Serialized answer
 *     'once_correct': Number of tries after which the user answered
 *        the question correctly
 *     'tries': Number of tries
 *     'unixtime': Timestamp of last answer
 * @return array (optional) The answer as you would like to be. Array should
 *                 contain the same keys as $answer
 */
function hook_cq_load_answer_alter($nid, $uid, &$answer) {
  $data['answer'] = $data['answer']; //usually you don't want to touch this one
  $data['once_correct'] = 0; //user never answered question correctly
  $data['tries'] = 0; //always first try
  $data['unixtime'] = 0;

  return $data;
}

/**
 * Called when user saves answer
 *
 * @param integer $nid
 *   The node id
 * #param integer $uid
 *   The user id
 * @param array &$answer
 *   The answer as it is the Closed Question db table. Keys:
 *     'answer': Serialized answer
 *     'once_correct': Number of tries after which the user answered
 *        the question correctly
 *     'tries': Number of tries
 *     'unixtime': Timestamp of last answer
 * @return boolean
 *   Whether Closed Question should store the answer or not. To prevent CQ
 *   storing the answer, all modules implementing this hook should return false
 */
function hook_cq_save_answer($nid, $uid, $answer) {
  //store answer at location of choice

  return false;
}

/**
 * Called when user resets answer
 *
 * @param array $data Array containing user and node references:
 *                     'nid': Closed Question node id
 *                     'uid': User id
 */
function hook_cq_reset_answer($nid, $uid, $data) {
  //update local answer storage
}
