==================================
AUTHOR
==================================
  Nate Mow
  natemow@gmail.com

==================================
INSTALLATION
==================================

  1) Contact ISG Solutions to install and configure your ISGweb for
  iMIS instance.

  2) Run the inc/drupal_imis_isgweb.sql script against your SQL Server
  iMIS database. This will create 3 new database objects:

      - dbo.drupal_imis_isgweb_users:
          A view that defines users that should be allowed to sign in
          to your Drupal site. If your organization uses specific business
          logic to enable/disable web users (e.g. active membership
          products, specific Activity records, etc.), those SQL clauses can
          be defined here.

      - dbo.drupal_imis_isgweb_sync
          A stored procedure that is called when a user attempts to sign
          in to your Drupal site; it will add/update the user and imis_isgweb
          records in Drupal. Additionally, this procedure is called from the
          module's cron implementation to add/update all active users.

      - dbo.drupal_imis_isgweb_sync_cleanup
          A stored procedure that is called from the module's cron
          implementation to disable Drupal users who are not included in the
          drupal_imis_isgweb_users record set.

  3) Enable the imis_isgweb module for your Drupal site.

  4) In some cases, you may also need to add an entry to your server's hosts
  file in order to correctly resolve outbound requests to the
  iservices.example.com domain.

  5) Set the connection parameters and integration options for the module
  at http://example.com/admin/config/people/imis_isgweb

  6) Test the sync routine; the module will create user records and populate
  the imis_isgweb table.

