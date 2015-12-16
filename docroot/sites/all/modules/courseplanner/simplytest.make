; --------------------------------------------
; Make file intended for http://simplytest.me/
; --------------------------------------------

; Base parameters
; ---------------
api = 2
core = 7.x
projects[drupal][version] = 7.x

; General-purpose modules
; -----------------------
projects[ctools][type] = "module"
projects[date][type] = "module"
; projects[date][patch][] = "https://www.drupal.org/files/date-call-to-member-functioin-getName-on-non-object-1697322-8.patch"
; projects[date][patch][] = "https://www.drupal.org/files/1930936-1-the_r_format_printed_twice.patch"
projects[entity][type] = "module"
projects[entityreference][type] = "module"
projects[features][type] = "module"
projects[flag][type] = "module"
projects[libraries][type] = "module"
projects[panels][type] = "module"
projects[rules][type] = "module"
projects[strongarm][type] = "module"
projects[views][type] = "module"
; projects[views][patch][] = "https://www.drupal.org/files/1874838-1-use_link_display_in_exposed_blocks.patch"
projects[views_bulk_operations][type] = "module"

; Special-purpose modules (and necessary patches)
; -----------------------------------------------
projects[conditional_fields][type] = "module"
projects[date_ical][type] = "module"
projects[date_ical][version] = "2.x-dev"
projects[eva][type] = "module"
projects[facetapi][type] = "module"
projects[field_group][type] = "module"
projects[fullcalendar][type] = "module"
; projects[fullcalendar][patch][] = "https://www.drupal.org/files/1439026-15-undefined_index_for_date_fields.patch"
projects[fullcalendar][patch][] = "https://www.drupal.org/files/1867756-5-keep_original_library_structure.patch"
projects[fullcalendar][patch][] = "https://www.drupal.org/files/issues/ajax_date_format-2185449-11.patch"
projects[field_collection][type] = "module"
projects[field_collection_table][type] = "module"
projects[field_collection][patch][] = "https://www.drupal.org/files/field_collection-1304214-28.patch"
projects[hierarchical_select][type] = "module"
projects[hierarchical_select][patch][] = "https://www.drupal.org/files/issues/hierarchical_select_attachbehavior-828418-37.patch"
projects[link][type] = "module"
projects[node_clone][type] = "module"
projects[panels_style_collapsible][type] = "module"
projects[pm_existing_pages][type] = "module"
projects[references_dialog][type] = "module"
; projects[references_dialog][patch][] = "https://www.drupal.org/files/references_dialog-wrong-call-to-entity_access-1780626-6.patch"
projects[rules_panes][type] = "module"
projects[search_api][type] = "module"
projects[search_api_db][type] = "module"
projects[unique_field][type] = "module"
projects[views_data_export][type] = "module"
projects[viewsnodefield][type] = "module"
projects[viewsnodefield][patch][] = "https://www.drupal.org/files/1864064-2-allow_skipping_previously_rendered_nodes.patch"

; Other modules
; -------------
projects[admin_menu][type] = "module"
projects[module_filter][type] = "module"

; Libraries
; ---------
libraries[fullcalendar][download][type] = "file"
libraries[fullcalendar][download][url] = "http://arshaw.com/fullcalendar/downloads/fullcalendar-1.5.4.zip"
libraries[iCalcreator][download][type] = "file"
libraries[iCalcreator][download][url] = "http://kigkonsult.se/downloads/dl.php?f=iCalcreator-2.16.6"
