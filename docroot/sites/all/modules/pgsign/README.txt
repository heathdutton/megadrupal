Description
-----------
All requests to database made by db_query will be made by DB user with the name
as current Drupal user. If user is anonymous or do not exists in DB default
database username will be used.

Usage
-----
Sample user creation:
CREATE USER %username WITH PASSWORD '%password'; GRANT drupal TO %username;

Can be configured according to your needs at the admin/config/people/pgsign page.
