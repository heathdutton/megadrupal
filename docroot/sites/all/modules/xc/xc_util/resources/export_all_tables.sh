#!/bin/bash
#################################################################################
# Exports all MySQL tables relevant to eXtensible Catalog Drupal Toolkit settings
#################################################################################

usage(){
  echo "Usage: $0 database user password (internals|data|defaults|nodata|all)"
  exit 1
}

export_csv() {
  if [ $1 ]
  then
    TABLE=$1
    mysql -u $USER -p$PW $DB -B -e "select * from $TABLE;" | sed 's/\t/";"/g;s/^/"/;s/$/"/;s/\n//g;s/"NULL"/NULL/g' > export/$DIR/$TABLE.csv
  fi
}

export_xml() {
  if [ $1 ]
  then
    TABLE=$1
    mysql -u $USER -p$PW $DB --xml -e "select * from $TABLE" | sed 's/<field name="\([^<>]*\)">\([^<>]*\)<\/field>/<\1>\2<\/\1>/g' | sed 's/&quot;/"/g' > export/$DIR/$TABLE.xml
  fi
}

# ========= Internal usage

export_internals() {
  DIR=internals
  if [ ! -d export/$DIR ]; then
    mkdir export/$DIR
  fi

  # ncip
  export_csv ncip_application
  export_csv ncip_connection

  # oaiharvester
  export_csv oaiharvester_providers
  export_csv oaiharvester_formats_to_providers
  export_csv oaiharvester_formats
  export_csv oaiharvester_harvester_schedules
  export_csv oaiharvester_sets
  export_csv oaiharvester_sets_to_providers
  export_csv oaiharvester_harvest_schedule_steps
  export_csv oaiharvester_harvest_queue
  export_xml oaiharvester_logs
  export_xml oaiharvester_batch

  # xc_account
  export_csv xc_account_bookmarked_items

  # xc_auth
  export_xml xc_auth_user
  export_xml xc_auth_type
  export_xml xc_auth_login_block

  # xc_external
  export_csv xc_external_connection
  export_xml xc_external

  # xc_ils
  export_csv xc_ils_settings

  # xc_metadata
  export_csv xc_metadata_entity
  export_csv xc_metadata_field
  export_csv xc_metadata_attribute
  export_csv xc_metadata_entity_entities
  export_csv xc_metadata_field_entities
  export_csv xc_metadata_field_attributes
  export_csv xc_metadata_namespace
  export_csv xc_metadata_entity_group # empty
  export_csv xc_path                  # empty
  export_csv xc_source
  export_csv xc_source_locations

  # xc_ncip_provider
  export_xml xc_ncip_provider

  # xc_oaiharvester_bridge
  export_csv xc_oaiharvester_bridge_header  # empty
  export_csv xc_oaiharvester_bridge_set     # empty
  export_csv xc_oaiharvester_bridge_schedule_source
  export_csv xc_oaiharvester_bridge_schedule_info
  export_csv xc_oaiharvester_bridge_changes # empty

  # xc_statistics
  export_csv xc_statistics_facet
  export_csv xc_statistics_search
}

export_data() {
  DIR=data
  if [ ! -d export/$DIR ]; then
    mkdir export/$DIR
  fi

  # data
  export_csv xc_entity_properties
  export_xml xc_sql_metadata
  export_csv xc_entity_relationships
}

export_defaults() {
  DIR=defaults
  if [ ! -d export/$DIR ]; then
    mkdir export/$DIR
  fi
  # ========= Importable/exportable
  # xc
  export_csv xc_settings

  # xc_browse
  export_csv xc_browse_ui
  export_csv xc_browse_tab
  export_csv xc_browse_element
  export_csv xc_browse_list

  # xc_index
  export_csv xc_index_attribute_to_field
  export_csv xc_index_field_to_facet
  export_csv xc_index_field_type
  export_csv xc_index_super_location
  export_csv xc_index_sortable_field
  export_xml xc_index_facet

  # xc_metadata
  export_csv xc_location

  # xc_search
  export_csv xc_search_mlt
  export_csv xc_search_highlighter
  export_csv xc_search_date_facet_properties
  export_csv xc_search_date_facet_properties_additional
  export_csv xc_search_field_facet_properties
  export_csv xc_search_query_facet_properties
  export_csv xc_search_facet_group
  export_csv xc_search_facet_field
  export_csv xc_search_ui
  export_csv xc_search_ui_boosting
  export_csv xc_search_sortoption
  export_csv xc_search_location
  export_csv xc_search_full_record_display
  export_xml xc_search_display_template
  export_xml xc_search_display_template_element

  # xc_solr
  export_csv xc_solr_field_type
  export_csv xc_solr_servers

  # xc_sql
  export_csv xc_sql_storage
}

if [ -z $1 ]
then
  echo ERROR: Missing database, user and password parameters
  usage
fi

if [ -z $2 ]
then
  echo ERROR: Missing user and password parameters
  usage
fi

if [ -z $3 ]
then
  echo ERROR: Missing password parameter
  usage
fi

if [ -z $4 ]; then
  echo ERROR: Missing type parameter. Select one of internals, data, defaults, nodata, all.
fi

DB=$1
USER=$2
PW=$3
CMD=$4
DIR=mix

if [ ! -d "export" ]; then
  mkdir export
fi

if [ "$CMD" = "internals" ]; then
  export_internals
elif [ "$CMD" = "data" ]; then
  export_data
elif [ "$CMD" = "defaults" ]; then
  export_defaults
elif [ "$CMD" = "nodata" ]; then
  export_internals
  export_defaults
elif [ "$CMD" = "all" ]; then
  export_internals
  export_data
  export_defaults
fi

