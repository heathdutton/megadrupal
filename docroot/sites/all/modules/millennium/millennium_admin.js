Drupal.behaviors.millenniumAdmin = {
  attach: function(context) {
    jQuery("#range-fieldset", context).hide();
    jQuery("#edit-source", context).change(
      function() {
        //alert("changed!");
        var choice = jQuery("input[@name=source]:checked").val();
        if (choice == "list") {
          jQuery("#range-fieldset").hide();
          jQuery("#list-fieldset").show();
          jQuery("#edit-offline-refresh-wrapper").hide();
          jQuery("input[name='offline_refresh']").change();
          jQuery("#baseurl-table").show();
          jQuery("#query-fieldset").hide();
          jQuery("#url-fieldset").hide();
          jQuery("#import-options").show();
          jQuery("#edit-using-cron").show();
        }
        if (choice == "range") {
          jQuery("#range-fieldset").show();
          jQuery("#list-fieldset").hide();
          jQuery("#edit-offline-refresh-wrapper").hide();
          jQuery("#options-fieldset").show();
          jQuery("#edit-using-cron").show();
          jQuery("#baseurl-table").show();
          jQuery("#query-fieldset").hide();
          jQuery("#url-fieldset").hide();
          jQuery("#import-options").show();
          jQuery("#edit-using-cron").show();
        }
        if (choice == "query") {
          jQuery("#range-fieldset").hide();
          jQuery("#list-fieldset").hide();
          jQuery("#edit-offline-refresh-wrapper").hide();
          jQuery("#baseurl-table").show();
          jQuery("#query-fieldset").show();
          jQuery("#url-fieldset").hide();
          jQuery("#import-options").show();
          jQuery("#edit-using-cron").hide();
        }
         if (choice == "url") {
          jQuery("#range-fieldset").hide();
          jQuery("#list-fieldset").hide();
          jQuery("#edit-offline-refresh-wrapper").hide();
          jQuery("#baseurl-table").hide();
          jQuery("#query-fieldset").hide();
          jQuery("#url-fieldset").show();
          jQuery("#import-options").show();
          jQuery("#edit-using-cron").hide();
        }
        if (choice == "existing") {
          jQuery("#range-fieldset").hide();
          jQuery("#list-fieldset").hide();
          jQuery("#edit-offline-refresh-wrapper").show();
          jQuery("#baseurl-table").hide();
          jQuery("#query-fieldset").hide();
          jQuery("#url-fieldset").hide();
          jQuery("#import-options").show();
          jQuery("#edit-using-cron").show();
        }
        if (choice == "test") {
          jQuery("#range-fieldset").hide();
          jQuery("#list-fieldset").hide();
          jQuery("#edit-offline-refresh-wrapper").hide();
          jQuery("#baseurl-table").hide();
          jQuery("#query-fieldset").hide();
          jQuery("#url-fieldset").hide();
          jQuery("#import-options").hide();
          jQuery("#edit-using-cron").hide();
        }
        if (choice == "queued") {
          jQuery("#range-fieldset").hide();
          jQuery("#list-fieldset").hide();
          jQuery("#edit-offline-refresh-wrapper").show();
          jQuery("#baseurl-table").hide();
          jQuery("#query-fieldset").hide();
          jQuery("#url-fieldset").hide();
          jQuery("#import-options").show();
          jQuery("#edit-using-cron").hide();
        }
        jQuery(this).blur();
      }
    );
    // Activate on page load.
    jQuery("#edit-source").change();
  }
};
