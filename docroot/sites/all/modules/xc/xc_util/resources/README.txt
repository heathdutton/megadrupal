This directory contains utility bash script files, to export the content of a MySQL table

- csv.sh
  Shows a MySQL table as CSV rows
  usage: csv.sh database user password table

- xml.sh
  Shows a MySQL table as XML
  usage: xml.sh database user password table

- export_csv.sh
  Exports a MySQL table as CSV dile
  usage: export_csv.sh database user password table

- export_xml.sh
  Exports a MySQL table as XML file
  usage: export_xml.sh database user password table

- export_all_tables.sh
  Exports all MySQL tables relevant to eXtensible Catalog Drupal Toolkit settings
  usage: export_all_tables.sh database user password

Parameters:
- database: the Drupal database name
- user: the MySQL user name
- password: the MySQL user's pasword
- table: the name of database table to display/export

Variables:
// ezproxy_url_rewrite
ezproxy_url_rewrite_enabled
ezproxy_url_rewrite_javascript_enabled
ezproxy_url_rewrite_fragment_regex
ezproxy_url_rewrite_host_regex
ezproxy_url_rewrite_infix
ezproxy_url_rewrite_ip_ranges
ezproxy_url_rewrite_pages
ezproxy_url_rewrite_pages_option
ezproxy_url_rewrite_path_regex
ezproxy_url_rewrite_port_regex
ezproxy_url_rewrite_prefix
ezproxy_url_rewrite_protocol_regex
ezproxy_url_rewrite_query_string_regex
ezproxy_url_rewrite_roles
ezproxy_url_rewrite_suffix
ezproxy_url_rewrite_url_type

// ncip
ncip_debugging
ncip_stateless_timeout
ncip_idle_timeout
ncip_waiting_timeout
ncip_processsing_timeout

// oaiharvester
oaiharvester_cron_last
oaiharvester_statistics

// syndetics
syndetics_settings

// xc_account
xc_account_pi

// xc_auth
xc_auth_override_usernames
xc_auth_truncate_long_usernames
xc_auth_authenticate_by_auth_type_perm
xc_auth_authenticate_by_method_perm

xc_auth_login_page_url

  variable_set('xc_browse_ui_installed', 0);
  variable_set('xc_browse_tab_installed', 0);
  variable_set('xc_browse_element_installed', 0);
  variable_set('xc_browse_list_installed', 0);

xc_dewey
xc_dewey_ajax

xc_external_last_checked

xc_index_definition_modification_time

xc_wordnet_host
xc_wordnet_port
xc_wordnet_path
