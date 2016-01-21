This README.txt describes the installation and usage of the Drupal tsort module

WHAT THIS MODULE DOES
This module allows you to sort a table by any column heading, where the table data
is not directly pulled from a database, but is simply in an array of rows. 


INSTALLATION
Put the module files in a tsort directory in the usual place for contributed modules 
on your site. This is usually in the /sites/all/modules directory so you will end up
with /sites/all/modules/tsort with all of the tsort files in that directory.


EXAMPLE USAGE
The following example shows a full example of usage.  The function which is provided 
by this module is the following one: 
  $rows = tsort_nonsql_sort($rows, $sort, $order['sql']);
  
All the rest of the sample code below is normal 

/**
 * Create a sorted version of the players table
 * The players table has the followin columns:
 * - uid
 * - name
 * - rating
 * - played
 * - won
 * - lost
 * - drawn
 * - rating
 * - rating change
 * - current
 */
function gamer_players() {
  $sql = "SELECT uid FROM {users} ORDER BY name";
  // Converted to the D7 database API syntax.
  $result = db_query($sql);
  foreach ($result as $record) {
    if ($record->uid <> 0) {
      $player_stats = gamer_load_user_stats($record->uid);
      $rows[] = array(
          'uid' => $record->uid,
          'name' => "<a href='" . url("vchess/player/" . $player_stats['name']) . "'>" . $player_stats['name'] . "</a>",
          'rating' => $player_stats['rating'],
          'played' => $player_stats['played'],
          'won' => $player_stats['won'],
          'lost' => $player_stats['lost'],
          'drawn' => $player_stats['drawn'],
          'rating_change' => $player_stats['rchange'],
          'current' => $player_stats['current']
      );
    }
  }

  $header = array(
      array('data' => t('uid'), 'field' => 'uid'),
      array('data' => t('name'), 'field' => 'name'),
      array('data' => t('rating'), 'field' => 'rating'),
      array('data' => t('played'), 'field' => 'played'),
      array('data' => t('won'), 'field' => 'won'),
      array('data' => t('lost'), 'field' => 'lost'),
      array('data' => t('drawn'), 'field' => 'drawn'),
      array('data' => t('rating change'), 'field' =>'rating_change'),
      array('data' => t('current'), 'field' => 'current')
  );
  
  // getting the current sort and order parameters from the url
  // e.g. q=vchess/my_current_games&sort=asc&order=White
  $order = tablesort_get_order($header);
  $sort = tablesort_get_sort($header);
  
  // sort the table data accordingly
  $rows = tsort_nonsql_sort($rows, $sort, $order['sql']);
  
  $table['header'] = $header;
  $table['attributes'] = array();
  $table['caption'] = array();
  $table['colgroups'] = array();
  $table['sticky'] = "";
  $table['empty'] = "The message to display in an extra row if table does not have any rows.";
  $table['rows'] = $rows;

  return theme_table($table);
}