jQuery(document).ready(function (){
  clean_module_list_hide();
  
  // Assign an action to clicks on the radio buttons
  jQuery('#edit-clean-module-list #edit-control input:radio#edit-control-0').click(function() {
    clean_module_list_show();
  });
  jQuery('#edit-clean-module-list #edit-control input:radio#edit-control-1').click(function() {
    clean_module_list_hide();
  });
});

function clean_module_list_hide() {
  jQuery('#system-modules .admin-requirements').hide();
  jQuery('#system-modules .admin-dependencies').hide();
  jQuery('#system-modules .admin-required').hide();
}

function clean_module_list_show() {
  jQuery('#system-modules .admin-requirements').show();
  jQuery('#system-modules .admin-dependencies').show();
  jQuery('#system-modules .admin-required').show();
}