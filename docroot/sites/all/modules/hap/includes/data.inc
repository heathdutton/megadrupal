<?php


/**
 * List of US states.
 *
 * If the opposite is required you may use array_flip().
 *
 * @code
 *  $options = array_flip(hap_data_us_states());
 * @endcode
 *
 * Or maybe you just want the abbreviations:
 *
 * @code
 *  $options = drupal_map_assoc(array_keys(hap_data_us_states()));
 * @endcode
 */
function hap_data_us_states() {
  return array(
    'AL'=>'Alabama',
    'AK'=>'Alaska',
    'AZ'=>'Arizona',
    'AR'=>'Arkansas',
    'CA'=>'California',
    'CO'=>'Colorado',
    'CT'=>'Connecticut',
    'DE'=>'Delaware',
    'DC'=>'District Of Columbia',
    'FL'=>'Florida',
    'GA'=>'Georgia',
    'HI'=>'Hawaii',
    'ID'=>'Idaho',
    'IL'=>'Illinois',
    'IN'=>'Indiana',
    'IA'=>'Iowa',
    'KS'=>'Kansas',
    'KY'=>'Kentucky',
    'LA'=>'Louisiana',
    'ME'=>'Maine',
    'MD'=>'Maryland',
    'MA'=>'Massachusetts',
    'MI'=>'Michigan',
    'MN'=>'Minnesota',
    'MS'=>'Mississippi',
    'MO'=>'Missouri',
    'MT'=>'Montana',
    'NE'=>'Nebraska',
    'NV'=>'Nevada',
    'NH'=>'New Hampshire',
    'NJ'=>'New Jersey',
    'NM'=>'New Mexico',
    'NY'=>'New York',
    'NC'=>'North Carolina',
    'ND'=>'North Dakota',
    'OH'=>'Ohio',
    'OK'=>'Oklahoma',
    'OR'=>'Oregon',
    'PA'=>'Pennsylvania',
    'RI'=>'Rhode Island',
    'SC'=>'South Carolina',
    'SD'=>'South Dakota',
    'TN'=>'Tennessee',
    'TX'=>'Texas',
    'UT'=>'Utah',
    'VT'=>'Vermont',
    'VA'=>'Virginia',
    'WA'=>'Washington',
    'WV'=>'West Virginia',
    'WI'=>'Wisconsin',
    'WY'=>'Wyoming',
  );
}


/**
 * Cheapy little helper function to query the nid and title from the node table
 * and return them as a form #options array would expect it. Optionally add a
 * single condition.
 *
 * @param $condition
 *   An array of conditions as you would enter them in
 *   PDOStatement::condition() method.
 */
function hap_data_node_options($condition = NULL) {
  $query = db_select('node', 'n');
  $query->fields('n', array('nid', 'title'));

  if ($condition) {
    call_user_func_array(array($query,'condition'), $condition);
  }

  $options = array();
  foreach ($query->execute()->fetchAll() as $result) {
    $options[$result->nid] = $result->title;
  }
  return $options;
}
