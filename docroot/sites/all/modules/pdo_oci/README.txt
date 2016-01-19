WHAT IS SIREN?
--------------

This is my personal project besides Drupal, which try to research the
possibility of other database supporting for Drupal, e.g. Oracle, IBM DB2,
MSSQL, SQLite, etc; on the other hand, I will provide unofficial database
supporting during Drupal-6.x life cycle, and further more contribute the
research progress for Drupal-7.x.

For more information on Siren visit
the Siren project homepage at http://siren.sourceforge.net/


CURRENT RESEARCH PROGRESS
-------------------------

Besides official Drupal-6.x mysql, mysqli and pgsql, Siren also provide
pdo_mysql, pdo_pgsql and oci8 database driver supporting. According to the
needs of PDO and Oracle drivers implementation, ALL core queries and some
APIs are hacked for compatibility concern.


3rd PARTY MODULE HACK HOWTO
---------------------------

Based on cross database compatibility concern, Siren come with lots of core
queries hack. Most works are done for you, and it should run across
MySQl/PgSQL/Oracle without any critical problem.

Anyway, you will also need to apply some 3rd party module. So how to let them
work together with this unofficial core?

1. ENCLOSE ALL TABLE/COLUMN/CONSTRAINT NAME WITH []

   Go though you target module, and enclose all table/column/constraint name
   with [] syntax, which similar as {} syntax for all table.

   This can be helped by using VI + regex: /{[A-Za-z0-9_]*}

2. REPLACE ALL '%s' SYNTAX WITH %s

   This is very important for unofficial PDO supporting.

   This can be helped by using VI + regex: :%s/'%s'/%s/gc

3. REMOVE ANY %% SYNTAX

   PDO don't allow inline % syntax when using with ? placeholder.

   This can be helped by using VI + regex: /%%

4. UTILIZE ALL DB_* ABSTRACTION IF POSSIBLE

   These abstraction are mainly duel with syntax different among different DB.

5. USE %s EVEN IT IS AN EMPTY STRING

   Using empty string as placeholder will cause problem among Oracle and
   MSSQL. Escape it by using %s. Oracle and MSSQL driver will able to catch
   them and replace it as required empty string placeholder internally.

6. ALWAYS USE BLOB IF POSSIBLE

   Most Database have size limitation in VARCHAR, e.g. Oralce only allow for
   maximum 4000 characters in VARCHAR2, where DB2 only have a good support of
   VARCHAR update to maximum 4000 characters, too.

   Therefore always try to use BLOB if possible, combine with
   drupal_write_record() and db_decode_blob() function, as most database
   support GB-scale BLOB field without any problem.


UPGRADING FOR SIREN
-------------------

As a flash new research project, Siren 1.x remove the direct upgrade handling
from Drupal 5.x. You should always use Siren for flash install.

On the other hand, you may first upgrade your site from Drupal 5.x to
Drupal 6.x, and handle the convert from Drupal 6.x to Siren 1.x manually.
There is no guarantee about this handling, and please backup and do this with
your own risk.
