<?php

/**
 * @file
 * The functions used for storing and loading session data for Mechanical tutor.
 */

/**
 * Loads any question parameters stored from previous page loads.
 *
 * This function checks if the parameters are already stored in the session
 * variable. When a question is already built, its parameters is stored as an
 * object property – this function is only called when setting up new questions.
 *
 * @param $storage_id
 *   The ID to the parameters are stored with. Prefix with module name to avoid
 *   name conflicts.
 * @return
 *   An array with parameters, as used by buildQuestion() and other methods, or
 *   FALSE if no parameters are found.
 * @see tutor_data_load()
 * @see tutor_parameters_store()
 */
function tutor_parameters_load($storage_id) {
  return tutor_data_load($storage_id, 'question_parameters');
}

/**
 * Stores question parameters in the $_SESSION variable, for persistance.
 *
 * This function wraps tutor_data_store, to store question data between page
 * loads.
 *
 * @param $storage_id
 *   The ID to store parameters with. Could use the question ID, but also
 *   something else if you want the same question to be stored in multiple
 *   instances for the same user. Prefix with module name, to avoid name
 *   conflicts.
 * @param $parameters
 *   The parameters to store.
 */
function tutor_parameters_store($storage_id, $parameters) {
  tutor_data_store($storage_id, 'question_parameters', $parameters);
}

/**
 * Loads question data stored from previous page loads.
 *
 * @param $storage_id
 *   The ID to the data is stored with.
 * @param $data_category
 *   The data category, for example 'question_parameters' or 'messages'.
 * @return
 *   The stored data, or FALSE if no parameters are found.
 */
function tutor_data_load($storage_id, $data_category) {
  // Check if the variables are set.
  if (isset($_SESSION['tutor'][$storage_id][$data_category])) {
    return $_SESSION['tutor'][$storage_id][$data_category];
  }
  else {
    return FALSE;
  }
}

/**
 * Stores question data in the $_SESSION variable, for persistance.
 *
 * This storage is used for things like question parameters or messages, to
 * display on future page loads.
 *
 * @param $storage_id
 *   The ID to store data with. Could use the question ID, but also something
 *   else if you want the same question to be stored in multiple instances for
 *   the same user. Prefix with module name to avoid name conflicts.
 * @param $data_category
 *   The data category, for example 'question_parameters' or 'messages'.
 * @param $data
 *   The data to store.
 * @return
 *   The stored data, or FALSE if no parameters are found.
 */
function tutor_data_store($storage_id, $data_category, $data) {
  $_SESSION['tutor'][$storage_id][$data_category] = $data;
}

/**
 * Removes all parameters for a given storage ID from the $_SESSION variable.
 *
 * @param $store_id
 *   The ID to the data is stored with.
 */
function tutor_parameters_reset($storage_id) {
  tutor_data_reset($storage_id, 'question_parameters');
}

/**
 * Removes all data for a given storage ID from the $_SESSION variable.
 *
 * @param $storage_id
 *   The ID to the data is stored with.
 * @param $data_category
 *   If set, only this data category will be reset. If ommitted (or FALSE), all
 *   data categories are reset.
 */
function tutor_data_reset($storage_id, $data_category = FALSE) {
  if (!$data_category) {
    if (isset($_SESSION['tutor'][$storage_id])) {
      unset($_SESSION['tutor'][$storage_id]);
    }
  } else {
    if (isset($_SESSION['tutor'][$storage_id][$data_category])) {
      unset($_SESSION['tutor'][$storage_id][$data_category]);
    }
  }
}
