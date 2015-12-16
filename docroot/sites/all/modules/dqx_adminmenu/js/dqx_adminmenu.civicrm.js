
(function(){

  var d = Drupal.dqx_adminmenu;


  d.behaviors.civicrm = function(context, settings, $divRoot) {
    if (typeof cj != 'undefined' && cj != null && false) {
      cj( function() {
        cj('#dqx_adminmenu-admin > ul > li > a').each(function() {
            if ( cj( this ).html( ) == 'CiviCRM' ) {
                cj( this ).click ( function( ) {
                    cj( "#civicrm-menu" ).toggle( );
                    return false;
                });
            }
         });

        var contactUrl = "/civicrm/ajax/rest?className=CRM_Contact_Page_AJAX&fnName=getContactList&json=1&context=navigation";

        cj( '#sort_name_navigation' ).autocomplete( contactUrl, {
            width: 200,
            selectFirst: false,
            minChars:1,
            matchContains: true      
        }).result(function(event, data, formatted) {
           document.location="/civicrm/contact/view?reset=1&cid="+data[1];
           return false;
        });
      });
    }
  };

})();
