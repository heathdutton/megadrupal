
PREFACE
-------
This module provides a way to optimize SQL queries or views by using index
tables which (partially) shadows the original query output. This differs from
materialized views since the shadow table does only contain columns which are
relevant for filtering / sorting. This module is capable of automatically
rewriting SQL queries to speed them up using the shadow tables.

CONTENTS
--------
1. Installation
2. Configuration
3. Integration

1. Installation
--------------------------------------------------------------------------------
Copy Shadow into your modules directory (i.e. sites/all/modules) and enable
Shadow (on admin/modules).

Optionally enable the Shadow Views module if you want to optimize views queries.

2. Configuration
--------------------------------------------------------------------------------
All configuration pages can be found on admin/structure/shadow. This
page has three tabs:

- Tables
  Overview of all shadow tables.
- Queries
  Overview of all queries registered in Shadow.
- Test query
  You can test the performance of arbitrary SQL codes here.

To optimize queries, Shadow first needs to know which queries exists and are
parsed though Shadow. This is automatically done by the shadow() function (see
§3). All views queries will be registered in shadow by the Shadow Views module
after viewing the views. Eventually, you may add a query by hand.

To generate a new Shadow table, start with a query. When you click on a query
you can choose which table you want to use to rewrite this query. There is also
an option to create a new table. You cannot generate a new table without using
a query because Shadow takes a lot of information from the query when generating
a new table.

When adding a table, you have to add all fields to the table which are relevant
for filtering and sorting. Most information is already filled in.

The new query will be build directly when checked. This table is directly used
to rewrite queries with the same "guid" (see queries overview).

Tables are automatically updated via hook_entity_$op() (for all entities stored
in the database). You can always rebuild the table manually. Go to the "Tables"
tab, click on the table and then on the "Rebuild" button.

3. Integration
--------------------------------------------------------------------------------
To speed up a query, pass the query though the shadow() function:

shadow(&$query, &$a2, $a3, $a4 = NULL)

This function can be used for both raw SQL codes and SelectQuery objects.
For SelectQuery objects, use the following syntax:

shadow(SelectQuery $query, $guid, $description);

Example:

$query = db_select('node', 'n')
    ->fields('n', array('nid', 'title'))
    ->condition('n.nid', 123);
$guid = 'query1';
shadow($query, $quid, 'Example query #1');
$results = $query->execute()->fetchAll();

Note that you must store guid in a variable, because only variables cna be
passed by reference. You may use shadow_object() directly to prevent that.

For raw SQL codes use the following syntax:

shadow($sql, $args, $guid, $description);

Example:

$sql = 'SELECT n.nid, n.title FROM {node} n WHERE n.nid = :nid';
$args = array(':nid' => 123);
shadow($sql, $args, 'query2', 'Example query #2');
$results = db_query($sql, $args)->fetchAll();

Always pass the arguments array to db_query, even if you do not use arguments
yourself. Shadow may add arguments.
