db remote
=========

db remote provides copies of core database APIs for interacting with one or more
remote databases.

You may set a default remote database using $conf['db_remote'] = 'the_key';
This must correspond to a database key and will be passed to 
Database:getConnection().

It is possible to set $options['key'] when calling the db_remote_*() functions
in order to use databases other than the one configured in $conf.

hook_db_remote_schema() will only create tables in the database specified in
$conf['db_remote'].
